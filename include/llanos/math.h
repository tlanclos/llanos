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
 * @brief Check if a  value is within a given range.
 *
 * @param value value to check.
 * @param range range to check for value.
 * @return true if value is with the range, 
 *      false if value is not in the range.
 */
extern bool in_range(s64 value, const range_t* range);

/**
 * @brief Get the intersection of 2 different ranges.
 *
 * @param intersection storage pointer for the result of the intersection (may be NULL).
 * @param range1 first range.
 * @param range2 second range.
 * @return true if an intersection was found, false if the ranges did not intersect.
 */
extern bool range_intersection(range_t* intersection, const range_t* range1, const range_t* range2);

/**
 * @brief Get the join 2 ranges.
 *
 * @param join storage pointer for the result of the join (may be NULL).
 * @param range1 first range.
 * @param range2 second range.
 * @return false if the ranges are disjoint and cannot be joined, true if the ranges were joinable.
 */
extern bool range_join(range_t* join, const range_t* range1, const range_t* range2);