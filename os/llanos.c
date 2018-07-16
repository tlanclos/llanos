#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/abort.h>
#include <llanos/video/vga.h>
#include <llanos/llanos.h>


int kmain(void) {
    vga_t vga;
    int a;

    reset_llanos_vga();

    vga_printf(
        get_llanos_vga(), 
        VGA_COLOR_BLACK, 
        VGA_COLOR_LIGHT_CYAN, 
        "test %d %s %e", 
        -123, 
        "this is some string",
        9.2343e+18
    );

    while (1);

    return 0;
}
