#include "openvc-core-config.h"

#include <openvc/instance.h>
#include <openvc/platform/misc.h>

#include "common/instance.hpp"
#include "common/logging.hpp"
#include "common/new.hpp"

using namespace vc;

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
vcInstance *vcInstanceInit(void *aInstanceBuffer, size_t *aInstanceBufferSize)
{
    Instance *instance;

    instance = Instance::Init(aInstanceBuffer, aInstanceBufferSize);
    vcLogInfoApi("vcInstance Initialized");

    return instance;
}
#else
vcInstance *vcInstanceInitSingle(void)
{
    return &Instance::InitSingle();
}
#endif

bool vcInstanceIsInitialized(vcInstance *aInstance)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    return instance.IsInitialized();
}

void vcInstanceFinalize(vcInstance *aInstance)
{
    Instance &instance = *static_cast<Instance *>(aInstance);
    instance.Finalize();
}

void vcInstanceReset(vcInstance *aInstance)
{
    Instance &instance = *static_cast<Instance *>(aInstance);
    instance.Reset();
}

const char *vcGetVersionString(void)
{
#ifdef PLATFORM_VERSION_ATTR_PREFIX
    PLATFORM_VERSION_ATTR_PREFIX
#else
    static
#endif
    const char sVersion[] = PACKAGE_NAME "/" PACKAGE_VERSION "; " OPENVC_CONFIG_PLATFORM_INFO
#if defined(__DATE__)
                                         "; " __DATE__ " " __TIME__
#endif
#ifdef PLATFORM_VERSION_ATTR_SUFFIX
                                             PLATFORM_VERSION_ATTR_SUFFIX
#endif
        ; // Trailing semicolon to end statement.

    return sVersion;
}
