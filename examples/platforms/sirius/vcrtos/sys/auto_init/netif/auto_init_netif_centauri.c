#include "thread.h"
#include "centauri.h"

static char centauri_stack[THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];
extern void *centauri_thread(void *arg);

void auto_init_centauri(void)
{
    centauri_t *dev = &cent_dev;

    dev->pid = thread_create(centauri_stack, sizeof(centauri_stack),
                             THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
                             centauri_thread, NULL, "centauri");
}
