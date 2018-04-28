#include <llanos/types.h>
#include <llanos/management/interrupts.h>

/* start and end interrupts are inclusive */
#define PIC1_START_INTERRUPT    0x20
#define PIC1_END_INTERTUPT      0x27

/* start and end interrupts are inclusive */
#define PIC2_START_INTERRUPT    0x28
#define PIC2_END_INTERRUPT      (PIC2_START_INTERRUPT + 7)

/* command and data ports */
#define PIC1_COMMAND_PORT   0x20
#define PIC1_DATA_PORT      (PIC1_COMMAND_PORT + 1)
#define PIC2_COMMAND_PORT   0xa0
#define PIC2_DATA_PORT      (PIC2_COMMAND_PORT + 1)

/* reset PIC command for command port */
#define PIC_RESET	0x11

/* acknowledge command for command port */
#define PIC_ACK     0x20


/* ICW4 (not) needed */
#define ICW1_ICW4       0x01

/* Single (cascade) mode */
#define ICW1_SINGLE     0x02

/* Call address interval 4 (8) */
#define ICW1_INTERVAL4  0x04

/* Level triggered (edge) mode */
#define ICW1_LEVEL      0x08

/* Initialization - required! */
#define ICW1_INIT       0x10


 
/* 8086/88 (MCS-80/85) mode */
#define ICW4_8086       0x01

/* Auto (normal) EOI */
#define ICW4_AUTO       0x02

/* Buffered mode/slave */
#define ICW4_BUF_SLAVE  0x08

/* Buffered mode/master */
#define ICW4_BUF_MASTER 0x0C

/* Special fully nested (not) */
#define ICW4_SFNM       0x10


extern void __output_byte(u16 port, u8 data);

static void __pic_acknowledge(u32 interrupt_number) {
    if (interrupt_number >= PIC1_START_INTERRUPT && interrupt_number <= PIC2_END_INTERRUPT) {
        if (interrupt_number < PIC2_START_INTERRUPT) {
            __output_byte(PIC1_COMMAND_PORT, PIC_ACK);
        } else {
            __output_byte(PIC2_COMMAND_PORT, PIC_ACK);
        }
    }
}

static void __interrupt_init(void) {
    /* reset both PIC controllers */
    __output_byte(PIC1_COMMAND_PORT, PIC_RESET);
    __output_byte(PIC2_COMMAND_PORT, PIC_RESET);

    /* remap PIC1 to start at 32 */
    __output_byte(PIC1_DATA_PORT, 32);

    /* remap PIC2 to start at 40 */
    __output_byte(PIC2_DATA_PORT, 40);

    __output_byte(PIC1_DATA_PORT, ICW1_INTERVAL4);
    __output_byte(PIC2_DATA_PORT, ICW1_SINGLE);

    __output_byte(PIC1_DATA_PORT, ICW4_8086);
    __output_byte(PIC2_DATA_PORT, ICW4_8086);
}


extern void load_interrupt_table(const interrupt_table_t* interrupt_table) {
	int i;
	interrupt_table_entry_t* entry;

	for (i = 0; i < interrupt_table->length; i++) {
		entry = &interrupt_table->entries[i];

	}
}