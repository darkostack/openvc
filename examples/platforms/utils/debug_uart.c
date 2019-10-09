#include <openvc-core-config.h>

#include <stdarg.h>
#include <stdio.h>
#include <openvc/config.h>

#include <openvc/platform/alarm-milli.h>
#include <openvc/platform/debug_uart.h>
#include <openvc/platform/toolchain.h>

VC_TOOL_WEAK
void vcPlatDebugUart_printf(const char *aFormat, ...)
{
    va_list ap;
    va_start(ap, aFormat);
    vcPlatDebugUart_vprintf(aFormat, ap);
    va_end(ap);
}

VC_TOOL_WEAK
void vcPlatDebugUart_vprintf(const char *aFormat, va_list ap)
{
    char buf[128];
    vsnprintf(buf, sizeof(buf), aFormat, ap);
    buf[sizeof(buf) - 1] = 0;
    vcPlatDebugUart_puts_no_nl(buf);
}

VC_TOOL_WEAK
void vcPlatDebugUart_write_bytes(const uint8_t *aBytes, int aNumBytes)
{
    while (aNumBytes > 0)
    {
        vcPlatDebugUart_putchar((int)(*aBytes));
        aBytes++;
        aNumBytes--;
    }
}

VC_TOOL_WEAK
void vcPlatDebugUart_puts_no_nl(const char *aString)
{
    while (*aString)
    {
        vcPlatDebugUart_putchar(*aString);
        aString++;
    }
}

VC_TOOL_WEAK
void vcPlatDebugUart_puts(const char *aString)
{
    vcPlatDebugUart_puts_no_nl(aString);
    vcPlatDebugUart_putchar('\n');
}

VC_TOOL_WEAK
void vcPlatDebugUart_putchar(int aChar)
{
    if (aChar == '\n')
    {
        vcPlatDebugUart_putchar_raw('\r');
    }

    vcPlatDebugUart_putchar_raw(aChar);
}

// provide WEAK stubs for platform that do not implement all these functions.

VC_TOOL_WEAK
void vcPlatDebugUart_putchar_raw(int aChar)
{
    VC_UNUSED_VARIABLE(aChar);
}

VC_TOOL_WEAK
int vcPlatDebugUart_kbhit(void)
{
    return 0;
}

VC_TOOL_WEAK
int vcPlatDebugUart_getc(void)
{
    return -1;
}

VC_TOOL_WEAK
vcError vcPlatDebugUart_logfile(const char *aFilename)
{
    VC_UNUSED_VARIABLE(aFilename);
    return VC_ERROR_FAILED;
}

#if (OPENVC_CONFIG_LOG_OUTPUT == OPENVC_CONFIG_LOG_OUTPUT_DEBUG_UART)
void vcPlatLog(vcLogLevel aLogLevel, vcLogRegion aLogRegion, const char *aFormat, ...)
{
    va_list ap;
    uint32_t now;

    VC_UNUSED_VARIABLE(aLogLevel);
    VC_UNUSED_VARIABLE(aLogRegion);

    now = vcPlatAlarmMilliGetNow();
    vcPlatDebugUart_printf("%3d.%03d | ", (int)(now / 1000), (int)(now % 1000));
    va_start(ap, aFormat);
    vcPlatDebugUart_vprintf(aFormat, ap);
    va_end(ap);

    vcPlatDebugUart_putchar('\n');
}
#endif
