#include "cli_uart.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/wrap_string.h"

#include <openvc/cli.h>
#include <openvc/platform/logging.h>
#include <openvc/platform/uart.h>

#include "cli/cli.hpp"
#include "common/code_utils.hpp"
#include "common/encoding.hpp"
#include "common/logging.hpp"
#include "common/new.hpp"
#include "common/tasklet.hpp"
#include "utils/static_assert.hpp"
#include "utils/wrap_string.h"

#if OPENVC_CONFIG_DEBUG_UART_ENABLE
#include <openvc/platform/debug_uart.h>
#endif

#ifdef VC_CLI_UART_LOCK_HDR_FILE

#include VC_CLI_UART_LOCK_HDR_FILE

#else

/**
 * Macro to acquire an exclusive lock of uart cli output
 * Default implementation does nothing
 *
 */
#ifndef VC_CLI_UART_OUTPUT_LOCK
#define VC_CLI_UART_OUTPUT_LOCK() \
    do                            \
    {                             \
    } while (0)
#endif

/**
 * Macro to release the exclusive lock of uart cli output
 * Default implementation does nothing
 *
 */
#ifndef VC_CLI_UART_OUTPUT_UNLOCK
#define VC_CLI_UART_OUTPUT_UNLOCK() \
    do                              \
    {                               \
    } while (0)
#endif

#endif // VC_CLI_UART_LOCK_HDR_FILE

VC_STATIC_ASSERT(OPENVC_CONFIG_CLI_MAX_LINE_LENGTH <= OPENVC_CONFIG_CLI_UART_RX_BUFFER_SIZE,
                 "command line should be should be smaller than CLI rx buffer");
