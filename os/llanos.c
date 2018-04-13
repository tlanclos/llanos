#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <llanos/management/memory.h>

extern uint8_t* __bss_start;
extern uint8_t* __bss_end;

int kmain(void) {
    memory_bss_clear(__bss_start, __bss_end - __bss_start);
    return 0;
}
