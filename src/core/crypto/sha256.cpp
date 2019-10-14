#include "sha256.hpp"

namespace vc {
namespace Crypto {

Sha256::Sha256()
{
    mbedtls_sha256_init(&mContext);
}

Sha256::~Sha256()
{
    mbedtls_sha256_free(&mContext);
}

void Sha256::Start(void)
{
    mbedtls_sha256_starts_ret(&mContext, 0);
}

void Sha256::Update(const uint8_t *aBuf, uint16_t aBufLength)
{
    mbedtls_sha256_update_ret(&mContext, aBuf, aBufLength);
}

void Sha256::Finish(uint8_t aHash[kHashSize])
{
    mbedtls_sha256_finish_ret(&mContext, aHash);
}

} // namespace Crypto
} // namespace vc
