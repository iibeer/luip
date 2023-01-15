#ifndef __NETIF_H
#define __NETIF_H

/* netdev interface layer */
#include "list.h"

extern struct netdev;
struct pkbuf {
    struct list_head pk_list;
    unsigned short pk_pro;      /* ethernet packet type ID */
    unsigned short pk_type;     /* packet hardware address type */
    int pk_len;
    int pk_refcnt;
    unsigned char pk_data[0];
} __attribute__((packed));

struct pkbuf* pkb_alloc();
void pkb_free(struct pkbuf* pkb);

int netif_init(void);
void netif_exit(void);

void netif_interrupt(void);
void netif_tx(struct netdev* nd, struct pkbuf* pkb, int len,
            unsigned proto, unsigned char* dst);

#endif