#include "vtprober.h"
#include "centauri_host.h"
#include "centauri_prober.h"
#include "centauri_mac.h"
#include "phy_csr.h"
#include "mac_csr.h"
#include "TransceiverMain.h"

static uint32_t Tx_loThr = (MAC_BUF_SIZE / 2);
static uint32_t Rx_HiThr = (MAC_BUF_SIZE / 2);

uint32_t bm_write_next;
uint32_t bm_read_next;

uint8_t cent_is_ack(void)
{
    uint32_t mhr;
    vtprober_read_w_delay(MAC_RX_BUF_ADDR, &mhr);
    if ((mhr & 0x7) == 0x2) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t cent_mac_tx_prefill(uint8_t *data, uint32_t length)
{
    uint32_t *pData = (uint32_t *)data;

    vtprober_set_base_addr(MAC_TX_BUF_ADDR);

    if (length > MAC_TXBUF_LEN) {
        length = MAC_TXBUF_LEN;
    }

    for (uint32_t i = 0; i < length; i += 4) {
        vtprober_write_w_indirect(i, *pData);
        pData++;
    }

    bm_write_next = length;

    if (bm_write_next >= MAC_TXBUF_LEN) {
        bm_write_next = 0;
    }

    return length;
}

uint32_t cent_txlo_transfer(uint8_t *data, uint32_t length)
{
    uint32_t write_len;
    uint32_t buf_empty_size;

    uint32_t *pData = (uint32_t *)data;

    buf_empty_size = MAC_TXBUF_LEN - Tx_loThr;

    if (length > buf_empty_size) {
        write_len = buf_empty_size;
    } else {
        write_len = length;
    }

    vtprober_set_base_addr(MAC_TX_BUF_ADDR);

    for (uint32_t i = 0; i < write_len; i+= 4) {
        vtprober_write_w_indirect(bm_write_next, *pData);
        pData++;
        bm_write_next += 4;
        if (bm_write_next >= MAC_TXBUF_LEN) {
            bm_write_next = 0;
        }
    }

    // update tx buffer size
    if (write_len){
        vtprober_write_w(MAC_BM_TXBSR, (BM_TXBSR_SWSIZE_KICK_MASK | (write_len << BM_TXBS_SWSIZE_POS)));
    }

    return write_len;
}

extern uint32_t rx_complete;

uint32_t cent_rxHi_receive(uint8_t *data, uint32_t len)
{
    uint32_t *pData = (uint32_t*)data;
    uint32_t read_len;

    if (len > Rx_HiThr) {
        read_len = Rx_HiThr;
    } else {
        read_len = len;
    }

    vtprober_set_base_addr(MAC_RX_BUF_ADDR);

    for (uint32_t i = 0; i < read_len; i += 4) {
        vtprober_read_w_indirect_delay(bm_read_next, pData);
        pData++;
        bm_read_next += 4;
        if (bm_read_next >= MAC_RXBUF_LEN) {
            bm_read_next = 0;
        }
    }

    if (rx_complete == 0) {
    	vtprober_write_w(MAC_BM_RXBSR, (BM_RXBSR_SWSIZE_KICK_MASK | (read_len << BM_RXBSR_SWSIZE_POS) | MAC_RXBUF_LEN));
    } else {
		rx_complete = 0;
    }

    return read_len;
}

uint8_t cent_is_fcs_match(void)
{
    uint32_t packet_status;
    packet_status = cent_get_pkt_sr();
    if (packet_status & SPC_PKT_SR__CSR_FCS_MATCH_MASK) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t cent_fcs_type(void)
{
    uint32_t packet_status;
	packet_status = cent_get_pkt_sr();
    if (packet_status & SPC_PKT_SR__CSR_IS_FCS32_MASK) {
        return FCS_32;
    } else {
        return FCS_16;
    }
}
