#include "platform-posix.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils/code_utils.h"

// linux microsecond timer
#if __linux__

#include <signal.h>
#include <time.h>

#ifndef OPENVC_CONFIG_MICRO_TIMER_SIGNAL
#define OPENVC_CONFIG_MICRO_TIMER_SIGNAL SIGRTMIN
#endif

timer_t sMicroTimer;
#endif // __linux__

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

#if __linux__
static void microTimerHandler(int aSignal, siginfo_t *aSignalInfo, void *aUserContext)
{
    assert(aSignal == OPENVC_CONFIG_MICRO_TIMER_SIGNAL);
    assert(aSignalInfo->si_value.sival_ptr == &sMicroTimer);
    (void)aUserContext;
}
#endif

void platformAlarmInit(uint32_t aSpeedUpFactor)
{
    sSpeedUpFactor = aSpeedUpFactor;

#if __linux__
    {
        struct sigaction sa;

        sa.sa_flags     = SA_SIGINFO;
        sa.sa_sigaction = microTimerHandler;
        sigemptyset(&sa.sa_mask);

        if (sigaction(OPENVC_CONFIG_MICRO_TIMER_SIGNAL, &sa, NULL) == -1)
        {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        struct sigevent sev;

        sev.sigev_notify          = SIGEV_SIGNAL;
        sev.sigev_signo           = OPENVC_CONFIG_MICRO_TIMER_SIGNAL;
        sev.sigev_value.sival_ptr = &sMicroTimer;

        if (-1 == timer_create(CLOCK_REALTIME, &sev, &sMicroTimer))
        {
            perror("timer_create");
            exit(EXIT_FAILURE);
        }
    }
#endif
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

#if __linux__
    {
        struct itimerspec its;
        uint32_t          diff = sUsAlarm - vcPlatAlarmMicroGetNow();

        its.it_value.tv_sec  = diff / US_PER_S;
        its.it_value.tv_nsec = (diff % US_PER_S) * NS_PER_US;

        its.it_interval.tv_sec  = 0;
        its.it_interval.tv_nsec = 0;

        if (-1 == timer_settime(sMicroTimer, 0, &its, NULL))
        {
            perror("vcPlatAlarmMicroStartAt timer_settime()");
            exit(EXIT_FAILURE);
        }
    }
#endif // __linux__
}

void vcPlatAlarmMicroStop(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);

    sIsUsRunning = false;

#if __linux__
    {
        struct itimerspec its = {{0, 0}, {0, 0}};

        if (-1 == timer_settime(sMicroTimer, 0, &its, NULL))
        {
            perror("vcPlatAlarmMicroStop timer_settime()");
            exit(EXIT_FAILURE);
        }
    }
#endif // __linux__
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
