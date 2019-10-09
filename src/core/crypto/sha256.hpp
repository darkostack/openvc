#ifndef SHA256_HPP_
#define SHA256_HPP_

#include "openvc-core-config.h"

#include "utils/wrap_stdint.h"

#include <mbedtls/sha256.h>

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
    mbedtls_sha256_context mContext;
};

} // namespace Crypto
} // namespace vc

#endif // SHA256_HPP_
