#include <testsuite.h>
#include <llanos/math.h>

static void test_min_on_left_of_2_integers__should__return_the_left_hand_size(void) {
    TEST_ASSERT_EQUAL(MIN(1, 2), 1);
}

static void test_min_on_right_of_2_integers__should__return_the_right_hand_size(void) {
    TEST_ASSERT_EQUAL(MIN(2, 1), 1);
}

static void test_max_on_left_of_2_integers__should__return_the_left_hand_size(void) {
    TEST_ASSERT_EQUAL(MAX(2, 1), 2);
}

static void test_max_on_right_of_2_integers__should__return_the_right_hand_size(void) {
    TEST_ASSERT_EQUAL(MAX(1, 2), 2);
}

static void test_min_on_left_of_2_float__should__return_the_left_hand_size(void) {
    TEST_ASSERT_EQUAL(MIN(1.1, 1.2), 1.1);
}

static void test_min_on_right_of_2_float__should__return_the_right_hand_size(void) {
    TEST_ASSERT_EQUAL(MIN(1.2, 1.1), 1.1);
}

static void test_max_on_left_of_2_float__should__return_the_left_hand_size(void) {
    TEST_ASSERT_EQUAL(MAX(2.2, 2.1), 2.2);
}

static void test_max_on_right_of_2_float__should__return_the_right_hand_size(void) {
    TEST_ASSERT_EQUAL(MAX(2.1, 2.2), 2.2);
}

static void test_range_init__should__set_start_value(void) {
    range_t range;

    range_init(&range, -100, 1000);

    TEST_ASSERT_EQUAL(range.start, -100);
}

static void test_range_init__should__set_end_value(void) {
    range_t range;

    range_init(&range, -10101, 2001);

    TEST_ASSERT_EQUAL(range.end, 2001);
}

static void test_range_init__should__set_span_value(void) {
    range_t range;

    range_init(&range, 2000, 2010);

    TEST_ASSERT_EQUAL(range.span, 10);
}

static void test_in_range__should__exclude_before_start(void) {
    range_t range;

    range_init(&range, -29, 100);

    TEST_ASSERT_FALSE(in_range(-30, &range));
}

static void test_in_range__should__include_start(void) {
    range_t range;

    range_init(&range, -30, 100);

    TEST_ASSERT_TRUE(in_range(-30, &range));
}

static void test_in_range__should__include_before_end(void) {
    range_t range;

    range_init(&range, -30, 100);

    TEST_ASSERT_TRUE(in_range(99, &range));
}

static void test_in_range__should__exclude_end(void) {
    range_t range;

    range_init(&range, -30, 100);

    TEST_ASSERT_FALSE(in_range(100, &range));
}

static void test_range_equal__should__not_equal_on_different_starts(void) {
    range_t range1;
    range_t range2;

    range_init(&range1, -1000, 400);
    range_init(&range2, -1001, 400);

    TEST_ASSERT_FALSE(range_equal(&range1, &range2));
}

static void test_range_equal__should__not_equal_on_different_ends(void) {
    range_t range1;
    range_t range2;

    range_init(&range1, -1000, 400);
    range_init(&range2, -1000, 401);

    TEST_ASSERT_FALSE(range_equal(&range1, &range2));
}

static void test_range_equal__should__equal_if_ranges_are_equal(void) {
    range_t range1;
    range_t range2;

    range_init(&range1, -1000, 400);
    range_init(&range2, -1000, 400);

    TEST_ASSERT_TRUE(range_equal(&range1, &range2));
}

static void test_range_intersection__should__return_false_on_no_intersection(void) {
    range_t first;
    range_t second;

    range_init(&first, -30, 100);
    range_init(&second, 100, 110);

    TEST_ASSERT_FALSE(range_intersection(NULL, &first, &second));
}

static void test_range_intersection__should__return_true_on_intersection(void) {
    range_t first;
    range_t second;

    range_init(&first, -30, 100);
    range_init(&second, 99, 110);

    TEST_ASSERT_TRUE(range_intersection(NULL, &first, &second));
}

static void test_range_intersection__should__get_intersection_of_lower_and_higher(void) {
    range_t intersection;
    range_t expected;
    range_t lower;
    range_t higher;

    range_init(&lower, -30, 100);
    range_init(&higher, 99, 110);
    range_intersection(&intersection, &lower, &higher);

    range_init(&expected, 99, 100);
    TEST_ASSERT_TRUE(range_equal(&expected, &intersection));
}

