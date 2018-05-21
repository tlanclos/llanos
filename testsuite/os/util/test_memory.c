#include <testsuite.h>
#include <llanos/types.h>
#include <llanos/util/memory.h>

static void test_memory_set_value__should__set_nothing_on_zero_length(void) {
    u8 data[] = {1, 2, 3, 4, 5};
    u8 expected[] = {1, 2, 3, 4, 5};

    memory_set_value(data, 100, 0);
    TEST_ASSERT_EQUAL_MEMORY(expected, data, sizeof(expected) / sizeof(u8));
}

static void test_memory_set_value__should__set_memory_starting_at_dest_pointer_for_length_bytes(void) {
    u8 data[] = {1, 2, 3, 4, 5};
    u8 expected[] = {1, 100, 100, 100, 5};

    memory_set_value(&data[1], 100, 3);
    TEST_ASSERT_EQUAL_MEMORY(expected, data, sizeof(expected) / sizeof(u8));
}

testfunc_container_t test_function_containers[] = {
    {"test_memory_set_value__should__set_nothing_on_zero_length", test_memory_set_value__should__set_nothing_on_zero_length},
    {"test_memory_set_value__should__set_memory_starting_at_dest_pointer_for_length_bytes", test_memory_set_value__should__set_memory_starting_at_dest_pointer_for_length_bytes}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
    return 0;
}