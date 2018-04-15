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

testfunc_container_t test_function_containers[] = {
    {"test_min_on_left_of_2_integers__should__return_the_left_hand_size", test_min_on_left_of_2_integers__should__return_the_left_hand_size},
    {"test_min_on_right_of_2_integers__should__return_the_right_hand_size", test_min_on_right_of_2_integers__should__return_the_right_hand_size},
    {"test_max_on_left_of_2_integers__should__return_the_left_hand_size", test_max_on_left_of_2_integers__should__return_the_left_hand_size},
    {"test_max_on_right_of_2_integers__should__return_the_right_hand_size", test_max_on_right_of_2_integers__should__return_the_right_hand_size},
    {"test_min_on_left_of_2_float__should__return_the_left_hand_size", test_min_on_left_of_2_float__should__return_the_left_hand_size},
    {"test_min_on_right_of_2_float__should__return_the_right_hand_size", test_min_on_right_of_2_float__should__return_the_right_hand_size},
    {"test_max_on_left_of_2_float__should__return_the_left_hand_size", test_max_on_left_of_2_float__should__return_the_left_hand_size},
    {"test_max_on_right_of_2_float__should__return_the_right_hand_size", test_max_on_right_of_2_float__should__return_the_right_hand_size}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}