#ifndef OPENVC_PLATFORM_DEBUG_UART_H_
#define OPENVC_PLATFORM_DEBUG_UART_H_

#include <openvc/error.h>
#include <openvc/platform/logging.h>

#if __cplusplus
extern "C" {
#endif

void vcPlatDebugUart_printf(const char *aFormat, ...);

void vcPlatDebugUart_vprintf(const char *aFormat, va_list ap);

void vcPlatDebugUart_putchar_raw(int aChar);

int vcPlatDebugUart_kbhit(void);

int vcPlatDebugUart_getc(void);

void vcPlatDebugUart_putchar(int aChar);

void vcPlatDebugUart_puts(const char *aString);

void vcPlatDebugUart_write_bytes(const uint8_t *aBytes, int aNumBytes);

void vcPlatDebugUart_puts_no_nl(const char *aString);

vcError vcPlatDebugUart_logfile(const char *aFilename);

#if __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_DEBUG_UART_H_
