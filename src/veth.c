/* netdev implementation layer */
#include <string.h>
#include "netdev.h"
#include "tap.h"

#define FAKE_IPADDR 0x0100000a		/* 10.0.0.1 */
#define FAKE_HWADDR "\x00\x34\x45\x67\x89\xab"
#define FAKE_NETMASK 0x00ffffff		/* 255.255.255.0 */

#define FAKE_TAP_ADDR 0x0100000b	/* 10.0.0.2 */
#define FAKE_TAP_NETMASK 0x00ffffff	/* 255.255.255.0 */


static int dev_init(void);
static void dev_exit(void);
static int dev_xmit(struct pkbuf* pkb);

struct tapdev {
    int fd;
    struct netdev nd;
};

struct tapdev *tap;
struct netdev veth = {
    .dev_ops.dev_init = dev_init,
    .dev_ops.dev_exit = dev_exit,
    .dev_ops.dev_xmit = dev_xmit
};

/* Altough dev is already created, this function is safe! */
static int tap_dev_init(void)
{
    tap = xmalloc(sizeof(*tap));
    tap->fd = alloc_tap("tap0");
    if (tap->fd < 0)
        goto free_tap;
    if (setpersist_tap(tap->fd) < 0)
        goto close_tap;
    /* set tap information */
    set_tap();
    getname_tap(tap->fd, tap->nd.dev_name);
    getmtu_tap(tap->nd.dev_name, &tap->nd.dev_mtu);
#ifndef CONFIG_TOP1
    gethwaddr_tap(tap->fd, tap->nd.dev_hwaddr);
    setipaddr_tap(tap->nd.dev_name, FAKE_TAP_ADDR);
    getipaddr_tap(tap->nd.dev_name, &tap->nd.dev_ip);
    setnetmask_tap(tap->nd.dev_name, FAKE_TAP_NETMASK);
    setup_tap(tap->nd.dev_name);
#endif
    unset_tap();
    /* Dont add tap device into local net device list */
    list_init(&tap->nd.dev_list);
    return 0;

close_tap:
    close(tap->fd);
free_tap:
    free(tap);
    return -1;
}


static int dev_init(void) {
    /* init tap: out network nic */
    if (tap_dev_init() < 0)
        perrx("Cannot init tap device");

    strncpy(veth.dev_name, "veth", MAX_NETDEV_NAME_SIZE);
    memcpy(veth.dev_hwaddr, FAKE_HWADDR, HWADDR_BYTE_SIZE);
    veth.dev_mtu = 1500;
    veth.dev_mtu = tap->nd.dev_mtu;
    veth.dev_mask = FAKE_NETMASK;
    veth.dev_ip = FAKE_IPADDR;
    list_init(&veth.dev_list);
    return 0;
}

static void dev_exit(void) {
    return;
}

static int dev_xmit(struct pkbuf* pkb) {
    //tap_xmit();
    return 0;
}