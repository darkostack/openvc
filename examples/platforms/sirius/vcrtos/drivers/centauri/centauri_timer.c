#include "centauri_timer.h"

#include "xtimer.h"

void cent_busy_wait(uint32_t us)
{
    xtimer_usleep(us);
}

uint32_t cent_usec_now(void)
{
    return xtimer_now().ticks32;
}
