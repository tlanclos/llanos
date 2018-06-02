#pragma once

#include <llanos/types.h>

/**
 * @brief Calculate CRC32 JAMCRC of data.
 *
 * @param data data bytes to calculate CRC32 of.
 * @param length number of bytes in length of data.
 * @return crc32 value.
 */
extern u32 crc32(u8* data, u32 length);

/**
 * @brief Calculate CRC32 JAMCRC of string.
 *
 * @param str string to calculate CRC32 of.
 * @return crc32 value.
 */
extern u32 crc32str(const char* str);