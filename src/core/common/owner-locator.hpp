#ifndef OWNER_LOCATOR_HPP_
#define OWNER_LOCATOR_HPP_

#include "openvc-core-config.h"

#include "common/instance.hpp"
#include "common/locator.hpp"

namespace vc {

template <typename OwnerType> OwnerType &OwnerLocator::GetOwner(void)
{
    return Instance::Get().Get<OwnerType>();
}

} // namespace vc

#endif // OWNER_LOCATOR_HPP_
