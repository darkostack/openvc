#include "sha256.hpp"

namespace vc {
namespace Crypto {

Sha256::Sha256()
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_sha256_init(&mContext);
#endif
}

Sha256::~Sha256()
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_sha256_free(&mContext);
#endif
}

void Sha256::Start(void)
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_sha256_starts_ret(&mContext, 0);
#endif
}

void Sha256::Update(const uint8_t *aBuf, uint16_t aBufLength)
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_sha256_update_ret(&mContext, aBuf, aBufLength);
#endif
}

void Sha256::Finish(uint8_t aHash[kHashSize])
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_sha256_finish_ret(&mContext, aHash);
#endif
}

} // namespace Crypto
} // namespace vc
