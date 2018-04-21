#include <llanos/video/vga.h>

static inline u16 __vga_get_entry(vga_color_t color_fg, vga_color_t color_bg, char c) {
    return (((u16)(color_fg) | ((u16)(color_bg) << 4)) << 8) | (u16)(c);
}

static void __vga_advance_cursor_by_1(vga_t* vga) {
    /* move to the next column */
    vga->cursor_col++;

    /* if we have reached the terminal width, then we need to advance to the next row */
    if (vga->cursor_col >= vga->terminal_width) {
        vga->cursor_col = 0;
        vga->cursor_row++;

        /* if we have reached the terminal height, then we need to go back to the top */
        if (vga->cursor_row >= vga->terminal_height) {
            vga->cursor_row = 0;
        }
    }
}

size_t vga_get_default_terminal_width(void) {
    return 80;
}

size_t vga_get_default_terminal_height(void) {
    return 25;
}

u16* vga_get_default_buffer_address(void) {
    return (u16*)0xb8000;
}

void vga_initialize(vga_t* vga, u16* buffer_address, size_t width, size_t height) {
    size_t row;
    size_t col;

    vga->buffer_address = buffer_address;
    vga->terminal_width = width;
    vga->terminal_height = height;
    vga->cursor_row = 0;
    vga->cursor_col = 0;

    for (col = 0; col < width; col++) {
        for (row = 0; row < height; row++) {
            *buffer_address++ = 0;
        }
    }
}

void vga_put_character(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, char c) {
    if (c == '\n') {
        vga->cursor_col = vga->terminal_width;
        __vga_advance_cursor_by_1(vga);
    } else {
        /* insert the wanted vga entry at the current cursor address */
        vga->buffer_address[vga->cursor_col + (vga->cursor_row * vga->terminal_width)] = \
            __vga_get_entry(color_fg, color_bg, c);

        /* advance the cursor on this vga by 1 */
        __vga_advance_cursor_by_1(vga);
    }
}

void vga_put_string(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, const char* str) {
    while (*str != '\0') {
        vga_put_character(vga, color_fg, color_bg, *str);
        str++;
    }
}