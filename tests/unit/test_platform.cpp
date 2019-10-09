#include "test_platform.h"

#include <sys/time.h>

bool                 gTestPlatAlarmSet     = false;
uint32_t             gTestPlatAlarmNext    = 0;
testPlatAlarmStop    gTestPlatAlarmStop    = NULL;
testPlatAlarmStartAt gTestPlatAlarmStartAt = NULL;
testPlatAlarmGetNow  gTestPlatAlarmGetNow  = NULL;

void testPlatResetToDefaults(void)
{
    gTestPlatAlarmSet     = false;
    gTestPlatAlarmNext    = 0;
    gTestPlatAlarmStop    = NULL;
    gTestPlatAlarmStartAt = NULL;
    gTestPlatAlarmGetNow  = NULL;
}

vc::Instance *testInitInstance(void)
{
    vcInstance *instance;

    instance = vcInstanceInitSingle();

    return static_cast<vc::Instance *>(instance);
}

void testFreeInstance(vcInstance *aInstance)
{
    vcInstanceFinalize(aInstance);
}

//
// Alarm
//
void vcPlatAlarmMilliStop(vcInstance *aInstance)
{
    if (gTestPlatAlarmStop)
    {
        gTestPlatAlarmStop(aInstance);
    }
    else
    {
        gTestPlatAlarmSet = false;
    }
}

void vcPlatAlarmMilliStartAt(vcInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    if (gTestPlatAlarmStartAt)
    {
        gTestPlatAlarmStartAt(aInstance, aT0, aDt);
    }
    else
    {
        gTestPlatAlarmSet  = true;
        gTestPlatAlarmNext = aT0 + aDt;
    }
}

uint32_t vcPlatAlarmMilliGetNow(void)
{
    if (gTestPlatAlarmGetNow)
    {
        return gTestPlatAlarmGetNow();
    }
    else
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (uint32_t)((tv.tv_sec * 1000) + (tv.tv_usec / 1000) + 123456);
    }
}

void vcPlatAlarmMicroStop(vcInstance *aInstance)
{
    if (gTestPlatAlarmStop)
    {
        gTestPlatAlarmStop(aInstance);
    }
    else
    {
        gTestPlatAlarmSet = false;
    }
}

void vcPlatAlarmMicroStartAt(vcInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    if (gTestPlatAlarmStartAt)
    {
        gTestPlatAlarmStartAt(aInstance, aT0, aDt);
    }
    else
    {
        gTestPlatAlarmSet  = true;
        gTestPlatAlarmNext = aT0 + aDt;
    }
}

uint32_t vcPlatAlarmMicroGetNow(void)
{
    if (gTestPlatAlarmGetNow)
    {
        return gTestPlatAlarmGetNow();
    }
    else
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (uint32_t)((tv.tv_sec * 1000000) + tv.tv_usec + 123456);
    }
}

//
// Misc
//

void vcPlatReset(vcInstance *aInstance)
{
    (void)aInstance;
}

vcPlatResetReason vcPlatGetResetReason(vcInstance *aInstance)
{
    (void)aInstance;
    return VC_PLAT_RESET_REASON_POWER_ON;
}

//
// Logging
//
void vcPlatLog(vcLogLevel, vcLogRegion, const char *, ...)
{
}
