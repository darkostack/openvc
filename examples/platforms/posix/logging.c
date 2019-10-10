#include "platform-posix.h"
#include <openvc-core-config.h>
#include <openvc/config.h>

#include <ctype.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <syslog.h>

#include <openvc/platform/logging.h>
#include <openvc/platform/toolchain.h>

#include "utils/code_utils.h"

// Macro to append content to end of the log string.

#define LOG_PRINTF(...)                                                                   \
    charsWritten = snprintf(&logString[offset], sizeof(logString) - offset, __VA_ARGS__); \
    vcEXPECT_ACTION(charsWritten >= 0, logString[offset] = 0);                            \
    offset += (unsigned int)charsWritten;                                                 \
    vcEXPECT_ACTION(offset < sizeof(logString), logString[sizeof(logString) - 1] = 0)

#if (OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
VC_TOOL_WEAK void vcPlatLog(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, ...)
{
    VC_UNUSED_VARIABLE(aLogLevel);
    VC_UNUSED_VARIABLE(aLogRegion);

    char         logString[512];
    unsigned int offset;
    int          charsWritten;
    va_list      args;

    offset = 0;

    LOG_PRINTF("[%d] ", gNodeId);

    va_start(args, aFormat);
    charsWritten = vsnprintf(&logString[offset], sizeof(logString) - offset, aFormat, args);
    va_end(args);

    vcEXPECT_ACTION(charsWritten >= 0, logString[offset] = 0);

exit:
    syslog(LOG_CRIT, "%s", logString);
}

#endif // #if (OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_PLATFORM_DEFINED)
