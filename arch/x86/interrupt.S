.intel_syntax noprefix

.section .text

int_handler:
    hlt

idt:
    .fill 50, 2, 0

idtr:
    .word (50*8)-1
    .long idt

.global __initialize_interrupt_table
__initialize_interrupt_table:
    lidt [idtr]
    mov %eax, int_handler
    mov [idt+49*8], %ax
    movw [idt+49*8+2], 0x08
    movw [idt+49*8+4], 0x8e00
    shr %eax, 16
    mov [idt+49*8+6], %ax
    int 49
