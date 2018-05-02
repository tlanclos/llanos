#pragma once

#include <llanos/types.h>

typedef struct pic8259_s pic8259_t;
typedef enum pic8259_buffered_mode_e pic8259_buffered_mode_t;

struct pic8259_s {
    u16 command_port;
    u16 data_port;
};

enum pic8259_buffered_mode_e {
    PIC8259_BUFFERED_MODE_NONE = 0,
    PIC8259_BUFFERED_MODE_SLAVE,
    PIC8259_BUFFERED_MODE_MASTER
};

/**
 * @brief Initialize a PIC structure with command port and data port.
 *
 * @param pic Pointer to PIC data structure to initialize.
 * @param command_port Command port to initialize the PIC data structure with.
 * @param data_port Data port to initialize the PIC data structure with.
 */
extern void pic8259_init(pic8259_t* pic, u16 command_port, u16 data_port);

/**
 * @brief Build an initialization command word 1 for PIC initialization.
 *
 * @param pic PIC struction to send command on.
 * @param init start initialization.
 * @param level_triggered If set to true, level triggered mode is enabled. If false, 
 *      then edge triggered mode is enabled.
 * @param single If true, use single PIC operation mode. If false, use cascade mode.
 * @param ic4 If true, the PIC will expect an ICW4 initialization command.
 */
extern void pic8259_send_icw1(pic8259_t* pic, bool init, bool level_triggered, bool single, bool ic4);

/**
 * @brief Build an initialization command word 2 for PIC initialization.
 *
 * @param pic PIC struction to send command on.
 * @param base Base location that we want this interrupt vector to start at.
 */
extern void pic8259_send_icw2(pic8259_t* pic, u8 base);

/**
 * @brief Build an initialization command word 3 for PIC initialization on the slave port.
 *
 * @param pic PIC struction to send command on.
 * @param slave_id The controller's slave ID.
 */
extern void pic8259_send_slave_icw3(pic8259_t* pic, u8 slave_id);

/**
 * @brief Build an initialization command word 3 for PIC initialization on the master port.
 *
 * @param pic PIC struction to send command on.
 * @param slave_mask A mask of slave bits on IRx that have slave controllers. For example:
 *      if IR1 and IR3 have slave controllers, pass (1 << 1) | (1 << 3).
 */
extern void pic8259_send_master_icw3(pic8259_t* pic, u8 slave_mask);

/**
 * @brief Build initialization command word 4 for PIC initialization.
 *
 * @param pic PIC struction to send command on.
 * @param sfnm Speical fully nested mode. If true, this allows the highest priority
 *      interrupt from a slave to be recognized by themaster while it is
 *      processing another interrupt from a slave. If false, one interrupt request 
 *      is processed at a time and others are ignored until the process is complete.
 * @param mode Buffered Mode. Select buffered operation or non-buffered operation of
 *      of the PIC.
 * @param aeoi Automatic End Of Interrupt. The EOI commands of OCW2 are used only if
 *      the AEOI mode is not selected. If selected, the interrupt automatically resets
 *      the interrupt request bit and does not modify priority. This is the preferred
 *      mode of operation and reduces the length of the ISR.
 */
extern void pic8259_send_icw4(pic8259_t* pic, bool sfnm, pic8259_buffered_mode_t mode, bool aeoi);
