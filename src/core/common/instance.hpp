#ifndef INSTANCE_HPP_
#define INSTANCE_HPP_

#include "openvc-core-config.h"

#include <stdbool.h>
#include <stdint.h>

#include <openvc/error.h>
#include <openvc/platform/logging.h>

#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
#include "crypto/mbedtls.hpp"
#endif
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
#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    static Instance *Init(void *aBuffer, size_t *aBufferSize);
#else
    static Instance &InitSingle(void);

    static Instance &Get(void);
#endif

    bool IsInitialized(void) const { return mIsInitialized; }

    void Reset(void);

    vcLogLevel GetLogLevel(void) const
#if OPENVC_CONFIG_DYNAMIC_LOG_LEVEL_ENABLE
    {
        return mLogLevel;
    }
#else
    {
        return static_cast<vcLogLevel>(OPENVC_CONFIG_LOG_LEVEL);
    }
#endif

#if OPENVC_CONFIG_DYNAMIC_LOG_LEVEL_ENABLE
    void SetLogLevel(vcLogLevel aLogLevel) { mLogLevel = aLogLevel; }
#endif

    void Finalize(void);

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    void HeapFree(void *aPointer) { mHeap.Free(aPointer); }

    void *HeapCAlloc(size_t aCount, size_t aSize) { return mHeap.CAlloc(aCount, aSize); }

    Utils::Heap &GetHeap(void) { return mHeap; }
#endif

    template <typename Type> inline Type &Get(void);

private:
    Instance(void);
    void AfterInit(void);

    TaskletScheduler    mTaskletScheduler;
    TimerMilliScheduler mTimerMilliScheduler;
#if OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE
    TimerMicroScheduler mTimerMicroScheduler;
#endif

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    Utils::Heap mHeap;
#endif
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    Crypto::MbedTls mMbedTls;
#endif

#if OPENVC_CONFIG_DYNAMIC_LOG_LEVEL_ENABLE
    vcLogLevel mLogLevel;
#endif

    bool mIsInitialized;
};

// Specializations of the `Get<Type>()` method.

template <> inline TaskletScheduler &Instance::Get(void)
{
    return mTaskletScheduler;
}

template <> inline TimerMilliScheduler &Instance::Get(void)
{
    return mTimerMilliScheduler;
}

#if OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE
template <> inline TimerMicroScheduler &Instance::Get(void)
{
    return mTimerMicroScheduler;
}
#endif

} // namespace vc

#endif // INSTANCE_HPP_
