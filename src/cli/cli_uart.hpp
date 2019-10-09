#ifndef CLI_UART_HPP_
#define CLI_UART_HPP_

#include "openvc-core-config.h"

#include "cli/cli.hpp"
#include "cli/cli_server.hpp"
#include "common/instance.hpp"
#include "common/tasklet.hpp"

namespace vc {
namespace Cli {

class Uart : public Server
{
public:
    Uart(Instance *aInstance);

    virtual int Output(const char *aBuf, uint16_t aBufLength);

    virtual int OutputFormat(const char *aFmt, ...);

    int OutputFormatV(const char *aFmt, va_list aAp);

    Interpreter &GetInterpreter(void) { return mInterpreter; }

    void ReceiveTask(const uint8_t *aBuf, uint16_t aBufLength);

    void SendDoneTask(void);

    static Uart *sUartServer;

private:
    enum
    {
        kRxBufferSize  = OPENVC_CONFIG_CLI_UART_RX_BUFFER_SIZE,
        kTxBufferSize  = OPENVC_CONFIG_CLI_UART_TX_BUFFER_SIZE,
        kMaxLineLength = OPENVC_CONFIG_CLI_MAX_LINE_LENGTH,
    };

    vcError ProcessCommand(void);
    void    Send(void);

    char     mRxBuffer[kRxBufferSize];
    uint16_t mRxLength;

    char     mTxBuffer[kTxBufferSize];
    uint16_t mTxHead;
    uint16_t mTxLength;

    uint16_t mSendLength;

    Interpreter mInterpreter;

    friend class Interpreter;
};

} // namespace Cli
} // namespace vc

#endif // CLI_UART_HPP_
