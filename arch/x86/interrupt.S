.intel_syntax noprefix

.section .text

.global interrupt_load_idtr
.align 4
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

.global interrupt_store_idtr
.align 4
store_idtr:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, [%ebp+8]
    sidt [%eax]
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret


.macro __isr_handle isrnum
.align 4
__isr_handle_\isrnum:
    pushad
    cmpw generic_interrupt_handle, 0
    je __isr_handle_nocall_\isrnum
    push 0
    call generic_interrupt_handle
__isr_handle_nocall_\isrnum:
    popad
    iret
.endm

__isr_handle 0
__isr_handle 1
__isr_handle 2
__isr_handle 3