#include "platform-posix.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils/code_utils.h"

#include <openvc/platform/alarm-micro.h>
#include <openvc/platform/alarm-milli.h>

#define MS_PER_S 1000
#define NS_PER_US 1000
#define US_PER_MS 1000
#define US_PER_S 1000000

#define DEFAULT_TIMEOUT 10 // seconds

static bool     sIsMsRunning = false;
static uint32_t sMsAlarm     = 0;

static bool     sIsUsRunning = false;
static uint32_t sUsAlarm     = 0;

static uint32_t sSpeedUpFactor = 1;

void platformAlarmInit(uint32_t aSpeedUpFactor)
{
    sSpeedUpFactor = aSpeedUpFactor;
}

#if defined(CLOCK_MONOTONIC_RAW) || defined(CLOCK_MONOTONIC)
uint64_t platformGetNow(void)
{
    struct timespec now;
    int             err;

#ifdef CLOCK_MONOTONIC_RAW
    err = clock_gettime(CLOCK_MONOTONIC_RAW, &now);
#else
    err = clock_gettime(CLOCK_MONOTONIC, &now);
#endif

    assert(err == 0);

    return (uint64_t)now.tv_sec * sSpeedUpFactor * US_PER_S + (uint64_t)now.tv_nsec * sSpeedUpFactor / NS_PER_US;
}
#else
uint64_t platformGetNow(void)
{
    struct timeval tv;
    int            err;

    err = gettimeofday(&tv, NULL);

    assert(err == 0);

    return (uint64_t)tv.tv_sec * sSpeedUpFactor * US_PER_S + (uint64_t)tv.tv_usec * sSpeedUpFactor;
}
#endif // defined(CLOCK_MONOTONIC_RAW) || defined(CLOCK_MONOTONIC)

uint32_t vcPlatAlarmMilliGetNow(void)
{
    return (uint32_t)(platformGetNow() / US_PER_MS);
}

void vcPlatAlarmMilliStartAt(vcInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    VC_UNUSED_VARIABLE(aInstance);

    sMsAlarm     = aT0 + aDt;
    sIsMsRunning = true;
}

void vcPlatAlarmMilliStop(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);

    sIsMsRunning = false;
}

uint32_t vcPlatAlarmMicroGetNow(void)
{
    return (uint32_t)platformGetNow();
}

void vcPlatAlarmMicroStartAt(vcInstance *aInstance, uint32_t aT0, uint32_t aDt)
{
    VC_UNUSED_VARIABLE(aInstance);

    sUsAlarm     = aT0 + aDt;
    sIsUsRunning = true;
}

void vcPlatAlarmMicroStop(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);

    sIsUsRunning = false;
}

void platformAlarmUpdateTimeout(struct timeval *aTimeout)
{
    int32_t usRemaining = DEFAULT_TIMEOUT * US_PER_S;
    int32_t msRemaining = DEFAULT_TIMEOUT * MS_PER_S;

    if (aTimeout == NULL)
    {
        return;
    }

    if (sIsUsRunning)
    {
        usRemaining = (int32_t)(sUsAlarm - vcPlatAlarmMicroGetNow());
    }

    if (sIsMsRunning)
    {
        msRemaining = (int32_t)(sMsAlarm - vcPlatAlarmMilliGetNow());
    }

    if (usRemaining <= 0 || msRemaining <= 0)
    {
        aTimeout->tv_sec  = 0;
        aTimeout->tv_usec = 0;
    }
    else
    {
        int64_t remaining = ((int64_t)msRemaining) * US_PER_MS;

        if (usRemaining < remaining)
        {
            remaining = usRemaining;
        }

        remaining /= sSpeedUpFactor;

        if (remaining == 0)
        {
            remaining = 1;
        }

        aTimeout->tv_sec  = (time_t)remaining / US_PER_S;
        aTimeout->tv_usec = remaining % US_PER_S;
    }
}

void platformAlarmProcess(vcInstance *aInstance)
{
    int32_t remaining;

    if (sIsMsRunning)
    {
        remaining = (int32_t)(sMsAlarm - vcPlatAlarmMilliGetNow());

        if (remaining <= 0)
        {
            sIsMsRunning = false;

            vcPlatAlarmMilliFired(aInstance);
        }
    }

#if OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE

    if (sIsUsRunning)
    {
        remaining = (int32_t)(sUsAlarm - vcPlatAlarmMicroGetNow());

        if (remaining <= 0)
        {
            sIsUsRunning = false;

            vcPlatAlarmMicroFired(aInstance);
        }
    }

#endif // OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE
}

uint64_t vcPlatTimeGet(void)
{
    return platformGetNow();
}

#if OPENTHREAD_CONFIG_TIME_SYNC_ENABLE
uint16_t vcPlatTimeGetXtalAccuracy(void)
{
    return 0;
}
#endif
