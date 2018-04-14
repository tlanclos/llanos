#include <testsuite.h>
#include <llanos/video/vga.h>

static void test_vga_get_default_terminal_width__should__return_80(void) {
    TEST_ASSERT_EQUAL(vga_get_default_terminal_width(), 80);
}

static void test_vga_get_default_terminal_height__should__return_25(void) {
    TEST_ASSERT_EQUAL(vga_get_default_terminal_height(), 25);
}

static void test_vga_get_default_buffer_address__should__return_0xB8000(void) {
    TEST_ASSERT_EQUAL_PTR(vga_get_default_buffer_address(), (uint16_t*)0xB8000);
}

static void test_vga_initialize__should__clear_buffer(void) {
    int i;
    vga_t vga;
    int length = vga_get_default_terminal_width() * vga_get_default_terminal_height();
    uint16_t buffer[length];
    uint16_t buffer_expected[length];

    for (i = 0; i < length; i++) {
        buffer_expected[i] = 0;
        buffer[i] = i % 256;
    }

    vga_initialize(&vga, buffer, vga_get_default_terminal_width(), vga_get_default_terminal_height());
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, vga.buffer_address, length);
}

static void test_vga_initialize__should__reset_cursor_row(void) {
    vga_t vga;
    vga.cursor_row = 10;
    vga_initialize(&vga, NULL, 0, 0);
    TEST_ASSERT_EQUAL(vga.cursor_row, 0);
}

static void test_vga_initialize__should__reset_cursor_col(void) {
    vga_t vga;
    vga.cursor_col = 10;
    vga_initialize(&vga, NULL, 0, 0);
    TEST_ASSERT_EQUAL(vga.cursor_col, 0);
}

static void test_vga_initialize__should__set_terminal_width_to_parameter(void) {
    vga_t vga;
    vga_initialize(&vga, NULL, 123, 0);
    TEST_ASSERT_EQUAL(vga.terminal_width, 123);
}

static void test_vga_initialize__should__set_terminal_height_to_parameter(void) {
    vga_t vga;
    vga_initialize(&vga, NULL, 0, 123);
    TEST_ASSERT_EQUAL(vga.terminal_height, 123);
}

static void test_vga_put_character__should__put_character_at_cursor(void) {

}

static void test_vga_put_character__should__newline_should_wrap(void) {

}

static void test_vga_put_string__should__put_string_at_cursor(void) {

}

static void test_vga_put_string__should__newline_should_wrap(void) {

}

testfunc_container_t test_function_containers[] = {
    {"test_vga_get_default_terminal_width__should__return_80", test_vga_get_default_terminal_width__should__return_80},
    {"test_vga_get_default_terminal_height__should__return_25", test_vga_get_default_terminal_height__should__return_25},
    {"test_vga_get_default_buffer_address__should__return_0xB8000", test_vga_get_default_buffer_address__should__return_0xB8000},
    {"test_vga_initialize__should__clear_buffer", test_vga_initialize__should__clear_buffer},
    {"test_vga_initialize__should__reset_cursor_row", test_vga_initialize__should__reset_cursor_row},
    {"test_vga_initialize__should__reset_cursor_col", test_vga_initialize__should__reset_cursor_col},
    {"test_vga_initialize__should__set_terminal_width_to_parameter", test_vga_initialize__should__set_terminal_width_to_parameter},
    {"test_vga_initialize__should__set_terminal_height_to_parameter", test_vga_initialize__should__set_terminal_height_to_parameter},
    {"test_vga_put_character__should__put_character_at_cursor", test_vga_put_character__should__put_character_at_cursor},
    {"test_vga_put_character__should__newline_should_wrap", test_vga_put_character__should__newline_should_wrap},
    {"test_vga_put_string__should__put_string_at_cursor", test_vga_put_string__should__put_string_at_cursor},
    {"test_vga_put_string__should__newline_should_wrap", test_vga_put_string__should__newline_should_wrap}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}