static void test_range_intersection__should__get_intersection_of_higher_and_lower(void) {
    range_t intersection;
    range_t expected;
    range_t lower;
    range_t higher;

    range_init(&lower, -30, 100);
    range_init(&higher, 99, 110);
    range_intersection(&intersection, &higher, &lower);

    range_init(&expected, 99, 100);
    TEST_ASSERT_TRUE(range_equal(&expected, &intersection));
}

static void test_range_intersection__should__get_intersection_of_inner_on_first(void) {
    range_t intersection;
    range_t expected;
    range_t inner;
    range_t outer;

    range_init(&inner, 0, 510);
    range_init(&outer, -1, 511);
    range_intersection(&intersection, &inner, &outer);

    range_init(&expected, 0, 510);
    TEST_ASSERT_TRUE(range_equal(&expected, &intersection));
}

static void test_range_intersection__should__get_intersection_of_inner_on_second(void) {
    range_t intersection;
    range_t expected;
    range_t inner;
    range_t outer;

    range_init(&inner, 0, 510);
    range_init(&outer, -1, 511);
    range_intersection(&intersection, &outer, &inner);

    range_init(&expected, 0, 510);
    TEST_ASSERT_TRUE(range_equal(&expected, &intersection));
}

static void test_range_intersection__should__set_span(void) {
    range_t intersection;
    range_t first;
    range_t second;

    range_init(&first, 0, 100);
    range_init(&second, 10, 110);
    range_intersection(&intersection, &first, &second);

    TEST_ASSERT_EQUAL(90, intersection.span);
}

static void test_range_join__should__return_false_on_not_joinable(void) {
    range_t first;
    range_t second;

    range_init(&first, 30, 100);
    range_init(&second, 101, 110);

    TEST_ASSERT_FALSE(range_join(NULL, &first, &second));
}

static void test_range_join__should__return_true_on_joinable(void) {
    range_t first;
    range_t second;

    range_init(&first, 30, 100);
    range_init(&second, 100, 110);

    TEST_ASSERT_TRUE(range_join(NULL, &first, &second));
}

static void test_range_join__should__get_join_of_lower_and_higher(void) {
    range_t join;
    range_t expected;
    range_t lower;
    range_t higher;

    range_init(&lower, -30, 100);
    range_init(&higher, 100, 110);
    range_init(&expected, -30, 110);

    range_join(&join, &lower, &higher);
    TEST_ASSERT_TRUE(range_equal(&expected, &join));
}

static void test_range_join__should__get_join_of_higher_and_lower(void) {
    range_t join;
    range_t expected;
    range_t lower;
    range_t higher;

    range_init(&lower, -30, 100);
    range_init(&higher, 100, 110);
    range_init(&expected, -30, 110);

    range_join(&join, &higher, &lower);
    TEST_ASSERT_TRUE(range_equal(&expected, &join));
}

static void test_range_join__should__choose_outer_on_outer_is_first(void) {
    range_t join;
    range_t expected;
    range_t inner;
    range_t outer;

    range_init(&inner, -30, 100);
    range_init(&outer, -31, 101);
    range_init(&expected, -31, 101);

    range_join(&join, &outer, &inner);
    TEST_ASSERT_TRUE(range_equal(&expected, &join));
}

static void test_range_join__should__choose_outer_on_outer_is_second(void) {
    range_t join;
    range_t expected;
    range_t inner;
    range_t outer;

    range_init(&inner, -30, 100);
    range_init(&outer, -31, 101);
    range_init(&expected, -31, 101);

    range_join(&join, &inner, &outer);
    TEST_ASSERT_TRUE(range_equal(&expected, &join));
}

static void test_range_join__should__set_span(void) {
    range_t join;
    range_t first;
    range_t second;

    range_init(&first, 0, 100);
    range_init(&second, 10, 110);
    range_join(&join, &first, &second);

    TEST_ASSERT_EQUAL(110, join.span);
}

static void test_divmod_u64__should__return_0_on_undefined_result(void) {
    TEST_ASSERT_EQUAL(0, divmod_u64(100, 0, NULL));
}

static void test_divmod_u64__should__return_0_on_0_over_value(void) {
    TEST_ASSERT_EQUAL(0, divmod_u64(0, 100, NULL));
}

static void test_divmod_u64__should__return_result_of_division(void) {
    TEST_ASSERT_EQUAL(7, divmod_u64(100, 13, NULL));
}

