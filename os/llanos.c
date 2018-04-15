#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>
#include <llanos/video/vga.h>

int kmain(void) {
    vga_t vga;
    int i;
    vga_initialize(
        &vga, 
        vga_get_default_buffer_address(),
        vga_get_default_terminal_width(), 
        vga_get_default_terminal_height()
    );
    for (i = 0; i < vga.terminal_height; i++) {
        vga_put_string(&vga, VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE, "I did NOT hit her!\n");   
    }
    vga_put_string(&vga, VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE, "Oh Hi, LlanOS!    \n");
    return 0;
}
