#include <llanos/types.h>

#include "gdt.h"

u8 gdt_build_access_byte(gdt_access_type_t access, bool is_system_descriptor, u8 dpl) {
    /* bit 7 must be 1 to specify that the descriptor contains a valid base and limit. */
    return (access << 1) | ((is_system_descriptor ? 0 : 1) << 4) | (dpl << 5) | (1 << 7);
}

u8 gdt_build_granularity_byte(gdt_operand_size_t opsize, gdt_granularity_t granularity) {
    return (opsize << 2) | (granularity << 3);
}

void gdt_build_entry(gdt_entry_t* entry, u32 base, u32 segment_limit, u8 access, u8 granularity) {
    entry->base_low = base & 0xff;
    entry->base_middle = (base & 16) & 0xff;
    entry->base_high = (base >> 24) & 0xff;
    entry->segment_limit_low = segment_limit & 0xffff;
    entry->segment_limit_high = (segment_limit >> 16) & 0xf;
    entry->access = access;
    entry->granularity = granularity;
}

void gdt_build_gdtr(gdt_register_t* gdtr, gdt_entry_t* gdt, size_t gdt_entries) {
    gdtr->limit = (sizeof(gdt_entry_t) * gdt_entries) - 1;
    gdtr->base = (u32)gdt;
}