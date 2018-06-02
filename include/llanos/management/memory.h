#pragma once

#include <llanos/types.h>

#define MAX_MEMORY_TABLE_ENTRIES    32

typedef struct memory_table_entry_s memory_table_entry_t;
typedef struct memory_table_s memory_table_t;

struct memory_table_entry_s {
    u64 base;
    u64 length;
};

struct memory_table_s {
    memory_table_entry_t entries[MAX_MEMORY_TABLE_ENTRIES];
    size_t length;
};

/**
 * @brief Fetch a copy of the memory table mapping for resource allocation.
 *
 * A copy of the available-memory map on the system will be fetched and if
 * not all of the regions fit into the memory_table, they will be ignored.
 *
 * @param memory_table pointer to where the memory table should be stored.
 */
extern void get_memory_table(memory_table_t* memory_table);

