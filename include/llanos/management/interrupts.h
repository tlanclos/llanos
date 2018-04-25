#pragma once

#include <llanos/types.h>

typedef struct interrupt_table_entry_s interrupt_table_entry_t;
typedef struct interrupt_table_s interrupt_table_t;

struct interrupt_table_entry_s {
    u16 hardware_interrupt_number;
    
};

struct interrupt_table_s {
    interrupt_table_entry_t* entries;
    size_t length;
};

extern void load_interrupt_table(const interrupt_table_t* interrupt_table);