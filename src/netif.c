
/* netdev interface layer */
#include "netif.h"
#include "netdev.h"

extern struct netdev veth;
//extern struct netdev loop;

struct pkbuf* pkb_alloc() {
    return NULL;
}

void pkb_free(struct pkbuf* pkb) {
    return;
}

int netif_init(void) {
    //netdev_init
    //register veth loop
    return 0;
}

void netif_exit(void) {
    //deregister veth loop
    //netdev_deinit
    return 0;
}

void netif_interrupt(void) {
    //call netdev_interrupt()
    return;
}

void netif_tx(struct netdev* nd, struct pkbuf* pkb, int len,
        unsigned proto, unsigned char* dst) {
    //call netdev_tx()
    return;
}