#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>
#include <llanos/management/abort.h>
#include <llanos/management/architecture.h>
#include <llanos/video/vga.h>


int kmain(void) {
    memory_table_t memory;
    vga_t vga;
    int a;

    initialize_architecture();

    vga_initialize(
        &vga,
        vga_get_default_buffer_address(),
        vga_get_default_terminal_width(),
        vga_get_default_terminal_height()
    );
    get_memory_table(&memory);

    load_interrupt_table(NULL);

    while (1);

    return 0;
}
