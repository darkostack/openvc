#include <stdint.h>

#include "sched.h"
#include "clist.h"
#include "bitarithm.h"
#include "thread.h"
#include "irq.h"
#include "log.h"

#ifdef MODULE_SCHEDSTATISTICS
#include "xtimer.h"
#endif

#define ENABLE_DEBUG (0)
#include "debug.h"

#if ENABLE_DEBUG
#include <inttypes.h>
#endif

volatile int sched_num_threads = 0;

volatile unsigned int sched_context_switch_request;

volatile thread_t *sched_threads[KERNEL_PID_LAST + 1];
volatile thread_t *sched_active_thread;

volatile kernel_pid_t sched_active_pid = KERNEL_PID_UNDEF;

clist_node_t sched_runqueues[SCHED_PRIO_LEVELS];
static uint32_t runqueue_bitcache = 0;

#ifdef MODULE_SCHEDSTATISTICS
static void (*sched_cb) (uint32_t timestamp, uint32_t value) = NULL;
schedstat_t sched_pidlist[KERNEL_PID_LAST + 1];
#endif

int __attribute__((used)) sched_run(void)
{
    sched_context_switch_request = 0;

    thread_t *active_thread = (thread_t *)sched_active_thread;

    /* The bitmask in runqueue_bitcache is never empty,
     * since the threading should not be started before at least the idle thread was started.
     */
    int nextrq = bitarithm_lsb(runqueue_bitcache);
    thread_t *next_thread = container_of(sched_runqueues[nextrq].next->next, thread_t, rq_entry);

    DEBUG("sched_run: active thread: %" PRIkernel_pid ", next thread: %" PRIkernel_pid ".\n",
          (kernel_pid_t)((active_thread == NULL) ? KERNEL_PID_UNDEF : active_thread->pid),
          next_thread->pid);

    if (active_thread == next_thread) {
        DEBUG("sched_run: done, sched_active_thread was not changed.\n");
        return 0;
    }

#ifdef MODULE_SCHEDSTATISTICS
    uint32_t now = xtimer_now().ticks32;
#endif

    if (active_thread) {
        if (active_thread->status == STATUS_RUNNING) {
            active_thread->status = STATUS_PENDING;
        }
#ifdef MODULE_SCHEDSTATISTICS
        schedstat_t *active_stat = &sched_pidlist[active_thread->pid];
        if (active_stat->laststart) {
            active_stat->runtime_ticks += now - active_stat->laststart;
        }
#endif
    }

#ifdef MODULE_SCHEDSTATISTICS
    schedstat_t *next_stat = &sched_pidlist[next_thread->pid];
    next_stat->laststart = now;
    next_stat->schedules++;
    if (sched_cb) {
        sched_cb(now, next_thread->pid);
    }
#endif

    next_thread->status = STATUS_RUNNING;
    sched_active_pid = next_thread->pid;
    sched_active_thread = (volatile thread_t *) next_thread;

    DEBUG("sched_run: done, changed sched_active_thread.\n");

    return 1;
}

#ifdef MODULE_SCHEDSTATISTICS
void sched_register_cb(void (*callback)(uint32_t, uint32_t))
{
    sched_cb = callback;
}
#endif

void sched_set_status(thread_t *process, thread_status_t status)
{
    if (status >= STATUS_ON_RUNQUEUE) {
        if (!(process->status >= STATUS_ON_RUNQUEUE)) {
            DEBUG("sched_set_status: adding thread %" PRIkernel_pid " to runqueue %u.\n",
                  process->pid, process->priority);
            clist_rpush(&sched_runqueues[process->priority], &(process->rq_entry));
            runqueue_bitcache |= 1 << process->priority;
        }
    } else {
        if (process->status >= STATUS_ON_RUNQUEUE) {
            DEBUG("sched_set_status: removing thread %" PRIkernel_pid " from runqueue %u.\n",
                  process->pid, process->priority);
            clist_lpop(&sched_runqueues[process->priority]);
            if (!sched_runqueues[process->priority].next) {
                runqueue_bitcache &= ~(1 << process->priority);
            }
        }
    }

    process->status = status;
}

void sched_switch(uint16_t other_prio)
{
    thread_t *active_thread = (thread_t *) sched_active_thread;
    uint16_t current_prio = active_thread->priority;
    int on_runqueue = (active_thread->status >= STATUS_ON_RUNQUEUE);

    DEBUG("sched_switch: active pid=%" PRIkernel_pid" prio=%" PRIu16 " on_runqueue=%i "
          ", other_prio=%" PRIu16 ".\n",
          active_thread->pid, current_prio, on_runqueue, other_prio);

    if (!on_runqueue || (current_prio > other_prio)) {
        if (irq_is_in()) {
            DEBUG("sched_switch: setting sched_context_switch_request.\n");
            sched_context_switch_request = 1;
        } else {
            DEBUG("sched_switch: yielding immediately.\n");
            thread_yield_higher();
        }
    } else {
        DEBUG("sched_switch: continuing without yield.\n");
    }
}

NORETURN void sched_task_exit(void)
{
    DEBUG("sched_task_exit: ending thread %" PRIkernel_pid "...\n", sched_active_thread->pid);

    (void) irq_disable();
    sched_threads[sched_active_pid] = NULL;
    sched_num_threads--;

    sched_set_status((thread_t *)sched_active_thread, STATUS_STOPPED);

    sched_active_thread = NULL;
    cpu_switch_context_exit();
}
