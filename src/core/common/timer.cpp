#include "timer.hpp"

#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "common/instance.hpp"
#include "common/locator-getters.hpp"
#include "common/logging.hpp"

namespace vc {

const TimerScheduler::AlarmApi TimerMilliScheduler::sAlarmMilliApi = {
    &vcPlatAlarmMilliStartAt,
    &vcPlatAlarmMilliStop,
    &vcPlatAlarmMilliGetNow
};

bool Timer::DoesFireBefore(const Timer &aSecondTimer, Time aNow)
{
    bool retval;
    bool isBeforeNow = (GetFireTime() < aNow);

    // Check if one timer is before `now` and the other one is not.
    if ((aSecondTimer.GetFireTime() < aNow) != isBeforeNow)
    {
        // One timer is before `now` and the other one is not, so if this timer's fire time is before `now` then
        // the second fire time would be after `now` and this timer would fire before the second timer.

        retval = isBeforeNow;
    }
    else
    {
        // Both timers are before `now` or both are after `now`. Either way the difference is guaranteed to be less
        // than `kMaxDt` so we can safely compare the fire times directly.

        retval = GetFireTime() < aSecondTimer.GetFireTime();
    }

    return retval;
}

void TimerMilli::Start(uint32_t aDelay)
{
    StartAt(GetNow(), aDelay);
}

void TimerMilli::StartAt(TimeMilli aStartTime, uint32_t aDelay)
{
    assert(aDelay <= kMaxDelay);
    mFireTime = aStartTime + aDelay;
    Get<TimerMilliScheduler>().Add(*this);
}

void TimerMilli::Stop(void)
{
    Get<TimerMilliScheduler>().Remove(*this);
}

void TimerScheduler::Add(Timer &aTimer, const AlarmApi &aAlarmApi)
{
    Remove(aTimer, aAlarmApi);

    if (mHead == NULL)
    {
        mHead        = &aTimer;
        aTimer.mNext = NULL;
        SetAlarm(aAlarmApi);
    }
    else
    {
        Timer *prev = NULL;
        Timer *cur;

        for (cur = mHead; cur; cur = cur->mNext)
        {
            Time now(aAlarmApi.AlarmGetNow());

            if (aTimer.DoesFireBefore(*cur, now))
            {
                if (prev)
                {
                    aTimer.mNext = cur;
                    prev->mNext  = &aTimer;
                }
                else
                {
                    aTimer.mNext = mHead;
                    mHead        = &aTimer;
                    SetAlarm(aAlarmApi);
                }

                break;
            }

            prev = cur;
        }

        if (cur == NULL)
        {
            prev->mNext  = &aTimer;
            aTimer.mNext = NULL;
        }
    }
}

void TimerScheduler::Remove(Timer &aTimer, const AlarmApi &aAlarmApi)
{
    VerifyOrExit(aTimer.mNext != &aTimer);

    if (mHead == &aTimer)
    {
        mHead = aTimer.mNext;
        SetAlarm(aAlarmApi);
    }
    else
    {
        for (Timer *cur = mHead; cur; cur = cur->mNext)
        {
            if (cur->mNext == &aTimer)
            {
                cur->mNext = aTimer.mNext;
                break;
            }
        }
    }

    aTimer.mNext = &aTimer;

exit:
    return;
}

void TimerScheduler::SetAlarm(const AlarmApi &aAlarmApi)
{
    if (mHead == NULL)
    {
        aAlarmApi.AlarmStop(&GetInstance());
    }
    else
    {
        Time     now(aAlarmApi.AlarmGetNow());
        uint32_t remaining;

        remaining = (now < mHead->mFireTime) ? (mHead->mFireTime - now) : 0;

        aAlarmApi.AlarmStartAt(&GetInstance(), now.GetValue(), remaining);
    }
}

void TimerScheduler::ProcessTimers(const AlarmApi &aAlarmApi)
{
    Timer *timer = mHead;

    if (timer)
    {
        Time now(aAlarmApi.AlarmGetNow());

        if (now >= timer->mFireTime)
        {
            Remove(*timer, aAlarmApi); // `Remove()` will `SetAlarm` for next timer if there is any.
            timer->Fired();
            ExitNow();
        }
    }

    SetAlarm(aAlarmApi);

exit:
    return;
}

extern "C" void vcPlatAlarmMilliFired(vcInstance *aInstance)
{
    Instance *instance = static_cast<Instance *>(aInstance);

    VerifyOrExit(vcInstanceIsInitialized(aInstance));
    instance->Get<TimerMilliScheduler>().ProcessTimers();

exit:
    return;
}

#if OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE
const TimerScheduler::AlarmApi TimerMicroScheduler::sAlarmMicroApi = {
    &vcPlatAlarmMicroStartAt,
    &vcPlatAlarmMicroStop,
    &vcPlatAlarmMicroGetNow
};

void TimerMicro::Start(uint32_t aDelay)
{
    StartAt(GetNow(), aDelay);
}

void TimerMicro::StartAt(TimeMicro aStartTime, uint32_t aDelay)
{
    assert(aDelay <= kMaxDelay);
    mFireTime = aStartTime + aDelay;
    Get<TimerMicroScheduler>().Add(*this);
}

void TimerMicro::Stop(void)
{
    Get<TimerMicroScheduler>().Remove(*this);
}

extern "C" void vcPlatAlarmMicroFired(vcInstance *aInstance)
{
    Instance *instance = static_cast<Instance *>(aInstance);

    VerifyOrExit(vcInstanceIsInitialized(aInstance));
    instance->Get<TimerMicroScheduler>().ProcessTimers();

exit:
    return;
}
#endif // OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE


} // namespace vc
