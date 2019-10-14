#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct priority_queue_node {
    struct priority_queue_node *next;
    uint32_t priority;
    unsigned int data;
} priority_queue_node_t;

typedef struct {
    priority_queue_node_t *first;
} priority_queue_t;

#define PRIORITY_QUEUE_NODE_INIT { NULL, 0, 0 }

static inline void priority_queue_node_init(
        priority_queue_node_t *priority_queue_node)
{
    priority_queue_node_t qn = PRIORITY_QUEUE_NODE_INIT;
    *priority_queue_node = qn;
}

#define PRIORITY_QUEUE_INIT { NULL }

static inline void priority_queue_init(priority_queue_t *priority_queue)
{
    priority_queue_t q = PRIORITY_QUEUE_INIT;
    *priority_queue = q;
}

priority_queue_node_t *priority_queue_remove_head(priority_queue_t *root);

void priority_queue_add(priority_queue_t *root, priority_queue_node_t *new_obj);

void priority_queue_remove(priority_queue_t *root, priority_queue_node_t *node);

#if ENABLE_DEBUG
void priority_queue_print(priority_queue_t *root);

void priority_queue_print_node(priority_queue_t *root);
#endif

#ifdef __cplusplus
}
#endif

#endif /* PRIORITY_QUEUE_H */
