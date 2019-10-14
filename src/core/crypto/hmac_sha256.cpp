#include "hmac_sha256.hpp"

namespace vc {
namespace Crypto {

HmacSha256::HmacSha256()
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    const mbedtls_md_info_t *mdInfo = NULL;
    mbedtls_md_init(&mContext);
    mdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    mbedtls_md_setup(&mContext, mdInfo, 1);
#endif
}

HmacSha256::~HmacSha256()
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_md_free(&mContext);
#endif
}

void HmacSha256::Start(const uint8_t *aKey, uint16_t aKeyLength)
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_md_hmac_starts(&mContext, aKey, aKeyLength);
#endif
}

void HmacSha256::Update(const uint8_t *aBuf, uint16_t aBufLength)
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_md_hmac_update(&mContext, aBuf, aBufLength);
#endif
}

void HmacSha256::Finish(uint8_t aHash[kHashSize])
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_md_hmac_finish(&mContext, aHash);
#endif
}

} // namespace Crypto
} // namespace vc
