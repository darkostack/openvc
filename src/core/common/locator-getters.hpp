#ifndef LOCATOR_GETTERS_HPP_
#define LOCATOR_GETTERS_HPP_

#include "openvc-core-config.h"

#include "common/instance.hpp"
#include "common/locator.hpp"

namespace vc {

template <> inline Instance &InstanceLocator::Get(void) const
{
    return GetInstance();
}

template <typename Type> inline Type &InstanceLocator::Get(void) const
{
    // This method uses the `Instance` template method `Get<Type>`
    // to get to the given `Type` from the single openvc
    // instance.
    return GetInstance().Get<Type>();
}

#if !OPENTHREAD_CONFIG_MULTIPLE_INSTANCE_ENABLE
template <typename OwnerType> OwnerType &OwnerLocator::GetOwner(void)
{
    return Instance::Get().Get<OwnerType>();
}
#endif

} // namespace vc

#endif /* LOCATOR_GETTERS_HPP_ */
