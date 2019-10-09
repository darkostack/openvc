#include "instance.hpp"

#include <openvc/platform/misc.h>

#include "common/logging.hpp"
#include "common/new.hpp"

namespace vc {

// Define the raw storage use for nesx instance (in single-instance case).
static vcDEFINE_ALIGNED_VAR(sInstanceRaw, sizeof(Instance), uint64_t);

Instance::Instance(void)
    : mTimerMilliScheduler(*this)
#if OPENVC_CONFIG_ENABLE_PLATFORM_USEC_TIMER
    , mTimerMicroScheduler(*this)
#endif
#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    , mLogLevel(static_cast<vcLogLevel>(OPENVC_CONFIG_LOG_LEVEL))
#endif
    , mIsInitialized(false)
{
}

Instance &Instance::InitSingle(void)
{
    Instance *instance = &Get();

    VerifyOrExit(instance->mIsInitialized == false);

    instance = new (&sInstanceRaw) Instance();

    instance->AfterInit();

exit:
    return *instance;
}

Instance &Instance::Get(void)
{
    void *instance = &sInstanceRaw;
    return *static_cast<Instance *>(instance);
}

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

exit:
    return;
}

} // namespace vc
