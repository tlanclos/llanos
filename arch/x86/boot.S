.set ALIGN, (1 << 0)
.set MEMINFO, (1 << 1)
.set FLAGS, (ALIGN | MEMINFO)
.set MAGIC, (0x1badb002)
.set CHECKSUM, (-(MAGIC + FLAGS))

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


.section .bss
.align 16
__stack_bottom:
.skip 16384
__stack_top:


.section .data
.global multiboot_info
multiboot_info:
.long 0

.section .text
.global _start
_start:
    lea %esp, __stack_top
    mov [multiboot_info], %ebx
    call kmain
    cli
_halt:
    hlt
    jmp _halt
