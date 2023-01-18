/* netdev abstraction layer */
#include <stdio.h>
#include <assert.h>
#include "netdev.h"
#include "plugin.h"

extern veth_init();
extern veth_exit();
extern veth_xmit();

struct list_head netdev_lists;

struct netdev* netdev_alloc(enum netdev_type type) {
    struct netdev* nd = malloc(sizeof(struct netdev));
    if (NULL == nd) {
        perror("malloc netdev");
        return NULL;
    }
    switch(type) {
        case LOOP:
            // nd->dev_ops.dev_init = loop_init;
            // nd->dev_ops.dev_exit = loop_exit;
            // nd->dev_ops.dev_xmit = loop_xmit;
            // break;
            return NULL;
        case VETH:
            nd->dev_ops.dev_init = veth_init;
            nd->dev_ops.dev_exit = veth_exit;
            nd->dev_ops.dev_xmit = veth_xmit;
            break;
        default:
            fprintf(stderr, "can't suppport type: %d\n", (int)type);
            free(nd);
            return NULL;
    }
    return nd;
}

void netdev_free(struct netdev* nd) {
    if (NULL != nd) {
        free(nd);
    }
}

int netdev_register(struct netdev* dev) {
    assert(NULL != dev && NULL != dev->dev_ops.dev_init);
    if (0 != dev->dev_ops.dev_init()) {
        fprintf(stderr, "dev_init err\n");
        return -1;
    }
    list_add(&dev->dev_list, &netdev_lists);
    return 0;
}

void netdev_deregister(struct netdev* dev) {
    assert(NULL != dev);
    if (NULL != dev->dev_ops.dev_exit) {
        dev->dev_ops.dev_exit();
    }
    list_del(&dev->dev_list);
}

int netdev_init(void) {
    list_init(&netdev_lists);
    return 0;
}

void netdev_exit(void){
    // struct netdev* entry;
    // struct netdev* next;
    // list_for_each_entry_safe(entry, next, &netdev_lists, dev_list) {
    //     netdev_deregister(entry);
    // }
    return 0;
}