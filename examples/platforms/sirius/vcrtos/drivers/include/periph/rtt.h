#ifndef PERIPH_RTT_H
#define PERIPH_RTT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RTT_FREQUENCY
#define RTT_FREQUENCY (1000000UL)
#endif

#define RTT_US_TO_TICKS(us)     ((uint32_t)((uint64_t)(us) * RTT_FREQUENCY / 1000000UL))

#define RTT_MS_TO_TICKS(ms)     ( RTT_US_TO_TICKS((ms) * 1000) )

#define RTT_SEC_TO_TICKS(sec)   ( RTT_MS_TO_TICKS((sec) * 1000) )

#define RTT_MIN_TO_TICKS(min)   ( RTT_SEC_TO_TICKS((min) * 60) )

#define RTT_TICKS_TO_US(ticks)  ((uint32_t)((uint64_t)(ticks) * 1000000UL / RTT_FREQUENCY))

#define RTT_TICKS_TO_MS(ticks)  (RTT_TICKS_TO_US(ticks) / 1000)

#define RTT_TICKS_TO_SEC(ticks) (RTT_TICKS_TO_MS(ticks) / 1000)

#define RTT_TICKS_TO_MIN(ticks) (RTT_TICKS_TO_SEC(ticks) / 60)

typedef void(*rtt_cb_t)(void *arg);

void rtt_init(void);

void rtt_set_overflow_cb(rtt_cb_t cb, void *arg);

void rtt_clear_overflow_cb(void);

uint32_t rtt_get_counter(void);

void rtt_set_counter(uint32_t counter);

void rtt_set_alarm(uint32_t alarm, rtt_cb_t cb, void *arg);

uint32_t rtt_get_alarm(void);

void rtt_clear_alarm(void);

void rtt_poweron(void);

void rtt_poweroff(void);

#ifdef __cplusplus
}
#endif

#endif /* PERIPH_RTT_H */
