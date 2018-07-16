#include <llanos/math.h>
#include "multiboot.h"
#include "memory.h"

extern u32 __kernel_load_address;
extern u32 __kernel_end_address;

u32 memory_get_kernel_load_address(void) {
    return (u32)&__kernel_load_address;
}

u32 memory_get_kernel_size(void) {
    return (u32)&__kernel_end_address - (u32)&__kernel_load_address;
}

void memory_get_kernel_addresses(range_t* kernel_addresses) {
    range_init(
        kernel_addresses, 
        (s64)memory_get_kernel_load_address(),
        (s64)memory_get_kernel_load_address() + (s64)memory_get_kernel_size()
    );
}

/* TODO: would be nice if this was tested, but testing of this could come later */
void memory_get_table(memory_table_t* memory_table) {
    u64 tempbase;
    u64 templength;
    size_t entry_index;
    multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)multiboot_info->mmap_address;
    memory_table_entry_t* entry;
    range_t kernel_addresses;
    range_t memory_section;

    /* get kernel addresses for later usages */
    memory_get_kernel_addresses(&kernel_addresses);

    memory_table->length = 0;

    for (entry_index = 0; entry_index < MAX_MEMORY_TABLE_ENTRIES; entry_index++) {
        entry = &memory_table->entries[entry_index];
        entry->base = 0;
        entry->length = 0;
    }

    while ((u32)memory_map < multiboot_info->mmap_address + multiboot_info->mmap_length) {
        entry = &memory_table->entries[memory_table->length];

        if (memory_map->type == MULTIBOOT_MEMORY_AVAILABLE) {
            range_init(&memory_section, (s64)memory_map->address, (s64)memory_map->address + (s64)memory_map->length);

            if (!range_intersection(NULL, &memory_section, &kernel_addresses)) {
                if (cast_s64_to_u64(&tempbase, memory_section.start) && cast_s64_to_u64(&templength, memory_section.end - memory_section.start)) {
                    entry->base = tempbase;
                    entry->length = templength;
                    memory_table->length++;
                }
                
                /* make sure we don't overflow the memory table since we have an extra memory entry */
                if (memory_table->length >= MAX_MEMORY_TABLE_ENTRIES) {
                    break;
                }
            } else if (!range_equal(&memory_section, &kernel_addresses)) {
                if (memory_section.start != kernel_addresses.start) {
                    if (cast_s64_to_u64(&tempbase, memory_section.start) && cast_s64_to_u64(&templength, kernel_addresses.start - memory_section.start)) {
                        entry->base = tempbase;
                        entry->length = templength;
                        memory_table->length++;
                    }
                
                    /* make sure we don't overflow the memory table since we have an extra memory entry */
                    if (memory_table->length >= MAX_MEMORY_TABLE_ENTRIES) {
                        break;
                    }
                }

                /* 
                 * there will be an additional entry for this type of intersection.
                 * since the kernel is entirely contained within this memory section
                 * and does not start or end on either edge, the memory section is
                 * actually split into 2 sections. For example:
                 *  ________________________________________
                 * |             memory section             |
                 * |----memory----===kernel===----memory----|
                 * |________________________________________|
                 */
                entry = &memory_table->entries[memory_table->length];

                if (memory_section.end != kernel_addresses.end) {
                    if (cast_s64_to_u64(&tempbase, kernel_addresses.end) && cast_s64_to_u64(&templength, memory_section.end - kernel_addresses.end)) {
                        entry->base = tempbase;
                        entry->length = templength;
                        memory_table->length++;
                    }
                
                    /* make sure we don't overflow the memory table since we have an extra memory entry */
                    if (memory_table->length >= MAX_MEMORY_TABLE_ENTRIES) {
                        break;
                    }
                }
            }
        }

        memory_map = (multiboot_memory_map_t*)((u32)memory_map + memory_map->size + sizeof(memory_map->size));
    }
}