#include "mbedtls.hpp"

#include <mbedtls/platform.h>

#include "common/instance.hpp"

namespace vc {
namespace Crypto {

static void *CAlloc(size_t aCount, size_t aSize)
{
    return Instance::Get().GetHeap().CAlloc(aCount, aSize);
}

static void Free(void *aPointer)
{
    Instance::Get().GetHeap().Free(aPointer);
}

MbedTls::MbedTls(void)
{
    mbedtls_platform_set_calloc_free(CAlloc, Free);
}

} // namespace Crypto
} // namespace vc
