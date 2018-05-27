#include <testsuite.h>
#include <llanos/types.h>
#include <llanos/limits.h>
#include <llanos/video/vga.h>
#include <string.h>

static vga_color_t __extract_vga_color_fg(uint16_t vga_entry) {
    return (vga_color_t)((vga_entry >> 8) & 0x0f);
}

static vga_color_t __extract_vga_color_bg(uint16_t vga_entry) {
    return (vga_color_t)(vga_entry >> 12);
}

static char __extract_vga_char(uint16_t vga_entry) {
    return (char)(vga_entry & 0x00ff);
}

static void __put_many_characters(vga_t* vga, char* chars, size_t length) {
    size_t i;

    for (i = 0; i < length; i++) {
        vga_put_character(vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, chars[i]);    
    }
}

static void test_vga_get_default_terminal_width__should__return_80(void) {
    TEST_ASSERT_EQUAL(80, vga_get_default_terminal_width());
}

static void test_vga_get_default_terminal_height__should__return_25(void) {
    TEST_ASSERT_EQUAL(25, vga_get_default_terminal_height());
}

static void test_vga_get_default_buffer_address__should__return_0xB8000(void) {
    TEST_ASSERT_EQUAL_PTR((uint16_t*)0xB8000, vga_get_default_buffer_address());
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
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_initialize__should__reset_cursor_row(void) {
    vga_t vga;
    vga.cursor_row = 10;
    vga_initialize(&vga, NULL, 0, 0);
    TEST_ASSERT_EQUAL(0, vga.cursor_row);
}

static void test_vga_initialize__should__reset_cursor_col(void) {
    vga_t vga;
    vga.cursor_col = 10;
    vga_initialize(&vga, NULL, 0, 0);
    TEST_ASSERT_EQUAL(0, vga.cursor_col);
}

static void test_vga_initialize__should__set_terminal_width_to_parameter(void) {
    vga_t vga;
    vga_initialize(&vga, NULL, 123, 0);
    TEST_ASSERT_EQUAL(123, vga.terminal_width);
}

static void test_vga_initialize__should__set_terminal_height_to_parameter(void) {
    vga_t vga;
    vga_initialize(&vga, NULL, 0, 123);
    TEST_ASSERT_EQUAL(123, vga.terminal_height);
}

static void test_vga_put_character__should__put_character_at_cursor(void) {
    vga_t vga;
    uint16_t buffer[10];
    vga_initialize(&vga, buffer, 5, 2);
    vga_put_character(&vga, VGA_COLOR_CYAN, VGA_COLOR_RED, 'a');
    vga_put_character(&vga, VGA_COLOR_CYAN, VGA_COLOR_RED, 'b');
    TEST_ASSERT_EQUAL('b', __extract_vga_char(buffer[1]));
}

static void test_vga_put_character__should__put_foreground_color_with_character(void) {
    vga_t vga;
    uint16_t buffer[10];
    vga_initialize(&vga, buffer, 5, 2);
    vga_put_character(&vga, VGA_COLOR_CYAN, VGA_COLOR_RED, 'a');
    vga_put_character(&vga, VGA_COLOR_CYAN, VGA_COLOR_RED, 'c');
    TEST_ASSERT_EQUAL(VGA_COLOR_CYAN, __extract_vga_color_fg(buffer[1]));
}

static void test_vga_put_character__should__put_background_color_with_character(void) {
    vga_t vga;
    uint16_t buffer[10];
    vga_initialize(&vga, buffer, 5, 2);
    vga_put_character(&vga, VGA_COLOR_CYAN, VGA_COLOR_RED, 'a');
    vga_put_character(&vga, VGA_COLOR_CYAN, VGA_COLOR_RED, 'c');
    TEST_ASSERT_EQUAL(VGA_COLOR_RED, __extract_vga_color_bg(buffer[1]));
}

static void test_vga_put_character__should__wrap_on_newlines(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'a', '\0', '\0', '\0', '\0',
        'b', '\0', '\0', '\0', '\0',
        'c', '\0', '\0', '\0', '\0'
    };
    char chars[] = {'a', '\n', 'b', '\n', 'c', '\n'};
    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information. 
     */
    __put_many_characters(&vga, chars, sizeof(chars) / sizeof(char));
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_put_character__should__wrap_to_top_at_end_of_buffer_with_newlines(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'f', ' ',  'c',  '\0', '\0',
        'd', '\0', '\0', '\0', '\0',
        'e', '\0', '\0', '\0', '\0'
    };
    char chars[] = {'a', 'b', 'c', '\n', 'd', '\n', 'e', '\n', 'f', ' '};
    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information. 
     */
    __put_many_characters(&vga, chars, sizeof(chars) / sizeof(char));
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_put_character__should__wrap_to_top_at_end_of_buffer_without_newlines(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'j', 'k',  'c',  '\0', '\0',
        'd', '\0', '\0', '\0', '\0',
        'e', 'f',  'g',  'h',  'i'
    };
    char chars[] = {'a', 'b', 'c', '\n', 'd', '\n', 'e', 'f', 'g', 'h', 'i', 'j', 'k'};
    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information. 
     */
    __put_many_characters(&vga, chars, sizeof(chars) / sizeof(char));
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_put_string__should__put_string_at_cursor(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'a',  'b',  'c',  't',  'e',
        's',  't',  '\0', '\0', '\0',
        '\0', '\0', '\0', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information. 
     */
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "abc");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "test");
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_put_string__should__wrap_newlines(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'a',  'b',  'c',  '\0', '\0',
        't',  'e',  's',  't',  '\0',
        '\0', '\0', '\0', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information.
     */
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "abc\n");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "test");
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_put_string__should__wrap_to_top_at_end_of_buffer_with_newlines(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        't',  'o',  'p',  '\0', '\0',
        't',  'e',  's',  't',  '\0',
        'm',  'i',  's',  's',  '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information.
     */
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "abc\n");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "test\n");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "miss\n");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "top\n");
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_put_string__should__wrap_to_top_at_end_of_buffer_without_newlines(void) {
    vga_t vga;
    int width = 5;
    int height = 3;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        't',  'o',  'p',  '\0', '\0',
        't',  'e',  's',  't',  '\0',
        'm',  'i',  's',  's',  ' '
    };

    vga_initialize(&vga, buffer, width, height);

    /* 
     * setting the color to black ensures that the data in the buffer should
     * only be the character and no color information.
     */
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "abc\n");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "test\n");
    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "miss top");
    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_signed_decimal_number_zero(void) {
    vga_t vga;
    int width = 10;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '0', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %d", 0);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_signed_decimal_number_positive(void) {
    vga_t vga;
    int width = 17;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '2', '1', '4', '7', '4', '8', '3', '6', '4', '7'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %d", S32MAX);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_signed_decimal_number_negative(void) {
    vga_t vga;
    int width = 18;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '-', '2', '1', '4', '7', '4', '8', '3', '6', '4', '8'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %d", S32MIN);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_unsigned_decimal(void) {
    vga_t vga;
    int width = 17;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '4', '2', '9', '4', '9', '6', '7', '2', '9', '5'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %u", U32MAX);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_unsigned_octal(void) {
    vga_t vga;
    int width = 18;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '3', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %o", U32MAX);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_unsigned_hex(void) {
    vga_t vga;
    int width = 15;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %x", U32MAX);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_unsigned_binary(void) {
    vga_t vga;
    int width = 39;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', 
        '1', '1', '1', '1', '1', '1', '1', '1', 
        '1', '1', '1', '1', '1', '1', '1', '1', 
        '1', '1', '1', '1', '1', '1', '1', '1', 
        '1', '1', '1', '1', '1', '1', '1', '1'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %b", U32MAX);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_floating_point(void) {
    vga_t vga;
    int width = 15;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '3', '2', '.', '2', '5', '\0', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %f", 32.25);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_scientific_notation(void) {
    vga_t vga;
    int width = 15;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '9', '.', '2', '5', 'e', '-', '1', '0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %e", 9.25e-10);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_character(void) {
    vga_t vga;
    int width = 15;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', 'a', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %c", 'a');

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_string(void) {
    vga_t vga;
    int width = 18;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', 's', 'o', 'm', 'e', ' ', 's', 't', 'r', 'i', 'n', 'g'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %s", "some string");

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_string_with_preceeding_length(void) {
    vga_t vga;
    int width = 20;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    char bufferstring[] = {'b', 'u', 'f', 'f', 'e', 'r', ' ', 's', 't', 'r', 'i', 'n', 'g'};
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', 'b', 'u', 'f', 'f', 'e', 'r', ' ', 's', 't', 'r', 'i', 'n', 'g'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %S", sizeof(bufferstring) / sizeof(char), bufferstring);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_percent_character(void) {
    vga_t vga;
    int width = 15;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '%', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %%");

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
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
    {"test_vga_put_character__should__put_foreground_color_with_character", test_vga_put_character__should__put_foreground_color_with_character},
    {"test_vga_put_character__should__put_background_color_with_character", test_vga_put_character__should__put_background_color_with_character},
    {"test_vga_put_character__should__wrap_on_newlines", test_vga_put_character__should__wrap_on_newlines},
    {"test_vga_put_character__should__wrap_to_top_at_end_of_buffer_with_newlines", test_vga_put_character__should__wrap_to_top_at_end_of_buffer_with_newlines},
    {"test_vga_put_character__should__wrap_to_top_at_end_of_buffer_without_newlines", test_vga_put_character__should__wrap_to_top_at_end_of_buffer_without_newlines},
    
    {"test_vga_put_string__should__put_string_at_cursor", test_vga_put_string__should__put_string_at_cursor},
    {"test_vga_put_string__should__wrap_newlines", test_vga_put_string__should__wrap_newlines},
    {"test_vga_put_string__should__wrap_to_top_at_end_of_buffer_with_newlines", test_vga_put_string__should__wrap_to_top_at_end_of_buffer_with_newlines},
    {"test_vga_put_string__should__wrap_to_top_at_end_of_buffer_without_newlines", test_vga_put_string__should__wrap_to_top_at_end_of_buffer_without_newlines},

    {"test_vga_printf__should__print_signed_decimal_number_zero", test_vga_printf__should__print_signed_decimal_number_zero},
    {"test_vga_printf__should__print_signed_decimal_number_positive", test_vga_printf__should__print_signed_decimal_number_positive},
    {"test_vga_printf__should__print_signed_decimal_number_negative", test_vga_printf__should__print_signed_decimal_number_negative},
    {"test_vga_printf__should__print_unsigned_decimal", test_vga_printf__should__print_unsigned_decimal},
    {"test_vga_printf__should__print_unsigned_octal", test_vga_printf__should__print_unsigned_octal},
    {"test_vga_printf__should__print_unsigned_hex", test_vga_printf__should__print_unsigned_hex},
    {"test_vga_printf__should__print_unsigned_binary", test_vga_printf__should__print_unsigned_binary},
    {"test_vga_printf__should__print_floating_point", test_vga_printf__should__print_floating_point},
    {"test_vga_printf__should__print_scientific_notation", test_vga_printf__should__print_scientific_notation},
    {"test_vga_printf__should__print_character", test_vga_printf__should__print_character},
    {"test_vga_printf__should__print_string", test_vga_printf__should__print_string},
    {"test_vga_printf__should__print_string_with_preceeding_length", test_vga_printf__should__print_string_with_preceeding_length},
    {"test_vga_printf__should__print_percent_character", test_vga_printf__should__print_percent_character}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
    return 0;
}