static void test_divmod_u64__should__set_result_of_modulus(void) {
    u64 rem;
    divmod_u64(100, 13, &rem);
    TEST_ASSERT_EQUAL(9, rem);
}

static void test_divmod_u64__should__return_1_if_numerator_and_denominator_are_equal(void) {
    TEST_ASSERT_EQUAL(1, divmod_u64(100, 100, NULL));
}

static void test_divmod_u64__should__set_remainder_equal_0_if_numerator_and_denominator_are_equal(void) {
    u64 rem;
    divmod_u64(100, 100, &rem);
    TEST_ASSERT_EQUAL(0, rem);
}

static void test_divmod_u64__should__handle_max_u64_division_result(void) {
    TEST_ASSERT_EQUAL(1, divmod_u64(
        0xffffffffffffffff,
        0xfffffffffffffffe,
        NULL
    ));
}

static void test_divmod_u64__should__handle_max_u64_modulus_result(void) {
    u64 rem;
    divmod_u64(0xffffffffffffffff, 0xfffffffffffffffe, &rem);
    TEST_ASSERT_EQUAL(1, rem);
}

static void test_divmod_u64__should__handle_numerator_lessthan_denominator_division_result(void) {
    TEST_ASSERT_EQUAL(0, divmod_u64(1, 10, NULL));
}

static void test_divmod_u64__should__handle_numerator_lessthan_denominator_modulus_result(void) {
    u64 rem;
    divmod_u64(1, 10, &rem);
    TEST_ASSERT_EQUAL(1, rem);
}

static void test_modulus_u64__should__return_0_on_undefined_result(void) {
    TEST_ASSERT_EQUAL(0, modulus_u64(10000, 0));
}

static void test_modulus_u64__should__return_result_of_modulus(void) {
    TEST_ASSERT_EQUAL(739, modulus_u64(10000, 1029));
}

static void test_modulus_u64__should__return_0_if_numerator_and_denominator_are_equal(void) {
    TEST_ASSERT_EQUAL(0, modulus_u64(10000, 10000));
}

static void test_modulus_u64__should__handle_max_u64_modulus_result(void) {
    TEST_ASSERT_EQUAL(1, modulus_u64(0xffffffffffffffff, 0xfffffffffffffffe));
}

static void test_modulus_u64__should__handle_numerator_lessthan_denominator_modulus_result(void) {
    TEST_ASSERT_EQUAL(1, modulus_u64(1, 10));
}

static void test_divide_u64__should__return_0_on_undefined_result(void) {
    TEST_ASSERT_EQUAL(0, divide_u64(10000, 0));
}

static void test_divide_u64__should__return_result_of_divide(void) {
    TEST_ASSERT_EQUAL(9, divide_u64(10000, 1029));
}

static void test_divide_u64__should__return_1_if_numerator_and_denominator_are_equal(void) {
    TEST_ASSERT_EQUAL(1, divide_u64(10000, 10000));
}

static void test_divide_u64__should__handle_max_u64_divide_result(void) {
    TEST_ASSERT_EQUAL(1, divide_u64(0xffffffffffffffff, 0xfffffffffffffffe));
}

static void test_divide_u64__should__handle_numerator_lessthan_denominator_divide_result(void) {
    TEST_ASSERT_EQUAL(0, divide_u64(1, 10));
}

