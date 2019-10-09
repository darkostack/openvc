#ifndef TEST_PLATFORM_H_
#define TEST_PLATFORM_H_

#include <string.h>

#include <openvc/config.h>
#include <openvc/platform/alarm-milli.h>
#include <openvc/platform/alarm-micro.h>
#include <openvc/platform/logging.h>
#include <openvc/platform/misc.h>
#include <openvc/platform/random.h>

#include "common/code_utils.hpp"
#include "common/instance.hpp"

#include "test_util.h"

//
// Alarm Platform
//
typedef void (*testPlatAlarmStop)(vcInstance *);
typedef void (*testPlatAlarmStartAt)(vcInstance *, uint32_t, uint32_t);
typedef uint32_t (*testPlatAlarmGetNow)(void);

extern bool                 gTestPlatAlarmSet;
extern uint32_t             gTestPlatAlarmNext;
extern testPlatAlarmStop    gTestPlatAlarmStop;
extern testPlatAlarmStartAt gTestPlatAlarmStartAt;
extern testPlatAlarmGetNow  gTestPlatAlarmGetNow;

vc::Instance *testInitInstance(void);
void          testFreeInstance(vcInstance *aInstance);

// Reset platform functions to defaults
void testPlatResetToDefaults(void);

#endif // TEST_PLATFORM_H_
