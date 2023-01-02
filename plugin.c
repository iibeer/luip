#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <dirent.h>

#include "plugin.h"

#define MAX_PLUGIN_SETS_NUM 128
#define PER_SET_MAX_PLUGIN_SIZE 128

struct plugin_sets_list {
    int psl_sets_num;
    struct list_head psl_ps_list;
};
static struct plugin_sets_list s_psl;

typedef int (*pl_init)(void);
typedef void (*pl_deinit)(void);
typedef int (*pl_name)(char* name);
typedef int (*pl_func)(void* input, void* output);

void __attribute__((constructor)) psl_init() {
    s_psl.psl_sets_num = 0;
    list_init(&s_psl.psl_ps_list);
}
static struct plugin* plugin_init(struct plugin_set* ps, const char* pl_so_path) {
    assert(NULL != pl_so_path);
    assert(NULL != ps);

    struct plugin* pl;
    pl_init pl_init_fun = NULL;
    pl_name pl_name_fun = NULL;

    pl = (struct plugin*)malloc(sizeof(struct plugin));
    if (NULL == pl) {
        perror("malloc err: ");
        return NULL;
    }
    pl->pl_handle = dlopen(pl_so_path, RTLD_LAZY);
    if (NULL == pl) {
        perror("dlopen err: ");
        free(pl);
        return NULL;
    }

    /* call plugin init func */
    pl_init_fun = dlsym(pl->pl_handle, "pl_init");
    if (NULL == pl_init_fun) {
        perror("dlsym init err: ");
        free(pl);
        return NULL;
    }
    if (0 != pl_init_fun()) {
        perror("pl_init_fun err: ");
        free(pl);
        return NULL;
    }

    /* get plugin name */
    pl_name_fun = dlsym(pl->pl_handle, "pl_name");
    if (NULL == pl_name_fun) {
        perror("dlsym init err: ");
        free(pl);
        return NULL;
    }
    if (0 != pl_name_fun(pl->pl_name)) {
        perror("pl_name_fun err: ");
        free(pl);
        return NULL;
    }

    /* add plugin to plugin_set */
    list_add(&pl->pl_node, &ps->pl_list);
    return pl;
}

static void plugin_deinit(struct plugin* pl) {
    assert(NULL != pl);

    /* call plugin deinit fun */
    pl_deinit pl_deinit_fun = NULL;
    pl_deinit_fun = dlsym(pl->pl_handle, "pl_deinit");
    if (NULL == pl_deinit_fun) {
        perror("dlsym deinit err: ");
        return;
    }

    pl_deinit_fun();
    dlclose(pl->pl_handle);
    /* del plugin from plugin_set */
    list_del(&pl->pl_node);
    free(pl);
}

struct plugin_set* ps_init(const char* dir) {
    if (NULL == dir) {
        fprintf(stderr, "dir is NULL\n");
        return NULL;
    }

    if (s_psl.psl_sets_num >= MAX_PLUGIN_SETS_NUM) {
        fprintf(stderr, "plugin_set is full\n");
        return NULL;
    } 

    DIR* dirp;
    struct dirent* entry;
    char* file_type;
    char file[1024] = {0};
    struct plugin_set* ps;
    struct plugin *pl;
    ps = (struct plugin_set*)malloc(sizeof(struct plugin_set));
    if (NULL == ps) {
        perror("malloc err: ");
        return NULL;
    }

    /* find all the dynamic libraries in the dir directory */
    ps->pl_size = 0;
    list_init(&ps->pl_list);
    dirp = opendir(dir);
    if (NULL == dirp) {
        perror("opendir err: ");
        free(ps);
        return NULL;
    }
    do {
        if (ps->pl_size >= PER_SET_MAX_PLUGIN_SIZE) {
            break;
        }
        entry = readdir(dirp);
        if (NULL == entry) {
            break;
        }
        if (DT_REG != entry->d_type) {
            continue;
        }
        snprintf(file, 1024, "%s/%s", dir, entry->d_name);
        file_type = strchr(file, '.');
        if (0 != strcmp(file_type, ".so")) {
            continue;
        }
        pl = plugin_init(ps, file);
        if (NULL == pl) {
            continue;
        }
        ps->pl_size++;
    } while(1);

    /* add plugin_set to s_pls */
    list_add(&ps->ps_node, &s_psl.psl_ps_list);
    s_psl.psl_sets_num++;
    return ps;
}

void ps_deinit(struct plugin_set* ps) {
    if (NULL == ps) {
        return;
    }
    struct plugin* pl;
    ps_walk(ps, pl) {
        plugin_deinit(pl);
    }
    list_del(&ps->ps_node);
    s_psl.psl_sets_num--;
    free(ps);
}

int pl_call_fun(struct plugin* pl, const char* fun_name, void* input, void* output) {
    if (NULL == pl || NULL == fun_name || NULL == input || NULL == output) {
        fprintf(stderr, "pl_call_fun: param err!\n");
        return -1;
    }
    if (NULL == pl->pl_handle) {
        fprintf(stderr, "pl->pl_handle is NULL\n");
        return -2;
    }

    pl_func func;
    func = dlsym(pl->pl_handle, fun_name);
    if (NULL == func) {
        perror("dlsym func err: ");
        return -3;
    }
    return func(input, output);
}