testfunc_container_t test_function_containers[] = {
    {"min_on_left_of_2_integers should return the left hand size", test_min_on_left_of_2_integers__should__return_the_left_hand_size},
    {"min_on_right_of_2_integers should return the right hand size", test_min_on_right_of_2_integers__should__return_the_right_hand_size},

    {"max_on_left_of_2_integers should return the left hand size", test_max_on_left_of_2_integers__should__return_the_left_hand_size},
    {"max_on_right_of_2_integers should return the right hand size", test_max_on_right_of_2_integers__should__return_the_right_hand_size},

    {"min_on_left_of_2_float should return the left hand size", test_min_on_left_of_2_float__should__return_the_left_hand_size},
    {"min_on_right_of_2_float should return the right hand size", test_min_on_right_of_2_float__should__return_the_right_hand_size},

    {"max_on_left_of_2_float should return the left hand size", test_max_on_left_of_2_float__should__return_the_left_hand_size},
    {"max_on_right_of_2_float should return the right hand size", test_max_on_right_of_2_float__should__return_the_right_hand_size},

    {"range_init should set start value", test_range_init__should__set_start_value},
    {"range_init should set end value", test_range_init__should__set_end_value},
    {"range_init should set span value", test_range_init__should__set_span_value},

    {"in_range should exclude before start", test_in_range__should__exclude_before_start},
    {"in_range should include start", test_in_range__should__include_start},
    {"in_range should include before end", test_in_range__should__include_before_end},
    {"in_range should exclude end", test_in_range__should__exclude_end},

    {"range_equal should not equal on different starts", test_range_equal__should__not_equal_on_different_starts},
    {"range_equal should not equal on different ends", test_range_equal__should__not_equal_on_different_ends},
    {"range_equal should equal if ranges are equal", test_range_equal__should__equal_if_ranges_are_equal},

    {"range_intersection should return false on no intersection", test_range_intersection__should__return_false_on_no_intersection},
    {"range_intersection should return true on intersection", test_range_intersection__should__return_true_on_intersection},
    {"range_intersection should get intersection of lower and higher", test_range_intersection__should__get_intersection_of_lower_and_higher},
    {"range_intersection should get intersection of higher and lower", test_range_intersection__should__get_intersection_of_higher_and_lower},
    {"range_intersection should get intersection of inner on first", test_range_intersection__should__get_intersection_of_inner_on_first},
    {"range_intersection should get intersection of inner on second", test_range_intersection__should__get_intersection_of_inner_on_second},
    {"range_intersection should set span", test_range_intersection__should__set_span},

    {"range_join should return false on not joinable", test_range_join__should__return_false_on_not_joinable},
    {"range_join should return true on joinable", test_range_join__should__return_true_on_joinable},
    {"range_join should get join of lower and higher", test_range_join__should__get_join_of_lower_and_higher},
    {"range_join should get join of higher and lower", test_range_join__should__get_join_of_higher_and_lower},
    {"range_join should choose outer on outer is first", test_range_join__should__choose_outer_on_outer_is_first},
    {"range_join should choose outer on outer is second", test_range_join__should__choose_outer_on_outer_is_second},
    {"range_join should set span", test_range_join__should__set_span},

    {"divmod_u64 should return 0 on undefined result", test_divmod_u64__should__return_0_on_undefined_result},
    {"divmod_u64 should return 0 on 0 over value", test_divmod_u64__should__return_0_on_0_over_value},
    {"divmod_u64 should return result of division", test_divmod_u64__should__return_result_of_division},
    {"divmod_u64 should set result of modulus", test_divmod_u64__should__set_result_of_modulus},
    {"divmod_u64 should return 1 if numerator and denominator are equal", test_divmod_u64__should__return_1_if_numerator_and_denominator_are_equal},
    {"divmod_u64 should set remainder equal 0 if numerator and denominator are equal", test_divmod_u64__should__set_remainder_equal_0_if_numerator_and_denominator_are_equal},
    {"divmod_u64 should handle max u64 division result", test_divmod_u64__should__handle_max_u64_division_result},
    {"divmod_u64 should handle max u64 modulus result", test_divmod_u64__should__handle_max_u64_modulus_result},
    {"divmod_u64 should handle numerator < denominator division result", test_divmod_u64__should__handle_numerator_lessthan_denominator_division_result},
    {"divmod_u64 should handle numerator < denominator modulus result", test_divmod_u64__should__handle_numerator_lessthan_denominator_modulus_result},

    {"modulus_u64 should return 0 on undefined result", test_modulus_u64__should__return_0_on_undefined_result},
    {"modulus_u64 should return result of modulus", test_modulus_u64__should__return_result_of_modulus},
    {"modulus_u64 should return 0 if numerator and denominator are equal", test_modulus_u64__should__return_0_if_numerator_and_denominator_are_equal},
    {"modulus_u64 should handle max u64 modulus result", test_modulus_u64__should__handle_max_u64_modulus_result},
    {"modulus_u64 should handle numerator < denominator modulus result", test_modulus_u64__should__handle_numerator_lessthan_denominator_modulus_result},

    {"divide_u64 should return 0 on undefined result", test_divide_u64__should__return_0_on_undefined_result},
    {"divide_u64 should return result of divide", test_divide_u64__should__return_result_of_divide},
    {"divide_u64 should return 1 if numerator and denominator are equal", test_divide_u64__should__return_1_if_numerator_and_denominator_are_equal},
    {"divide_u64 should handle max u64 divide result", test_divide_u64__should__handle_max_u64_divide_result},
    {"divide_u64 should handle numerator < denominator divide result", test_divide_u64__should__handle_numerator_lessthan_denominator_divide_result}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}
