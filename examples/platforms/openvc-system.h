#ifndef OPENVC_SYSTEM_H_
#define OPENVC_SYSTEM_H_

#include <openvc/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void vcSysInit(int argc, char *argv[]);

void vcSysDeInit(void);

bool vcSysPseudoResetWasRequested(void);

void vcSysProcessDrivers(vcInstance *aInstance);

extern void vcSysEventSignalPending(void);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_SYSTEM_H_
