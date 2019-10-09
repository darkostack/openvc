#ifndef OPENVC_PLATFORM_TIME_H_
#define OPENVC_PLATFORM_TIME_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint64_t vcPlatTimeGet(void);

uint16_t vcPlatTimeGetXtalAccuracy(void);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_TIME_H_
