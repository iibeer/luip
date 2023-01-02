#if 0
#include "uni_test.h"
#include <stdlib.h>

int main(void) {
    int n;
    SRunner *sr;
    sr = srunner_create(make_add_suite()); // 将Suite加入到SRunner
    srunner_run_all(sr, CK_NORMAL);
    n = srunner_ntests_failed(sr);         // 运行所有测试用例
    srunner_free(sr);
    return (n == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
#endif

#include <stdio.h>
#include "netif.h"
#include "plugin.h"
int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "./test plugin_dir\n");
        return -1;
    }

    char* dir = argv[1];
    struct plugin_set* ps;
    ps = ps_init(dir);
    if (NULL == ps) {
        perror("ps_init err: ");
        return -1;
    }

    struct plugin* pl;
    char* func_name = "pl_add";
    struct {
        int num1;
        int num2;
    } param;
    param.num1 = 1;
    param.num2 = 2;
    int add_result;
    int ret;
    ps_walk(ps, pl) {
        ret = pl_call_fun(pl, func_name, (void*)&param, (void*)&add_result);
        if (0 != ret) {
            fprintf(stderr, "pl_call_fun err\n");
            return -1;
        }
    }
    printf("add_result: %d\n", add_result);

    ps_deinit(ps);
    return 0;
}