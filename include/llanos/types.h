#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

/**
 * @brief Perform a safe cast from 64-bit signed to 64-bit unsigned value.
 *
 * @param destination place to store the casted value.
 * @param source 64-bit signed value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_s64_to_u64(u64* destination, s64 source);

/**
 * @brief Perform a safe cast from 64-bit unsigned to 64-bit signed value.
 *
 * @param destination place to store the casted value.
 * @param source 64-bit unsigned value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_u64_to_s64(s64* destination, u64 source);

/**
 * @brief Perform a safe cast from 32-bit signed to 32-bit unsigned value.
 *
 * @param destination place to store the casted value.
 * @param source 32-bit signed value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_s32_to_u32(u32* destination, s32 source);

/**
 * @brief Perform a safe cast from 32-bit unsigned to 32-bit signed value.
 *
 * @param destination place to store the casted value.
 * @param source 32-bit unsigned value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_u32_to_s32(s32* destination, u32 source);

/**
 * @brief Perform a safe cast from 16-bit signed to 16-bit unsigned value.
 *
 * @param destination place to store the casted value.
 * @param source 16-bit signed value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_s16_to_u16(u16* destination, s16 source);

/**
 * @brief Perform a safe cast from 16-bit unsigned to 16-bit signed value.
 *
 * @param destination place to store the casted value.
 * @param source 16-bit unsigned value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_u16_to_s16(s16* destination, u16 source);

/**
 * @brief Perform a safe cast from 8-bit signed to 8-bit unsigned value.
 *
 * @param destination place to store the casted value.
 * @param source 8-bit signed value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_s8_to_u8(u8* destination, s8 source);

/**
 * @brief Perform a safe cast from 8-bit unsigned to 8-bit signed value.
 *
 * @param destination place to store the casted value.
 * @param source 8-bit unsigned value to cast.
 * @return true if the value was successfully casted, false if the value was not successfully casted.
 */
extern bool cast_u8_to_s8(s8* destination, u8 source);
