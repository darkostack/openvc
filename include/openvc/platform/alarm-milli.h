#ifndef OPENVC_PLATFORM_ALARM_MILLI_H_
#define OPENVC_PLATFORM_ALARM_MILLI_H_

#include <stdint.h>

#include <openvc/instance.h>

#ifdef __cplusplus
extern "C" {
#endif

void vcPlatAlarmMilliStartAt(vcInstance *aInstance, uint32_t aT0, uint32_t aDt);

void vcPlatAlarmMilliStop(vcInstance *aInstance);

uint32_t vcPlatAlarmMilliGetNow(void);

extern void vcPlatAlarmMilliFired(vcInstance *aInstance);

extern void vcPlatDiagAlarmFired(vcInstance *aInstance);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_ALARM_MILLI_H_
