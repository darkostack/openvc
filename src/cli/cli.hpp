#ifndef CLI_HPP_
#define CLI_HPP_

#include "openvc-core-config.h"

#include "cli_config.h"

#include <stdarg.h>

#include <openvc/cli.h>

#include "common/code_utils.hpp"
#include "common/instance.hpp"

namespace vc {
namespace Cli {

class Interpreter;
class Server;

struct Command
{
    const char *mName;
    void (Interpreter::*mCommand)(int argc, char *argv[]);
};

class Interpreter
{
public:
    explicit Interpreter(Instance *aInstance);

    void ProcessLine(char *aBuf, uint16_t aBufLength, Server &aServer);

    static vcError ParseLong(char *aString, long &aLong);

    static vcError ParseUnsignedLong(char *aString, unsigned long &aUnsignedLong);

    static int Hex2Bin(const char *aHex, uint8_t *aBin, uint16_t aBinLength);

    void AppendResult(vcError aError) const;

    void OutputBytes(const uint8_t *aBytes, uint8_t aLength) const;

    void SetUserCommands(const vcCliCommand *aCommands, uint8_t aLength);

private:
    enum
    {
        kMaxArgs          = 32,
        kMaxAutoAddresses = 8,
    };

    void ProcessHelp(int argc, char *argv[]);
    void ProcessVersion(int argc, char *argv[]);

    static Interpreter &GetOwner(OwnerLocator &aOwnerLocator);

    static const struct Command sCommands[];
    const vcCliCommand *        mUserCommands;
    uint8_t                     mUserCommandsLength;
    Server *                    mServer;

    Instance *mInstance;
};

} // namespace Cli
} // namespace vc

#endif // CLI_HPP_
