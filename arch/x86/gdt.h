#pragma once

typedef struct gdt_register_s gdt_register_t;
typedef struct gdt_entry_s gdt_entry_t;
typedef enum gdt_operand_size_e gdt_operand_size_t;
typedef enum gdt_granularity_e gdt_granularity_t;
typedef enum gdt_access_type_e gdt_access_type_t;

enum gdt_operand_size_e {
    GDT_OPERAND_SIZE_16BIT = 0,
    GDT_OPERAND_SIZE_32BIT = 1
};

enum gdt_granularity_e {
    GDT_GRANULARITY_1BYTE = 0,
    GDT_GRANULARITY_1KILO = 1
};

enum gdt_access_type_e {
    GDT_ACCESS_TYPE_DATA_READ_ONLY = 0,
    GDT_ACCESS_TYPE_DATA_READ_WRITE = 1,
    GDT_ACCESS_TYPE_STACK_READ_ONLY = 2,
    GDT_ACCESS_TYPE_STACK_READ_WRITE = 3,
    GDT_ACCESS_TYPE_CODE_EXECUTE_ONLY = 4,
    GDT_ACCESS_TYPE_CODE_EXECUTE_READ = 5,
    GDT_ACCESS_TYPE_CODE_EXECUTE_ONLY_CONFORMING = 6,
    GDT_ACCESS_TYPE_CODE_EXECUTE_READ_CONFORMING = 7
};

/**
 * @brief Global Descriptor Table Register
 *
 *                        | gdt entry n |
 *                              ...
 *                        | gdt entry 1 |
 * gdtr.base (4 bytes) -> | gdt entry 0 |
 *
 * If the GDT has 3 entries, then the limit would be (3*8)-1=23 or 0x17.
 *
 * @member limit limit represents the upper limit of gdt.
 * @member base base address of gdt.
 */
struct gdt_register_s {
    u16 limit;
    u32 base;
} __attribute__((packed));

/**
 * @brief Global Descriptor Table Entry.
 *
 * @member segment_limit_low lower 16-bits of segment_limit value.
 * @member base_low lower 16-bits of base address value.
 * @member base_middle middle 8-bits of the base address value.
 * @member access access control bits.
 * @member segment_limit_high upper 4-bits of segment_limit value.
 * @member granularity granularity control bits.
 * @member base_high upper 8-bits of the base address value.
 */
struct gdt_entry_s {
    u16 segment_limit_low;
    u16 base_low;
    u8 base_middle;
    u8 access;
    u8 segment_limit_high : 4;
    u8 granularity : 4;
    u8 base_high;
} __attribute__((packed));

/**
 * @brief Build an access entry for the GDT entry.
 *
 * @param access access type, choose from gdt_access_type_t.
 * @param is_system_descriptor if true, access will be set as a system descriptor; 
 *      if false, access will be set as code,data,stack descriptor.
 * @param dpl data privilege level (0-3 inclusive).
 * @return access entry for GDT entry.
 */
extern u8 gdt_build_access_byte(gdt_access_type_t access, bool is_system_descriptor, u8 dpl);

/**
 * @brief Build a granularity entry for a GDT entry.
 *
 * @param segment_limit_high upper 4 bits of the segment length.
 * @param opsize operand size, choose from one of gdt_operand_size_t.
 * @param granularity granularity, choose from one of gdt_granularity_t.
 * @return granularity entry for GDT entry.
 */
extern u8 gdt_build_granularity_byte(gdt_operand_size_t opsize, gdt_granularity_t granularity);

/**
 * @brief Build GDT entry and store its value in `entry`.
 *
 * @param entry destination entry register to set.
 * @param base base address of segment.
 * @param segment_limit segment limit for the entry.
 * @param access access byte, built with gdt_build_access_byte.
 * @param granularity granularity byte, built with gdt_build_granularity_byte.
 */
extern void gdt_build_entry(gdt_entry_t* entry, u32 base, u32 segment_limit, u8 access, u8 granularity);

/**
 * @brief Build the GDT register and store its value in `gdtr`.
 *
 * @param gdtr destination to store the GDT register.
 * @param gdt pointer to the GDT entry.
 * @param gdt_entries number of entries that are in the gdt entry list.
 */
extern void gdt_build_gdtr(gdt_register_t* gdtr, gdt_entry_t* gdt, size_t gdt_entries);

/**
 * @brief Load GDT register into memory.
 * 
 * @param gdtr GDT register to load into memory.
 */
extern void gdt_load_gdtr(gdt_register_t* gdtr);
