#ifndef CLI_SERVER_HPP_
#define CLI_SERVER_HPP_

#include "openvc-core-config.h"

#include "cli/cli.hpp"

namespace vc {
namespace Cli {

class Server
{
public:
    explicit Server(Instance *aInstance)
        : mInterpreter(aInstance)
    {
    }

    virtual int Output(const char *aBuf, uint16_t aBufLength)
    {
        VC_UNUSED_VARIABLE(aBuf);
        VC_UNUSED_VARIABLE(aBufLength);
        return -1;
    }

    int OutputFormat(const char *aFmt, ...);

    int OutputFormatV(const char *aFormat, va_list aArguments);

    Interpreter &GetInterpreter(void) { return mInterpreter; }

    static Server *sServer;

protected:
    enum
    {
        kMaxLineLength = OPENVC_CONFIG_CLI_MAX_LINE_LENGTH,
    };

    Interpreter mInterpreter;
};

} // namespace Cli
} // namespace vc

#endif // CLI_SERVER_HPP_
