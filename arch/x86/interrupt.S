.intel_syntax noprefix

.section .text

.global __interrupt_load_idtr
.align 4
__interrupt_load_idtr:
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

.global interrupt_store_idtr
.align 4
interrupt_store_idtr:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, [%ebp+8]
    sidt [%eax]
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret


.macro __isr_handler isrnum
.align 4
.global __isr_handler_\isrnum
__isr_handler_\isrnum:
    pushad
    cmpw __generic_interrupt_handler, 0
    je __isr_handle_nocall_\isrnum
    push \isrnum
    call [__generic_interrupt_handler]
__isr_handle_nocall_\isrnum:
    popad
    iret
.endm

__isr_handler 0
__isr_handler 1
__isr_handler 2
__isr_handler 3