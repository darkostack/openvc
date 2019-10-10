#include "mbedtls.hpp"

#include <mbedtls/ctr_drbg.h>
#include <mbedtls/debug.h>
#include <mbedtls/entropy.h>
#include <mbedtls/error.h>
#include <mbedtls/platform.h>
#include <mbedtls/threading.h>

#include "common/instance.hpp"

namespace vc {
namespace Crypto {

#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE && OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
static void *CAlloc(size_t aCount, size_t aSize)
{
    return Instance::Get().GetHeap().CAlloc(aCount, aSize);
}

static void Free(void *aPointer)
{
    Instance::Get().GetHeap().Free(aPointer);
}
#endif

MbedTls::MbedTls(void)
{
#if !OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE && OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
#ifdef MBEDTLS_DEBUG_C
    mbedtls_debug_set_threshold(OPENVC_CONFIG_LOG_LEVEL);
#endif
    mbedtls_platform_set_calloc_free(CAlloc, Free);
#endif
}

vcError MbedTls::MapError(int aRval)
{
    vcError error = VC_ERROR_NONE;

    switch (aRval)
    {
    case MBEDTLS_ERR_SSL_BAD_INPUT_DATA:
    case MBEDTLS_ERR_CTR_DRBG_REQUEST_TOO_BIG:
    case MBEDTLS_ERR_CTR_DRBG_INPUT_TOO_BIG:
        error = VC_ERROR_INVALID_ARGS;
        break;

    case MBEDTLS_ERR_SSL_ALLOC_FAILED:
    case MBEDTLS_ERR_SSL_WANT_WRITE:
    case MBEDTLS_ERR_ENTROPY_MAX_SOURCES:
        error = VC_ERROR_NO_BUFS;
        break;

    case MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED:
    case MBEDTLS_ERR_ENTROPY_SOURCE_FAILED:
    case MBEDTLS_ERR_ENTROPY_NO_SOURCES_DEFINED:
    case MBEDTLS_ERR_ENTROPY_NO_STRONG_SOURCE:
    case MBEDTLS_ERR_SSL_PEER_VERIFY_FAILED:
    case MBEDTLS_ERR_THREADING_BAD_INPUT_DATA:
    case MBEDTLS_ERR_THREADING_MUTEX_ERROR:
        error = VC_ERROR_SECURITY;
        break;

    case MBEDTLS_ERR_SSL_TIMEOUT:
    case MBEDTLS_ERR_SSL_WANT_READ:
        error = VC_ERROR_BUSY;
        break;

    default:
        assert(aRval >= 0);
        break;
    }

    return error;
}

} // namespace Crypto
} // namespace vc
