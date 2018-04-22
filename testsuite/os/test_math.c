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

testfunc_container_t test_function_containers[] = {
    {"test_min_on_left_of_2_integers__should__return_the_left_hand_size", test_min_on_left_of_2_integers__should__return_the_left_hand_size},
    {"test_min_on_right_of_2_integers__should__return_the_right_hand_size", test_min_on_right_of_2_integers__should__return_the_right_hand_size},
    
    {"test_max_on_left_of_2_integers__should__return_the_left_hand_size", test_max_on_left_of_2_integers__should__return_the_left_hand_size},
    {"test_max_on_right_of_2_integers__should__return_the_right_hand_size", test_max_on_right_of_2_integers__should__return_the_right_hand_size},
    
    {"test_min_on_left_of_2_float__should__return_the_left_hand_size", test_min_on_left_of_2_float__should__return_the_left_hand_size},
    {"test_min_on_right_of_2_float__should__return_the_right_hand_size", test_min_on_right_of_2_float__should__return_the_right_hand_size},
    
    {"test_max_on_left_of_2_float__should__return_the_left_hand_size", test_max_on_left_of_2_float__should__return_the_left_hand_size},
    {"test_max_on_right_of_2_float__should__return_the_right_hand_size", test_max_on_right_of_2_float__should__return_the_right_hand_size},

    {"test_range_init__should__set_start_value", test_range_init__should__set_start_value},
    {"test_range_init__should__set_end_value", test_range_init__should__set_end_value},
    {"test_range_init__should__set_span_value", test_range_init__should__set_span_value},

    {"test_in_range__should__exclude_before_start", test_in_range__should__exclude_before_start},
    {"test_in_range__should__include_start", test_in_range__should__include_start},
    {"test_in_range__should__include_before_end", test_in_range__should__include_before_end},
    {"test_in_range__should__exclude_end", test_in_range__should__exclude_end},

    {"test_range_equal__should__not_equal_on_different_starts", test_range_equal__should__not_equal_on_different_starts},
    {"test_range_equal__should__not_equal_on_different_ends", test_range_equal__should__not_equal_on_different_ends},
    {"test_range_equal__should__equal_if_ranges_are_equal", test_range_equal__should__equal_if_ranges_are_equal},

    {"test_range_intersection__should__return_false_on_no_intersection", test_range_intersection__should__return_false_on_no_intersection},
    {"test_range_intersection__should__return_true_on_intersection", test_range_intersection__should__return_true_on_intersection},
    {"test_range_intersection__should__get_intersection_of_lower_and_higher", test_range_intersection__should__get_intersection_of_lower_and_higher},
    {"test_range_intersection__should__get_intersection_of_higher_and_lower", test_range_intersection__should__get_intersection_of_higher_and_lower},
    {"test_range_intersection__should__get_intersection_of_inner_on_first", test_range_intersection__should__get_intersection_of_inner_on_first},
    {"test_range_intersection__should__get_intersection_of_inner_on_second", test_range_intersection__should__get_intersection_of_inner_on_second},
    {"test_range_intersection__should__set_span", test_range_intersection__should__set_span},

    {"test_range_join__should__return_false_on_not_joinable", test_range_join__should__return_false_on_not_joinable},
    {"test_range_join__should__return_true_on_joinable", test_range_join__should__return_true_on_joinable},
    {"test_range_join__should__get_join_of_lower_and_higher", test_range_join__should__get_join_of_lower_and_higher},
    {"test_range_join__should__get_join_of_higher_and_lower", test_range_join__should__get_join_of_higher_and_lower},
    {"test_range_join__should__choose_outer_on_outer_is_first", test_range_join__should__choose_outer_on_outer_is_first},
    {"test_range_join__should__choose_outer_on_outer_is_second", test_range_join__should__choose_outer_on_outer_is_second},
    {"test_range_join__should__set_span", test_range_join__should__set_span},
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}