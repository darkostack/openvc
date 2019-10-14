#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "centauri.h"
#include "centauri_prober.h"
#include "centauri_cfg.h"
#include "centauri_host.h"
#include "centauri_queue.h"
#include "centauri_cmd.h"
#include "centauri_mac.h"
#include "centauri_pmu_csr.h"
#include "centauri_timer.h"

#include "phy_csr.h"
#include "TransceiverInit.h"
#include "TransceiverCsr.h"
#include "vtprober.h"

#include "nvsets.h"

#define CENT_MSG_EVENT_IRQ (0x10)

centauri_t cent_dev;
const centauri_params_t cent_params[] = { CENTAURI_DEFAULT_PARAMS };

static uint8_t cent_rxbuf[CENTAURI_BUFFER_SIZE];

/* variable used globaly by centauri */
uint32_t rx_complete;
extern uint32_t bm_read_next;

static void cent_spi_acquire(centauri_t *dev);
static void cent_spi_release(centauri_t *dev);
static uint32_t cent_tx_prefill(uint8_t *data, uint16_t length);
static void cent_gpio_int_cb(void *arg);

void centauri_init(centauri_t *dev, const centauri_params_t *params)
{
    /* centauri initial parameter */
    dev->rxdata = (uint8_t *)&cent_rxbuf[0];
    dev->txfcs = FCS_MODE;
    dev->ccathreshold = -100; /* default cca threshold */
    dev->params = *params;

    /* centauri reset gpio */
    gpio_init(dev->params.reset_pin, GPIO_OUT);
    gpio_set(dev->params.reset_pin);

    /* centauri interrupt gpio */
    gpio_init_int(dev->params.int_pin, GPIO_IN, GPIO_BOTH, &cent_gpio_int_cb, (void *)dev);

    /* trigger centauri reset */
    centauri_reset(dev);

    /* initialize centauri spi interface */
    spi_init(dev->params.spi);
    vtprober_init();

    /* start listening to centauri gpio interrupt */
    dev->irqlisten = 1;

    /* stage 1 -------------------------------------------------------------- */

    cent_spi_acquire(dev);

    msg_queue_init();   /* initialize centauri msg queue interface */
    cent_if_config();   /* config spi delay & centauri side interrupt io */
    cent_cmd_status();  /* check centauri status */

    cent_spi_release(dev);

    cent_busy_wait(2000); /* give some delay to receive centauri alive status */

    /* stage 2 -------------------------------------------------------------- */

    cent_spi_acquire(dev);

    cent_misc_config(CENTAURI_PAYLOAD_SIZE + 4); /* payload + fcs */
    cent_radio_config(nvsets.cent.frequency);
    cent_cmd_init();
	RfLdo12vCOvREF(7);  //LDO output 1.3V
	FilterIfSetting();
    cent_cal_load();
    dev->txpwr = nvsets.cent.tx_power;
	/* set rssi config to avoid rx_rssi_ib_lvl transfer to dB-domain siturate */
	PhyInBandRssiCalcuShift(4); /* rx_rssi_ib_lvl[21:4] */
	cent_cmd_rssi_offset(6);
	CentauriCsrWrite(0x40028030, 0x00000000 | (3 << 8) );  /* CAP=3 */
	TransceiverDcocSetting();
    cent_cmd_cal_iq();
    cent_cmd_cal_32k();
    cent_cmd_cal_4m();
    cent_spi_writeW(CENT_PMU_ADDR(PMU_4M_TIMER_SR), 1012); /* (4.6Mhz*1.1*200us)=1012 */
    cent_spi_writeW(CENT_PMU_ADDR(PMU_CGCR), PMU_CGCR_IDLE);

    cent_cmd_set_rate(50000, 2);
    cent_cmd_set_modi(1000);
    cent_cmd_data_whitening(1);

    cent_spi_release(dev);

    /* stage 3 -------------------------------------------------------------- */

    centauri_set_freq(dev, nvsets.cent.frequency);
    centauri_rx(dev);

    BOARD_RXLED_OFF();
    BOARD_TXLED_OFF();
}

void centauri_reset(centauri_t *dev)
{
    gpio_clear(dev->params.reset_pin);
    cent_busy_wait(2000);
    gpio_set(dev->params.reset_pin);
    cent_busy_wait(10000);
}

void centauri_rx(centauri_t *dev)
{
    cent_spi_acquire(dev);
    if (cent_get_state() != SYS_RX) {
        msg_queue_clear();
        dev->rxindex = 0;
        cent_cmd_rx(SYS_RX);
    }
    cent_spi_release(dev);
}

