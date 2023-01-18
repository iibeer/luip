/* netdev abstraction layer */
#ifndef __NETDEV_H
#define __NETDEV_H
#include "list.h"

#define MAX_NETDEV_NAME_SIZE    16
#define HWADDR_BYTE_SIZE        6

struct netdev;

enum netdev_type {
    LOOP = 0,
    VETH = 1,
};

struct netdev_ops {
    int (*dev_init)(void);
    void (*dev_exit)(void);
    int (*dev_xmit)(struct pkbuf* pkb);
};

struct netdev {
    char dev_name[MAX_NETDEV_NAME_SIZE];
    unsigned char dev_hwaddr[HWADDR_BYTE_SIZE];
    unsigned int dev_mtu;
    unsigned int dev_ip;
    unsigned int dev_mask;
    struct netdev_ops dev_ops;
    struct list_head dev_list;
};

int netdev_init(void);
void netdev_exit(void);

struct netdev* netdev_alloc(enum netdev_type type);
void netdev_free(struct netdev* nd);
int netdev_register(struct netdev* dev);
void netdev_deregister(struct netdev* dev);

void netdev_interrupt(void);
void netdev_tx(struct netdev* nd, struct pkbuf* pkb, int len,
        unsigned proto, unsigned char* dst);
#endif