.intel_syntax noprefix


.global gdt_load_gdtr
gdt_load_gdtr:
    push %ebp
    mov %ebp, %esp
    
    push %ebx
    mov %eax, [%ebp+8]
    lgdt [%eax]
    mov %ax, 0x10
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
    mov %ss, %ax

    mov %esp, %ebp
    pop %ebp
    jmp 0x08:__gdt_load_gdtr_jump
__gdt_load_gdtr_jump:
    ret

