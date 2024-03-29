#include "hmac_sha256.hpp"

namespace vc {
namespace Crypto {

HmacSha256::HmacSha256()
{
    const mbedtls_md_info_t *mdInfo = NULL;
    mbedtls_md_init(&mContext);
    mdInfo = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    mbedtls_md_setup(&mContext, mdInfo, 1);
}

HmacSha256::~HmacSha256()
{
    mbedtls_md_free(&mContext);
}

void HmacSha256::Start(const uint8_t *aKey, uint16_t aKeyLength)
{
    mbedtls_md_hmac_starts(&mContext, aKey, aKeyLength);
}

void HmacSha256::Update(const uint8_t *aBuf, uint16_t aBufLength)
{
    mbedtls_md_hmac_update(&mContext, aBuf, aBufLength);
}

void HmacSha256::Finish(uint8_t aHash[kHashSize])
{
    mbedtls_md_hmac_finish(&mContext, aHash);
}

} // namespace Crypto
} // namespace vc
