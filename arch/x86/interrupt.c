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

/* acknowledge command for command port */
#define PIC_ACK     0x20


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