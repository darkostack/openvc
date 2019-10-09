#ifndef OPENVC_PLATFORM_LOGGING_H_
#define OPENVC_PLATFORM_LOGGING_H_

#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VC_LOG_LEVEL_NONE 0
#define VC_LOG_LEVEL_CRIT 1
#define VC_LOG_LEVEL_WARN 2
#define VC_LOG_LEVEL_NOTE 3
#define VC_LOG_LEVEL_INFO 4
#define VC_LOG_LEVEL_DEBG 5

typedef uint8_t vcLogLevel;

typedef enum vcLogRegion {
    VC_LOG_REGION_API      = 1,
    VC_LOG_REGION_MEM      = 2,
    VC_LOG_REGION_PLATFORM = 3,
    VC_LOG_REGION_CLI      = 4,
    VC_LOG_REGION_CORE     = 5,
    VC_LOG_REGION_UTIL     = 6,
} vcLogRegion;

void vcPlatLog(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, ...);

void vcPlatLogv(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, va_list ap);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_LOGGING_H_
