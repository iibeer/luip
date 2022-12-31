#ifndef __NETIF_H
#define __NETIF_H

#include "list.h"

#define MAX_NETDEV_NAME_SIZE    16
#define HWADDR_BYTE_SIZE        6

struct netdev;

struct pkbuf {
    struct list_head pk_list;
    unsigned short pk_pro;      /* ethernet packet type ID */
    unsigned short pk_type;     /* packet hardware address type */
    int pk_len;
    int pk_refcnt;
    unsigned char pk_data[0];
} __attribute__((packed));

struct netdev_ops {
    int (*dev_init)(struct netdev* dev);
    void (*dev_exit)(struct netdev* dev);
    int (*dev_xmit)(struct netdev* dev, struct pkbuf* pkb);
};

struct netdev {
    unsigned char dev_name[MAX_NETDEV_NAME_SIZE];
    unsigned char dev_hwaddr[HWADDR_BYTE_SIZE];
    unsigned int dev_mtu;
    unsigned int dev_ip;
    unsigned int dev_mask;
    struct netdev_ops dev_ops;
    struct list_head dev_list;
};

struct pkbuf* pkb_alloc();
void pkb_free(struct pkbuf* pkb);

// struct netdev* netdev_alloc(unsigned char* dev_name, struct netdev_ops* dev_ops);
// void netdev_free(struct netdev* dev);

int netdev_register(struct netdev* dev);
void netdev_deregister(struct netdev* dev);

int netdev_init(struct netdev* dev);
void netdev_exit(struct netdev* dev);

#endif