namespace vc {
namespace Cli {

static vcDEFINE_ALIGNED_VAR(sCliUartRaw, sizeof(Uart), uint64_t);

extern "C" void vcCliUartInit(vcInstance *aInstance)
{
    Instance *instance = static_cast<Instance *>(aInstance);

    Server::sServer = new (&sCliUartRaw) Uart(instance);
}

Uart::Uart(Instance *aInstance)
    : Server(aInstance)
{
    mRxLength   = 0;
    mTxHead     = 0;
    mTxLength   = 0;
    mSendLength = 0;

    vcPlatUartEnable();
}

extern "C" void vcPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength)
{
    static_cast<Uart *>(Server::sServer)->ReceiveTask(aBuf, aBufLength);
}

void Uart::ReceiveTask(const uint8_t *aBuf, uint16_t aBufLength)
{
#if !OPENVC_CONFIG_UART_CLI_RAW
    static const char sEraseString[] = {'\b', ' ', '\b'};
    static const char CRNL[]         = {'\r', '\n'};
#endif
    static const char sCommandPrompt[] = {'>', ' '};
    const uint8_t *   end;

    end = aBuf + aBufLength;

    for (; aBuf < end; aBuf++)
    {
        switch (*aBuf)
        {
        case '\r':
        case '\n':
#if !OPENVC_CONFIG_UART_CLI_RAW
            Output(CRNL, sizeof(CRNL));
#endif
            if (mRxLength > 0)
            {
                mRxBuffer[mRxLength] = '\0';
                ProcessCommand();
            }

            Output(sCommandPrompt, sizeof(sCommandPrompt));

            break;

#if !OPENVC_CONFIG_UART_CLI_RAW
#if OPENVC_POSIX
        case 0x04: // ASCII for Ctrl-D
            exit(EXIT_SUCCESS);
            break;
#endif

        case '\b':
        case 127:
            if (mRxLength > 0)
            {
                Output(sEraseString, sizeof(sEraseString));
                mRxBuffer[--mRxLength] = '\0';
            }

            break;
#endif // !OPENVC_CONFIG_UART_CLI_RAW

        default:
            if (mRxLength < kRxBufferSize - 1)
            {
#if !OPENVC_CONFIG_UART_CLI_RAW
                Output(reinterpret_cast<const char *>(aBuf), 1);
#endif
                mRxBuffer[mRxLength++] = static_cast<char>(*aBuf);
            }

            break;
        }
    }
}

vcError Uart::ProcessCommand(void)
{
    vcError error = VC_ERROR_NONE;

    while (mRxBuffer[mRxLength - 1] == '\n' || mRxBuffer[mRxLength - 1] == '\r')
    {
        mRxBuffer[--mRxLength] = '\0';
    }

#if OPENVC_CONFIG_LOG_OUTPUT != OPENVC_CONFIG_LOG_OUTPUT_NONE
        /*
         * Note this is here for this reason:
         *
         * TEXT (command) input ... in a test automation script occurs
         * rapidly and often without gaps between the command and the
         * terminal CR
         *
         * In contrast as a human is typing there is a delay between the
         * last character of a command and the terminal CR which executes
         * a command.
         *
         * During that human induced delay a tasklet may be scheduled and
         * the LOG becomes confusing and it is hard to determine when
         * something happened.  Which happened first? the command-CR or
         * the tasklet.
         *
         * Yes, while rare it is a race condition that is hard to debug.
         *
         * Thus this is here to affirmatively LOG exactly when the CLI
         * command is being executed.
         */
#if OPENVC_CONFIG_MULTIPLE_INSTANCE_ENABLE
        /* TODO: how exactly do we get the instance here? */
#else
    vcLogInfoCli("execute command: %s", mRxBuffer);
#endif
#endif
    if (mRxLength > 0)
    {
        mInterpreter.ProcessLine(mRxBuffer, mRxLength, *this);
    }

    mRxLength = 0;

    return error;
}

int Uart::Output(const char *aBuf, uint16_t aBufLength)
{
    VC_CLI_UART_OUTPUT_LOCK();
    uint16_t sent = 0;

    while (aBufLength > 0)
    {
        uint16_t remaining = kTxBufferSize - mTxLength;
        uint16_t tail;
        uint16_t sendLength = aBufLength;

        if (sendLength > remaining)
        {
            sendLength = remaining;
        }

        for (uint16_t i = 0; i < sendLength; i++)
        {
            tail            = (mTxHead + mTxLength) % kTxBufferSize;
            mTxBuffer[tail] = *aBuf++;
            aBufLength--;
            mTxLength++;
        }

        Send();

        sent += sendLength;

        if (aBufLength > 0)
        {
            // More to send, so flush what's waiting now
            vcError err = vcPlatUartFlush();

            if (err == VC_ERROR_NONE)
            {
                // Flush successful, reset the pointers
                SendDoneTask();
            }
            else
            {
                // Flush did not succeed, so abort here.
                break;
            }
        }
    }

    VC_CLI_UART_OUTPUT_UNLOCK();

    return sent;
}

void Uart::Send(void)
{
    VerifyOrExit(mSendLength == 0);

    if (mTxLength > kTxBufferSize - mTxHead)
    {
        mSendLength = kTxBufferSize - mTxHead;
    }
    else
    {
        mSendLength = mTxLength;
    }

    if (mSendLength > 0)
    {
#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
        /* duplicate the output to the debug uart */
        vcPlatDebugUart_write_bytes(reinterpret_cast<uint8_t *>(mTxBuffer + mTxHead), mSendLength);
#endif
        vcPlatUartSend(reinterpret_cast<uint8_t *>(mTxBuffer + mTxHead), mSendLength);
    }

exit:
    return;
}

extern "C" void vcPlatUartSendDone(void)
{
    static_cast<Uart *>(Server::sServer)->SendDoneTask();
}

void Uart::SendDoneTask(void)
{
    mTxHead = (mTxHead + mSendLength) % kTxBufferSize;
    mTxLength -= mSendLength;
    mSendLength = 0;

    Send();
}

} // namespace Cli
} // namespace vc
