#include <llanos/types.h>
#include <llanos/management/interrupts.h>

#include "pic8259.h"

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





/* reset PIC command for command port */
#define PIC_RESET	0x11

/* acknowledge command for command port */
#define PIC_ACK     0x20





/*
 * Interrupt descriptor table for storing pointers to current interrupts.
 */
static u64 __interrupt_descriptor_table[32] __attribute__((section(".interrupts")));

/*
 * PIC controllers on the standard x86 system.
 */
static pic8259_t __pic1;
static pic8259_t __pic2;


static void __pic_acknowledge(u32 interrupt_number) {
    if (interrupt_number >= PIC1_START_INTERRUPT && interrupt_number <= PIC2_END_INTERRUPT) {
        if (interrupt_number < PIC2_START_INTERRUPT) {
            __output_byte(PIC1_COMMAND_PORT, PIC_ACK);
        } else {
            __output_byte(PIC2_COMMAND_PORT, PIC_ACK);
        }
    }
}

static void __pic_init(void) {
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


extern void load_interrupt_table(const interrupt_table_t* interrupt_table) {
	int i;
	interrupt_table_entry_t* entry;

    __pic_init();
    __setup_base_interrupts();

	for (i = 0; i < interrupt_table->length; i++) {
		entry = &interrupt_table->entries[i];

	}
}