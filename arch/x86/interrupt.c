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

/* interrupt table entries */
#define NUMBER_OF_INTERRUPTS    256

typedef struct interrupt_descriptor_table_entry_s interrupt_descriptor_table_entry_t;
typedef struct interrupt_table_descriptor_s interrupt_table_descriptor_t;

struct interrupt_descriptor_table_entry_s {
    u16 isr_address_base_low;
    u16 kernel_segment_selector;
    u8 reserved0;
    u8 flags;
    u16 isr_address_base_high;
} __attribute__((packed));

struct interrupt_table_descriptor_s {
    u16 limit;
    u32 base;
} __attribute__((packed));

/*
 * PIC controllers on the standard x86 system.
 */
static pic8259_t __pic1;
static pic8259_t __pic2;

/*
 * Interrupt Descriptor Table and Interrupt Table Descriptor
 */
interrupt_descriptor_table_entry_t __interrupt_descriptor_table_entries[NUMBER_OF_INTERRUPTS] \
    __attribute__((section(".interrupt_table")));
interrupt_table_descriptor_t __interrupt_table_descriptor \
    __attribute__((section(".interrupt_table")));

/*
 * Internal interrupt table structure.
 * TODO: how to make common entries size.
 */
interrupt_table_entry_t __interrupt_table_entries[NUMBER_OF_INTERRUPTS];
interrupt_table_t __interrupt_table = {
    .entries = __interrupt_table_entries,
    .length = sizeof(__interrupt_table_entries) / sizeof(interrupt_table_entry_t)
};


static void __set_interrupt_gate(u8 isrnum, u32 interrupt_base_address, u16 segment_selector, u8 flags) {
    interrupt_descriptor_table_entry_t* entry;

    entry = &__interrupt_descriptor_table_entries[isrnum];

    entry->isr_address_base_low = interrupt_base_address & 0xffff;
    entry->isr_address_base_high = (interrupt_base_address >> 16) & 0xffff;

    entry->kernel_segment_selector = segment_selector;
    entry->reserved0 = 0;
    entry->flags = flags | 0x60;
}

extern void __load_idt(void);

/**
 * @brief Initialize interrupt table
 *
 * This function's sole purpose is to initialize the x86 IDT with a basic
 * set of interrupt routines that will immediately return.
 */
// static void __initialize_interrupt_table(void) {
//     size_t i;

//     for (i = 0; i < NUMBER_OF_INTERRUPTS; i++) {
//         interrupt_descriptor_table_entry_t* entry = &__interrupt_descriptor_table_entries[i];

//         entry->isr_address_base_low = 0;
//         entry->isr_address_base_high = 0;
//         entry->kernel_segment_selector = 0;
//         entry->reserved0 = 0;
//         entry->flags = 0;
//     }

//     __interrupt_table_descriptor.limit = (sizeof(interrupt_descriptor_table_entry_t) * NUMBER_OF_INTERRUPTS) - 1;
//     __interrupt_table_descriptor.base = (u32)__interrupt_descriptor_table_entries;

