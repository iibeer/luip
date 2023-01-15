/* netdev abstraction layer */
#include <stdio.h>
#include <assert.h>
#include "netdev.h"
#include "plugin.h"

struct list_head netdev_lists;

struct netdev* netdev_alloc(void) {
    return NULL;
}

void netdev_free(void) {
    return;
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