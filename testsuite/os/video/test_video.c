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

static void test_vga_printf__should__print_floating_point_positive(void) {
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

static void test_vga_printf__should__print_floating_point_negative(void) {
    vga_t vga;
    int width = 15;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '-', '3', '2', '.', '2', '5', '\0', '\0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %f", -32.25);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_floating_point_large(void) {
    vga_t vga;
    int width = 27;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '1', '3', '5', '2', '3', '1', '2', '4', '5', '3', '8', '.', '0', '2', '7', '3', '4', '3', '7', '5'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %f", 13523124538.02734375);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_floating_point_small(void) {
    vga_t vga;
    int width = 19;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '0', '.', '0', '0', '0', '9', '7', '6', '5', '6', '2', '5'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %f", 0.0009765625);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_floating_point_switch_scientific(void) {
    vga_t vga;
    int width = 14;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '1', '.', '0', 'e', '+', '1', '9'
    };

    vga_initialize(&vga, buffer, width, height);
                                                                    
    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %f", 10000000000000000000.0);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_scientific_notation_zero(void) {
    vga_t vga;
    int width = 13;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '0', '.', '0', 'e', '+', '0'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %e", 0.0);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_scientific_notation_less_than_one(void) {
    vga_t vga;
    int width = 16;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '1', '.', '5', '6', '2', '5', 'e', '-', '2'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %e", 0.015625);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_scientific_notation_less_than_zero(void) {
    vga_t vga;
    int width = 17;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '-', '1', '.', '5', '6', '2', '5', 'e', '+', '1'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %e", -15.625);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_scientific_notation_greater_than_negative_one(void) {
    vga_t vga;
    int width = 17;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '-', '1', '.', '5', '6', '2', '5', 'e', '-', '2'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %e", -0.015625);

    TEST_ASSERT_EQUAL_MEMORY(buffer_expected, buffer, length * sizeof(uint16_t));
}

static void test_vga_printf__should__print_scientific_notation_greater_than_zero(void) {
    vga_t vga;
    int width = 16;
    int height = 1;
    int length = width * height;
    uint16_t buffer[length];
    uint16_t buffer_expected[] = {
        'v', 'a', 'l', 'u', 'e', ':', ' ', '1', '.', '5', '6', '2', '5', 'e', '+', '1'
    };

    vga_initialize(&vga, buffer, width, height);

    vga_printf(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "value: %e", 15.625);

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

static void test_vga_equal__should__equal_if_all_components_are_equal(void) {
    vga_t first;
    vga_t second;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&first, buffer, width, height);
    vga_initialize(&second, buffer, width, height);

    TEST_ASSERT_TRUE(vga_equal(&first, &second));
}

static void test_vga_equal__should__not_equal_if_buffer_addresses_are_not_equal(void) {
    vga_t first;
    vga_t second;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];
    uint16_t buffer2[width * height];

    vga_initialize(&first, buffer, width, height);
    vga_initialize(&second, buffer2, width, height);

    TEST_ASSERT_FALSE(vga_equal(&first, &second));
}

static void test_vga_equal__should__not_equal_if_widths_are_not_equal(void) {
    vga_t first;
    vga_t second;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&first, buffer, width - 1, height);
    vga_initialize(&second, buffer, width, height);

    TEST_ASSERT_FALSE(vga_equal(&first, &second));
}

static void test_vga_equal__should__not_equal_if_heights_are_not_equal(void) {
    vga_t first;
    vga_t second;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&first, buffer, width, height - 1);
    vga_initialize(&second, buffer, width, height);

    TEST_ASSERT_FALSE(vga_equal(&first, &second));
}

static void test_vga_equal__should__not_equal_if_cursor_col_are_not_equal(void) {
    vga_t first;
    vga_t second;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&first, buffer, width, height);
    vga_initialize(&second, buffer, width, height);

    /* put 1 character so the rows are equal, but the columns are not */
    vga_put_character(&first, VGA_COLOR_BLACK, VGA_COLOR_BLACK, 'a');

    TEST_ASSERT_FALSE(vga_equal(&first, &second));
}

static void test_vga_equal__should__not_equal_if_cursor_row_are_not_equal(void) {
    vga_t first;
    vga_t second;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&first, buffer, width, height);
    vga_initialize(&second, buffer, width, height);

    /* fill up the first row where the columns are equal, but rows are not */
    vga_put_string(&first, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "aaaaaaaaaa");

    TEST_ASSERT_FALSE(vga_equal(&first, &second));
}

static void test_vga_copy__should__copy_all_parameters_from_source_vga_to_dest_vga(void) {
    vga_t source;
    vga_t destination;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];
    uint16_t otherbuffer[(width - 1) * (height - 1)];

    vga_initialize(&source, buffer, width, height);

    /* put a string to move current column and row to different values */
    vga_put_string(&source, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "aaaaaaaaaaaa");

    vga_initialize(&destination, otherbuffer, width - 1, height - 1);

    vga_copy(&destination, &source);
    TEST_ASSERT_TRUE(vga_equal(&destination, &source));
}

