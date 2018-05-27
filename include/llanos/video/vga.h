#pragma once

#include <llanos/types.h>

typedef enum vga_color_e vga_color_t;
typedef struct vga_s vga_t;

enum vga_color_e {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE
};

struct vga_s {
    u16* buffer_address;
    size_t cursor_row;
    size_t cursor_col;
    size_t terminal_width;
    size_t terminal_height;
};

/**
 * @brief Get the default terminal width for the VGA terminal.
 *
 * Default terminal width for VGA is 80.
 *
 * @return The default width for VGA.
 */
extern size_t vga_get_default_terminal_width(void);

/**
 * @brief Get the default terminal heigh tfor the VGA terminal.
 *
 * Default terminal height for VGA is 25.
 *
 * @return The default height for VGA.
 */
extern size_t vga_get_default_terminal_height(void);

/**
 * @brief Get the default buffer address.
 *
 * Default buffer address for VGA is 0xB8000
 *
 * @return The default buffer address.
 */
extern u16* vga_get_default_buffer_address(void);

/**
 * @brief Initialize a vga driver.
 *
 * This should clear the buffer. And setup all values
 * in the vga to their defaults.
 *
 * @param vga vga to initialize.
 * @param buffer_address address to set as the base address of the VGA buffer;
 * @param width width of the terminal.
 * @param height height of the terminal.
 */
extern void vga_initialize(vga_t* vga, u16* buffer_address, size_t width, size_t height);

/**
 * @brief Put a single colored character into the vga.
 *
 * @param vga vga to put the character in.
 * @param color_fg color of the character in the forground.
 * @param color_bg color of the character in the background.
 * @param c character to put into the vga.
 */
extern void vga_put_character(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, char c);

/**
 * @brief Put a string into the vga.
 *
 * @param vga vga to put the string in.
 * @param color_fg color of the character in the forground.
 * @param color_bg color of the character in the background.
 * @param str string to into the vga.
 */
extern void vga_put_string(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, const char* str);


/**
 * @brief Print a formatted string to the terminal.
 *
 * @param vga vga to print the formatted string to.
 * @param color_fg color of the character in the forground.
 * @param color_bg color of the character in the background.
 * @param format format speicifier for the string.
 *      A format specifier follows this prototype: %[specifier]
 *          where the specifier character at the end is the most signuifican component.
 *
 *      | speicifier    | output                        | example   |
 *      |---------------|-------------------------------|-----------|
 *      | d             | signed integer                | -239      |
 *      | u             | unsigned integer              | 234       |
 *      | o             | unsigned octal                | 610       |
 *      | x             | unsigned hex                  | 7aa       |
 *      | b             | unsigned binary               | 1101001   |
 *      | f             | floating point                | 32.25     |
 *      | e             | scientific notation           | 2.25e-3   |
 *      | c             | character                     | a         |
 *      | s             | string                        | abc       |
 *      | S             | string with preceeding length | abc       |
 *      | %             | the percent character         | %         |
 *
 * @param ... additional arguments.
 */
extern void vga_printf(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, const char* format, ...);