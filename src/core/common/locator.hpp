#ifndef LOCATOR_HPP_
#define LOCATOR_HPP_

#include "openvc-core-config.h"

#include <openvc/platform/toolchain.h>

#include <stdint.h>

namespace vc {

class Instance;

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
extern uint64_t gInstanceRaw[];
#endif

class InstanceLocator
{
public:
#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    Instance &GetInstance(void) const { return mInstance; }
#else
    Instance &GetInstance(void) const { return *reinterpret_cast<Instance *>(&gInstanceRaw); }
#endif

    template <typename Type> inline Type &Get(void) const;

protected:
    explicit InstanceLocator(Instance &aInstance)
#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
        : mInstance(aInstance)
#endif
    {
        VC_UNUSED_VARIABLE(aInstance);
    }

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
private:
    Instance &mInstance;
#endif
};

class OwnerLocator
{
public:
    template <typename OwnerType> OwnerType &GetOwner(void)
#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    {
        return *static_cast<OwnerType *>(mOwner);
    }
#else
    ;
#endif

protected:
    explicit OwnerLocator(void *aOwner)
#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
        : mOwner(aOwner)
#endif
    {
        VC_UNUSED_VARIABLE(aOwner);
    }

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
private:
    void *mOwner;
#endif
};

} // namespace vc

#endif // LOCATOR_HPP_
