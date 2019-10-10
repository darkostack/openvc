#include "parse_cmdline.hpp"

#include "common/code_utils.hpp"

namespace vc {
namespace Utils {

static bool IsSpaceOrNewLine(char aChar)
{
    return (aChar == ' ') || (aChar == '\t') || (aChar == '\r') || (aChar == '\n');
}

vcError CmdLineParser::ParseCmd(char *aString, uint8_t &aArgc, char *aArgv[], uint8_t aArgcMax)
{
    vcError error = VC_ERROR_NONE;
    char *  cmd;

    aArgc = 0;

    for (cmd = aString; IsSpaceOrNewLine(*cmd) && *cmd; cmd++)
        ;

    if (*cmd)
    {
        aArgv[aArgc++] = cmd++; // the first argument
    }

    for (; *cmd; cmd++)
    {
        if (IsSpaceOrNewLine(*cmd))
        {
            *cmd = '\0';
        }
        else if (*(cmd - 1) == '\0')
        {
            VerifyOrExit(aArgc < aArgcMax, error = VC_ERROR_INVALID_ARGS);
            aArgv[aArgc++] = cmd;
        }
    }

exit:
    return error;
}

} // namespace Utils
} // namespace vc
