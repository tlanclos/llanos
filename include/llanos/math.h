#pragma once

#include <llanos/types.h>

#define MIN(x, y)   ((x) < (y) ? (x) : (y))
#define MAX(x, y)   ((x) > (y) ? (x) : (y))

typedef struct range_s range_t;

struct range_s {
    s64 start;
    s64 end;
};

/**
 * Check if a  value is within a given range.
 *
 * @param value value to check.
 * @param range range to check for value.
 * @return true if value is with the range, 
 *      false if value is not in the range.
 */
extern bool in_range(s64 value, const range_t* range);
