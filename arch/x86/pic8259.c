#include <llanos/types.h>

#include "pic8259.h"

extern void __output_byte(u16 port, u8 data);

static void pic8259_send_command(pic8259_t* pic, u8 command) {
    __output_byte(pic->command_port, command);
}

static void pic8259_send_data(pic8259_t* pic, u8 data) {
    __output_byte(pic->data_port, data);
}

static u8 pic8259_get_buffered_mode_byte(pic8259_buffered_mode_t mode) {
    if (mode == PIC8259_BUFFERED_MODE_NONE) {
        return 0;
    } else if (mode == PIC8259_BUFFERED_MODE_SLAVE) {
        return 1;
    } else if (mode == PIC8259_BUFFERED_MODE_MASTER) {
        return 3;
    } else {
        /* todo: should abort kernel here */
        return 0;
    }
}

void pic8259_init(pic8259_t* pic, u16 command_port, u16 data_port) {
    pic->command_port = command_port;
    pic->data_port = data_port;
}

void pic8259_send_icw1(pic8259_t* pic, bool init, bool level_triggered, bool single, bool ic4) {
    pic8259_send_data(
        pic, 
        (init ? (1 << 4) : 0) | (level_triggered ? (1 << 3) : 0) | (single ? (1 << 1) : 0) | (ic4 ? (1 << 0) : 0)
    );
}

void pic8259_send_icw2(pic8259_t* pic, u8 base) {
    pic8259_send_data(pic, base & 0x7);
}

void pic8259_send_slave_icw3(pic8259_t* pic, u8 slave_id) {
    pic8259_send_data(pic, slave_id);
}

void pic8259_send_master_icw3(pic8259_t* pic, u8 slave_mask) {
    pic8259_send_data(pic, slave_mask);
}

void pic8259_send_icw4(pic8259_t* pic, bool sfnm, pic8259_buffered_mode_t mode, bool aeoi) {
    pic8259_send_data(
        pic,
        (sfnm ? (1 << 4) : 0) | (pic8259_get_buffered_mode_byte(mode) << 2) | (aeoi ? (1 << 1) : 0)  | 1
    );
}
