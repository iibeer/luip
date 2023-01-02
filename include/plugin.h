#ifndef __PLUGIN_H
#define __PLUGIN_H

#include "list.h"
#define MAX_PLUGIN_NAME 16

struct plugin {
    char pl_name[MAX_PLUGIN_NAME];
    void* pl_handle;
    struct list_head pl_node;
};

struct plugin_set {
    int pl_size;
    struct list_head pl_list;
    struct list_head ps_node; 
};

/* ps function */
struct plugin_set* ps_init(const char* dir);
void ps_deinit(struct plugin_set* ps);
#define ps_walk(ps, pl)\
    struct plugin* next;\
    list_for_each_entry_safe(pl, next, &ps->pl_list, pl_node)


/* pl function */
struct plugin* pl_get_by_name(struct plugin_set* ps, const char* name);
int pl_call_fun(struct plugin* pl, const char* fun_name, void* param, void* result);
#endif