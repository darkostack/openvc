#include "logging.hpp"

#include "common/instance.hpp"

#if (OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_DEBUG_UART) && (!OPENVC_CONFIG_ENABLE_DEBUG_UART)
#error OPENVC_CONFIG_ENABLE_DEBUG_UART_LOG requires OPENVC_CONFIG_ENABLE_DEBUG_UART
#endif

#define vcLogDump(aFormat, ...) \
    _vcDynamicLog(aInstance, aLogLevel, aLogRegion, aFormat OPENVC_CONFIG_LOG_SUFFIX, ##__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

#if OPENVC_CONFIG_LOG_PKT_DUMP == 1
void vcDump(vcInstance * aIntsance,
            vcLogLevel   aLevel,
            vcLogRegion  aRegion,
            const char * aId,
            const void * aBuf,
            const size_t aLength)
{
    size_t       idlen = strlen(aId);
    const size_t width = 72;
    char         buf[80];
    char *       cur = buf;

    for (size_t i = 0; i < (width - idlen) / 2 - 5; i++)
    {
        snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "=");
        cur += strlen(cur);
    }

    snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "[%s len=%03u]", aId, static_cast<uint16_t>(aLength));
    cur += strlen(cur);

    for (size_t i = 0; i < (width - idlen) / 2 - 4; i++)
    {
        snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "=");
        cur += strlen(cur);
    }

    vcLogDump("%s", buf);

    for (size_t i = 0; i < aLength; i += 16)
    {
        DumpLine(aInstance, aLogLevel, aLogRegion, (uint8_t *)(aBuf) + i, (aLength - i) < 16 ? (aLength - i) : 16);
    }

    cur = buf;

    for (size_t i = 0; i < width; i++)
    {
        snprintf(cur, sizeof(buf) - static_cast<size_t>(cur - buf), "-");
        cur += strlen(cur);
    }

    vcLogDump("%s", buf);
}
#else
void vcDump(vcInstance *, vcLogLevel, vcLogRegion, const char *, const void *, const size_t)
{
}
#endif // OPENVC_CONFIG_LOG_PKT_DUMP == 1

const char *vcErrorToString(vcError aError)
{
    const char *retval;

    switch (aError)
    {
    case VC_ERROR_NONE:
        retval = "OK";
        break;

    case VC_ERROR_FAILED:
        retval = "Failed";
        break;

    case VC_ERROR_DROP:
        retval = "Drop";
        break;

    case VC_ERROR_NO_BUFS:
        retval = "NoBufs";
        break;

    case VC_ERROR_BUSY:
        retval = "Busy";
        break;

    case VC_ERROR_PARSE:
        retval = "Parse";
        break;

    case VC_ERROR_INVALID_ARGS:
        retval = "InvalidArgs";
        break;

    case VC_ERROR_SECURITY:
        retval = "Security";
        break;

    case VC_ERROR_ABORT:
        retval = "Abort";
        break;

    case VC_ERROR_NOT_IMPLEMENTED:
        retval = "NotImplemented";
        break;

    case VC_ERROR_INVALID_STATE:
        retval = "InvalidState";
        break;

    case VC_ERROR_NOT_FOUND:
        retval = "NotFound";
        break;

    case VC_ERROR_ALREADY:
        retval = "Already";
        break;

    case VC_ERROR_NOT_CAPABLE:
        retval = "NotCapable";
        break;

    case VC_ERROR_RESPONSE_TIMEOUT:
        retval = "ResponseTimeout";
        break;

    case VC_ERROR_DISABLED_FEATURE:
        retval = "DisabledFeature";
        break;

    case VC_ERROR_GENERIC:
        retval = "GenericError";
        break;

    default:
        retval = "UnknownErrorType";
        break;
    }

    return retval;
}

const char *vcLogLevelToPrefixString(vcLogLevel aLogLevel)
{
    const char *retval = "";

    switch (aLogLevel)
    {
    case VC_LOG_LEVEL_NONE:
        retval = _VC_LEVEL_NONE_PREFIX;
        break;

    case VC_LOG_LEVEL_CRIT:
        retval = _VC_LEVEL_CRIT_PREFIX;
        break;

    case VC_LOG_LEVEL_WARN:
        retval = _VC_LEVEL_WARN_PREFIX;
        break;

    case VC_LOG_LEVEL_NOTE:
        retval = _VC_LEVEL_NOTE_PREFIX;
        break;

    case VC_LOG_LEVEL_INFO:
        retval = _VC_LEVEL_INFO_PREFIX;
        break;

    case VC_LOG_LEVEL_DEBG:
        retval = _VC_LEVEL_DEBG_PREFIX;
        break;
    }

    return retval;
}

#if OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_NONE
/* this provides a stub, incase something uses the function */
void vcPlatLog(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, ...)
{
    VC_UNUSED_VARIABLE(aLogLevel);
    VC_UNUSED_VARIABLE(aLogRegion);
    VC_UNUSED_VARIABLE(aFormat);
}
#endif

#ifdef __cplusplus
}
#endif
