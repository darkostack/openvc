#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include "openvc-core-config.h"

#include "utils/wrap_stdint.h"

#include <openvc/platform/random.h>

namespace vc {
namespace Random {

inline uint8_t GetUint8(void)
{
    return static_cast<uint8_t>(vcPlatRandomGet() & 0xff);
}

inline uint16_t GetUint16(void)
{
    return static_cast<uint16_t>(vcPlatRandomGet() & 0xffff);
}

inline uint32_t GetUint32(void)
{
    return vcPlatRandomGet();
}

inline uint8_t GetUint8InRange(uint8_t aMin, uint8_t aMax)
{
    return (aMin + (GetUint8() % (aMax - aMin)));
}

inline uint16_t GetUint16InRange(uint16_t aMin, uint16_t aMax)
{
    return (aMin + (GetUint16() % (aMax - aMin)));
}

inline uint32_t GetUint32InRange(uint32_t aMin, uint32_t aMax)
{
    return (aMin + (GetUint32() % (aMax - aMin)));
}

inline void FillBuffer(uint8_t *aBuffer, uint16_t aSize)
{
    while (aSize-- != 0)
    {
        *aBuffer++ = GetUint8();
    }
}

inline uint32_t AddJitter(uint32_t aValue, uint16_t aJitter)
{
    aJitter = (aJitter <= aValue) ? aJitter : static_cast<uint16_t>(aValue);

    return aValue + GetUint32InRange(0, 2 * aJitter + 1) - aJitter;
}

} // namespace Random
} // namespace vc

#endif // RANDOM_HPP_
