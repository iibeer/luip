#ifndef __PLUGIN_H
#define __PLUGIN_H

#include "list.h"
#define MAX_PLUGIN_NAME 16

/*插件，每个so对应一个插件*/
struct plugin {
    char pl_name[MAX_PLUGIN_NAME];
    void* pl_handle;
    struct list_head pl_node;
};

/*插件集，一个插件集包含多个插件*/
struct plugin_set {
    int pl_size;
    struct list_head pl_list;
    struct list_head ps_node; 
};

struct plugin_set* ps_init(const char* dir);
void ps_deinit(struct plugin_set* ps);
/*
例子：
    struct plugin_set ps,
    ...
    struct plugin* pl;
    ps_walk(&ps, pl) {
        printf("pl->pl_name: %s", pl->pl_name);
    }
*/
#define ps_walk(ps, pl)\
    struct plugin* next;\
    list_for_each_entry_safe(pl, next, &ps->pl_list, pl_node)

int pl_call_fun(struct plugin* pl, const char* fun_name, void* param, void* result);
#endif