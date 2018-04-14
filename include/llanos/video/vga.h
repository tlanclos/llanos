#pragma once

#include <stdint.h>
#include <stddef.h>

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
    uint16_t* buffer_address;
    size_t cursor_row;
    size_t cursor_col;
    size_t terminal_width;
    size_t terminal_height;
};

/**
 * Get the default terminal width for the VGA terminal.
 *
 * Default terminal width for VGA is 80.
 *
 * @return The default width for VGA.
 */
extern size_t vga_get_default_terminal_width(void);

/**
 * Get the default terminal heigh tfor the VGA terminal.
 *
 * Default terminal height for VGA is 25.
 *
 * @return The default height for VGA.
 */
extern size_t vga_get_default_terminal_height(void);

/**
 * Get the default buffer address.
 *
 * Default buffer address for VGA is 0xB8000
 *
 * @return The default buffer address.
 */
extern uint16_t* vga_get_default_buffer_address(void);

/**
 * Initialize a vga driver.
 *
 * This should clear the buffer. And setup all values
 * in the vga to their defaults.
 *
 * @param vga vga to initialize.
 * @param buffer_address address to set as the base address of the VGA buffer;
 * @param width width of the terminal.
 * @param height height of the terminal.
 */
extern void vga_initialize(vga_t* vga, uint16_t* buffer_address, size_t width, size_t height);

/**
 * Put a single colored character into the vga.
 *
 * @param vga vga to put the character in.
 * @param color color of the character.
 * @param c character to put into the vga.
 */
extern void vga_put_character(vga_t* vga, vga_color_t color, char c);

/**
 * Put a string into the vga.
 *
 * @param vga vga to put the string in.
 * @param color color of the string.
 * @param str string to into the vga.
 */
extern void vga_put_string(vga_t* vga, vga_color_t color, const char* str);
