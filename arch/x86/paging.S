.intel_syntax noprefix

.section .text

.global paging_set_page_directory
paging_set_page_directory:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, [%ebp+8]
    mov %cr3, %eax
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret

.global paging_enable
paging_enable:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, %cr0
    /* Enable paging (PG) bit and protection (PE) bit in cr0 */
    or %eax, 0x80000001
    mov %cr0, %eax
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret

.global paging_disable
paging_disable:
    push %ebp
    mov %ebp, %esp

    push %eax
    mov %eax, %cr0
    /* Disable paging (PG) bit and protection (PE) bit in cr0 */
    and %eax, 0x7ffffffe
    mov %cr0, %eax
    pop %eax

    mov %esp, %ebp
    pop %ebp
    ret
