#include <stdint.h>
#include <llanos/management/memory.h>

void memory_bss_clear(uint8_t* bss_start, uint32_t bss_length) {
    while (bss_length--) {
        *bss_start = 0;
        bss_start++;
    }
}
