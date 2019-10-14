#ifndef RANDOM_H
#define RANDOM_H

#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RANDOM_SEED_DEFAULT
#define RANDOM_SEED_DEFAULT (1)
#endif

void random_init(uint32_t s);

void random_init_by_array(uint32_t init_key[], int key_length);

uint32_t random_uint32(void);

void random_bytes(uint8_t *buf, size_t size);

uint32_t random_uint32_range(uint32_t a, uint32_t b);

#ifdef __cplusplus
}
#endif

#endif /* RANDOM_H */
