#include <llanos/types.h>
#include <llanos/management/architecture.h>
#include <llanos/util/memory.h>

#include "gdt.h"
#include "interrupt.h"
#include "pic8259.h"
#include "isrhandler.h"

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
 * Global Descriptor Table
 */
gdt_entry_t __gdt[3];

/*
 * Interrupt Descriptor Table
 */
idt_entry_t __idt[256] \
    __attribute__((aligned(4))) \
    __attribute__((section(".interrupt_table")));

/*
 * PIC controllers on the standard x86 system.
 */
static pic8259_t __pic1;
static pic8259_t __pic2;


static void __generic_interrupt_handler(u32 isrnum) {
    return;
}


/**
 * @brief Initialize Global Descriptor Table.
 *
 * This function will setup the Global Descriptor Table Register.
 */
static void initialize_global_descriptor_table(void) {
    gdt_register_t gdtr;

    gdt_build_entry(&__gdt[0], 0, 0, 0, 0);
    /* entry 1 will be used for code */
    gdt_build_entry(
        &__gdt[1], 
        0, 
        0xffffffff, 
        gdt_build_access_byte(GDT_ACCESS_TYPE_CODE_EXECUTE_READ, false, 0), 
        gdt_build_granularity_byte(GDT_OPERAND_SIZE_32BIT, GDT_GRANULARITY_1KILO)
    );
    /* entry 2 will be used for data */
    gdt_build_entry(
        &__gdt[2], 
        0, 
        0xffffffff, 
        gdt_build_access_byte(GDT_ACCESS_TYPE_DATA_READ_WRITE, false, 0), 
        gdt_build_granularity_byte(GDT_OPERAND_SIZE_32BIT, GDT_GRANULARITY_1KILO)
    );
    
    gdt_build_gdtr(&gdtr, __gdt, sizeof(__gdt) / sizeof(gdt_entry_t));
    gdt_load_gdtr(&gdtr);
}

/**
 * @brief Initialize the PIC (Programmable Interrupt Controller).
 *
 * This function will start the initialization process for the PIC8259
 * controller sending all ICW (Initialization Command Words) in the 
 * appropriate order to configure the PIC controller.
 */
static void initialize_pic(void) {
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

/**
 * @brief Initialize the Interrupt Table.
 *
 * This function will setup the Interrupt Descriptor Table Register, 
 * clear the interrupt descriptor table.
 */
static void initialize_interrupt_descriptor_table(void) {
    idt_register_t idtr;
    idt_entry_t* entry;

    memory_set_value((u8*)__idt, 0, sizeof(__idt));
    interrupt_build_idtr(&idtr, __idt, sizeof(__idt) / sizeof(idt_entry_t));
    interrupt_load_idtr(&idtr, __generic_interrupt_handler);
}

static void initiailize_interrupt_functions(void) {
    interrupt_setup(&__idt[0], __isr_handler_0, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
}

void initialize_architecture(void) {
    initialize_global_descriptor_table();
    initialize_pic();
    initialize_interrupt_descriptor_table();
    initiailize_interrupt_functions();
}