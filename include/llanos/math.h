#pragma once

#include <llanos/types.h>

#define MIN(x, y)   ((x) < (y) ? (x) : (y))
#define MAX(x, y)   ((x) > (y) ? (x) : (y))

typedef struct range_s range_t;

struct range_s {
    s64 start;
    s64 end;
    s64 span;
};

/**
 * @brief Initialize a range with its start and end values.
 *
 * @param start starting value for the range.
 * @param end ending value for the range.
 */
extern void range_init(range_t* range, s64 start, s64 end);

/**
 * @brief Check if a  value is within a given range.
 *
 * @param value value to check.
 * @param range range to check for value.
 * @return true if value is with the range, 
 *      false if value is not in the range.
 */
extern bool in_range(s64 value, range_t* range);

/**
 * @brief Check if the 2 passed in ranges are equal.
 *
 * @param range1 range to check against range2.
 * @param range2 range to check against range1.
 * @return true if the ranges are equal, false if the ranges are not equal.
 */
extern bool range_equal(range_t* range1, range_t* range2);

/**
 * @brief Get the intersection of 2 different ranges.
 *
 * @param intersection storage pointer for the result of the intersection (may be NULL).
 * @param range1 first range.
 * @param range2 second range.
 * @return true if an intersection was found, false if the ranges did not intersect.
 */
extern bool range_intersection(range_t* intersection, range_t* range1, range_t* range2);

/**
 * @brief Get the join 2 ranges.
 *
 * @param join storage pointer for the result of the join (may be NULL).
 * @param range1 first range.
 * @param range2 second range.
 * @return false if the ranges are disjoint and cannot be joined, true if the ranges were joinable.
 */
extern bool range_join(range_t* join, range_t* range1, range_t* range2);
