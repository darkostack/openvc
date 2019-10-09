#ifndef LOCATOR_HPP_
#define LOCATOR_HPP_

#include "openvc-core-config.h"

#include <openvc/platform/toolchain.h>

namespace vc {

class Instance;

class InstanceLocator
{
public:
    Instance &GetInstance(void) const;

protected:
    InstanceLocator(Instance &aInstance) { VC_UNUSED_VARIABLE(aInstance); }
};

class OwnerLocator
{
public:
    template <typename OwnerType> OwnerType &GetOwner(void);

protected:
    OwnerLocator(void *aOwner) { VC_UNUSED_VARIABLE(aOwner); }
};

} // namespace vc

#endif // LOCATOR_HPP_
