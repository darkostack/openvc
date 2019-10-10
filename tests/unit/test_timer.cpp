#include "test_platform.h"

#include "common/code_utils.hpp"
#include "common/debug.hpp"
#include "common/instance.hpp"
#include "common/timer.hpp"

enum
{
    kCallCountIndexAlarmStop = 0,
    kCallCountIndexAlarmStart,
    kCallCountIndexTimerHandler,
    kCallCountIndexMax
};

uint32_t sNow;
uint32_t sPlatT0;
uint32_t sPlatDt;
bool     sTimerOn;
uint32_t sCallCount[kCallCountIndexMax];

void testTimerAlarmStop(vcInstance *)
{
    sTimerOn = false;
    sCallCount[kCallCountIndexAlarmStop]++;
}

void testTimerAlarmStartAt(vcInstance *, uint32_t aT0, uint32_t aDt)
{
    sTimerOn = true;
    sCallCount[kCallCountIndexAlarmStart]++;
    sPlatT0 = aT0;
    sPlatDt = aDt;
}

uint32_t testTimerAlarmGetNow(void)
{
    return sNow;
}

void InitTestTimer(void)
{
    gTestPlatAlarmStop    = testTimerAlarmStop;
    gTestPlatAlarmStartAt = testTimerAlarmStartAt;
    gTestPlatAlarmGetNow  = testTimerAlarmGetNow;
}

void InitCounters(void)
{
    memset(sCallCount, 0, sizeof(sCallCount));
}

/**
 * `TestTimer` sub-classes `vc::TimerMilli` and provides a handler and a counter
 * to keep track of number of times timer gets fired.
 */
class TestTimer : public vc::TimerMilli
{
public:
    TestTimer(vc::Instance &aInstance)
        : vc::TimerMilli(aInstance, TestTimer::HandleTimerFired, NULL)
        , mFiredCounter(0)
    {
    }

    static void HandleTimerFired(vc::Timer &aTimer) { static_cast<TestTimer &>(aTimer).HandleTimerFired(); }

    void HandleTimerFired(void)
    {
        sCallCount[kCallCountIndexTimerHandler]++;
        mFiredCounter++;
    }

    uint32_t GetFiredCounter(void) { return mFiredCounter; }

    void ResetFiredCounter(void) { mFiredCounter = 0; }

private:
    uint32_t mFiredCounter; //< Number of times timer has been fired so far
};

/**
 * Test the TimerScheduler's behavior of one time started and fired.
 */
int TestOneTimer(void)
{
    const uint32_t kTimeT0        = 1000;
    const uint32_t kTimerInterval = 10;
    vc::Instance * instance       = testInitInstance();
    TestTimer      timer(*instance);

    // Test one Timer basic operation.

    InitTestTimer();
    InitCounters();

    printf("TestOneTimer() ");

    sNow = kTimeT0;
    timer.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == 1000 && sPlatDt == 10, "TestOneTimer: Start Params Failed.\n");
    VerifyOrQuit(timer.IsRunning(), "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestOneTimer: Platform Timer State Failed.\n");

    sNow += kTimerInterval;

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(timer.IsRunning() == false, "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestOneTimer: Platform Timer State Failed.\n");

    // Test one Timer that spans the 32-bit wrap.

    InitCounters();

    sNow = 0 - (kTimerInterval - 2); // now: 4294967288
    timer.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == 0 - (kTimerInterval - 2) && sPlatDt == 10, "TestOneTimer: Start Params Failed.\n");
    VerifyOrQuit(timer.IsRunning(), "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestOneTimer: Platform Timer State Failed.\n");

    sNow += kTimerInterval; // now: 2

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(timer.IsRunning() == false, "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestOneTimer: Platform Timer State Failed.\n");
    VerifyOrQuit(sNow == 2, "TestOneTimer: Timer Now Failed.\n");

    // Test one Timer that is late by several msec

    InitCounters();

    sNow = kTimeT0;
    timer.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == 1000 && sPlatDt == 10, "TestOneTimer: Start Params Failed.\n");
    VerifyOrQuit(timer.IsRunning(), "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestOneTimer: Platform Timer State Failed.\n");

    sNow += kTimerInterval + 5;

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(timer.IsRunning() == false, "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestOneTimer: Platform Timer State Failed.\n");

    // Test one Timer that is early by several msec

    InitCounters();

    sNow = kTimeT0;
    timer.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == 1000 && sPlatDt == 10, "TestOneTimer: Start Params Failed.\n");
    VerifyOrQuit(timer.IsRunning(), "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestOneTimer: Platform Timer State Failed.\n");

    sNow += kTimerInterval - 2;

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 2, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(timer.IsRunning() == true, "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == true, "TestOneTimer: Platform Timer State Failed.\n");

    sNow += kTimerInterval;

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 2, "TestOneTimer: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestOneTimer: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestOneTimer: Handler CallCount Failed.\n");
    VerifyOrQuit(timer.IsRunning() == false, "TestOneTimer: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestOneTimer: Platform Timer State Failed.\n");

    printf("--> PASSED\n");

    testFreeInstance(instance);

    return 0;
}

