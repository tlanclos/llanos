#pragma once

#include <stdint.h>

/**
 * Clear BSS Memory section.
 * 
 * @param bss_start starting pointer to the memory location where the BSS memory section starts.
 * @param bss_length length of the BSS memory section.
 */
extern void memory_bss_clear(uint8_t* bss_start, uint32_t bss_length);
