#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include "openvc-core-config.h"

#include "utils/wrap_stdbool.h"
#include "utils/wrap_stdint.h"

#include <openvc/error.h>
#include <openvc/platform/logging.h>

#include "crypto/mbedtls.hpp"
#include "utils/heap.hpp"

#include "common/code_utils.hpp"
#include "common/timer.hpp"

typedef struct vcInstance
{
} vcInstance;

namespace vc {

class Instance : public vcInstance
{
public:
    static Instance &InitSingle(void);

    static Instance &Get(void);

    bool IsInitialized(void) const { return mIsInitialized; }

    void Reset(void);

    TimerMilliScheduler &GetTimerMilliScheduler(void) { return mTimerMilliScheduler; }

#if OPENVC_CONFIG_ENABLE_PLATFORM_USEC_TIMER
    TimerMicroScheduler &GetTimerMicroScheduler(void) { return mTimerMicroScheduler; }
#endif

    TaskletScheduler &GetTaskletScheduler(void) { return mTaskletScheduler; }

#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    vcLogLevel GetDynamicLogLevel(void) const { return mLogLevel; }

    void SetDynamicLogLevel(vcLogLevel aLogLevel) { mLogLevel = aLogLevel; }
#endif

    vcLogLevel GetLogLevel(void) const
#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    {
        return GetDynamicLogLevel();
    }
#else
    {
        return static_cast<vcLogLevel>(OPENVC_CONFIG_LOG_LEVEL);
    }
#endif

    void Finalize(void);

#if !OPENVC_ENABLE_MULTIPLE_INSTANCES
    Utils::Heap &GetHeap(void) { return mHeap; }
#endif

    template <typename Type> inline Type &Get(void);

private:
    Instance(void);
    void AfterInit(void);

    TimerMilliScheduler mTimerMilliScheduler;
#if OPENVC_CONFIG_ENABLE_PLATFORM_USEC_TIMER
    TimerMicroScheduler mTimerMicroScheduler;
#endif
    TaskletScheduler mTaskletScheduler;

    Crypto::MbedTls mMbedTls;
    Utils::Heap     mHeap;

#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    vcLogLevel mLogLevel;
#endif

    bool mIsInitialized;
};

// Specializations of the `Get<Type>()` method.

template <> inline TaskletScheduler &Instance::Get(void)
{
    return GetTaskletScheduler();
}

} // namespace vc

#endif // INSTANCE_HPP_
