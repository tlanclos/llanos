#include <llanos/math.h>

/**
 * @brief Get the intersection 2 ranges.
 *
 * Get the intersection of 2 ranges, but assume the parameters
 * lower and higher represent lower and higher starting values 
 * in their respective values.
 *
 * @param intersection storage for the resulting intersection (may be NULL).
 * @param lower represents the range with the lower starting value.
 * @param higher represents the range with the higher starting value.
 * @return false if the ranges did not have an intersection, true if they did.
 */
static bool __range_intersection__sorted(range_t* intersection, const range_t* lower, const range_t* higher) {
    if (lower->end <= higher->start) {
        /*
         * the ranges definitely do not intersect. for example:
         *   range(10, 50) intersects range(50, 60)
         *   will give us no valid range.
         */
        return false;
    } else {
        if (intersection != NULL) {
            intersection->start = higher->start;
            intersection->end = MIN(lower->end, higher->end);
        }
        return true;
    }
}

/**
 * @brief Get the join of 2 ranges.
 *
 * Get the join of 2 ranges, but assume the parameters
 * lower and higher represent lower and higher starting
 * values in their respective values.
 *
 * @param join storage for the resulting join (may be NULL).
 * @param lower represents the range with the lower starting value.
 * @param higher represents the range with the higher starting value.
 * @return false if ther ranges were not joinable, true if they were.
 */
static bool __range_join__sorted(range_t* join, const range_t* lower, const range_t* higher) {
    if (lower->end < higher->start) {
        /*
         * If the last value of lower's end is lower than higher's
         * start, then there is no join available. For example:
         *   range(30, 100) and range(101, 111) could not be joined,
         *   because there ranges do not overlap and are not adjacent,
         *   but range(30, 100) and range(100, 111) are adjacent so
         *   joining is allowed. If the ranges overlap, then joining is
         *   allowed as well.
         */
        return false;
    } else {
        if (join != NULL) {
            join->start = lower->start;
            join->end = MAX(lower->end, higher->end);
        }
        return true;
    }
}

bool in_range(s64 value, const range_t* range) {
    return value >= range->start && value < range->end;
}

bool range_intersection(range_t* intersection, const range_t* range1, const range_t* range2) {
    if (range1->start < range2->start) {
        return __range_intersection__sorted(intersection, range1, range2);
    } else {
        return __range_intersection__sorted(intersection, range2, range1);
    }
}

bool range_join(range_t* join, const range_t* range1, const range_t* range2) {
    if (range1->start < range2->start) {
        return __range_join__sorted(join, range1, range2);
    } else {
        return __range_join__sorted(join, range2, range1);
    }
}