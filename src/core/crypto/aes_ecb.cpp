#include "aes_ecb.hpp"

namespace vc {
namespace Crypto {

AesEcb::AesEcb()
{
    mbedtls_aes_init(&mContext);
}

void AesEcb::SetKey(const uint8_t *aKey, uint16_t aKeyLength)
{
    mbedtls_aes_setkey_enc(&mContext, aKey, aKeyLength);
}

void AesEcb::Encrypt(const uint8_t aInput[kBlockSize], uint8_t aOutput[kBlockSize])
{
    mbedtls_aes_crypt_ecb(&mContext, MBEDTLS_AES_ENCRYPT, aInput, aOutput);
}

AesEcb::~AesEcb()
{
    mbedtls_aes_free(&mContext);
}

} // namespace Crypto
} // namespace vc
