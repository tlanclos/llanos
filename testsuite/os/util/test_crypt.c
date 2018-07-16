#include <testsuite.h>
#include <llanos/util/crypt.h>

static void test_crc32__should__on_empty_string_return_0xffffffff(void) {
    TEST_ASSERT_EQUAL_UINT32(0xffffffff, crc32((u8*)"", 0));
}

static void test_crc32__should__on_words_return_crc32_jamcrc(void) {
    TEST_ASSERT_EQUAL_UINT32(0x83bd9e78, crc32((u8*)"1234abcd!@#$", 12));
}

static void test_crc32str__should__on_empty_string_return_0xffffffff(void) {
    TEST_ASSERT_EQUAL_UINT32(0xffffffff, crc32str(""));
}

static void test_crc32str__should__on_words_return_crc32_jamcrc(void) {
    TEST_ASSERT_EQUAL_UINT32(0xe38007d5, crc32str("$$123AbCd"));
}

testfunc_container_t test_function_containers[] = {
    {"crc32 should return 0xffffffff on empty string", test_crc32__should__on_empty_string_return_0xffffffff},
    {"crc32 should return crc32 jamcrc on words", test_crc32__should__on_words_return_crc32_jamcrc},
    {"crc32str should return 0xffffffff on empty string", test_crc32str__should__on_empty_string_return_0xffffffff},
    {"crc32str should return crc32 jamcrc on words", test_crc32str__should__on_words_return_crc32_jamcrc}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
    return 0;
}