/**
 * Test the TimerScheduler's behavior of two timers started and fired.
 */
int TestTwoTimers(void)
{
    const uint32_t kTimeT0        = 1000;
    const uint32_t kTimerInterval = 10;
    vc::Instance * instance       = testInitInstance();
    TestTimer      timer1(*instance);
    TestTimer      timer2(*instance);

    InitTestTimer();

    printf("TestTwoTimers() ");

    // Test when second timer stars at the fire time of first timer (before alarm callback).

    InitCounters();

    sNow = kTimeT0;
    timer1.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == kTimeT0 && sPlatDt == kTimerInterval, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning(), "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestTwoTimers: Platform Timer State Failed.\n");

    sNow += kTimerInterval;

    timer2.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == kTimeT0 && sPlatDt == kTimerInterval, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestTwoTimers: Platform Timer State Failed.\n");

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 2, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer1.GetFiredCounter() == 1, "TestTwoTimers: Fire Counter failed.\n");
    VerifyOrQuit(sPlatT0 == sNow && sPlatDt == kTimerInterval, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == true, "TestTwoTimers: Platform Timer State Failed.\n");

    sNow += kTimerInterval;
    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 2, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 2, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer2.GetFiredCounter() == 1, "TestTwoTimers: Fire Counter failed.\n");
    VerifyOrQuit(timer1.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestTwoTimers: Platform Timer State Failed.\n");

    // Test when second timer starts at the fire time of first timer
    // (before vcPlatAlarmMilliFired()) and its fire time
    // is before the first timer.
    // Ensure that the second timer handler is invoked before the first one.

    InitCounters();
    timer1.ResetFiredCounter();
    timer2.ResetFiredCounter();

    sNow = kTimeT0;
    timer1.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == kTimeT0 && sPlatDt == kTimerInterval, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning(), "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestTwoTimers: Platform Timer State Failed.\n");

    sNow += kTimerInterval;

    timer2.StartAt(vc::TimeMilli(kTimeT0), kTimerInterval - 2); // Timer 2 is even before timer 1

    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer1.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestTwoTimers: Platform Timer State Failed.\n");

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer2.GetFiredCounter() == 1, "TestTwoTimers: Fire Counter failed.\n");
    VerifyOrQuit(sPlatT0 == sNow && sPlatDt == 0, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == true, "TestTwoTimers: Platform Timer State Failed.\n");

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 2, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer1.GetFiredCounter() == 1, "TestTwoTimers: Fire Counter failed.\n");
    VerifyOrQuit(timer1.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestTwoTimers: Platform Timer State Failed.\n");

    // Timer 1 fire callback is late by some ticks/ms,
    // and second timer is scheduled (before call to vcPlatAlarmMilliFired)
    // with a maximum interval.
    // This is to test (corner-case) scenario where the fire time of two
    // timers spanning over the maximum interval.

    InitCounters();
    timer1.ResetFiredCounter();
    timer2.ResetFiredCounter();

    sNow = kTimeT0;
    timer1.Start(kTimerInterval);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(sPlatT0 == kTimeT0 && sPlatDt == kTimerInterval, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning(), "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestTwoTimers: Platform Timer State Failed.\n");

    sNow += kTimerInterval + 5;

    timer2.Start(vc::Timer::kMaxDelay);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 1, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 0, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer1.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn, "TestTwoTimers: Platform Timer State Failed.\n");

    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 2, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 0, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 1, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer1.GetFiredCounter() == 1, "TestTwoTimers: Fire Counter failed.\n");
    VerifyOrQuit(sPlatT0 == sNow, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(sPlatDt == vc::Timer::kMaxDelay, "TestTwoTimers: Start Params Failed.\n");
    VerifyOrQuit(timer1.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == true, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == true, "TestTwoTimers: Platform Timer State Failed.\n");

    sNow += vc::Timer::kMaxDelay;
    vcPlatAlarmMilliFired(instance);

    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStart] == 2, "TestTwoTimers: Start CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexAlarmStop] == 1, "TestTwoTimers: Stop CallCount Failed.\n");
    VerifyOrQuit(sCallCount[kCallCountIndexTimerHandler] == 2, "TestTwoTimers: Handler CallCount Failed.\n");
    VerifyOrQuit(timer2.GetFiredCounter() == 1, "TestTwoTimers: Fire Counter failed.\n");
    VerifyOrQuit(timer1.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(timer2.IsRunning() == false, "TestTwoTimers: Timer Running Failed.\n");
    VerifyOrQuit(sTimerOn == false, "TestTwoTimers: Platform Timer State Failed.\n");

    printf("--> PASSED\n");

    testFreeInstance(instance);

    return 0;
}

#ifdef ENABLE_TEST_MAIN
int main(void)
{
    TestOneTimer();
    TestTwoTimers();
    printf("All tests passed\n");
    return 0;
}
#endif
