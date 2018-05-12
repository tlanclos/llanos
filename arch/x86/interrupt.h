#pragma once

#include <llanos/types.h>

#define INTERRUPT_TYPE_INTERRUPT_GATE   (0b01110)
#define INTERRUPT_TYPE_TRAP_GATE        (0b01111)
#define INTERRUPT_TYPE_TASK_GATE        (0b00101)

#define SEGMENT_SELECTOR_KERNEL_CODE_32BIT      (0x08)
#define SEGMENT_SELECTOR_KERNEL_DATA_32BIT      (0x10)
#define SEGMENT_SELECTOR_KERNEL_STACK_32BIT     (0x18)
#define SEGMENT_SELECTOR_KERNEL_CODE_16BIT      (0x20)
#define SEGMENT_SELECTOR_KERNEL_STACK_16BIT     (0x28)
#define SEGMENT_SELECTOR_USER_CODE_32BIT        (0x33)
#define SEGMENT_SELECTOR_USER_DATA_32BIT        (0x3b)
#define SEGMENT_SELECTOR_USER_STACK_32BIT       (0x43)

typedef struct idt_register_s idt_register_t;
typedef struct idt_entry_s idt_entry_t;

/**
 * @brief Interrupt Descriptor Table Register
 *
 * idtr.limit (lower 2 bytes) --> | isr255 |
 *                                   ....
 *                                |  isr1  |
 * idtr.base (4 bytes) ---------> |  isr0  |
 *
 * If the IDT has 256 entries then and the base address is 0x10000000,
 * then limit would be (256*4)-1=1023 or 0x3ff.
 *
 * @member limit limit represents the upper limit of idtr.
 * @member base base address of idtr.
 */
struct idt_register_s {
    u16 limit;
    u32 base;
} __attribute__((packed));

/**
 * @brief Interrupt Descriptor Table Entry.
 *
 * @member base_low lower 16-bits of interrupt function address (unused in Task Gate Mode).
 * @member sel segment selector of ISR chosen from SEGMENT_SELECTOR_* (Trap Gate/Interrupt Gate Mode) or 
 *      TSS descriptor (Task Gate Mode)
 * @member always0 Always zero, just set it to zero.
 * @member interrupt_type choose from one of the INTERRUPT_TYPE_* values
 * @member dpl Descriptor Priviledge Level (0 is highest priority, 3 is lowest priority).
 * @member present Present bit 1=available 0=unavailable (whether or not kernel segment is available in RAM)
 * @member base_high higher 16-bits of interrupt function address (unused in Task Gate Mode).
 */
struct idt_entry_s {
    u16 base_low;
    u16 sel;
    u8 always0;
    u8 interrupt_type: 5;
    u8 dpl: 2
    u8 present: 1;
    u16 base_high;
} __attribute__((packed));


/**
 * @brief Build an IDTR from a base address and a size.
 *
 * @param idtr Destination idtr register.
 * @param idt IDT to load.
 * @param count Number of entries in IDT.
 */
extern void build_idtr(idt_register_t* idtr, idt_entry_t* idt, size_t count);


/**
 * @brief Load Interrupt Descriptor Table Register.
 *
 * This is equivalent to the assembly instruction `lidt [idtr]`
 *
 * @param idtr pointer to interrupt descriptor table register to load.
 */
extern void load_idtr(idt_register_t* idtr);

/**
 * @brief load interrupt to IDT.
 *
 * @param isrnum interrupt table number.
 * @param isr interrupt function base pointer.
 * @param sel segment selector or TSS segment (refer to idt_entry_s.sel).
 * @param interrupt_type interrup type (refer to idt_entry_s.interrupt_type).
 * @param dpl descriptor priviledge level (refer to idt_entry_s.dpl).
 * @param present whether or not kernel segment is available in RAM.
 */
extern void setup_interrupt(u8 isrnum, (void (*isr)(void)), u8 sel, u8 interrupt_type, u8 dpl, bool present);
