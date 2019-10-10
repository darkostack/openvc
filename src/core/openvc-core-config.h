#ifndef OPENVC_CORE_CONFIG_H_
#define OPENVC_CORE_CONFIG_H_

#include <openvc/config.h>

#define OPENVC_CORE_CONFIG_H_IN

#ifdef OPENVC_PROJECT_CORE_CONFIG_FILE
#include OPENVC_PROJECT_CORE_CONFIG_FILE
#endif

#include "config/openvc-core-default-config.h"

#include "config/logging.h"
#include "config/platform.h"

#undef OPENVC_CORE_CONFIG_H_IN

#endif // OPENVC_CORE_CONFIG_H_
