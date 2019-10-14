#include "aes_ecb.hpp"

namespace vc {
namespace Crypto {

AesEcb::AesEcb()
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_aes_init(&mContext);
#endif
}

void AesEcb::SetKey(const uint8_t *aKey, uint16_t aKeyLength)
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_aes_setkey_enc(&mContext, aKey, aKeyLength);
#endif
}

void AesEcb::Encrypt(const uint8_t aInput[kBlockSize], uint8_t aOutput[kBlockSize])
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_aes_crypt_ecb(&mContext, MBEDTLS_AES_ENCRYPT, aInput, aOutput);
#endif
}

AesEcb::~AesEcb()
{
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_aes_free(&mContext);
#endif
}

} // namespace Crypto
} // namespace vc
