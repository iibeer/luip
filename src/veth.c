/* netdev implementation layer */
#include <stdio.h>
#include <string.h>
#include "netdev.h"
#include "tap.h"

// #define FAKE_IPADDR 0x0100000a		/* 10.0.0.1 */
// #define FAKE_HWADDR "\x00\x34\x45\x67\x89\xab"
// #define FAKE_NETMASK 0x00ffffff		/* 255.255.255.0 */

#define FAKE_TAP_ADDR 0x0100000b	/* 10.0.0.2 */
#define FAKE_TAP_NETMASK 0x00ffffff	/* 255.255.255.0 */

struct tapdev {
    int fd;
    struct netdev* nd;
};

static struct tapdev* tapdev_alloc(struct netdev* nd)
{
    if (NULL == nd) {
        fprintf(stderr, "tap_dev_init param err\n");
        return NULL;
    }
    struct tapdev *tap = (struct tapdev*)malloc(sizeof(struct tapdev));
    if (NULL == td) {
        perror("malloc tapdev");
        return NULL;
    }
    tap->nd = nd;
    tap->fd = alloc_tap(tap->nd->dev_name);
    if (tap->fd < 0)
        goto free_tap;
    if (setpersist_tap(tap->fd) < 0)
        goto close_tap;
    /* set tap information */
    if (set_tap() < 0)
        goto close_tap;
    
// #ifndef CONFIG_TOP1
//     if (gethwaddr_tap(tap->fd, tap->nd.dev_hwaddr) < 0)
//         goto close_tap;
//     if (setipaddr_tap(tap->nd.dev_name, FAKE_TAP_ADDR) < 0)
//         goto close_tap;
//     if (getipaddr_tap(tap->nd.dev_name, &tap->nd.dev_ip) < 0)
//         goto close_tap;
//     if (setnetmask_tap(tap->nd.dev_name, FAKE_TAP_NETMASK) < 0)
//         goto close_tap;
//     if (setup_tap(tap->nd.dev_name) < 0)
//         goto close_tap;
// #endif
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


int veth_init(struct netdev* nd, char* name) {
    if (NULL == nd || NULL == name) {
        fprintf(stderr, "veth_init param err\n");
        return -1;
    }
    /* init tap: out network nic */
    if (tap_dev_init(nd) < 0) {
        fprintf(stderr, "cannot init tap device\n");
        return -1;
    }

    strncpy(nd->dev_name, name, MAX_NETDEV_NAME_SIZE);
    memcpy(nd->dev_hwaddr, FAKE_HWADDR, HWADDR_BYTE_SIZE);
    veth.dev_mtu = 1500;
    veth.dev_mtu = tap->nd.dev_mtu;
    veth.dev_mask = FAKE_NETMASK;
    veth.dev_ip = FAKE_IPADDR;
    list_init(&veth.dev_list);
    return 0;
}

void veth_exit(void) {
    return;
}

int veth_xmit(struct pkbuf* pkb) {
    //tap_xmit();
    return 0;
}