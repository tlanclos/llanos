.intel_syntax noprefix

.section .text

.global load_idtr
load_idtr:
    push %ebp
    mov %ebp, %esp
    push %eax
    mov %eax, [%ebp+8]
    lidt [%eax]
    sti
    pop %eax
    mov %esp, %ebp
    pop %ebp
    ret


.global __isr_handle_0
__isr_handle_0:
    iret