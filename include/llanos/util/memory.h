#pragma once

#include <llanos/types.h>

/**
 * Set memory at destination to value for length bytes.
 *
 * @param dest destination memory pointer to star setting value.
 * @param value value to set at each byte of memory.
 * @param length how may bytes to set to the given value.
 */
extern void memory_set_value(u8* dest, u8 value, u32 length);