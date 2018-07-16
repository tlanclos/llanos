#include <testsuite.h>
#include <llanos/limits.h>

static void test_u64_max__should__equal_max_u64(void) {
    TEST_ASSERT_EQUAL_UINT64((u64)0xffffffffffffffff, U64MAX);
}

static void test_u32_max__should__equal_max_u32(void) {
    TEST_ASSERT_EQUAL_UINT32((u32)0xffffffff, U32MAX);
}

static void test_u16_max__should__equal_max_u16(void) {
    TEST_ASSERT_EQUAL_UINT16((u16)0xffff, U16MAX);
}

static void test_u8_max__should__equal_max_u8(void) {
    TEST_ASSERT_EQUAL_UINT8((u8)0xff, U8MAX);
}

static void test_u64_min__should__equal_min_u64(void) {
    TEST_ASSERT_EQUAL_UINT64(0, U64MIN);
}

static void test_u32_min__should__equal_min_u32(void) {
    TEST_ASSERT_EQUAL_UINT32(0, U32MIN);
}

static void test_u16_min__should__equal_min_u16(void) {
    TEST_ASSERT_EQUAL_UINT16(0, U16MIN);
}

static void test_u8_min__should__equal_min_u8(void) {
    TEST_ASSERT_EQUAL_UINT8(0, U8MIN);
}

static void test_s64_max__should__equal_max_s64(void) {
    TEST_ASSERT_EQUAL_INT64((s64)0x7fffffffffffffff, S64MAX);
}

static void test_s32_max__should__equal_max_s32(void) {
    TEST_ASSERT_EQUAL_INT32((s32)0x7fffffff, S32MAX);
}

static void test_s16_max__should__equal_max_s16(void) {
    TEST_ASSERT_EQUAL_INT16((s16)0x7fff, S16MAX);
}

static void test_s8_max__should__equal_max_s8(void) {
    TEST_ASSERT_EQUAL_INT8((s8)0x7f, S8MAX);
}

static void test_s64_min__should__equal_min_s64(void) {
    TEST_ASSERT_EQUAL_INT64((s64)0x8000000000000000, S64MIN);
}

static void test_s32_min__should__equal_min_s32(void) {
    TEST_ASSERT_EQUAL_INT32((s32)0x80000000, S32MIN);
}

static void test_s16_min__should__equal_min_s16(void) {
    TEST_ASSERT_EQUAL_INT16((s16)0x8000, S16MIN);
}

static void test_s8_min__should__equal_min_s8(void) {
    TEST_ASSERT_EQUAL_INT8((s8)0x80, S8MIN);
}

testfunc_container_t test_function_containers[] = {
    {"u64_max should equal max u64", test_u64_max__should__equal_max_u64},
    {"u32_max should equal max u32", test_u32_max__should__equal_max_u32},
    {"u16_max should equal max u16", test_u16_max__should__equal_max_u16},
    {"u8_max should equal max u8", test_u8_max__should__equal_max_u8},

    {"u64_min should equal min u64", test_u64_min__should__equal_min_u64},
    {"u32_min should equal min u32", test_u32_min__should__equal_min_u32},
    {"u16_min should equal min u16", test_u16_min__should__equal_min_u16},
    {"u8_min should equal min u8", test_u8_min__should__equal_min_u8},

    {"s64_max should equal max s64", test_s64_max__should__equal_max_s64},
    {"s32_max should equal max s32", test_s32_max__should__equal_max_s32},
    {"s16_max should equal max s16", test_s16_max__should__equal_max_s16},
    {"s8_max should equal max s8", test_s8_max__should__equal_max_s8},

    {"s64_min should equal min s64", test_s64_min__should__equal_min_s64},
    {"s32_min should equal min s32", test_s32_min__should__equal_min_s32},
    {"s16_min should equal min s16", test_s16_min__should__equal_min_s16},
    {"s8_min should equal min s8", test_s8_min__should__equal_min_s8}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}