#include "openvc-core-config.h"

#include <openvc/tasklet.h>

#include "common/code_utils.hpp"
#include "common/instance.hpp"
#include "common/logging.hpp"

void vcTaskletsProcess(vcInstance *aInstance)
{
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);

    VerifyOrExit(vcInstanceIsInitialized(aInstance));
    instance.GetTaskletScheduler().ProcessQueuedTasklets();

exit:
    return;
}

bool vcTaskletsArePending(vcInstance *aInstance)
{
    bool retval = false;
    vc::Instance &instance = *static_cast<vc::Instance *>(aInstance);

    VerifyOrExit(vcInstanceIsInitialized(aInstance));
    retval = instance.GetTaskletScheduler().AreTaskletsPending();

exit:
    return retval;
}

VC_TOOL_WEAK void vcTaskletsSignalPending(vcInstance *)
{
}
