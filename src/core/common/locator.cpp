#include "locator.hpp"

#include "common/instance.hpp"

namespace vc {

Instance &InstanceLocator::GetInstance(void) const
{
    return Instance::Get();
}

} // namespace vc
