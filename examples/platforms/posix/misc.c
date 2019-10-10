#include "platform-posix.h"

#include <setjmp.h>
#include <unistd.h>

#include <openvc/platform/misc.h>

#include "openvc-system.h"

extern jmp_buf gResetJump;

static vcPlatResetReason   sPlatResetReason = VC_PLAT_RESET_REASON_POWER_ON;
bool                       gPlatformPseudoResetWasRequested;
static vcPlatMcuPowerState gPlatMcuPowerState = VC_PLAT_MCU_POWER_STATE_ON;

void vcPlatReset(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);

#if OPENVC_PLATFORM_USE_PSEUDO_RESET
    gPlatformPseudoResetWasRequested = true;
    sPlatResetReason                 = VC_PLAT_RESET_REASON_SOFTWARE;
#else
    vcSysDeInit();
    platformUartRestore();

    longjmp(gResetJump, 1);
    assert(false);
#endif
}

vcPlatResetReason vcPlatGetResetReason(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);

    return sPlatResetReason;
}

void vcPlatWakeHost(void)
{
    // TODO: implement an operation to wake the host from sleep state.
}

vcError vcPlatSetMcuPowerState(vcInstance *aInstance, vcPlatMcuPowerState aState)
{
    vcError error = VC_ERROR_NONE;

    VC_UNUSED_VARIABLE(aInstance);

    switch (aState)
    {
    case VC_PLAT_MCU_POWER_STATE_ON:
    case VC_PLAT_MCU_POWER_STATE_LOW_POWER:
        gPlatMcuPowerState = aState;
        break;

    default:
        error = VC_ERROR_FAILED;
        break;
    }

    return error;
}

vcPlatMcuPowerState vcPlatGetMcuPowerState(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);

    return gPlatMcuPowerState;
}
