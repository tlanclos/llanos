#include <llanos/types.h>
#include <llanos/management/architecture.h>
#include <llanos/util/memory.h>

#include "gdt.h"
#include "interrupt.h"
#include "pic8259.h"
#include "isrhandler.h"
#include "paging.h"

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

/*
 * Paging Directory
 */
static page_directory_entry_t __page_directory[1024];


static void __generic_interrupt_handler(u32 isrnum) {

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

/**
 * @brief Initialize low level interrupt functions to point to our generic interrupt function.
 *
 * This function will initialize all interrupts with a function that will
 * call the generic interrupt function.
 */
static void initialize_interrupt_functions(void) {
    interrupt_setup(&__idt[0], __isr_handler_0, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[1], __isr_handler_1, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[2], __isr_handler_2, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[3], __isr_handler_3, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[4], __isr_handler_4, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[5], __isr_handler_5, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[6], __isr_handler_6, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[7], __isr_handler_7, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[8], __isr_handler_8, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[9], __isr_handler_9, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[10], __isr_handler_10, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[11], __isr_handler_11, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[12], __isr_handler_12, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[13], __isr_handler_13, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[14], __isr_handler_14, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[15], __isr_handler_15, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[16], __isr_handler_16, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[17], __isr_handler_17, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[18], __isr_handler_18, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[19], __isr_handler_19, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[20], __isr_handler_20, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[21], __isr_handler_21, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[22], __isr_handler_22, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[23], __isr_handler_23, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[24], __isr_handler_24, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[25], __isr_handler_25, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[26], __isr_handler_26, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[27], __isr_handler_27, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[28], __isr_handler_28, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[29], __isr_handler_29, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[30], __isr_handler_30, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[31], __isr_handler_31, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[32], __isr_handler_32, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[33], __isr_handler_33, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[34], __isr_handler_34, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[35], __isr_handler_35, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[36], __isr_handler_36, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[37], __isr_handler_37, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[38], __isr_handler_38, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[39], __isr_handler_39, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[40], __isr_handler_40, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[41], __isr_handler_41, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[42], __isr_handler_42, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[43], __isr_handler_43, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[44], __isr_handler_44, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[45], __isr_handler_45, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[46], __isr_handler_46, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[47], __isr_handler_47, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[48], __isr_handler_48, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[49], __isr_handler_49, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[50], __isr_handler_50, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[51], __isr_handler_51, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[52], __isr_handler_52, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[53], __isr_handler_53, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[54], __isr_handler_54, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[55], __isr_handler_55, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[56], __isr_handler_56, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[57], __isr_handler_57, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[58], __isr_handler_58, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[59], __isr_handler_59, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[60], __isr_handler_60, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[61], __isr_handler_61, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[62], __isr_handler_62, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[63], __isr_handler_63, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[64], __isr_handler_64, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[65], __isr_handler_65, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[66], __isr_handler_66, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[67], __isr_handler_67, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[68], __isr_handler_68, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[69], __isr_handler_69, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[70], __isr_handler_70, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[71], __isr_handler_71, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[72], __isr_handler_72, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[73], __isr_handler_73, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[74], __isr_handler_74, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[75], __isr_handler_75, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[76], __isr_handler_76, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[77], __isr_handler_77, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[78], __isr_handler_78, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[79], __isr_handler_79, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[80], __isr_handler_80, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[81], __isr_handler_81, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[82], __isr_handler_82, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[83], __isr_handler_83, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[84], __isr_handler_84, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[85], __isr_handler_85, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[86], __isr_handler_86, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[87], __isr_handler_87, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[88], __isr_handler_88, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[89], __isr_handler_89, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[90], __isr_handler_90, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[91], __isr_handler_91, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[92], __isr_handler_92, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[93], __isr_handler_93, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[94], __isr_handler_94, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[95], __isr_handler_95, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[96], __isr_handler_96, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[97], __isr_handler_97, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[98], __isr_handler_98, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[99], __isr_handler_99, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[100], __isr_handler_100, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[101], __isr_handler_101, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[102], __isr_handler_102, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[103], __isr_handler_103, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[104], __isr_handler_104, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[105], __isr_handler_105, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[106], __isr_handler_106, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[107], __isr_handler_107, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[108], __isr_handler_108, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[109], __isr_handler_109, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[110], __isr_handler_110, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[111], __isr_handler_111, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[112], __isr_handler_112, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[113], __isr_handler_113, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[114], __isr_handler_114, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[115], __isr_handler_115, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[116], __isr_handler_116, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[117], __isr_handler_117, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[118], __isr_handler_118, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[119], __isr_handler_119, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[120], __isr_handler_120, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[121], __isr_handler_121, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[122], __isr_handler_122, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[123], __isr_handler_123, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[124], __isr_handler_124, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[125], __isr_handler_125, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[126], __isr_handler_126, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[127], __isr_handler_127, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[128], __isr_handler_128, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[129], __isr_handler_129, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[130], __isr_handler_130, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[131], __isr_handler_131, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[132], __isr_handler_132, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[133], __isr_handler_133, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[134], __isr_handler_134, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[135], __isr_handler_135, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[136], __isr_handler_136, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[137], __isr_handler_137, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[138], __isr_handler_138, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[139], __isr_handler_139, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[140], __isr_handler_140, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[141], __isr_handler_141, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[142], __isr_handler_142, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[143], __isr_handler_143, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[144], __isr_handler_144, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[145], __isr_handler_145, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[146], __isr_handler_146, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[147], __isr_handler_147, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[148], __isr_handler_148, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[149], __isr_handler_149, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[150], __isr_handler_150, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[151], __isr_handler_151, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[152], __isr_handler_152, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[153], __isr_handler_153, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[154], __isr_handler_154, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[155], __isr_handler_155, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[156], __isr_handler_156, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[157], __isr_handler_157, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[158], __isr_handler_158, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[159], __isr_handler_159, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[160], __isr_handler_160, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[161], __isr_handler_161, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[162], __isr_handler_162, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[163], __isr_handler_163, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[164], __isr_handler_164, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[165], __isr_handler_165, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[166], __isr_handler_166, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[167], __isr_handler_167, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[168], __isr_handler_168, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[169], __isr_handler_169, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[170], __isr_handler_170, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[171], __isr_handler_171, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[172], __isr_handler_172, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[173], __isr_handler_173, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[174], __isr_handler_174, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[175], __isr_handler_175, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[176], __isr_handler_176, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[177], __isr_handler_177, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[178], __isr_handler_178, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[179], __isr_handler_179, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[180], __isr_handler_180, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[181], __isr_handler_181, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[182], __isr_handler_182, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[183], __isr_handler_183, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[184], __isr_handler_184, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[185], __isr_handler_185, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[186], __isr_handler_186, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[187], __isr_handler_187, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[188], __isr_handler_188, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[189], __isr_handler_189, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[190], __isr_handler_190, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[191], __isr_handler_191, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[192], __isr_handler_192, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[193], __isr_handler_193, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[194], __isr_handler_194, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[195], __isr_handler_195, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[196], __isr_handler_196, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[197], __isr_handler_197, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[198], __isr_handler_198, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[199], __isr_handler_199, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[200], __isr_handler_200, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[201], __isr_handler_201, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[202], __isr_handler_202, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[203], __isr_handler_203, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[204], __isr_handler_204, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[205], __isr_handler_205, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[206], __isr_handler_206, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[207], __isr_handler_207, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[208], __isr_handler_208, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[209], __isr_handler_209, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[210], __isr_handler_210, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[211], __isr_handler_211, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[212], __isr_handler_212, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[213], __isr_handler_213, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[214], __isr_handler_214, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[215], __isr_handler_215, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[216], __isr_handler_216, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[217], __isr_handler_217, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[218], __isr_handler_218, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[219], __isr_handler_219, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[220], __isr_handler_220, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[221], __isr_handler_221, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[222], __isr_handler_222, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[223], __isr_handler_223, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[224], __isr_handler_224, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[225], __isr_handler_225, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[226], __isr_handler_226, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[227], __isr_handler_227, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[228], __isr_handler_228, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[229], __isr_handler_229, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[230], __isr_handler_230, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[231], __isr_handler_231, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[232], __isr_handler_232, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[233], __isr_handler_233, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[234], __isr_handler_234, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[235], __isr_handler_235, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[236], __isr_handler_236, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[237], __isr_handler_237, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[238], __isr_handler_238, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[239], __isr_handler_239, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[240], __isr_handler_240, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[241], __isr_handler_241, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[242], __isr_handler_242, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[243], __isr_handler_243, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[244], __isr_handler_244, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[245], __isr_handler_245, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[246], __isr_handler_246, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[247], __isr_handler_247, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[248], __isr_handler_248, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[249], __isr_handler_249, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[250], __isr_handler_250, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[251], __isr_handler_251, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[252], __isr_handler_252, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[253], __isr_handler_253, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[254], __isr_handler_254, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
    interrupt_setup(&__idt[255], __isr_handler_255, SEGMENT_SELECTOR_KERNEL_CODE_32BIT, GATE_TYPE_INTERRUPT_GATE_32BIT, 0, true);
}


static void initialize_paging(void) {
    
}

void initialize_architecture(void) {
    initialize_global_descriptor_table();
    initialize_pic();
    initialize_interrupt_descriptor_table();
    initialize_interrupt_functions();
    initialize_paging();
}