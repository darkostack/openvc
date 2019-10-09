#ifndef OPENVC_PLATFORM_ALARM_MICRO_H_
#define OPENVC_PLATFORM_ALARM_MICRO_H_

#include <stdint.h>

#include <openvc/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void vcPlatAlarmMicroStartAt(vcInstance *aInstance, uint32_t aT0, uint32_t aDt);

void vcPlatAlarmMicroStop(vcInstance *aInstance);

uint32_t vcPlatAlarmMicroGetNow(void);

extern void vcPlatAlarmMicroFired(vcInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_ALARM_MICRO_H_
