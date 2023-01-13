#include <stdio.h>
#include "plugin.h"

#define PLUGIN_NAME "sample_plugin"
struct param{
    int num1;
    int num2;
};

int pl_init(void) {
    printf("sample_plugin[pl_init]: success\n");
    return 0;
}

void pl_deinit(void) {
    printf("sample_plugin[pl_deinit]: success\n");
}

int pl_name(char* name) {
    snprintf(name, MAX_PLUGIN_NAME, PLUGIN_NAME);
    printf("sample_plugin[l_name]: success\n");
    return 0;
}

int pl_add(void* input, void* output) {
    if (NULL == input || NULL == output) {
        fprintf(stderr, "sample_plugin[pl_add]: input or output is NULL\n");
        return -1;
    }
    struct param* param = (struct param*)input;
    int* result = (int *)output;
    *result = param->num1 + param->num2;
    return 0;
}