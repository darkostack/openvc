#ifndef ECDSA_HPP_
#define ECDSA_HPP_

#include "openvc-core-config.h"

#include <stdint.h>
#include <stdlib.h>

#include <openvc/error.h>

namespace vc {
namespace Crypto {

class Ecdsa
{
public:
    static vcError Sign(uint8_t *      aOutput,
                        uint16_t *     aOutputLength,
                        const uint8_t *aInputHash,
                        uint16_t       aInputHashLength,
                        const uint8_t *aPrivateKey,
                        uint16_t       aPrivateKeyLength);
};

} // namespace Crypto
} // namespace vc

#endif // ECDSA_HPP_
