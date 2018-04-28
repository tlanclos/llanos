#pragma once

#include <llanos/types.h>

typedef struct interrupt_table_entry_s interrupt_table_entry_t;
typedef struct interrupt_table_s interrupt_table_t;
typedef enum interrupt_return_e interrupt_return_t;

struct interrupt_table_entry_s {
    u16 hardware_interrupt_number;
    void* interrupt_arg;
    interrupt_return_t (*interrupt_function)(u16 hardware_interrupt_number, void* interrupt_arg);
};

struct interrupt_table_s {
    interrupt_table_entry_t* entries;
    size_t length;
};

enum interrupt_return_e {
	INTERRUPT_HANDLED = 0
};

extern void load_interrupt_table(const interrupt_table_t* interrupt_table);