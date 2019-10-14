#ifndef SHA256_HPP_
#define SHA256_HPP_

#include "openvc-core-config.h"

#include <stdint.h>

#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
#include <mbedtls/sha256.h>
#endif

namespace vc {
namespace Crypto {

class Sha256
{
public:
    enum
    {
        kHashSize = 32, // SHA-256 hash size (bytes)
    };

    Sha256();

    ~Sha256();

    void Start(void);

    void Update(const uint8_t *aBuf, uint16_t aBufLength);

    void Finish(uint8_t aHash[kHashSize]);

private:
#if OPENVC_CONFIG_BUILTIN_MBEDTLS_ENABLE
    mbedtls_sha256_context mContext;
#endif
};

} // namespace Crypto
} // namespace vc

#endif // SHA256_HPP_
