#ifndef OPENVC_CLI_H_
#define OPENVC_CLI_H_

#include <stdarg.h>
#include <stdint.h>

#include <openvc/instance.h>
#include <openvc/platform/logging.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vcCliCommand
{
    const char *mName;
    void (*mCommand)(int aArgc, char *aArgv[]);
} vcCliCommand;

typedef int (*vcCliCommandOuputCallback)(const char *aBuf, uint16_t aBufLength, void *aContext);

void vcCliUartInit(vcInstance *aInstance);

void vcCliSetUserCommands(const vcCliCommand *aUserCommands, uint8_t aLength);

void vcCliOutputBytes(const uint8_t *aBytes, uint8_t aLength);

void vcCliOutputFormat(const char *aFmt, ...);

void vcCliOutput(const char *aString, uint16_t aLength);

void vcCliAppendResult(vcError aError);

void vcCliPlatLogv(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, va_list aArgs);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_CLI_H_
