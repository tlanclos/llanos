#include <testsuite.h>
#include <llanos/types.h>
#include <llanos/limits.h>

static void test_cast_s64_to_u64__should__return_false_on_negative_values(void) {
    TEST_ASSERT_FALSE(cast_s64_to_u64(NULL, (s64)-1));
}

static void test_cast_s64_to_u64__should__return_true_if_can_fit(void) {
    TEST_ASSERT_TRUE(cast_s64_to_u64(NULL, (s64)U64MIN));
}

static void test_cast_s64_to_u64__should__cast_to_the_correct_value(void) {
    u64 value;

    cast_s64_to_u64(&value, S64MAX);

    TEST_ASSERT_EQUAL(S64MAX, value);
}

static void test_cast_u64_to_s64__should__return_false_on_value_too_large(void) {
    TEST_ASSERT_FALSE(cast_u64_to_s64(NULL, (u64)S64MAX + 1));
}

static void test_cast_u64_to_s64__should__return_true_on_value_can_fit(void) {
    TEST_ASSERT_TRUE(cast_u64_to_s64(NULL, (u64)S64MAX));
}

static void test_cast_u64_to_s64__should__cast_to_the_correct_value(void) {
    s64 value;

    cast_u64_to_s64(&value, S64MAX);

    TEST_ASSERT_EQUAL(S64MAX, value);
}

static void test_cast_s32_to_u32__should__return_false_on_negative_values(void) {
    TEST_ASSERT_FALSE(cast_s32_to_u32(NULL, (s32)-1));
}

static void test_cast_s32_to_u32__should__return_true_if_can_fit(void) {
    TEST_ASSERT_TRUE(cast_s32_to_u32(NULL, (s32)U32MIN));
}

static void test_cast_s32_to_u32__should__cast_to_the_correct_value(void) {
    u32 value;

    cast_s32_to_u32(&value, S32MAX);

    TEST_ASSERT_EQUAL(S32MAX, value);
}

static void test_cast_u32_to_s32__should__return_false_on_value_too_large(void) {
    TEST_ASSERT_FALSE(cast_u32_to_s32(NULL, (u32)S32MAX + 1));
}

static void test_cast_u32_to_s32__should__return_true_on_value_can_fit(void) {
    TEST_ASSERT_TRUE(cast_u32_to_s32(NULL, (u32)S32MAX));
}

static void test_cast_u32_to_s32__should__cast_to_the_correct_value(void) {
    s32 value;

    cast_u32_to_s32(&value, S32MAX);

    TEST_ASSERT_EQUAL(S32MAX, value);
}

static void test_cast_s16_to_u16__should__return_false_on_negative_values(void) {
    TEST_ASSERT_FALSE(cast_s16_to_u16(NULL, (s16)-1));
}

static void test_cast_s16_to_u16__should__return_true_if_can_fit(void) {
    TEST_ASSERT_TRUE(cast_s16_to_u16(NULL, (s16)U16MIN));
}

static void test_cast_s16_to_u16__should__cast_to_the_correct_value(void) {
    u16 value;

    cast_s16_to_u16(&value, S16MAX);

    TEST_ASSERT_EQUAL(S16MAX, value);
}


static void test_cast_u16_to_s16__should__return_false_on_value_too_large(void) {
    TEST_ASSERT_FALSE(cast_u16_to_s16(NULL, (u16)S16MAX + 1));
}

static void test_cast_u16_to_s16__should__return_true_on_value_can_fit(void) {
    TEST_ASSERT_TRUE(cast_u16_to_s16(NULL, (u16)S16MAX));
}

static void test_cast_u16_to_s16__should__cast_to_the_correct_value(void) {
    s16 value;

    cast_u16_to_s16(&value, S16MAX);

    TEST_ASSERT_EQUAL(S16MAX, value);
}

static void test_cast_s8_to_u8__should__return_false_on_negative_values(void) {
    TEST_ASSERT_FALSE(cast_s8_to_u8(NULL, (s8)-1));
}

static void test_cast_s8_to_u8__should__return_true_if_can_fit(void) {
    TEST_ASSERT_TRUE(cast_s8_to_u8(NULL, (s16)U8MIN));
}

