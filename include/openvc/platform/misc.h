#ifndef OPENVC_PLATFORM_MISC_H_
#define OPENVC_PLATFORM_MISC_H_

#include <stdint.h>

#include <openvc/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void vcPlatReset(vcInstance *aInstance);

typedef enum {
    VC_PLAT_RESET_REASON_POWER_ON = 0,
    VC_PLAT_RESET_REASON_EXTERNAL = 1,
    VC_PLAT_RESET_REASON_SOFTWARE = 2,
    VC_PLAT_RESET_REASON_FAULT    = 3,
    VC_PLAT_RESET_REASON_CRASH    = 4,
    VC_PLAT_RESET_REASON_ASSERT   = 5,
    VC_PLAT_RESET_REASON_OTHER    = 6,
    VC_PLAT_RESET_REASON_UNKNOWN  = 7,
    VC_PLAT_RESET_REASON_WATCHDOG = 8,
    VC_PLAT_RESET_REASON_COUNT,
} vcPlatResetReason;

vcPlatResetReason vcPlatGetResetReason(vcInstance *aInstance);

void vcPlatAssertFail(const char *aFilename, int aLineNumber);

void vcPlatWakeHost(void);

typedef enum {
    VC_PLAT_MCU_POWER_STATE_ON        = 0,
    VC_PLAT_MCU_POWER_STATE_LOW_POWER = 1,
    VC_PLAT_MCU_POWER_STATE_OFF       = 2,
} vcPlatMcuPowerState;

vcError vcPlatSetMcuPowerState(vcInstance *aInstance, vcPlatMcuPowerState aState);

vcPlatMcuPowerState vcPlatGetMcuPowerState(vcInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_MISC_H_
