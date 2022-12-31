#include <stdio.h>
#include "netif.h"
#include "plugin.h"

struct list_head s_netdevs;

static struct netdev* netdev_alloc(unsigned char* dev_name, struct netdev_ops* dev_ops) {
    return NULL;
}

static void netdev_free(struct netdev* dev) {
    return;
}

int netdev_register(struct netdev* dev) {
    return 0;
}

void netdev_deregister(struct netdev* dev) {
    return;
}

int netdev_init(struct netdev* dev) {
    return 0;
}

void netdev_exit(struct netdev* dev){
    return;
}
