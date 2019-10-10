#ifndef OPENVC_INSTANCE_H_
#define OPENVC_INSTANCE_H_

#include <stdlib.h>

#include <openvc/error.h>
#include <openvc/platform/logging.h>
#include <openvc/platform/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vcInstance vcInstance;

vcInstance *vcInstanceInit(void *aInstanceBuffer, size_t *aInstanceBufferSize);

vcInstance *vcInstanceInitSingle(void);

bool vcInstanceIsInitialized(vcInstance *aInstance);

void vcInstanceFinalize(vcInstance *aInstance);

void vcInstanceReset(vcInstance *aInstance);

const char *vcGetVersionString(void);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_INSTANCE_H_