void centauri_idle(centauri_t *dev)
{
    cent_spi_acquire(dev);
    if (cent_get_state() != SYS_IDLE) {
        cent_cmd_idle();
    }
    cent_spi_release(dev);
}

void centauri_tx(centauri_t *dev, uint8_t *data, uint16_t length)
{
    assert(length <= CENTAURI_PAYLOAD_SIZE);

    cent_spi_acquire(dev);

    if (cent_get_state() != SYS_IDLE) {
        cent_cmd_idle();
    }

    dev->txdata = data;
    dev->txlen = length;
    dev->txindex = 0;

    dev->txindex = cent_tx_prefill(dev->txdata, dev->txlen);
    dev->mactxcmp = 0;
    dev->phytxcmp = 0;

    BOARD_TXLED_ON();

    msg_queue_clear();

    cent_cmd_tx(dev->txfcs, dev->txlen);

    cent_spi_release(dev);

    uint32_t timeout = cent_usec_now() + 100000; /* 100 ms */
    while (!dev->phytxcmp && timeout > cent_usec_now());

    if (dev->phytxcmp != 1 && timeout < cent_usec_now()) {
        /* Note: for some reason the phytxcmp event is not generated but we
         * must get mactxcmp event instead */
        assert(dev->mactxcmp == 1);
    }

    BOARD_TXLED_OFF();
}

uint32_t centauri_get_freq(centauri_t *dev)
{
    cent_spi_acquire(dev);
    uint32_t freq = cent_cmd_read_channel();
    cent_spi_release(dev);
    return freq;
}

void centauri_set_freq(centauri_t *dev, uint32_t freq)
{
    cent_spi_acquire(dev);
    if (cent_get_version() == 0x7000b3) {
        RomChannelInvalidSet(0); /* workaround the invalid bit not getting back issue */
        cent_cmd_set_channel(freq);
        assert(!RomChannelInvalidRead());
    }
    cent_spi_release(dev);
}

int centauri_get_cca_rssi(centauri_t *dev)
{
    cent_spi_acquire(dev);
    assert(cent_get_state() == SYS_RX);
    int rssi = (int)cent_cmd_rssi();
    cent_spi_release(dev);
    return rssi;
}

int centauri_is_receiving(centauri_t *dev)
{
    cent_spi_acquire(dev);
    int status = cent_get_receiving_status();
    cent_spi_release(dev);
    return status;
}

/* private functions implementation ----------------------------------------- */

static void cent_spi_acquire(centauri_t *dev)
{
    spi_acquire(dev->params.spi, dev->params.cs_pin, SPI_MODE_0, SPI_CLK_1MHZ);
}

static void cent_spi_release(centauri_t *dev)
{
    spi_release(dev->params.spi);
}

static uint32_t cent_tx_prefill(uint8_t *data, uint16_t length)
{
    return cent_mac_tx_prefill(data, length);
}

static void cent_rxcmp_packet_process(centauri_t *dev)
{
    if (dev->rxindex == 3) {
        if (!cent_is_ack()) {
            return;
        }
    }
    if (dev->rftest) {
        if (dev->rxcmp.rftest != NULL) {
            dev->rxcmp.rftest((void *)dev);
        }
    } else {
        if (dev->rxcmp.app != NULL) {
            dev->rxcmp.app((void *)dev);
        }
    }
}

static void cent_handle_event_mac_rxcmp(centauri_t *dev)
{
    /* fcs should match */
    if (!cent_is_fcs_match()) {
        goto exit;
    }

    uint8_t fcs_size = (cent_fcs_type() == FCS_32) ? 4 : 2;
    uint32_t packet_len = cent_get_pkt_len();
    uint32_t rxindex = dev->rxindex;

    /* phr length originaly should always equal with packet length */
    if (dev->phrlen != packet_len) {
        /* for some reason we miss phr interrupt especially in small packet, so
         * make sure the phr_len was 0 instead */
        assert(dev->phrlen == 0);
    }

    /* exclude fcs byte in received length */
    if ((packet_len - rxindex) < fcs_size) {
        uint32_t offset;
        offset = packet_len - rxindex;
        rxindex -= fcs_size - offset;
        dev->rxindex = rxindex;
    }

    /* make sure packet length is always bigger than fcs size */
    if (packet_len < fcs_size) {
        goto exit;
    }

    uint32_t remain_len = packet_len - rxindex - fcs_size;

    if (remain_len == 0) {
        cent_rxcmp_packet_process(dev);
        goto exit;
    }

    /* make sure received packet is less than the max buffer size */
    assert((remain_len + rxindex) < CENTAURI_BUFFER_SIZE);

    /* receive remain frame */
    if (remain_len >= (MAC_BUF_SIZE / 2)) {
        if (remain_len == (MAC_BUF_SIZE / 2)) {
            rx_complete = 1;
        }
        uint8_t received_frame = (packet_len - rxindex) / (MAC_BUF_SIZE / 2);
        for (uint8_t i = 0; i < received_frame; i++) {
            rxindex += cent_rxHi_receive((uint8_t *)&dev->rxdata[rxindex], (MAC_BUF_SIZE / 2));
            remain_len = packet_len - rxindex - fcs_size;
        }
    }

    /* make sure rxindex is correct */
    assert((packet_len - fcs_size) == (rxindex + remain_len));

    /* get the rest of the bytes */
    if ((remain_len != 0) && (remain_len < (MAC_BUF_SIZE / 2))) {
        rx_complete = 1;
        rxindex += cent_rxHi_receive((uint8_t *)&dev->rxdata[rxindex], remain_len);
        remain_len = packet_len - rxindex - fcs_size;
    }

    /* update received index */
    dev->rxindex = rxindex;

    if (remain_len == 0) {
        cent_rxcmp_packet_process(dev);
    }

exit:
    /* reset rx parameter */
    bm_read_next = 0;
    rx_complete = 0;
    dev->rxindex = 0;
    dev->phrlen = 0;

    msg_queue_clear();
}

