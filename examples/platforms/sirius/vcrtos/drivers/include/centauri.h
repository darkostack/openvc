#ifndef CENTAURI_H
#define CENTAURI_H

#include <stdint.h>

#include "thread.h"
#include "net/netdev.h"
#include "net/netdev/ieee802154.h"
#include "net/gnrc/nettype.h"

#include "board.h"
#include "periph/spi.h"
#include "periph/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CENTAURI_BUFFER_SIZE    (256)
#define CENTAURI_PAYLOAD_SIZE   (128)

#ifndef CENTAURI_DEFAULT_PARAMS
#define CENTAURI_DEFAULT_PARAMS { .spi = BOARD_CENTAURI_PARAM_SPI,          \
                                  .spi_clk = BOARD_CENTAURI_PARAM_SPI_CLK,  \
                                  .cs_pin = BOARD_CENTAURI_PARAM_SPI_CS,    \
                                  .int_pin = BOARD_CENTAURI_PARAM_INT,      \
                                  .reset_pin = BOARD_CENTAURI_PARAM_RESET }
#endif

typedef struct centauri_params {
    spi_t spi;
    spi_clk_t spi_clk;
    spi_cs_t cs_pin;
    gpio_t int_pin;
    gpio_t reset_pin;
} centauri_params_t;

typedef void (*cent_rxcmp_cb_t)(void *arg);

typedef struct {
    netdev_ieee802154_t netdev;
    centauri_params_t params;
    kernel_pid_t pid;
    unsigned alivestatus;
    uint32_t timestamp;
    int pktrssi;
    int ccathreshold;
    uint32_t pktlqi;
    uint8_t *rxdata;
    uint16_t rxindex;
    uint8_t *txdata;
    uint16_t txlen;
    uint16_t txindex;
    unsigned mactxcmp;
    unsigned phytxcmp;
    int16_t txpwr;
    uint32_t txfcs;
    uint32_t phrlen;
    unsigned irqlisten;
    unsigned rftest;
    union {
        cent_rxcmp_cb_t app;
        cent_rxcmp_cb_t rftest;
    } rxcmp;
} centauri_t;

void centauri_init(centauri_t *dev, const centauri_params_t *params);

void centauri_reset(centauri_t *dev);

void centauri_rx(centauri_t *dev);

void centauri_idle(centauri_t *dev);

void centauri_tx(centauri_t *dev, uint8_t *data, uint16_t length);

uint32_t centauri_get_freq(centauri_t *dev);

void centauri_set_freq(centauri_t *dev, uint32_t freq);

int centauri_get_cca_rssi(centauri_t *dev);

int centauri_is_receiving(centauri_t *dev);

extern centauri_t cent_dev;

extern const centauri_params_t cent_params[];

#ifdef __cplusplus
}
#endif

#endif /* CENTAURI_H */
