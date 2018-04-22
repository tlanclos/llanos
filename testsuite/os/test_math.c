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

static void test_in_range__should__exclude_before_start(void) {
    const range_t range = {
        .start = -29,
        .end = 100
    };

    TEST_ASSERT_FALSE(in_range(-30, &range));
}

static void test_in_range__should__include_start(void) {
    const range_t range = {
        .start = -30,
        .end = 100
    };
    
    TEST_ASSERT_TRUE(in_range(-30, &range));
}

static void test_in_range__should__include_before_end(void) {
    const range_t range = {
        .start = -30,
        .end = 100
    };
    
    TEST_ASSERT_TRUE(in_range(99, &range));
}

static void test_in_range__should__exclude_end(void) {
    const range_t range = {
        .start = -30,
        .end = 100
    };
    
    TEST_ASSERT_FALSE(in_range(100, &range));
}

static void test_range_intersection__should__return_false_on_no_intersection(void) {
    const range_t first = {
        .start = -30,
        .end = 100
    };
    const range_t second = {
        .start = 100,
        .end = 110
    };

    TEST_ASSERT_FALSE(range_intersection(NULL, &first, &second));
}

static void test_range_intersection__should__return_true_on_intersection(void) {
    const range_t first = {
        .start = -30,
        .end = 100
    };
    const range_t second = {
        .start = 99,
        .end = 110
    };

    TEST_ASSERT_TRUE(range_intersection(NULL, &first, &second));
}

static void test_range_intersection__should__get_intersection_of_lower_and_higher(void) {
    range_t intersection;
    const range_t lower = {
        .start = -30,
        .end = 100
    };
    const range_t higher = {
        .start = 99,
        .end = 110
    };

    range_intersection(&intersection, &lower, &higher);

    TEST_ASSERT_EQUAL(99, intersection.start);
    TEST_ASSERT_EQUAL(100, intersection.end);
}

static void test_range_intersection__should__get_intersection_of_higher_and_lower(void) {
    range_t intersection;
    const range_t higher = {
        .start = 99,
        .end = 110
    };
    const range_t lower = {
        .start = -30,
        .end = 100
    };

    range_intersection(&intersection, &higher, &lower);

    TEST_ASSERT_EQUAL(99, intersection.start);
    TEST_ASSERT_EQUAL(100, intersection.end);
}

static void test_range_intersection__should__get_intersection_of_inner_on_first(void) {
    range_t intersection;
    const range_t inner = {
        .start = 0,
        .end = 510
    };
    const range_t outer = {
        .start = -1,
        .end = 511
    };

    range_intersection(&intersection, &inner, &outer);

    TEST_ASSERT_EQUAL(0, intersection.start);
    TEST_ASSERT_EQUAL(510, intersection.end);
}

static void test_range_intersection__should__get_intersection_of_inner_on_second(void) {
    range_t intersection;
    const range_t inner = {
        .start = 0,
        .end = 510
    };
    const range_t outer = {
        .start = -1,
        .end = 511
    };

    range_intersection(&intersection, &outer, &inner);

    TEST_ASSERT_EQUAL(0, intersection.start);
    TEST_ASSERT_EQUAL(510, intersection.end);
}

static void test_range_join__should__return_false_on_not_joinable(void) {
    const range_t first = {
        .start = 30,
        .end = 100
    };
    const range_t second = {
        .start = 101,
        .end = 110
    };

    TEST_ASSERT_FALSE(range_join(NULL, &first, &second));
}

static void test_range_join__should__return_true_on_joinable(void) {
    const range_t first = {
        .start = 30,
        .end = 100
    };
    const range_t second = {
        .start = 100,
        .end = 110
    };

    TEST_ASSERT_TRUE(range_join(NULL, &first, &second));
}

static void test_range_join__should__get_join_of_lower_and_higher(void) {
    range_t join;
    const range_t lower = {
        .start = -30,
        .end = 100
    };
    const range_t higher = {
        .start = 100,
        .end = 110
    };

    range_join(&join, &lower, &higher);
    TEST_ASSERT_EQUAL(-30, join.start);
    TEST_ASSERT_EQUAL(110, join.end);
}

static void test_range_join__should__get_join_of_higher_and_lower(void) {
    range_t join;
    const range_t lower = {
        .start = -30,
        .end = 100
    };
    const range_t higher = {
        .start = 100,
        .end = 110
    };

    range_join(&join, &higher, &lower);
    TEST_ASSERT_EQUAL(-30, join.start);
    TEST_ASSERT_EQUAL(110, join.end);
}

static void test_range_join__should__choose_outer_on_outer_is_first(void) {
    range_t join;
    const range_t inner = {
        .start = -30,
        .end = 100
    };
    const range_t outer = {
        .start = -31,
        .end = 101
    };

    range_join(&join, &outer, &inner);
    TEST_ASSERT_EQUAL(-31, join.start);
    TEST_ASSERT_EQUAL(101, join.end);
}

static void test_range_join__should__choose_outer_on_outer_is_second(void) {
    range_t join;
    const range_t inner = {
        .start = -30,
        .end = 100
    };
    const range_t outer = {
        .start = -31,
        .end = 101
    };

    range_join(&join, &inner, &outer);
    TEST_ASSERT_EQUAL(-31, join.start);
    TEST_ASSERT_EQUAL(101, join.end);
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

    {"test_in_range__should__exclude_before_start", test_in_range__should__exclude_before_start},
    {"test_in_range__should__include_start", test_in_range__should__include_start},
    {"test_in_range__should__include_before_end", test_in_range__should__include_before_end},
    {"test_in_range__should__exclude_end", test_in_range__should__exclude_end},

    {"test_range_intersection__should__return_false_on_no_intersection", test_range_intersection__should__return_false_on_no_intersection},
    {"test_range_intersection__should__return_true_on_intersection", test_range_intersection__should__return_true_on_intersection},
    {"test_range_intersection__should__get_intersection_of_lower_and_higher", test_range_intersection__should__get_intersection_of_lower_and_higher},
    {"test_range_intersection__should__get_intersection_of_higher_and_lower", test_range_intersection__should__get_intersection_of_higher_and_lower},
    {"test_range_intersection__should__get_intersection_of_inner_on_first", test_range_intersection__should__get_intersection_of_inner_on_first},
    {"test_range_intersection__should__get_intersection_of_inner_on_second", test_range_intersection__should__get_intersection_of_inner_on_second},

    {"test_range_join__should__return_false_on_not_joinable", test_range_join__should__return_false_on_not_joinable},
    {"test_range_join__should__return_true_on_joinable", test_range_join__should__return_true_on_joinable},
    {"test_range_join__should__get_join_of_lower_and_higher", test_range_join__should__get_join_of_lower_and_higher},
    {"test_range_join__should__get_join_of_higher_and_lower", test_range_join__should__get_join_of_higher_and_lower},
    {"test_range_join__should__choose_outer_on_outer_is_first", test_range_join__should__choose_outer_on_outer_is_first},
    {"test_range_join__should__choose_outer_on_outer_is_second", test_range_join__should__choose_outer_on_outer_is_second},
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}