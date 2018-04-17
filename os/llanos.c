#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>
#include <llanos/video/vga.h>

char to_hex(char b) {
    if (b >= 0 && b <= 9) {
        return b + '0';
    } else if (b >= 10 && b <= 15) {
        return b = 'a';
    } else {
        return '\0';
    }
}

int kmain(void) {
    memory_table_t memory;
    vga_t vga;
    int i;
    int j;

    get_memory_table(&memory);

    vga_initialize(
        &vga, 
        vga_get_default_buffer_address(),
        vga_get_default_terminal_width(), 
        vga_get_default_terminal_height()
    );

    for (i = 0; i < memory.length; i++) {
        for (j = 7; j >= 0; j--) {
            vga_put_character(&vga, VGA_COLOR_BLUE, VGA_COLOR_BLACK, to_hex(((uint32_t)memory.entries[i].base & (0xf << (j * 4))) >> (j * 4)));
        }
        vga_put_character(&vga, VGA_COLOR_BLUE, VGA_COLOR_BLACK, ' ');
        for (j = 7; j >= 0; j--) {
            vga_put_character(&vga, VGA_COLOR_BLUE, VGA_COLOR_BLACK, to_hex(((uint32_t)memory.entries[i].length & (0xf << (j * 4))) >> (j * 4)));
        }
        vga_put_character(&vga, VGA_COLOR_BLUE, VGA_COLOR_BLACK, '\n');
    }
    return 0;
}
