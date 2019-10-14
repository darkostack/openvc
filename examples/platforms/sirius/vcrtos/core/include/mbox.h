#ifndef MBOX_H
#define MBOX_H

#include "list.h"
#include "cib.h"
#include "msg.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Static initializer for mbox objects */
#define MBOX_INIT(queue, queue_size) {{0}, {0}, CIB_INIT(queue_size), queue}

typedef struct {
    list_node_t readers;    /* list of threads waiting for message    */
    list_node_t writers;    /* list of threads waiting to send        */
    cib_t cib;              /* cib for msg array                      */
    msg_t *msg_array;       /* ptr to array of msg queue              */
} mbox_t;

enum {
    NON_BLOCKING = 0,       /* non-blocking mode */
    BLOCKING,               /* blocking mode */
};

static inline void mbox_init(mbox_t *mbox, msg_t *queue, unsigned int queue_size)
{
    mbox_t m = MBOX_INIT(queue, queue_size);
    *mbox = m;
}

int _mbox_put(mbox_t *mbox, msg_t *msg, int blocking);

int _mbox_get(mbox_t *mbox, msg_t *msg, int blocking);

static inline void mbox_put(mbox_t *mbox, msg_t *msg)
{
    _mbox_put(mbox, msg, BLOCKING);
}

static inline int mbox_try_put(mbox_t *mbox, msg_t *msg)
{
    return _mbox_put(mbox, msg, NON_BLOCKING);
}

static inline void mbox_get(mbox_t *mbox, msg_t *msg)
{
    _mbox_get(mbox, msg, BLOCKING);
}

static inline int mbox_try_get(mbox_t *mbox, msg_t *msg)
{
    return _mbox_get(mbox, msg, NON_BLOCKING);
}

#ifdef __cplusplus
}
#endif

#endif /* MBOX_H */
