#include <stdint.h>
#include <string.h>

#include "tinymt32/tinymt32.h"

static tinymt32_t _random;

void random_init(uint32_t seed)
{
    tinymt32_init(&_random, seed);
}

uint32_t random_uint32(void)
{
    return tinymt32_generate_uint32(&_random);
}

void random_init_by_array(uint32_t init_key[], int key_length)
{
    tinymt32_init_by_array(&_random, init_key, key_length);
}
