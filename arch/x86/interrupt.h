#pragma once

#include <llanos/types.h>

#define GATE_TYPE_TASK_GATE_32BIT       (0b0101)
#define GATE_TYPE_INTERRUPT_GATE_16BIT  (0b0110)
#define GATE_TYPE_TRAP_GATE_16BIT       (0b0111)
#define GATE_TYPE_INTERRUPT_GATE_32BIT  (0b1110)
#define GATE_TYPE_TRAP_GATE_32BIT       (0b1111)

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
 *                        | isr255 |
 *                           ....
 *                        |  isr1  |
 * idtr.base (4 bytes) -> |  isr0  |
 *
 * If the IDT has 256 entries, then limit would be (256*4)-1=1023 or 0x3ff.
 *
 * @member limit limit represents the upper limit of idt.
 * @member base base address of idt.
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
 * @member gate_type choose from one of the GATE_TYPE_* values
 * @member storage_segment this should be set to 0 for interrupt and trap gates.
 * @member dpl Descriptor Priviledge Level (0 is highest priority, 3 is lowest priority).
 * @member present Present bit 1=available 0=unavailable (whether or not kernel segment is available in RAM)
 * @member base_high higher 16-bits of interrupt function address (unused in Task Gate Mode).
 */
struct idt_entry_s {
    u16 base_low;
    u16 sel;
    u8 always0;
    u8 gate_type : 4;
    u8 storage_segment : 1;
    u8 dpl : 2;
    u8 present : 1;
    u16 base_high;
} __attribute__((packed));


/**
 * @brief Build an IDTR from a base address and a size.
 *
 * @param idtr Destination idtr register.
 * @param idt IDT to load.
 * @param count Number of entries in IDT.
 */
extern void interrupt_build_idtr(idt_register_t* idtr, idt_entry_t* idt, size_t count);


/**
 * @brief Load Interrupt Descriptor Table Register and interrupt handler function to handle interrupts.
 *
 * This is equivalent to the assembly instruction `lidt [idtr]` along with the addition of setting
 * the generic interrupt handler in order to process interrupts.
 *
 * @param idtr pointer to interrupt descriptor table register to load.
 * @param interrupt_handler generic interrupt handler function that takes in the ISR number 
 *      and switches to the OS passed interrupt functions.
 */
extern void interrupt_load_idtr(idt_register_t* idtr, void (*interrupt_handler)(u32 isrnum));


/**
 * @brief Store Interrupt Descriptor Table Register.
 *
 * This is equivalent to the assembly instruction `sidt [idtr]`
 *
 * @param idtr pointer to destination interrupt descriptor table register.
 */
extern void interrupt_store_idtr(idt_register_t* idtr);

/**
 * @brief load interrupt to IDT.
 *
 * @param entry destination IDT entry to setup.
 * @param isr interrupt function base pointer.
 * @param sel segment selector or TSS segment (refer to idt_entry_s.sel).
 * @param gate_type gate type (refer to idt_entry_s.gate_type).
 * @param dpl descriptor priviledge level (refer to idt_entry_s.dpl).
 * @param present whether or not kernel segment is available in RAM.
 */
extern void interrupt_setup(idt_entry_t* entry, void (*isr)(void), u8 sel, u8 gate_type, u8 dpl, bool present);
