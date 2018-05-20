.intel_syntax noprefix

.global __output_byte
.align 4
__output_byte:
    push %eax
    push %edx
    mov %dx, word [%esp]
    mov %al, byte [%esp + 4]
    out %dx, %al
    pop %edx
    pop %eax
    ret
