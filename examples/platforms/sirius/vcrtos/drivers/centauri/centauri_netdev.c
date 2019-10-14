#include <string.h>
#include <assert.h>
#include <errno.h>

#include "net/netdev.h"
#include "net/netdev/ieee802154.h"

#include "centauri.h"

#define ENABLE_DEBUG (0)
#include "debug.h"

static int _send(netdev_t *netdev, const iolist_t *iolist);
static int _recv(netdev_t *netdev, void *buf, size_t len, void *info);
static int _init(netdev_t *netdev);
static void _isr(netdev_t *netdev);
static int _get(netdev_t *netdev, netopt_t opt, void *val, size_t max_len);
static int _set(netopt_t *netdev, netopt_t opt, const void *val, size_t len);

const netdev_driver_t centauri_driver = {
    .send = _send,
    .recv = _recv,
    .init = _init,
    .isr = _isr,
    .get = _get,
    .set = _set,
};
