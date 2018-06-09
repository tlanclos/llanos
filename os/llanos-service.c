#include <llanos/llanos.h>
#include <llanos/video/vga.h>


static vga_t __vga;


void reset_llanos_vga(void) {
    vga_initialize(
        &__vga, 
        vga_get_default_buffer_address(),
        vga_get_default_terminal_width(),
        vga_get_default_terminal_height()
    );
}

vga_t* get_llanos_vga(void) {
    return &__vga;
}

void set_llanos_vga(vga_t* vga) {
    vga_copy(&__vga, vga);
}