static void cent_handle_event_mac(centauri_t *dev, RF_CMD_T *event)
{
    if (event->param & ISR_TXB_LOTHR) {
        uint32_t txindex = dev->txindex;
        assert(txindex <= dev->txlen);
        if (txindex < dev->txlen) {
            txindex += cent_txlo_transfer((uint8_t *)&dev->txdata[txindex], (dev->txlen - txindex));
            dev->txindex = txindex;
        }
    }

    if (event->param & ISR_TXE_TXCMP) {
        dev->mactxcmp = 1;
    }

    if (event->param & ISR_RXB_HITHR) {
        uint32_t rxindex = dev->rxindex;
        assert(rxindex < CENTAURI_BUFFER_SIZE);
        rxindex += cent_rxHi_receive((uint8_t *)&dev->rxdata[rxindex], (MAC_BUF_SIZE / 2));
        assert(rxindex < CENTAURI_BUFFER_SIZE);
        dev->rxindex = rxindex;
    }

    if (event->param & ISR_RXE_RXCMP) {
        BOARD_RXLED_ON();
        cent_handle_event_mac_rxcmp(dev);
        BOARD_RXLED_OFF();
    }
}

static void cent_handle_event_phy(centauri_t *dev, RF_CMD_T *event)
{
    if (event->param & PHY_IRQ_STATUS__RG_PHY_TX_IRQ_MASK) {
        dev->phytxcmp = 1;
    }

    if (event->param & PHY_IRQ_STATUS__RG_PHY_RX_PMB_IRQ_MASK) {
        if (dev->rxindex != 0) {
            dev->rxindex = 0;
        }
    }

    if (event->param & PHY_IRQ_STATUS__RG_PHY_RX_PHR_IRQ_MASK) {
        dev->phrlen = (CentauriCsrRead(PHY_CSR__RX_DEFRAMER_PHR_DATA) & 0x07ff0000) >> 16;
    }
}

static void cent_handle_event_status(centauri_t *dev, RF_CMD_T *event)
{
    assert(event->param & 0x5a5a);
    dev->alivestatus = 1;
}

static void cent_handle_event_irq(centauri_t *dev)
{
    cent_spi_acquire(dev);

    if (!have_cent_event()) {
        get_msg_queue();
        if (!have_cent_event()) {
            goto exit;
        }
    }

    RF_CMD_T *event = (RF_CMD_T *)get_cent_event();
    assert(event != NULL);

    switch (event->cmd) {
    case RF_EVENT_MAC_IRQn:
        cent_handle_event_mac(dev, event);
        break;
    case RF_EVENT_PHY_IRQn:
        cent_handle_event_phy(dev, event);
        break;
    case RF_EVENT_STATUS:
        cent_handle_event_status(dev, event);
        break;
    default:
        break;
    }

exit:
    cent_spi_release(dev);
}

static void cent_gpio_int_cb(void *arg)
{
    centauri_t *dev = (centauri_t *)arg;
    if (dev->irqlisten) {
        msg_t msg;
        msg.type = CENT_MSG_EVENT_IRQ;
        msg.content.ptr = (void *)dev;
        msg_try_send(&msg, dev->pid);
    }
}

void *centauri_thread(void *arg)
{
    (void) arg;

    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            if (msg.type == CENT_MSG_EVENT_IRQ) {
                centauri_t *dev = (centauri_t *)msg.content.ptr;
                cent_handle_event_irq(dev);
            }
        }
    }

    return NULL;
}
