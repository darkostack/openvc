#include "auto_init.h"
#include "thread.h"

#ifdef MODULE_RANDOM
#include "random.h"
#endif

#ifdef MODULE_XTIMER
#include "xtimer.h"
#endif

#define ENABLE_DEBUG (1)
#include "debug.h"

void auto_init(void)
{
#ifdef MODULE_RANDOM
    DEBUG("Auto init random module.\n");
    extern void auto_init_random(void);
    auto_init_random();
#endif
#ifdef MODULE_XTIMER
    DEBUG("Auto init xtimer module.\n");
    xtimer_init();
#endif
#ifdef MODULE_CENTAURI
    DEBUG("Auto init centauri module.\n");
    extern void auto_init_centauri(void);
    auto_init_centauri();
#endif
}
