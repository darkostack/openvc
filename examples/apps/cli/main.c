#include <assert.h>
#include <openvc-core-config.h>
#include <openvc/config.h>

#include <openvc/cli.h>
#include <openvc/tasklet.h>
#include <openvc/platform/logging.h>

#include "openvc-system.h"

void vcTaskletsSignalPending(vcInstance *aInstance)
{
    (void)aInstance;
}

int main(int argc, char *argv[])
{
    vcInstance *sInstance;

pseudo_reset:

    vcSysInit(argc, argv);

    sInstance = vcInstanceInitSingle();

    assert(sInstance);

    vcCliUartInit(sInstance);

    while (!vcSysPseudoResetWasRequested())
    {
        vcTaskletsProcess(sInstance);
        vcSysProcessDrivers(sInstance);
    }

    vcInstanceFinalize(sInstance);

    goto pseudo_reset;

    return 0;
}

/*
 * Provide, if required an "vcPlatLog()" function
 */
#if OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_APP
void vcPlatLog(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, ...)
{
    va_list ap;
    va_start(ap, aFormat);
    vcCliPlatLogv(aLogLevel, aLogRegion, aFormat, ap);
    va_end(ap);
}
#endif
