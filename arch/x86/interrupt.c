#include <llanos/types.h>
#include <llanos/management/interrupt.h>

#include "pic8259.h"
#include "interrupt.h"

/* PIC start and end addresses [start, end) */
#define PIC1_START_ADDRESS      32
#define PIC1_END_ADDRESS        (PIC1_START_ADDRESS + 8)
#define PIC2_START_ADDRESS      (PIC1_START_ADDRESS + 8)
#define PIC2_END_ADDRESS        (PIC2_START_ADDRESS + 8)

/* command and data ports */
#define PIC1_COMMAND_PORT   0x20
#define PIC1_DATA_PORT      (PIC1_COMMAND_PORT + 1)
#define PIC2_COMMAND_PORT   0xa0
#define PIC2_DATA_PORT      (PIC2_COMMAND_PORT + 1)

/*
 * PIC controllers on the standard x86 system.
 */
static pic8259_t __pic1;
static pic8259_t __pic2;

/*
 * Interrupt Descriptor Table
 */
idt_entry_t idt[256] \
    __attribute__((align(4))) \
    __attribute__((section(".interrupt_table")));


void build_idtr(idt_register_t* idtr, idt_entry_t* idt, size_t count) {
    idtr->base = (u32)idt;
    idtr->limit = (count * sizeof(u32)) - 1;
}

void setup_interrupt(u8 isrnum, (void (*isr)(void)), u8 sel, u8 interrupt_type, u8 dpl, bool present) {
    idt_entry_t* entry;

    entry = &idt[isrnum];

    entry->base_low = ((u32)(isr) & 0xffff);
    entry->base_high = ((u32)(isr) >> 16);

    entry->sel = sel;
    entry->always0 = 0;
    entry->interrupt_type = interrupt_type;
    entry->dpl = dpl;
    entry->present = present ? 1 : 0;
}

static void __set_interrupt_gate(u8 isrnum, u32 interrupt_base_address, u16 segment_selector, u8 flags) {
    interrupt_descriptor_table_entry_t* entry;

    entry = &__interrupt_descriptor_table_entries[isrnum];

    entry->isr_address_base_low = interrupt_base_address & 0xffff;
    entry->isr_address_base_high = (interrupt_base_address >> 16) & 0xffff;

    entry->kernel_segment_selector = segment_selector;
    entry->reserved0 = 0;
    entry->flags = flags | 0x60;
}

/**
 * @brief Initialize the PIC (Programmable Interrupt Controller).
 *
 * This function will start the initialization process for the PIC8259
 * controller sending all ICW (Initialization Command Words) in the 
 * appropriate order to configure the PIC controller.
 */
static void __initialize_pic(void) {
    pic8259_init(&__pic1, PIC1_COMMAND_PORT, PIC1_DATA_PORT);
    pic8259_init(&__pic2, PIC2_COMMAND_PORT, PIC2_DATA_PORT);

    /* setup PIC controllers ICW1 (start initialization) */
    pic8259_send_icw1(&__pic1, true, false, false, true);
    pic8259_send_icw1(&__pic2, true, false, false, true);

    /* setup PIC controllers ICW2 (starting addresses) */
    pic8259_send_icw2(&__pic1, PIC1_START_ADDRESS);
    pic8259_send_icw2(&__pic2, PIC2_START_ADDRESS);

    /* 
     * setup PIC controllers ICW3 (because we are setting up cascased mode) 
     * there is a PIC alive at IR2, so setup cascading in that way.
     */
    pic8259_send_master_icw3(&__pic1, (1 << 2));
    pic8259_send_slave_icw3(&__pic2, 2);

    /* setup PIC controllers ICW4 (because we said we need to) */
    pic8259_send_icw4(&__pic1, true, PIC8259_BUFFERED_MODE_NONE, true);
    pic8259_send_icw4(&__pic2, true, PIC8259_BUFFERED_MODE_NONE, true);
}

extern void __isr_handle_0(void);

/**
 * @brief Initialize the Interrupt Table.
 *
 * This function will setup the Interrupt Descriptor Table Register, 
 * clear the interrupt descriptor table.
 */
extern void __initialize_interrupt_table(void) {
    size_t i;
    size_t idt_length;
    idt_register_t idtr;
    idt_entry_t* entry;

    idt_length = sizeof(idt) / sizeof(idt_entry_t);

    for (i = 0; i < idt_length; i++) {
        entry = &idt[i];

        entry->base_low = 0;
        entry->base_high = 0;
        entry->sel = 0;
        entry->always0 = 0;
        entry->interrupt_type = 0;
        entry->dpl = 0;
        entry->present = 0;
    }

    build_idtr(&idtr, &idt, idt_length);
    load_idtr(&idtr);

    setup_interrupt(0, __isr_handle_0, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, INTERRUPT_TYPE_INTERRUPT_GATE, 0, true);
}

extern void load_interrupt_table(const interrupt_table_t* interrupt_table) {
	int i;
	interrupt_table_entry_t* entry;

    __initialize_pic();
    __initialize_interrupt_table();

	// for (i = 0; i < interrupt_table->length; i++) {
	// 	entry = &interrupt_table->entries[i];

	// }
}