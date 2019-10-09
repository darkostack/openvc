#ifndef OPENVC_PLATFORM_RANDOM_H_
#define OPENVC_PLATFORM_RANDOM_H_

#include <stdint.h>

#include <openvc/error.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t vcPlatRandomGet(void);

vcError vcPlatRandomGetTrue(uint8_t *aOutput, uint16_t aOutputLength);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_RANDOM_H_
