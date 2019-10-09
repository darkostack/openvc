#include "string.hpp"

namespace vc {

vcError StringBase::Write(char *aBuffer, uint16_t aSize, uint16_t &aLength, const char *aFormat, va_list aArgs)
{
    vcError error = VC_ERROR_NONE;
    int     len;

    len = vsnprintf(aBuffer + aLength, aSize - aLength, aFormat, aArgs);

    if (len < 0)
    {
        aLength    = 0;
        aBuffer[0] = 0;
        error      = VC_ERROR_INVALID_ARGS;
    }
    else if (len >= aSize - aLength)
    {
        aLength = aSize - 1;
        error   = VC_ERROR_NO_BUFS;
    }
    else
    {
        aLength += static_cast<uint16_t>(len);
    }

    return error;
};

} // namespace vc
