#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>
#include <llanos/video/vga.h>

int kmain(void) {
    vga_t vga;
    vga_initialize(
        &vga, 
        vga_get_default_buffer_address(),
         vga_get_default_terminal_width(), 
         vga_get_default_terminal_height()
    );
    vga_put_string(&vga, VGA_COLOR_LIGHT_RED, VGA_COLOR_BLUE, "abc123\n");
    return 0;
}
