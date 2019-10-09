#ifndef OPENVC_TASKLET_H_
#define OPENVC_TASKLET_H_

#include <openvc/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void vcTaskletsProcess(vcInstance *aInstance);

bool vcTaskletsArePending(vcInstance *aInstance);

extern void vcTaskletsSignalPending(vcInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_TASKLET_H_
