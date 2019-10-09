#ifndef OPENVC_PLATFORM_UART_H_
#define OPENVC_PLATFORM_UART_H_

#include <stdint.h>

#include <openvc/error.h>

#ifdef __cplusplus
extern "C" {
#endif

vcError vcPlatUartEnable(void);

vcError vcPlatUartDisable(void);

vcError vcPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength);

extern void vcPlatUartSendDone(void);

extern void vcPlatUartReceived(const uint8_t *aBuf, uint16_t aBufLength);

#ifdef __cplusplus
}
#endif

#endif // OPENVC_PLATFORM_UART_H_
