#include "instance.hpp"

#include <openvc/platform/misc.h>

#include "common/logging.hpp"
#include "common/new.hpp"

namespace vc {

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
// Define the raw storage use for nesx instance (in single-instance case).
vcDEFINE_ALIGNED_VAR(gInstanceRaw, sizeof(Instance), uint64_t);
#endif

Instance::Instance(void)
    : mTaskletScheduler()
    , mTimerMilliScheduler(*this)
#if OPENVC_CONFIG_PLATFORM_USEC_TIMER_ENABLE
    , mTimerMicroScheduler(*this)
#endif
#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    , mHeap()
#endif
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    , mMbedTls()
#endif
#if OPENVC_CONFIG_DYNAMIC_LOG_LEVEL_ENABLE
    , mLogLevel(static_cast<vcLogLevel>(OPENVC_CONFIG_LOG_LEVEL))
#endif
    , mIsInitialized(false)
{
}

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
Instance &Instance::InitSingle(void)
{
    Instance *instance = &Get();

    VerifyOrExit(instance->mIsInitialized == false);

    instance = new (&gInstanceRaw) Instance();

    instance->AfterInit();

exit:
    return *instance;
}

Instance &Instance::Get(void)
{
    void *instance = &gInstanceRaw;
    return *static_cast<Instance *>(instance);
}
#else
Instance *Instance::Init(void *aBuffer, size_t *aBufferSize)
{
    Instance *instance = NULL;

    VerifyOrExit(aBufferSize != NULL);

    // Make sure the input buffer is big enough
    VerifyOrExit(sizeof(Instance) <= *aBufferSize, *aBufferSize = sizeof(Instance));

    VerifyOrExit(aBuffer != NULL);

    instance = new (aBuffer) Instance();

    instance->AfterInit();

exit:
    return instance;
}
#endif

void Instance::Reset(void)
{
    vcPlatReset(this);
}

void Instance::AfterInit(void)
{
    mIsInitialized = true;
}

void Instance::Finalize(void)
{
    VerifyOrExit(mIsInitialized == true);

    mIsInitialized = false;

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    this->~Instance();
#endif

exit:
    return;
}

} // namespace vc
