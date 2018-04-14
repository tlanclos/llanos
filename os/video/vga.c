#include <llanos/video/vga.h>

size_t vga_get_default_terminal_width(void) {
    return 80;
}

size_t vga_get_default_terminal_height(void) {
    return 25;
}

uint16_t* vga_get_default_buffer_address(void) {
    return 0xb8000;
}

void vga_initialize(vga_t* vga, uint16_t* buffer_addres, size_t width, size_t height) {

}

void vga_put_character(vga_t* vga, vga_color_t color, char c) {

}

void vga_put_string(vga_t* vga, vga_color_t color, const char* str) {

}