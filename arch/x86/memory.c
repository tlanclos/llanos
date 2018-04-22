#include <llanos/management/memory.h>
#include "multiboot.h"

extern u32 __kernel_load_address;
extern u32 __kernel_end_address;

static memory_table_t __memory_table;

/**
 * @brief Get the address in which the kernel was loaded.
 *
 * @return pointer to the address in which the kernel was loaded.
 */
static u32* get_kernel_load_address(void) {
    return (u32*)&__kernel_load_address;
}

/**
 * @brief Get size of the kernel.
 *
 * @return the size of the kernel in bytes.
 */
static u32 get_kernel_size(void) {
    return (u32)&__kernel_end_address - (u32)&__kernel_load_address;
}


void get_memory_table(memory_table_t* memory_table) {
    size_t entry_index;
    multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)multiboot_info->mmap_address;
    memory_table_entry_t* entry;

    memory_table->length = 0;

    for (entry_index = 0; entry_index < MAX_MEMORY_TABLE_ENTRIES; entry_index++) {
        entry = &memory_table->entries[entry_index];
        entry->base = 0;
        entry->length = 0;
    }

    while ((u32)memory_map < multiboot_info->mmap_address + multiboot_info->mmap_length) {
        entry = &memory_table->entries[memory_table->length];

        if (memory_map->type == MULTIBOOT_MEMORY_AVAILABLE) {
            entry->base = (u64*)memory_map->address;
            entry->length = memory_map->length;
            memory_table->length++;
        }

        memory_map = (multiboot_memory_map_t*)((u32)memory_map + memory_map->size + sizeof(memory_map->size));
    }
}