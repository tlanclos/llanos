#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>
#include <llanos/management/abort.h>
#include <llanos/management/architecture.h>
#include <llanos/video/vga.h>
#include <llanos/llanos.h>


int kmain(void) {
    memory_table_t memory;
    vga_t vga;
    int a;

    initialize_architecture();
    reset_llanos_vga();

    get_memory_table(&memory);
    vga_printf(get_llanos_vga(), VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN, "test %d %s %e", -123, "this is some string", 9.2343e+18);

    while (1);

    return 0;
}
