/* Note: this mcu doesn't have rtt (real time timer) use xtimer instead. */

#include "xtimer.h"
#include "periph/rtt.h"

static xtimer_t xtimer_rtt;

typedef struct {
    rtt_cb_t cb;
    void *arg;
} rtt_alarm_ctx_t;

static rtt_alarm_ctx_t rtt_alarm_ctx;

static void _xtimer_rtt_alarm_cb(void *arg)
{
    rtt_alarm_ctx_t *alarm = (rtt_alarm_ctx_t *)arg;
    if (alarm->cb != NULL) {
        alarm->cb(alarm->arg);
    }
}

void rtt_init(void)
{
    assert(RTT_FREQUENCY == XTIMER_HZ);
}

void rtt_set_overflow_cb(rtt_cb_t cb, void *arg)
{
    (void) cb;
    (void) arg;
}

void rtt_clear_overflow_cb(void)
{

}

uint32_t rtt_get_counter(void)
{
    return xtimer_now().ticks32;
}

void rtt_set_counter(uint32_t counter)
{
    (void) counter;
}

void rtt_set_alarm(uint32_t alarm, rtt_cb_t cb, void *arg)
{
    rtt_alarm_ctx.cb = cb;
    rtt_alarm_ctx.arg = arg;

    xtimer_rtt.callback = _xtimer_rtt_alarm_cb;
    xtimer_rtt.arg = (void *)&rtt_alarm_ctx;
    xtimer_rtt.target = xtimer_rtt.long_target = 0;

    xtimer_set(&xtimer_rtt, alarm);
}

uint32_t rtt_get_alarm(void)
{
    return xtimer_now().ticks32;
}

void rtt_clear_alarm(void)
{

}

void rtt_poweron(void)
{

}

void rtt_poweroff(void)
{

}
