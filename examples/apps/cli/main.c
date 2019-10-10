#include <assert.h>
#include <openvc-core-config.h>
#include <openvc/config.h>

#include <openvc/cli.h>
#include <openvc/tasklet.h>
#include <openvc/platform/logging.h>

#include "openvc-system.h"

#if OPENVC_EXAMPLES_POSIX
#include <setjmp.h>
#include <unistd.h>

jmp_buf gResetJump;

void __gcov_flush();
#endif

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
void *vcPlatCAlloc(size_t aNum, size_t aSize)
{
    return calloc(aNum, aSize);
}

void vcPlatFree(void *aPtr)
{
    free(aPtr);
}
#endif

void vcTaskletsSignalPending(vcInstance *aInstance)
{
    VC_UNUSED_VARIABLE(aInstance);
}

int main(int argc, char *argv[])
{
    vcInstance *instance;

#if OPENVC_EXAMPLES_POSIX
    if (setjmp(gResetJump))
    {
        alarm(0);
#ifdef OPENVC_ENABLE_COVERAGE
        __gcov_flush();
#endif
        execvp(argv[0], argv);
    }
#endif

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    size_t   vcInstanceBufferLength = 0;
    uint8_t *vcInstanceBuffer       = NULL;
#endif

pseudo_reset:

    vcSysInit(argc, argv);

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    // Call to query the buffer size
    (void)vcInstanceInit(NULL, &vcInstanceBufferLength);

    // Call to allocate the buffer
    vcInstanceBuffer = (uint8_t *)malloc(vcInstanceBufferLength);
    assert(vcInstanceBuffer);

    instance = vcInstanceInit(vcInstanceBuffer, &vcInstanceBufferLength);
#else
    instance = vcInstanceInitSingle();
#endif

    assert(instance);

    vcCliUartInit(instance);

    while (!vcSysPseudoResetWasRequested())
    {
        vcTaskletsProcess(instance);
        vcSysProcessDrivers(instance);
    }

    vcInstanceFinalize(instance);

#if OPENVC_CONFIG_MULTIPLE_INSTANCES_ENABLE
    free(vcInstanceBuffer);
#endif

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
