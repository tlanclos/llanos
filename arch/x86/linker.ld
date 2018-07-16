ENTRY(_start)

SECTIONS {
    . = 1M;
    __kernel_load_address = .;

    .text BLOCK(4K) : ALIGN(4K) {
        KEEP(*(.multiboot))
        *(.interrupt_table)
        *(.page_directory)
        *(.page_tables)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K) {
        *(.COMMON)
        *(.bss)
    }

    __kernel_end_address = .;
}