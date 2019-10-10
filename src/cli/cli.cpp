#include "cli.hpp"

#include <stdio.h>
#include <stdlib.h>
#include "utils/parse_cmdline.hpp"
#include "utils/wrap_string.h"

#include "cli_uart.hpp"

#include "common/encoding.hpp"

using vc::Encoding::BigEndian::HostSwap16;
using vc::Encoding::BigEndian::HostSwap32;

namespace vc {
namespace Cli {

const struct Command Interpreter::sCommands[] = {
    {"help", &Interpreter::ProcessHelp},
    {"version", &Interpreter::ProcessVersion},
};

Interpreter::Interpreter(Instance *aInstance)
    : mUserCommands(NULL)
    , mUserCommandsLength(0)
    , mServer(NULL)
    , mInstance(aInstance)
{
    VC_UNUSED_VARIABLE(mInstance);
}

int Interpreter::Hex2Bin(const char *aHex, uint8_t *aBin, uint16_t aBinLength)
{
    size_t      hexLength = strlen(aHex);
    const char *hexEnd    = aHex + hexLength;
    uint8_t *   cur       = aBin;
    uint8_t     numChars  = hexLength & 1;
    uint8_t     byte      = 0;
    int         rval;

    VerifyOrExit((hexLength + 1) / 2 <= aBinLength, rval = -1);

    while (aHex < hexEnd)
    {
        if ('A' <= *aHex && *aHex <= 'F')
        {
            byte |= 10 + (*aHex - 'A');
        }
        else if ('a' <= *aHex && *aHex <= 'f')
        {
            byte |= 10 + (*aHex - 'a');
        }
        else if ('0' <= *aHex && *aHex <= '9')
        {
            byte |= *aHex - '0';
        }
        else
        {
            ExitNow(rval = -1);
        }

        aHex++;
        numChars++;

        if (numChars >= 2)
        {
            numChars = 0;
            *cur++   = byte;
            byte     = 0;
        }
        else
        {
            byte <<= 4;
        }
    }

    rval = static_cast<int>(cur - aBin);

exit:
    return rval;
}

void Interpreter::AppendResult(vcError aError) const
{
    if (aError == VC_ERROR_NONE)
    {
        mServer->OutputFormat("Done\r\n");
    }
    else
    {
        mServer->OutputFormat("Error %d: %s\r\n", aError, vcErrorToString(aError));
    }
}

void Interpreter::OutputBytes(const uint8_t *aBytes, uint8_t aLength) const
{
    for (int i = 0; i < aLength; i++)
    {
        mServer->OutputFormat("%02x", aBytes[i]);
    }
}

vcError Interpreter::ParseLong(char *aString, long &aLong)
{
    char *endptr;
    aLong = strtol(aString, &endptr, 0);
    return (*endptr == '\0') ? VC_ERROR_NONE : VC_ERROR_PARSE;
}

vcError Interpreter::ParseUnsignedLong(char *aString, unsigned long &aUnsignedLong)
{
    char *endptr;
    aUnsignedLong = strtoul(aString, &endptr, 0);
    return (*endptr == '\0') ? VC_ERROR_NONE : VC_ERROR_PARSE;
}

void Interpreter::ProcessHelp(int argc, char *argv[])
{
    VC_UNUSED_VARIABLE(argc);
    VC_UNUSED_VARIABLE(argv);

    for (unsigned int i = 0; i < VC_ARRAY_LENGTH(sCommands); i++)
    {
        mServer->OutputFormat("%s\r\n", sCommands[i].mName);
    }

    for (unsigned int i = 0; i < mUserCommandsLength; i++)
    {
        mServer->OutputFormat("%s\r\n", mUserCommands[i].mName);
    }
}

void Interpreter::ProcessVersion(int argc, char *argv[])
{
    VC_UNUSED_VARIABLE(argc);
    VC_UNUSED_VARIABLE(argv);

    const char *version = vcGetVersionString();
    mServer->OutputFormat("%s\r\n", (const char *)version);
    AppendResult(VC_ERROR_NONE);
}

void Interpreter::ProcessLine(char *aBuf, uint16_t aBufLength, Server &aServer)
{
    char *  argv[kMaxArgs];
    char *  cmd;
    uint8_t argc = 0, i = 0;

    mServer = &aServer;

    VerifyOrExit(aBuf != NULL && strnlen(aBuf, aBufLength + 1) <= aBufLength);

    VerifyOrExit(Utils::CmdLineParser::ParseCmd(aBuf, argc, argv, kMaxArgs) == VC_ERROR_NONE,
                 mServer->OutputFormat("Error: too many args (max %d)\r\n", kMaxArgs));
    VerifyOrExit(argc >= 1, mServer->OutputFormat("Error: no given command.\r\n"));

    cmd = argv[0];

    for (i = 0; i < VC_ARRAY_LENGTH(sCommands); i++)
    {
        if (strcmp(cmd, sCommands[i].mName) == 0)
        {
            (this->*sCommands[i].mCommand)(argc - 1, &argv[1]);
            break;
        }
    }

    // Check user defined commands if built-in command
    // has not been found
    if (i == VC_ARRAY_LENGTH(sCommands))
    {
        for (i = 0; i < mUserCommandsLength; i++)
        {
            if (strcmp(cmd, mUserCommands[i].mName) == 0)
            {
                mUserCommands[i].mCommand(argc - 1, &argv[1]);
                break;
            }
        }

        if (i == mUserCommandsLength)
        {
            AppendResult(VC_ERROR_PARSE);
        }
    }

exit:
    return;
}

void Interpreter::SetUserCommands(const vcCliCommand *aCommands, uint8_t aLength)
{
    mUserCommands       = aCommands;
    mUserCommandsLength = aLength;
}

Interpreter &Interpreter::GetOwner(OwnerLocator &aOwnerLocator)
{
#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    Interpreter &interpreter = Uart::sUartServer->GetInterpreter();
#else
    VC_UNUSED_VARIABLE(aOwnerLocator);

    Interpreter &interpreter = Server::sServer->GetInterpreter();
#endif
    return interpreter;
}

extern "C" void vcCliSetUserCommands(const vcCliCommand *aUserCommands, uint8_t aLength)
{
    Server::sServer->GetInterpreter().SetUserCommands(aUserCommands, aLength);
}

extern "C" void vcCliOutputBytes(const uint8_t *aBytes, uint8_t aLength)
{
    Server::sServer->GetInterpreter().OutputBytes(aBytes, aLength);
}

extern "C" void vcCliOutputFormat(const char *aFmt, ...)
{
    va_list aAp;
    va_start(aAp, aFmt);
    Server::sServer->OutputFormatV(aFmt, aAp);
    va_end(aAp);
}

extern "C" void vcCliOutput(const char *aString, uint16_t aLength)
{
    Server::sServer->Output(aString, aLength);
}

extern "C" void vcCliAppendResult(vcError aError)
{
    Server::sServer->GetInterpreter().AppendResult(aError);
}

extern "C" void vcCliPlatLogv(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, va_list aArgs)
{
    VC_UNUSED_VARIABLE(aLogLevel);
    VC_UNUSED_VARIABLE(aLogRegion);

    VerifyOrExit(Server::sServer != NULL);

    Server::sServer->OutputFormatV(aFormat, aArgs);
    Server::sServer->OutputFormat("\r\n");

exit:
    return;
}

} // namespace Cli
} // namespace vc