//     __set_interrupt_gate(0, (u32)__isr_handler_0, 0x08, 0x8e);
//     // __set_interrupt_gate(1, (u32)__isr_handler_1, 0x08, 0x8e);
//     // __set_interrupt_gate(2, (u32)__isr_handler_2, 0x08, 0x8e);
//     // __set_interrupt_gate(3, (u32)__isr_handler_3, 0x08, 0x8e);
//     // __set_interrupt_gate(4, (u32)__isr_handler_4, 0x08, 0x8e);
//     // __set_interrupt_gate(5, (u32)__isr_handler_5, 0x08, 0x8e);
//     // __set_interrupt_gate(6, (u32)__isr_handler_6, 0x08, 0x8e);
//     // __set_interrupt_gate(7, (u32)__isr_handler_7, 0x08, 0x8e);
//     // __set_interrupt_gate(8, (u32)__isr_handler_8, 0x08, 0x8e);
//     // __set_interrupt_gate(9, (u32)__isr_handler_9, 0x08, 0x8e);
//     // __set_interrupt_gate(10, (u32)__isr_handler_10, 0x08, 0x8e);
//     // __set_interrupt_gate(11, (u32)__isr_handler_11, 0x08, 0x8e);
//     // __set_interrupt_gate(12, (u32)__isr_handler_12, 0x08, 0x8e);
//     // __set_interrupt_gate(13, (u32)__isr_handler_13, 0x08, 0x8e);
//     // __set_interrupt_gate(14, (u32)__isr_handler_14, 0x08, 0x8e);
//     // __set_interrupt_gate(15, (u32)__isr_handler_15, 0x08, 0x8e);
//     // __set_interrupt_gate(16, (u32)__isr_handler_16, 0x08, 0x8e);
//     // __set_interrupt_gate(17, (u32)__isr_handler_17, 0x08, 0x8e);
//     // __set_interrupt_gate(18, (u32)__isr_handler_18, 0x08, 0x8e);
//     // __set_interrupt_gate(19, (u32)__isr_handler_19, 0x08, 0x8e);
//     // __set_interrupt_gate(20, (u32)__isr_handler_20, 0x08, 0x8e);
//     // __set_interrupt_gate(21, (u32)__isr_handler_21, 0x08, 0x8e);
//     // __set_interrupt_gate(22, (u32)__isr_handler_22, 0x08, 0x8e);
//     // __set_interrupt_gate(23, (u32)__isr_handler_23, 0x08, 0x8e);
//     // __set_interrupt_gate(24, (u32)__isr_handler_24, 0x08, 0x8e);
//     // __set_interrupt_gate(25, (u32)__isr_handler_25, 0x08, 0x8e);
//     // __set_interrupt_gate(26, (u32)__isr_handler_26, 0x08, 0x8e);
//     // __set_interrupt_gate(27, (u32)__isr_handler_27, 0x08, 0x8e);
//     // __set_interrupt_gate(28, (u32)__isr_handler_28, 0x08, 0x8e);
//     // __set_interrupt_gate(29, (u32)__isr_handler_29, 0x08, 0x8e);
//     // __set_interrupt_gate(30, (u32)__isr_handler_30, 0x08, 0x8e);
//     // __set_interrupt_gate(31, (u32)__isr_handler_31, 0x08, 0x8e);
//     // __set_interrupt_gate(32, (u32)__isr_handler_32, 0x08, 0x8e);
//     // __set_interrupt_gate(33, (u32)__isr_handler_33, 0x08, 0x8e);
//     // __set_interrupt_gate(34, (u32)__isr_handler_34, 0x08, 0x8e);
//     // __set_interrupt_gate(35, (u32)__isr_handler_35, 0x08, 0x8e);
//     // __set_interrupt_gate(36, (u32)__isr_handler_36, 0x08, 0x8e);
//     // __set_interrupt_gate(37, (u32)__isr_handler_37, 0x08, 0x8e);
//     // __set_interrupt_gate(38, (u32)__isr_handler_38, 0x08, 0x8e);
//     // __set_interrupt_gate(39, (u32)__isr_handler_39, 0x08, 0x8e);
//     // __set_interrupt_gate(40, (u32)__isr_handler_40, 0x08, 0x8e);
//     // __set_interrupt_gate(41, (u32)__isr_handler_41, 0x08, 0x8e);
//     // __set_interrupt_gate(42, (u32)__isr_handler_42, 0x08, 0x8e);
//     // __set_interrupt_gate(43, (u32)__isr_handler_43, 0x08, 0x8e);
//     // __set_interrupt_gate(44, (u32)__isr_handler_44, 0x08, 0x8e);
//     // __set_interrupt_gate(45, (u32)__isr_handler_45, 0x08, 0x8e);
//     // __set_interrupt_gate(46, (u32)__isr_handler_46, 0x08, 0x8e);
//     // __set_interrupt_gate(47, (u32)__isr_handler_47, 0x08, 0x8e);
//     // __set_interrupt_gate(48, (u32)__isr_handler_48, 0x08, 0x8e);
//     // __set_interrupt_gate(49, (u32)__isr_handler_49, 0x08, 0x8e);
//     // __set_interrupt_gate(50, (u32)__isr_handler_50, 0x08, 0x8e);
//     // __set_interrupt_gate(51, (u32)__isr_handler_51, 0x08, 0x8e);
//     // __set_interrupt_gate(52, (u32)__isr_handler_52, 0x08, 0x8e);
//     // __set_interrupt_gate(53, (u32)__isr_handler_53, 0x08, 0x8e);
//     // __set_interrupt_gate(54, (u32)__isr_handler_54, 0x08, 0x8e);
//     // __set_interrupt_gate(55, (u32)__isr_handler_55, 0x08, 0x8e);
//     // __set_interrupt_gate(56, (u32)__isr_handler_56, 0x08, 0x8e);
//     // __set_interrupt_gate(57, (u32)__isr_handler_57, 0x08, 0x8e);
//     // __set_interrupt_gate(58, (u32)__isr_handler_58, 0x08, 0x8e);
//     // __set_interrupt_gate(59, (u32)__isr_handler_59, 0x08, 0x8e);
//     // __set_interrupt_gate(60, (u32)__isr_handler_60, 0x08, 0x8e);
//     // __set_interrupt_gate(61, (u32)__isr_handler_61, 0x08, 0x8e);
//     // __set_interrupt_gate(62, (u32)__isr_handler_62, 0x08, 0x8e);
//     // __set_interrupt_gate(63, (u32)__isr_handler_63, 0x08, 0x8e);
//     // __set_interrupt_gate(64, (u32)__isr_handler_64, 0x08, 0x8e);
//     // __set_interrupt_gate(65, (u32)__isr_handler_65, 0x08, 0x8e);
//     // __set_interrupt_gate(66, (u32)__isr_handler_66, 0x08, 0x8e);
//     // __set_interrupt_gate(67, (u32)__isr_handler_67, 0x08, 0x8e);
//     // __set_interrupt_gate(68, (u32)__isr_handler_68, 0x08, 0x8e);
//     // __set_interrupt_gate(69, (u32)__isr_handler_69, 0x08, 0x8e);
//     // __set_interrupt_gate(70, (u32)__isr_handler_70, 0x08, 0x8e);
//     // __set_interrupt_gate(71, (u32)__isr_handler_71, 0x08, 0x8e);
//     // __set_interrupt_gate(72, (u32)__isr_handler_72, 0x08, 0x8e);
//     // __set_interrupt_gate(73, (u32)__isr_handler_73, 0x08, 0x8e);
//     // __set_interrupt_gate(74, (u32)__isr_handler_74, 0x08, 0x8e);
//     // __set_interrupt_gate(75, (u32)__isr_handler_75, 0x08, 0x8e);
//     // __set_interrupt_gate(76, (u32)__isr_handler_76, 0x08, 0x8e);
//     // __set_interrupt_gate(77, (u32)__isr_handler_77, 0x08, 0x8e);
//     // __set_interrupt_gate(78, (u32)__isr_handler_78, 0x08, 0x8e);
//     // __set_interrupt_gate(79, (u32)__isr_handler_79, 0x08, 0x8e);
//     // __set_interrupt_gate(80, (u32)__isr_handler_80, 0x08, 0x8e);
//     // __set_interrupt_gate(81, (u32)__isr_handler_81, 0x08, 0x8e);
//     // __set_interrupt_gate(82, (u32)__isr_handler_82, 0x08, 0x8e);
//     // __set_interrupt_gate(83, (u32)__isr_handler_83, 0x08, 0x8e);
//     // __set_interrupt_gate(84, (u32)__isr_handler_84, 0x08, 0x8e);
//     // __set_interrupt_gate(85, (u32)__isr_handler_85, 0x08, 0x8e);
//     // __set_interrupt_gate(86, (u32)__isr_handler_86, 0x08, 0x8e);
//     // __set_interrupt_gate(87, (u32)__isr_handler_87, 0x08, 0x8e);
//     // __set_interrupt_gate(88, (u32)__isr_handler_88, 0x08, 0x8e);
//     // __set_interrupt_gate(89, (u32)__isr_handler_89, 0x08, 0x8e);
//     // __set_interrupt_gate(90, (u32)__isr_handler_90, 0x08, 0x8e);
//     // __set_interrupt_gate(91, (u32)__isr_handler_91, 0x08, 0x8e);
//     // __set_interrupt_gate(92, (u32)__isr_handler_92, 0x08, 0x8e);
//     // __set_interrupt_gate(93, (u32)__isr_handler_93, 0x08, 0x8e);
//     // __set_interrupt_gate(94, (u32)__isr_handler_94, 0x08, 0x8e);
//     // __set_interrupt_gate(95, (u32)__isr_handler_95, 0x08, 0x8e);
//     // __set_interrupt_gate(96, (u32)__isr_handler_96, 0x08, 0x8e);
//     // __set_interrupt_gate(97, (u32)__isr_handler_97, 0x08, 0x8e);
//     // __set_interrupt_gate(98, (u32)__isr_handler_98, 0x08, 0x8e);
//     // __set_interrupt_gate(99, (u32)__isr_handler_99, 0x08, 0x8e);
//     // __set_interrupt_gate(100, (u32)__isr_handler_100, 0x08, 0x8e);
//     // __set_interrupt_gate(101, (u32)__isr_handler_101, 0x08, 0x8e);
//     // __set_interrupt_gate(102, (u32)__isr_handler_102, 0x08, 0x8e);
//     // __set_interrupt_gate(103, (u32)__isr_handler_103, 0x08, 0x8e);
//     // __set_interrupt_gate(104, (u32)__isr_handler_104, 0x08, 0x8e);
//     // __set_interrupt_gate(105, (u32)__isr_handler_105, 0x08, 0x8e);
//     // __set_interrupt_gate(106, (u32)__isr_handler_106, 0x08, 0x8e);
//     // __set_interrupt_gate(107, (u32)__isr_handler_107, 0x08, 0x8e);
//     // __set_interrupt_gate(108, (u32)__isr_handler_108, 0x08, 0x8e);
//     // __set_interrupt_gate(109, (u32)__isr_handler_109, 0x08, 0x8e);
//     // __set_interrupt_gate(110, (u32)__isr_handler_110, 0x08, 0x8e);
//     // __set_interrupt_gate(111, (u32)__isr_handler_111, 0x08, 0x8e);
//     // __set_interrupt_gate(112, (u32)__isr_handler_112, 0x08, 0x8e);
//     // __set_interrupt_gate(113, (u32)__isr_handler_113, 0x08, 0x8e);
//     // __set_interrupt_gate(114, (u32)__isr_handler_114, 0x08, 0x8e);
//     // __set_interrupt_gate(115, (u32)__isr_handler_115, 0x08, 0x8e);
//     // __set_interrupt_gate(116, (u32)__isr_handler_116, 0x08, 0x8e);
//     // __set_interrupt_gate(117, (u32)__isr_handler_117, 0x08, 0x8e);
//     // __set_interrupt_gate(118, (u32)__isr_handler_118, 0x08, 0x8e);
//     // __set_interrupt_gate(119, (u32)__isr_handler_119, 0x08, 0x8e);
//     // __set_interrupt_gate(120, (u32)__isr_handler_120, 0x08, 0x8e);
//     // __set_interrupt_gate(121, (u32)__isr_handler_121, 0x08, 0x8e);
//     // __set_interrupt_gate(122, (u32)__isr_handler_122, 0x08, 0x8e);
//     // __set_interrupt_gate(123, (u32)__isr_handler_123, 0x08, 0x8e);
//     // __set_interrupt_gate(124, (u32)__isr_handler_124, 0x08, 0x8e);
//     // __set_interrupt_gate(125, (u32)__isr_handler_125, 0x08, 0x8e);
//     // __set_interrupt_gate(126, (u32)__isr_handler_126, 0x08, 0x8e);
//     // __set_interrupt_gate(127, (u32)__isr_handler_127, 0x08, 0x8e);
//     // __set_interrupt_gate(128, (u32)__isr_handler_128, 0x08, 0x8e);
//     // __set_interrupt_gate(129, (u32)__isr_handler_129, 0x08, 0x8e);
//     // __set_interrupt_gate(130, (u32)__isr_handler_130, 0x08, 0x8e);
//     // __set_interrupt_gate(131, (u32)__isr_handler_131, 0x08, 0x8e);
//     // __set_interrupt_gate(132, (u32)__isr_handler_132, 0x08, 0x8e);
//     // __set_interrupt_gate(133, (u32)__isr_handler_133, 0x08, 0x8e);
//     // __set_interrupt_gate(134, (u32)__isr_handler_134, 0x08, 0x8e);
//     // __set_interrupt_gate(135, (u32)__isr_handler_135, 0x08, 0x8e);
//     // __set_interrupt_gate(136, (u32)__isr_handler_136, 0x08, 0x8e);
//     // __set_interrupt_gate(137, (u32)__isr_handler_137, 0x08, 0x8e);
//     // __set_interrupt_gate(138, (u32)__isr_handler_138, 0x08, 0x8e);
//     // __set_interrupt_gate(139, (u32)__isr_handler_139, 0x08, 0x8e);
//     // __set_interrupt_gate(140, (u32)__isr_handler_140, 0x08, 0x8e);
//     // __set_interrupt_gate(141, (u32)__isr_handler_141, 0x08, 0x8e);
//     // __set_interrupt_gate(142, (u32)__isr_handler_142, 0x08, 0x8e);
//     // __set_interrupt_gate(143, (u32)__isr_handler_143, 0x08, 0x8e);
//     // __set_interrupt_gate(144, (u32)__isr_handler_144, 0x08, 0x8e);
//     // __set_interrupt_gate(145, (u32)__isr_handler_145, 0x08, 0x8e);
//     // __set_interrupt_gate(146, (u32)__isr_handler_146, 0x08, 0x8e);
//     // __set_interrupt_gate(147, (u32)__isr_handler_147, 0x08, 0x8e);
//     // __set_interrupt_gate(148, (u32)__isr_handler_148, 0x08, 0x8e);
//     // __set_interrupt_gate(149, (u32)__isr_handler_149, 0x08, 0x8e);
//     // __set_interrupt_gate(150, (u32)__isr_handler_150, 0x08, 0x8e);
//     // __set_interrupt_gate(151, (u32)__isr_handler_151, 0x08, 0x8e);
//     // __set_interrupt_gate(152, (u32)__isr_handler_152, 0x08, 0x8e);
//     // __set_interrupt_gate(153, (u32)__isr_handler_153, 0x08, 0x8e);
//     // __set_interrupt_gate(154, (u32)__isr_handler_154, 0x08, 0x8e);
//     // __set_interrupt_gate(155, (u32)__isr_handler_155, 0x08, 0x8e);
//     // __set_interrupt_gate(156, (u32)__isr_handler_156, 0x08, 0x8e);
//     // __set_interrupt_gate(157, (u32)__isr_handler_157, 0x08, 0x8e);
//     // __set_interrupt_gate(158, (u32)__isr_handler_158, 0x08, 0x8e);
//     // __set_interrupt_gate(159, (u32)__isr_handler_159, 0x08, 0x8e);
//     // __set_interrupt_gate(160, (u32)__isr_handler_160, 0x08, 0x8e);
//     // __set_interrupt_gate(161, (u32)__isr_handler_161, 0x08, 0x8e);
//     // __set_interrupt_gate(162, (u32)__isr_handler_162, 0x08, 0x8e);
//     // __set_interrupt_gate(163, (u32)__isr_handler_163, 0x08, 0x8e);
//     // __set_interrupt_gate(164, (u32)__isr_handler_164, 0x08, 0x8e);
//     // __set_interrupt_gate(165, (u32)__isr_handler_165, 0x08, 0x8e);
//     // __set_interrupt_gate(166, (u32)__isr_handler_166, 0x08, 0x8e);
//     // __set_interrupt_gate(167, (u32)__isr_handler_167, 0x08, 0x8e);
//     // __set_interrupt_gate(168, (u32)__isr_handler_168, 0x08, 0x8e);
//     // __set_interrupt_gate(169, (u32)__isr_handler_169, 0x08, 0x8e);
//     // __set_interrupt_gate(170, (u32)__isr_handler_170, 0x08, 0x8e);
//     // __set_interrupt_gate(171, (u32)__isr_handler_171, 0x08, 0x8e);
//     // __set_interrupt_gate(172, (u32)__isr_handler_172, 0x08, 0x8e);
//     // __set_interrupt_gate(173, (u32)__isr_handler_173, 0x08, 0x8e);
//     // __set_interrupt_gate(174, (u32)__isr_handler_174, 0x08, 0x8e);
//     // __set_interrupt_gate(175, (u32)__isr_handler_175, 0x08, 0x8e);
//     // __set_interrupt_gate(176, (u32)__isr_handler_176, 0x08, 0x8e);
//     // __set_interrupt_gate(177, (u32)__isr_handler_177, 0x08, 0x8e);
//     // __set_interrupt_gate(178, (u32)__isr_handler_178, 0x08, 0x8e);
//     // __set_interrupt_gate(179, (u32)__isr_handler_179, 0x08, 0x8e);
//     // __set_interrupt_gate(180, (u32)__isr_handler_180, 0x08, 0x8e);
//     // __set_interrupt_gate(181, (u32)__isr_handler_181, 0x08, 0x8e);
//     // __set_interrupt_gate(182, (u32)__isr_handler_182, 0x08, 0x8e);
//     // __set_interrupt_gate(183, (u32)__isr_handler_183, 0x08, 0x8e);
//     // __set_interrupt_gate(184, (u32)__isr_handler_184, 0x08, 0x8e);
//     // __set_interrupt_gate(185, (u32)__isr_handler_185, 0x08, 0x8e);
//     // __set_interrupt_gate(186, (u32)__isr_handler_186, 0x08, 0x8e);
//     // __set_interrupt_gate(187, (u32)__isr_handler_187, 0x08, 0x8e);
//     // __set_interrupt_gate(188, (u32)__isr_handler_188, 0x08, 0x8e);
//     // __set_interrupt_gate(189, (u32)__isr_handler_189, 0x08, 0x8e);
//     // __set_interrupt_gate(190, (u32)__isr_handler_190, 0x08, 0x8e);
//     // __set_interrupt_gate(191, (u32)__isr_handler_191, 0x08, 0x8e);
//     // __set_interrupt_gate(192, (u32)__isr_handler_192, 0x08, 0x8e);
//     // __set_interrupt_gate(193, (u32)__isr_handler_193, 0x08, 0x8e);
//     // __set_interrupt_gate(194, (u32)__isr_handler_194, 0x08, 0x8e);
//     // __set_interrupt_gate(195, (u32)__isr_handler_195, 0x08, 0x8e);
//     // __set_interrupt_gate(196, (u32)__isr_handler_196, 0x08, 0x8e);
//     // __set_interrupt_gate(197, (u32)__isr_handler_197, 0x08, 0x8e);
//     // __set_interrupt_gate(198, (u32)__isr_handler_198, 0x08, 0x8e);
//     // __set_interrupt_gate(199, (u32)__isr_handler_199, 0x08, 0x8e);
//     // __set_interrupt_gate(200, (u32)__isr_handler_200, 0x08, 0x8e);
//     // __set_interrupt_gate(201, (u32)__isr_handler_201, 0x08, 0x8e);
//     // __set_interrupt_gate(202, (u32)__isr_handler_202, 0x08, 0x8e);
//     // __set_interrupt_gate(203, (u32)__isr_handler_203, 0x08, 0x8e);
//     // __set_interrupt_gate(204, (u32)__isr_handler_204, 0x08, 0x8e);
//     // __set_interrupt_gate(205, (u32)__isr_handler_205, 0x08, 0x8e);
//     // __set_interrupt_gate(206, (u32)__isr_handler_206, 0x08, 0x8e);
//     // __set_interrupt_gate(207, (u32)__isr_handler_207, 0x08, 0x8e);
//     // __set_interrupt_gate(208, (u32)__isr_handler_208, 0x08, 0x8e);
//     // __set_interrupt_gate(209, (u32)__isr_handler_209, 0x08, 0x8e);
//     // __set_interrupt_gate(210, (u32)__isr_handler_210, 0x08, 0x8e);
//     // __set_interrupt_gate(211, (u32)__isr_handler_211, 0x08, 0x8e);
//     // __set_interrupt_gate(212, (u32)__isr_handler_212, 0x08, 0x8e);
//     // __set_interrupt_gate(213, (u32)__isr_handler_213, 0x08, 0x8e);
//     // __set_interrupt_gate(214, (u32)__isr_handler_214, 0x08, 0x8e);
//     // __set_interrupt_gate(215, (u32)__isr_handler_215, 0x08, 0x8e);
//     // __set_interrupt_gate(216, (u32)__isr_handler_216, 0x08, 0x8e);
//     // __set_interrupt_gate(217, (u32)__isr_handler_217, 0x08, 0x8e);
//     // __set_interrupt_gate(218, (u32)__isr_handler_218, 0x08, 0x8e);
//     // __set_interrupt_gate(219, (u32)__isr_handler_219, 0x08, 0x8e);
//     // __set_interrupt_gate(220, (u32)__isr_handler_220, 0x08, 0x8e);
//     // __set_interrupt_gate(221, (u32)__isr_handler_221, 0x08, 0x8e);
//     // __set_interrupt_gate(222, (u32)__isr_handler_222, 0x08, 0x8e);
//     // __set_interrupt_gate(223, (u32)__isr_handler_223, 0x08, 0x8e);
//     // __set_interrupt_gate(224, (u32)__isr_handler_224, 0x08, 0x8e);
//     // __set_interrupt_gate(225, (u32)__isr_handler_225, 0x08, 0x8e);
//     // __set_interrupt_gate(226, (u32)__isr_handler_226, 0x08, 0x8e);
//     // __set_interrupt_gate(227, (u32)__isr_handler_227, 0x08, 0x8e);
//     // __set_interrupt_gate(228, (u32)__isr_handler_228, 0x08, 0x8e);
//     // __set_interrupt_gate(229, (u32)__isr_handler_229, 0x08, 0x8e);
//     // __set_interrupt_gate(230, (u32)__isr_handler_230, 0x08, 0x8e);
//     // __set_interrupt_gate(231, (u32)__isr_handler_231, 0x08, 0x8e);
//     // __set_interrupt_gate(232, (u32)__isr_handler_232, 0x08, 0x8e);
//     // __set_interrupt_gate(233, (u32)__isr_handler_233, 0x08, 0x8e);
//     // __set_interrupt_gate(234, (u32)__isr_handler_234, 0x08, 0x8e);
//     // __set_interrupt_gate(235, (u32)__isr_handler_235, 0x08, 0x8e);
//     // __set_interrupt_gate(236, (u32)__isr_handler_236, 0x08, 0x8e);
//     // __set_interrupt_gate(237, (u32)__isr_handler_237, 0x08, 0x8e);
//     // __set_interrupt_gate(238, (u32)__isr_handler_238, 0x08, 0x8e);
//     // __set_interrupt_gate(239, (u32)__isr_handler_239, 0x08, 0x8e);
//     // __set_interrupt_gate(240, (u32)__isr_handler_240, 0x08, 0x8e);
//     // __set_interrupt_gate(241, (u32)__isr_handler_241, 0x08, 0x8e);
//     // __set_interrupt_gate(242, (u32)__isr_handler_242, 0x08, 0x8e);
//     // __set_interrupt_gate(243, (u32)__isr_handler_243, 0x08, 0x8e);
//     // __set_interrupt_gate(244, (u32)__isr_handler_244, 0x08, 0x8e);
//     // __set_interrupt_gate(245, (u32)__isr_handler_245, 0x08, 0x8e);
//     // __set_interrupt_gate(246, (u32)__isr_handler_246, 0x08, 0x8e);
//     // __set_interrupt_gate(247, (u32)__isr_handler_247, 0x08, 0x8e);
//     // __set_interrupt_gate(248, (u32)__isr_handler_248, 0x08, 0x8e);
//     // __set_interrupt_gate(249, (u32)__isr_handler_249, 0x08, 0x8e);
//     // __set_interrupt_gate(250, (u32)__isr_handler_250, 0x08, 0x8e);
//     // __set_interrupt_gate(251, (u32)__isr_handler_251, 0x08, 0x8e);
//     // __set_interrupt_gate(252, (u32)__isr_handler_252, 0x08, 0x8e);
//     // __set_interrupt_gate(253, (u32)__isr_handler_253, 0x08, 0x8e);
//     // __set_interrupt_gate(254, (u32)__isr_handler_254, 0x08, 0x8e);
//     // __set_interrupt_gate(255, (u32)__isr_handler_255, 0x08, 0x8e);

//     __load_idt();
// }

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

// void __generic_isr(u8 interrupt_number) {
//     if (interrupt_number == 33) {
//         return;
//     }
// }

extern void __initialize_interrupt_table(void);

extern void load_interrupt_table(const interrupt_table_t* interrupt_table) {
	int i;
	interrupt_table_entry_t* entry;

    __initialize_pic();
    __initialize_interrupt_table();

	// for (i = 0; i < interrupt_table->length; i++) {
	// 	entry = &interrupt_table->entries[i];

	// }
}