testfunc_container_t test_function_containers[] = {
    {"vga_get_default_terminal_width should return 80", test_vga_get_default_terminal_width__should__return_80},
    
    {"vga_get_default_terminal_height should return 25", test_vga_get_default_terminal_height__should__return_25},
    
    {"vga_get_default_buffer_address should return 0xb8000", test_vga_get_default_buffer_address__should__return_0xB8000},
    
    {"vga_initialize should clear buffer", test_vga_initialize__should__clear_buffer},
    {"vga_initialize should reset cursor row", test_vga_initialize__should__reset_cursor_row},
    {"vga_initialize should reset cursor col", test_vga_initialize__should__reset_cursor_col},
    {"vga_initialize should set terminal width to parameter", test_vga_initialize__should__set_terminal_width_to_parameter},
    {"vga_initialize should set terminal height to parameter", test_vga_initialize__should__set_terminal_height_to_parameter},
    
    {"vga_put_character should put character at cursor", test_vga_put_character__should__put_character_at_cursor},
    {"vga_put_character should put foreground color with character", test_vga_put_character__should__put_foreground_color_with_character},
    {"vga_put_character should put background color with character", test_vga_put_character__should__put_background_color_with_character},
    {"vga_put_character should wrap on newlines", test_vga_put_character__should__wrap_on_newlines},
    {"vga_put_character should wrap to top at end of buffer with newlines", test_vga_put_character__should__wrap_to_top_at_end_of_buffer_with_newlines},
    {"vga_put_character should wrap to top at end of buffer without newlines", test_vga_put_character__should__wrap_to_top_at_end_of_buffer_without_newlines},
    
    {"vga_put_string should put string at cursor", test_vga_put_string__should__put_string_at_cursor},
    {"vga_put_string should wrap newlines", test_vga_put_string__should__wrap_newlines},
    {"vga_put_string should wrap to top at end of buffer with newlines", test_vga_put_string__should__wrap_to_top_at_end_of_buffer_with_newlines},
    {"vga_put_string should wrap to top at end of buffer without newlines", test_vga_put_string__should__wrap_to_top_at_end_of_buffer_without_newlines},

    {"vga_printf should print signed decimal number zero", test_vga_printf__should__print_signed_decimal_number_zero},
    {"vga_printf should print signed decimal number positive", test_vga_printf__should__print_signed_decimal_number_positive},
    {"vga_printf should print signed decimal number negative", test_vga_printf__should__print_signed_decimal_number_negative},
    {"vga_printf should print unsigned decimal", test_vga_printf__should__print_unsigned_decimal},
    {"vga_printf should print unsigned octal", test_vga_printf__should__print_unsigned_octal},
    {"vga_printf should print unsigned hex", test_vga_printf__should__print_unsigned_hex},
    {"vga_printf should print unsigned binary", test_vga_printf__should__print_unsigned_binary},
    {"vga_printf should print floating point positive", test_vga_printf__should__print_floating_point_positive},
    {"vga_printf should print floating point negative", test_vga_printf__should__print_floating_point_negative},
    {"vga_printf should print floating point large", test_vga_printf__should__print_floating_point_large},
    {"vga_printf should print floating point small", test_vga_printf__should__print_floating_point_small},
    {"vga_printf should print floating point switch scientific", test_vga_printf__should__print_floating_point_switch_scientific},
    {"vga_printf should print scientific notation zero", test_vga_printf__should__print_scientific_notation_zero},
    {"vga_printf should print scientific notation less than one", test_vga_printf__should__print_scientific_notation_less_than_one},
    {"vga_printf should print scientific notation less than zero", test_vga_printf__should__print_scientific_notation_less_than_zero},
    {"vga_printf should print scientific notation greater than negative one", test_vga_printf__should__print_scientific_notation_greater_than_negative_one},
    {"vga_printf should print scientific notation greater than zero", test_vga_printf__should__print_scientific_notation_greater_than_zero},
    {"vga_printf should print character", test_vga_printf__should__print_character},
    {"vga_printf should print string", test_vga_printf__should__print_string},
    {"vga_printf should print string with preceeding length", test_vga_printf__should__print_string_with_preceeding_length},
    {"vga_printf should print percent character", test_vga_printf__should__print_percent_character},

    {"vga_equal should equal if all components are equal", test_vga_equal__should__equal_if_all_components_are_equal},
    {"vga_equal should not equal if buffer addresses are not equal", test_vga_equal__should__not_equal_if_buffer_addresses_are_not_equal},
    {"vga_equal should not equal if widths are not equal", test_vga_equal__should__not_equal_if_widths_are_not_equal},
    {"vga_equal should not equal if heights are not equal", test_vga_equal__should__not_equal_if_heights_are_not_equal},
    {"vga_equal should not equal if cursor col are not equal", test_vga_equal__should__not_equal_if_cursor_col_are_not_equal},
    {"vga_equal should not equal if cursor row are not equal", test_vga_equal__should__not_equal_if_cursor_row_are_not_equal},

    {"vga_copy should copy all parameters from source vga to dest vga", test_vga_copy__should__copy_all_parameters_from_source_vga_to_dest_vga}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
    return 0;
}