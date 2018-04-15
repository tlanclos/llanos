#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAX_MEMORY_TABLE_ENTRIES    32

typedef struct memory_table_entry_s memory_table_entry_t;
typedef struct memory_table_s memory_table_t;

struct memory_table_entry_s {
    uint64_t* base;
    uint64_t length;
};

struct memory_table_s {
    memory_table_entry_t entries[MAX_MEMORY_TABLE_ENTRIES];
    size_t length;
};

/**
 * Fetch a copy of the memory table mapping for resource allocation.
 *
 * @param memory_table pointer to where the memory table should be stored.
 */
extern void get_memory_table(memory_table_t* memory_table);
