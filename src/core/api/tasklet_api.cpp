#include "openvc-core-config.h"

#include <openvc/tasklet.h>

#include "common/code_utils.hpp"
#include "common/instance.hpp"
#include "common/locator-getters.hpp"

using namespace vc;

void vcTaskletsProcess(vcInstance *aInstance)
{
    Instance &instance = *static_cast<Instance *>(aInstance);

    VerifyOrExit(vcInstanceIsInitialized(aInstance));
    instance.Get<TaskletScheduler>().ProcessQueuedTasklets();

exit:
    return;
}

bool vcTaskletsArePending(vcInstance *aInstance)
{
    bool retval = false;
    Instance &instance = *static_cast<Instance *>(aInstance);

    VerifyOrExit(vcInstanceIsInitialized(aInstance));
    retval = instance.Get<TaskletScheduler>().AreTaskletsPending();

exit:
    return retval;
}

VC_TOOL_WEAK void vcTaskletsSignalPending(vcInstance *)
{
}
