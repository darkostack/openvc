#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "openvc-core-config.h"

#include <stddef.h>
#include <stdint.h>

#include <openvc/platform/alarm-micro.h>
#include <openvc/platform/alarm-milli.h>

#include "common/debug.hpp"
#include "common/locator.hpp"
#include "common/tasklet.hpp"
#include "common/time.hpp"

namespace vc {

class TimerMilliScheduler;

class Timer : public InstanceLocator, public OwnerLocator
{
    friend class TimerScheduler;

public:
    static const uint32_t kMaxDelay = (Time::kMaxDuration >> 1);

    typedef void (*Handler)(Timer &aTimer);

    Timer(Instance &aInstance, Handler aHandler, void *aOwner)
        : InstanceLocator(aInstance)
        , OwnerLocator(aOwner)
        , mHandler(aHandler)
        , mFireTime(0)
        , mNext(this)
    {
    }

    Time GetFireTime(void) const { return mFireTime; }

    bool IsRunning(void) const { return (mNext != this); }

protected:
    bool DoesFireBefore(const Timer &aTimer, Time aNow);

    void Fired(void) { mHandler(*this); }

    Handler mHandler;
    Time    mFireTime;
    Timer * mNext;
};

class TimerMilli : public Timer
{
public:
    TimerMilli(Instance &aInstance, Handler aHandler, void *aOwner)
        : Timer(aInstance, aHandler, aOwner)
    {
    }

    void Start(uint32_t aDelay);

    void StartAt(TimeMilli aStartTime, uint32_t aDelay);

    void Stop(void);

    static TimeMilli GetNow(void) { return TimeMilli(vcPlatAlarmMilliGetNow()); }
};

class TimerMilliContext : public TimerMilli
{
public:
    TimerMilliContext(Instance &aInstance, Handler aHandler, void *aContext)
        : TimerMilli(aInstance, aHandler, aContext)
        , mContext(aContext)
    {
    }

    void *GetContext(void) { return mContext; }

private:
    void *mContext;
};

class TimerScheduler : public InstanceLocator
{
    friend class Timer;

protected:
    struct AlarmApi
    {
        void (*AlarmStartAt)(vcInstance *aInstance, uint32_t aT0, uint32_t aDt);
        void (*AlarmStop)(vcInstance *aInstance);
        uint32_t (*AlarmGetNow)(void);
    };

    explicit TimerScheduler(Instance &aInstance)
        : InstanceLocator(aInstance)
        , mHead(NULL)
    {
    }

    void Add(Timer &aTimer, const AlarmApi &aAlarmApi);

    void Remove(Timer &aTimer, const AlarmApi &aAlarmApi);

    void ProcessTimers(const AlarmApi &aAlarmApi);

    void SetAlarm(const AlarmApi &aAlarmApi);

    Timer *mHead;
};

class TimerMilliScheduler : public TimerScheduler
{
public:
    explicit TimerMilliScheduler(Instance &aInstance)
        : TimerScheduler(aInstance)
    {
    }

    void Add(TimerMilli &aTimer) { TimerScheduler::Add(aTimer, sAlarmMilliApi); }

    void Remove(TimerMilli &aTimer) { TimerScheduler::Remove(aTimer, sAlarmMilliApi); }

    void ProcessTimers(void) { TimerScheduler::ProcessTimers(sAlarmMilliApi); }

private:
    static const AlarmApi sAlarmMilliApi;
};

#if OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE
class TimerMicroScheduler;

class TimerMicro : public Timer
{
public:
    TimerMicro(Instance &aInstance, Handler &aHandler, void *aOwner)
        : Timer(aInstance, aHandler, aOwner)
    {
    }

    void Start(uint32_t aDelay);

    void StartAt(TimeMicro aT0, uint32_t aDelay);

    void Stop(void);

    static TimeMicro GetNow(void) { return Time(vcPlatAlarmMicroGetNow()); }
};

class TimerMicroScheduler : public TimerScheduler
{
public:
    TimerMicroScheduler(Instance &aInstance)
        : TimerScheduler(aInstance)
    {
    }

    void Add(TimerMicro &aTimer) { TimerScheduler::Add(aTimer, sAlarmMicroApi); }

    void Remove(TimerMicro &aTimer) { TimerScheduler::Remove(aTimer, sAlarmMicroApi); }

    void ProcessTimers(void) { TimerScheduler::ProcessTimers(sAlarmMicroApi); }

private:
    static const AlarmApi sAlarmMicroApi;
};

#endif // OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE

} // namespace vc

#endif // TIMER_HPP_
