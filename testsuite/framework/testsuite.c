#include <stdio.h>
#include <testsuite.h>

extern void testsuite_run_tests(const testsuite_t* testsuite) {
    int i;

    for (i = 0; i < testsuite->num_test_function_containers; i++) {
        printf("RUNNING %s...\n", testsuite->test_function_containers[i].name);
        testsuite->test_function_containers[i].test_function();
    }
}