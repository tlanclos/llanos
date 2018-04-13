#include <stdint.h>
#include <stddef.h>
#include <testsuite.h>
#include <llanos/management/memory.h>

void test_memory_bss_clear__should_not__segfault_on_zero_length(void) {
    memory_bss_clear(NULL, 0);
}

void test_memory_bss_clear__should_not__segfault_on_zero_length_invalid_pointer(void) {
    memory_bss_clear((uint8_t*)123456, 0);
}

void test_memory_bss_clear__should_not__segfault_on_zero_length_valid_pointer(void) {
    uint8_t mem[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    memory_bss_clear(mem, 0);
    TEST_ASSERT_EQUAL_MEMORY(mem, mem, sizeof(mem));
}

void test_memory_bss_clear__should__clear_memory(void) {
    uint8_t mem[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t mem_expected[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    memory_bss_clear(mem, sizeof(mem));
    TEST_ASSERT_EQUAL_MEMORY(mem, mem_expected, sizeof(mem_expected));
}

int main(void) {
    const testfunc_container_t test_function_containers[] = {
        {"test_memory_bss_clear__should_not__segfault_on_zero_length", test_memory_bss_clear__should_not__segfault_on_zero_length},
        {"test_memory_bss_clear__should_not__segfault_on_zero_length_invalid_pointer", test_memory_bss_clear__should_not__segfault_on_zero_length_invalid_pointer},
        {"test_memory_bss_clear__should_not__segfault_on_zero_length_valid_pointer", test_memory_bss_clear__should_not__segfault_on_zero_length_valid_pointer},
        {"test_memory_bss_clear__should__clear_memory", test_memory_bss_clear__should__clear_memory}
    };
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
    return 0;
}
