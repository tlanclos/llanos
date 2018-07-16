#include <testsuite.h>
#include <llanos/util/string.h>

static void test_string_length__should__return_zero_on_empty_string(void) {
    TEST_ASSERT_EQUAL(0, string_length(""));
}

static void test_string_length__should__return_length_of_string(void) {
    TEST_ASSERT_EQUAL(14, string_length("abcteststring!"));
}

static void test_string_length__should__stop_on_null_character(void) {
    TEST_ASSERT_EQUAL(4, string_length("TEST\0this is not accounted for"));
}

testfunc_container_t test_function_containers[] = {
    {"string_length should return 0 on empty string", test_string_length__should__return_zero_on_empty_string},
    {"string_length should return length of string", test_string_length__should__return_length_of_string},
    {"string_length should stop on null character", test_string_length__should__stop_on_null_character},
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}