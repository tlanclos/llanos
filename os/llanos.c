#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>
#include <llanos/management/interrupt.h>
#include <llanos/video/vga.h>


int kmain(void) {
    memory_table_t memory;
    vga_t vga;
    int a;

    vga_initialize(
        &vga,
        vga_get_default_buffer_address(),
        vga_get_default_terminal_width(),
        vga_get_default_terminal_height()
    );
    get_memory_table(&memory);

    load_interrupt_table(NULL);

    a=1/0;

    while (1);

    return 0;
}
