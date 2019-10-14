/*
 *Recursive Mutex for thread synchronization
 */
#ifndef RMUTEX_H
#define RMUTEX_H

#include <stdint.h>
#include <stdatomic.h>

#include "mutex.h"
#include "kernel_types.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct rmutex_t {
    mutex_t mutex;
    uint16_t refcount;
    atomic_int_least16_t owner;
} rmutex_t;

#define RMUTEX_INIT { MUTEX_INIT, 0, ATOMIC_VAR_INIT(KERNEL_PID_UNDEF) }

static inline void rmutex_init(rmutex_t *rmutex)
{
    rmutex_t empty_rmutex = RMUTEX_INIT;
    *rmutex = empty_rmutex;
}

int rmutex_trylock(rmutex_t *rmutex);

void rmutex_lock(rmutex_t *rmutex);

void rmutex_unlock(rmutex_t *rmutex);

#ifdef __cplusplus
}
#endif

#endif /* RMUTEX_H */
