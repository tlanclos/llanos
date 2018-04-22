#pragma once

#include <unity.h>

typedef struct testsuite_s testsuite_t;
typedef struct testfunc_container_s testfunc_container_t;
typedef void (*testfunc_t)(void);

struct testfunc_container_s {
    const char* name;
    testfunc_t test_function;
};

struct testsuite_s {
    const testfunc_container_t* test_function_containers;
    int num_test_function_containers;
};

/**
 * @brief Run a testsuite of functions.
 *
 * @param testsuite testsuite of functions.
 */
extern void testsuite_run_tests(const testsuite_t* testsuite);
