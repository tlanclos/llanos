#include <llanos/types.h>
#include <llanos/management/interrupt.h>

#include <string.h>

#include "interrupt.h"

void generic_interrupt_handle(u32 isrnum) {

}

void interrupt_build_idtr(idt_register_t* idtr, idt_entry_t* idt, size_t count) {
    idtr->base = (u32)idt;
    idtr->limit = (count * sizeof(u32)) - 1;
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

extern void load_interrupt_table(const interrupt_table_t* interrupt_table) {
	int i;
	interrupt_table_entry_t* entry;


	// for (i = 0; i < interrupt_table->length; i++) {
	// 	entry = &interrupt_table->entries[i];

	// }
}