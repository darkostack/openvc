#ifndef AES_ECB_HPP_
#define AES_ECB_HPP_

#include "openvc-core-config.h"

#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
#include <mbedtls/aes.h>
#endif

namespace vc {
namespace Crypto {

class AesEcb
{
public:
    enum
    {
        kBlockSize = 16, // AES-128 block size (bytes).
    };

    AesEcb();

    ~AesEcb();

    void SetKey(const uint8_t *aKey, uint16_t aKeyLength);

    void Encrypt(const uint8_t aInput[kBlockSize], uint8_t aOutput[kBlockSize]);

private:
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_aes_context mContext;
#endif
};

} // namespace Crypto
} // namespace vc

#endif // AES_ECB_HPP_