static void test_cast_s8_to_u8__should__cast_to_the_correct_value(void) {
    u8 value;

    cast_s8_to_u8(&value, S8MAX);

    TEST_ASSERT_EQUAL(S8MAX, value);
}

static void test_cast_u8_to_s8__should__return_false_on_value_too_large(void) {
    TEST_ASSERT_FALSE(cast_u8_to_s8(NULL, (u8)S8MAX + 1));
}

static void test_cast_u8_to_s8__should__return_true_on_value_can_fit(void) {
    TEST_ASSERT_TRUE(cast_u8_to_s8(NULL, (u8)S8MAX));
}

static void test_cast_u8_to_s8__should__cast_to_the_correct_value(void) {
    s8 value;

    cast_u8_to_s8(&value, S8MAX);

    TEST_ASSERT_EQUAL(S8MAX, value);
}

testfunc_container_t test_function_containers[] = {
    {"test_cast_s64_to_u64__should__return_false_on_negative_values", test_cast_s64_to_u64__should__return_false_on_negative_values},
    {"test_cast_s64_to_u64__should__return_true_if_can_fit", test_cast_s64_to_u64__should__return_true_if_can_fit},
    {"test_cast_s64_to_u64__should__cast_to_the_correct_value", test_cast_s64_to_u64__should__cast_to_the_correct_value},

    {"test_cast_u64_to_s64__should__return_false_on_value_too_large", test_cast_u64_to_s64__should__return_false_on_value_too_large},
    {"test_cast_u64_to_s64__should__return_true_on_value_can_fit", test_cast_u64_to_s64__should__return_true_on_value_can_fit},
    {"test_cast_u64_to_s64__should__cast_to_the_correct_value", test_cast_u64_to_s64__should__cast_to_the_correct_value},

    {"test_cast_s32_to_u32__should__return_false_on_negative_values", test_cast_s32_to_u32__should__return_false_on_negative_values},
    {"test_cast_s32_to_u32__should__return_true_if_can_fit", test_cast_s32_to_u32__should__return_true_if_can_fit},
    {"test_cast_s32_to_u32__should__cast_to_the_correct_value", test_cast_s32_to_u32__should__cast_to_the_correct_value},

    {"test_cast_u32_to_s32__should__return_false_on_value_too_large", test_cast_u32_to_s32__should__return_false_on_value_too_large},
    {"test_cast_u32_to_s32__should__return_true_on_value_can_fit", test_cast_u32_to_s32__should__return_true_on_value_can_fit},
    {"test_cast_u32_to_s32__should__cast_to_the_correct_value", test_cast_u32_to_s32__should__cast_to_the_correct_value},
    
    {"test_cast_s16_to_u16__should__return_false_on_negative_values", test_cast_s16_to_u16__should__return_false_on_negative_values},
    {"test_cast_s16_to_u16__should__return_true_if_can_fit", test_cast_s16_to_u16__should__return_true_if_can_fit},
    {"test_cast_s16_to_u16__should__cast_to_the_correct_value", test_cast_s16_to_u16__should__cast_to_the_correct_value},

    {"test_cast_u16_to_s16__should__return_false_on_value_too_large", test_cast_u16_to_s16__should__return_false_on_value_too_large},
    {"test_cast_u16_to_s16__should__return_true_on_value_can_fit", test_cast_u16_to_s16__should__return_true_on_value_can_fit},
    {"test_cast_u16_to_s16__should__cast_to_the_correct_value", test_cast_u16_to_s16__should__cast_to_the_correct_value},
    
    {"test_cast_s8_to_u8__should__return_false_on_negative_values", test_cast_s8_to_u8__should__return_false_on_negative_values},
    {"test_cast_s8_to_u8__should__return_true_if_can_fit", test_cast_s8_to_u8__should__return_true_if_can_fit},
    {"test_cast_s8_to_u8__should__cast_to_the_correct_value", test_cast_s8_to_u8__should__cast_to_the_correct_value},

    {"test_cast_u8_to_s8__should__return_false_on_value_too_large", test_cast_u8_to_s8__should__return_false_on_value_too_large},
    {"test_cast_u8_to_s8__should__return_true_on_value_can_fit", test_cast_u8_to_s8__should__return_true_on_value_can_fit},
    {"test_cast_u8_to_s8__should__cast_to_the_correct_value", test_cast_u8_to_s8__should__cast_to_the_correct_value},
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}