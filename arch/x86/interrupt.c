#include <llanos/types.h>

#include "interrupt.h"

void (*__generic_interrupt_handler)(u32 isrnum);

/**
 * @brief Load IDT register into memory.
 *
 * This is equivalent to the assembly instruction `lidt [idtr]`.
 *
 * @param idtr pointer to interrupt descriptor table register to load.
 */
extern void __interrupt_load_idtr(idt_register_t* idtr);

void interrupt_build_idtr(idt_register_t* idtr, idt_entry_t* idt, size_t count) {
    idtr->base = (u32)idt;
    idtr->limit = (count * sizeof(u32)) - 1;
}

void interrupt_load_idtr(idt_register_t* idtr, void (*interrupt_handler)(u32 isrnum)) {
    __generic_interrupt_handler = interrupt_handler;
    __interrupt_load_idtr(idtr);
}

void interrupt_setup(idt_entry_t* entry, void (*isr)(void), u8 sel, u8 gate_type, u8 dpl, bool present) {
    entry->base_low = ((u32)(isr) & 0xffff);
    entry->base_high = ((u32)(isr) >> 16);

    entry->sel = sel;
    entry->always0 = 0;
    entry->gate_type = gate_type;
    entry->storage_segment = 0;
    entry->dpl = dpl;
    entry->present = present ? 1 : 0;
}
