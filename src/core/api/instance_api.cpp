#include "openvc-core-config.h"

#include <openvc/instance.h>
#include <openvc/platform/misc.h>

#include "common/instance.hpp"
#include "common/logging.hpp"
#include "common/new.hpp"

vcInstance *vcInstanceInitSingle(void)
{
    return &vc::Instance::InitSingle();
}

bool vcInstanceIsInitialized(vcInstance *aInstance)
{
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);

    return instance.IsInitialized();
}

void vcInstanceFinalize(vcInstance *aInstance)
{
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);
    instance.Finalize();
}

void vcInstanceReset(vcInstance *aInstance)
{
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);
    instance.Reset();
}

vcLogLevel vcGetDynamicLogLevel(vcInstance *aInstance)
{
    vcLogLevel logLevel;

#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);
    logLevel = instance.GetDynamicLogLevel();
#else
    logLevel = static_cast<vcLogLevel>(OPENVC_CONFIG_LOG_LEVEL);
    VC_UNUSED_VARIABLE(aInstance);
#endif

    return logLevel;
}

vcError vcSetDynamicLogLevel(vcInstance *aInstance, vcLogLevel aLogLevel)
{
    vcError error = VC_ERROR_NONE;

#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);
    instance.SetDynamicLogLevel(aLogLevel);
#else
    error = VC_ERROR_DISABLED_FEATURE;
    VC_UNUSED_VARIABLE(aInstance);
    VC_UNUSED_VARIABLE(aLogLevel);
#endif

    return error;
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
