#ifndef PARSE_CMDLINE_HPP_
#define PARSE_CMDLINE_HPP_

#include <stdint.h>
#include <openvc/error.h>

namespace vc {
namespace Utils {

class CmdLineParser
{
public:
    static vcError ParseCmd(char *aString, uint8_t &aArgc, char *aArgv[], uint8_t aArgcMax);
};

} // namespace Utils
} // namespace vc

#endif /* PARSE_CMDLINE_HPP_ */
