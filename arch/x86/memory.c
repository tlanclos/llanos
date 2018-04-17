#include <llanos/management/memory.h>
#include "multiboot.h"

static memory_table_t __memory_table;

void get_memory_table(memory_table_t* memory_table) {
    int entry_index;
    multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)multiboot_info->mmap_address;
    memory_table_entry_t* entry;

    memory_table->length = 0;

    for (entry_index = 0; entry_index < MAX_MEMORY_TABLE_ENTRIES; entry_index++) {
        entry = &memory_table->entries[entry_index];
        entry->base = 0;
        entry->length = 0;
    }

    while (memory_map < multiboot_info->mmap_address + multiboot_info->mmap_length) {
        entry = &memory_table->entries[memory_table->length];

        if (memory_map->type == MULTIBOOT_MEMORY_AVAILABLE) {
            entry->base = memory_map->address;
            entry->length = memory_map->length;
            memory_table->length++;
        }

        memory_map = (multiboot_memory_map_t*)((uint32_t)memory_map + memory_map->size + sizeof(memory_map->size));
    }
}