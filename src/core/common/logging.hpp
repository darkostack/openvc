#ifndef LOGGING_HPP_
#define LOGGING_HPP_

#include "openvc-core-config.h"

#include <ctype.h>
#include <stdio.h>
#include "utils/wrap_string.h"

#include <openvc/instance.h>
#include <openvc/platform/logging.h>

#ifdef __cplusplus
extern "C" {
#endif

#if OPENVC_CONFIG_LOG_PREPEND_LEVEL
#define _VC_LEVEL_NONE_PREFIX "[NONE]"
#define _VC_LEVEL_CRIT_PREFIX "[CRIT]"
#define _VC_LEVEL_WARN_PREFIX "[WARN]"
#define _VC_LEVEL_NOTE_PREFIX "[NOTE]"
#define _VC_LEVEL_INFO_PREFIX "[INFO]"
#define _VC_LEVEL_DEBG_PREFIX "[DEBG]"
#define _VC_REGION_SUFFIX ": "
#else
#define _VC_LEVEL_NONE_PREFIX ""
#define _VC_LEVEL_CRIT_PREFIX ""
#define _VC_LEVEL_WARN_PREFIX ""
#define _VC_LEVEL_NOTE_PREFIX ""
#define _VC_LEVEL_INFO_PREFIX ""
#define _VC_LEVEL_DEBG_PREFIX ""
#define _VC_REGION_SUFFIX
#endif

#if OPENVC_CONFIG_LOG_PREPEND_REGION
#define _VC_REGION_API_PREFIX "-API-----: "
#define _VC_REGION_MEM_PREFIX "-MEM-----: "
#define _VC_REGION_PLATFORM_PREFIX "-PLAT----: "
#define _VC_REGION_CLI_PREFIX "-CLI-----: "
#define _VC_REGION_CORE_PREFIX "-CORE----: "
#define _VC_REGION_UTIL_PREFIX "-UTIL----: "
#else
#define _VC_REGION_API_PREFIX _VC_REGION_SUFFIX
#define _VC_REGION_PLATFORM_PREFIX _VC_REGION_SUFFIX
#define _VC_REGION_CLI_PREFIX _VC_REGION_SUFFIX
#define _VC_REGION_CORE_PREFIX _VC_REGION_SUFFIX
#define _VC_REGION_UTIL_PREFIX _VC_REGION_SUFFIX
#endif

// Logging at level critical

#if OPENVC_CONFIG_LOG_LEVEL >= VC_LOG_LEVEL_CRIT
#define vcLogCrit(aInstance, aRegion, aFormat, ...) \
    _vcLogFormatter(aInstance, VC_LOG_LEVEL_CRIT, aRegion, _VC_LEVEL_CRIT_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogCrit(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level warning

#if OPENVC_CONFIG_LOG_LEVEL >= VC_LOG_LEVEL_WARN
#define vcLogWarn(aInstance, aRegion, aFormat, ...) \
    _vcLogFormatter(aInstance, VC_LOG_LEVEL_WARN, aRegion, _VC_LEVEL_WARN_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogWarn(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level note

#if OPENVC_CONFIG_LOG_LEVEL >= VC_LOG_LEVEL_NOTE
#define vcLogNote(aInstance, aRegion, aFormat, ...) \
    _vcLogFormatter(aInstance, VC_LOG_LEVEL_NOTE, aRegion, _VC_LEVEL_NOTE_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogNote(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level info

#if OPENVC_CONFIG_LOG_LEVEL >= VC_LOG_LEVEL_INFO
#define vcLogInfo(aInstance, aRegion, aFormat, ...) \
    _vcLogFormatter(aInstance, VC_LOG_LEVEL_INFO, aRegion, _VC_LEVEL_INFO_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogInfo(aInstance, aRegion, aFormat, ...)
#endif

// Logging at level debug

#if OPENVC_CONFIG_LOG_LEVEL >= VC_LOG_LEVEL_DEBG
#define vcLogDebg(aInstance, aRegion, aFormat, ...) \
    _vcLogFormatter(aInstance, VC_LOG_LEVEL_DEBG, aRegion, _VC_LEVEL_DEBG_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogDebg(aInstance, aRegion, aFormat, ...)
#endif

// Loging at region API

#if OPENVC_CONFIG_LOG_API == 1
#define vcLogCritApi(aInstance, aFormat, ...) \
    vcLogCrit(&aInstance, VC_LOG_REGION_API, _VC_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogWarnApi(aInstance, aFormat, ...) \
    vcLogWarn(&aInstance, VC_LOG_REGION_API, _VC_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogNoteApi(aInstance, aFormat, ...) \
    vcLogNote(&aInstance, VC_LOG_REGION_API, _VC_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoApi(aInstance, aFormat, ...) \
    vcLogInfo(&aInstance, VC_LOG_REGION_API, _VC_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogDebgApi(aInstance, aFormat, ...) \
    vcLogDebg(&aInstance, VC_LOG_REGION_API, _VC_REGION_API_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogCritApi(aInstance, aFormat, ...)
#define vcLogWarnApi(aInstance, aFormat, ...)
#define vcLogNoteApi(aInstance, aFormat, ...)
#define vcLogInfoApi(aInstance, aFormat, ...)
#define vcLogDebgApi(aInstance, aFormat, ...)
#endif

// Logging at region Memory

#if OPENVC_CONFIG_LOG_MEM == 1
#define vcLogCritMem(aInstance, aFormat, ...) \
    vcLogCrit(&aInstance, VC_LOG_REGION_MEM, _VC_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogWarnMem(aInstance, aFormat, ...) \
    vcLogWarn(&aInstance, VC_LOG_REGION_MEM, _VC_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogNoteMem(aInstance, aFormat, ...) \
    vcLogNote(&aInstance, VC_LOG_REGION_MEM, _VC_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoMem(aInstance, aFormat, ...) \
    vcLogInfo(&aInstance, VC_LOG_REGION_MEM, _VC_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogDebgMem(aInstance, aFormat, ...) \
    vcLogDebg(&aInstance, VC_LOG_REGION_MEM, _VC_REGION_MEM_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogCritMem(aInstance, aFormat, ...)
#define vcLogWarnMem(aInstance, aFormat, ...)
#define vcLogNoteMem(aInstance, aFormat, ...)
#define vcLogInfoMem(aInstance, aFormat, ...)
#define vcLogDebgMem(aInstance, aFormat, ...)
#endif

// Logging at region Platform

#if OPENVC_CONFIG_LOG_PLATFORM == 1
#define vcLogCritPlat(aInstance, aFormat, ...) \
    vcLogCrit(aInstance, VC_LOG_REGION_PLATFORM, _VC_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogWarnPlat(aInstance, aFormat, ...) \
    vcLogWarn(aInstance, VC_LOG_REGION_PLATFORM, _VC_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogNotePlat(aInstance, aFormat, ...) \
    vcLogNote(aInstance, VC_LOG_REGION_PLATFORM, _VC_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoPlat(aInstance, aFormat, ...) \
    vcLogInfo(aInstance, VC_LOG_REGION_PLATFORM, _VC_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogDebgPlat(aInstance, aFormat, ...) \
    vcLogDebg(aInstance, VC_LOG_REGION_PLATFORM, _VC_REGION_PLATFORM_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogCritPlat(aInstance, aFormat, ...)
#define vcLogWarnPlat(aInstance, aFormat, ...)
#define vcLogInfoPlat(aInstance, aFormat, ...)
#define vcLogDebgPlat(aInstance, aFormat, ...)
#endif

// Logging at region CLI

#if OPENVC_CONFIG_LOG_CLI == 1

#define vcLogCritCli(aInstance, aFormat, ...) \
    vcLogCrit(&aInstance, VC_LOG_REGION_CLI, _VC_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogWarnCli(aInstance, aFormat, ...) \
    vcLogWarn(&aInstance, VC_LOG_REGION_CLI, _VC_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogNoteCli(aInstance, aFormat, ...) \
    vcLogNote(&aInstance, VC_LOG_REGION_CLI, _VC_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoCli(aInstance, aFormat, ...) \
    vcLogInfo(&aInstance, VC_LOG_REGION_CLI, _VC_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoCliErr(aInstance, aError, aFormat, ...) \
    vcLogInfo(&aInstance, VC_LOG_REGION_CLI, "Error %s: " aFormat, vcErrorToString(aError), ##__VA_ARGS__)
#define vcLogDebgCli(aInstance, aFormat, ...) \
    vcLogDebg(&aInstance, VC_LOG_REGION_CLI, _VC_REGION_CLI_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogCritCli(aInstance, aFormat, ...)
#define vcLogWarnCli(aInstance, aFormat, ...)
#define vcLogNoteCli(aInstance, aFormat, ...)
#define vcLogInfoCli(aInstance, aFormat, ...)
#define vcLogInfoCliErr(aInstance, aError, aFormat, ...)
#define vcLogDebgCli(aInstance, aFormat, ...)
#endif

// Logging at region Core

#if OPENVC_CONFIG_LOG_CORE == 1
#define vcLogCritCore(aInstance, aFormat, ...) \
    vcLogCrit(&aInstance, VC_LOG_REGION_CORE, _VC_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogWarnCore(aInstance, aFormat, ...) \
    vcLogWarn(&aInstance, VC_LOG_REGION_CORE, _VC_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogNoteCore(aInstance, aFormat, ...) \
    vcLogNote(&aInstance, VC_LOG_REGION_CORE, _VC_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoCore(aInstance, aFormat, ...) \
    vcLogInfo(&aInstance, VC_LOG_REGION_CORE, _VC_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogDebgCore(aInstance, aFormat, ...) \
    vcLogDebg(&aInstance, VC_LOG_REGION_CORE, _VC_REGION_CORE_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogDebgCoreErr(aInstance, aError, aFormat, ...)                                                          \
    vcLogWarn(aInstance, VC_LOG_REGION_CORE, _VC_REGION_CORE_PREFIX "Error %s: " aFormat, vcErrorToString(aError), \
              ##__VA_ARGS__)
#else
#define vcLogCritCore(aInstance, aFormat, ...)
#define vcLogWarnCore(aInstance, aFormat, ...)
#define vcLogInfoCore(aInstance, aFormat, ...)
#define vcLogDebgCore(aInstance, aFormat, ...)
#define vcLogDebgCoreErr(aInstance, aError, aFormat, ...)
#endif

// Logging at region Util

#if OPENVC_CONFIG_LOG_UTIL == 1
#define vcLogCritUtil(aInstance, aFormat, ...) \
    vcLogCrit(&aInstance, VC_LOG_REGION_UTIL, _VC_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogWarnUtil(aInstance, aFormat, ...) \
    vcLogWarn(&aInstance, VC_LOG_REGION_UTIL, _VC_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogNoteUtil(aInstance, aFormat, ...) \
    vcLogNote(&aInstance, VC_LOG_REGION_UTIL, _VC_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoUtil(aInstance, aFormat, ...) \
    vcLogInfo(&aInstance, VC_LOG_REGION_UTIL, _VC_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#define vcLogInfoUtilErr(aInstance, aError, aFormat, ...)                                                           \
    vcLogInfo(&aInstance, VC_LOG_REGION_UTIL, _VC_REGION_CORE_PREFIX "Error %s: " aFormat, vcErrorToString(aError), \
              ##__VA_ARGS__)
#define vcLogDebgUtil(aInstance, aFormat, ...) \
    vcLogDebg(&aInstance, VC_LOG_REGION_UTIL, _VC_REGION_UTIL_PREFIX aFormat, ##__VA_ARGS__)
#else
#define vcLogCritUtil(aInstance, aFormat, ...)
#define vcLogWarnUtil(aInstance, aFormat, ...)
#define vcLogNoteUtil(aInstance, aFormat, ...)
#define vcLogInfoUtil(aInstance, aFormat, ...)
#define vcLogInfoUtilErr(aInstance, aError, aFormat, ...)
#define vcLogDebgUtil(aInstance, aFormat, ...)
#endif

void vcDump(vcInstance * aIntsance,
            vcLogLevel   aLevel,
            vcLogRegion  aRegion,
            const char * aId,
            const void * aBuf,
            const size_t aLength);

const char *vcLogLevelToPrefixString(vcLogLevel aLogLevel);

#define _vcLogFormatter(aInstance, aLogLevel, aRegion, aFormat, ...) \
    _vcDynamicLog(aInstance, aLogLevel, aRegion, aFormat OPENVC_CONFIG_LOG_SUFFIX, ##__VA_ARGS__)

#if OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL == 1

#define _vcDynamicLog(aInstance, aLogLevel, aRegion, aFormat, ...)  \
    do                                                              \
    {                                                               \
        if (vcGetDynamicLogLevel(aInstance) >= aLogLevel)           \
            _vcPlatLog(aLogLevel, aRegion, aFormat, ##__VA_ARGS__); \
    } while (false)

#else // OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL

#define _vcDynamicLog(aInstance, aLogLevel, aRegion, aFormat, ...) \
    _vcPlatLog(aLogLevel, aRegion, aFormat, ##__VA_ARGS__)

#endif // OPENVC_CONFIG_ENABLE_DYNAMIC_LOG_LEVEL

#define _vcPlatLog(aLogLevel, aRegion, aFormat, ...) \
    OPENVC_CONFIG_PLAT_LOG_FUNCTION(aLogLevel, aRegion, aFormat, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // LOGGING_HPP_
