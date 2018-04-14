#include <llanos/video/vga.h>

size_t vga_get_default_terminal_width(void) {
    return 80;
}

size_t vga_get_default_terminal_height(void) {
    return 25;
}

uint16_t* vga_get_default_buffer_address(void) {
    return (uint16_t*)0xb8000;
}

void vga_initialize(vga_t* vga, uint16_t* buffer_address, size_t width, size_t height) {
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

void vga_put_character(vga_t* vga, vga_color_t color, char c) {

}

void vga_put_string(vga_t* vga, vga_color_t color, const char* str) {

}