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
static bool __range_intersection__sorted(range_t* intersection, range_t* lower, range_t* higher) {
    if (lower->end <= higher->start) {
        /*
         * the ranges definitely do not intersect. for example:
         *   range(10, 50) intersects range(50, 60)
         *   will give us no valid range.
         */
        return false;
    } else {
        if (intersection != NULL) {
            range_init(intersection, higher->start, MIN(lower->end, higher->end));
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
static bool __range_join__sorted(range_t* join, range_t* lower, range_t* higher) {
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
            range_init(join, lower->start, MAX(lower->end, higher->end));
        }
        return true;
    }
}

void range_init(range_t* range, s64 start, s64 end) {
    range->start = start;
    range->end = end;
    range->span = end - start;
}

bool in_range(s64 value, range_t* range) {
    return value >= range->start && value < range->end;
}

bool range_equal(range_t* range1, range_t* range2) {
    return range1->start == range2->start && range1->end == range2->end;
}

bool range_intersection(range_t* intersection, range_t* range1, range_t* range2) {
    if (range1->start < range2->start) {
        return __range_intersection__sorted(intersection, range1, range2);
    } else {
        return __range_intersection__sorted(intersection, range2, range1);
    }
}

bool range_join(range_t* join, range_t* range1, range_t* range2) {
    if (range1->start < range2->start) {
        return __range_join__sorted(join, range1, range2);
    } else {
        return __range_join__sorted(join, range2, range1);
    }
}

u64 divmod_u64(u64 numerator, u64 denominator, u64* remainder) {
    /*
     * This algorithm is a simple implementation of the
     * align divisor shift dividend method.
     */
    u64 remainder_quotient_chain;
    u64 rem;
    u64 aligned_denominator;
    u8 shift;

    if (denominator == 0) {
        /*
         * if the denominator is 0, then this
         * would cause a divide by zero error.
         */
        if (remainder != NULL) {
            *remainder = 0;
        }
        return 0;
    } else if (numerator == 0 || numerator < denominator) {
        /*
         * if the numerator is 0 or numerator < denominator, then
         * the result is the numerator and the
         * remainder is the denominator
         */
        if (remainder != NULL) {
            *remainder = numerator;
        }
        return 0;
    } else {
        /*
         * Calculate how much we need to shift the denominitor
         * by to align it with the numerator.
         */
        shift = __builtin_clzll(denominator) - __builtin_clzll(numerator);
        aligned_denominator = denominator << shift;
        remainder_quotient_chain = numerator;

        for (u8 i = 0; i <= shift; i++) {
            /*
             * For each iteration, if the quotient is larger than the
             * aligned denominator, then we can subtract. For example,
             * if we have 1000 / 62, we would have the following situation
             * on the first iteration:
             *
             *   1000     | 1111101000
             *   62 << 4  | 1111100000
             *
             *   In this example, we can subtract because 1000 < 992.
             *
             * If the quotient is too
             * remainder_quotient_chain < aligned_denominator
             * then we need to shift it over until it is >= than
             * aligned_denominator, but since we only have "shift" + 1
             * operations to do this, it could potentially never
             * be >= aligned_denominator.
             *
             * Shifting in this form is essentially like appending a 0 or 1
             * to the quotient because we are using remainder_quotient_chain
             * as a remainder and a quotient.
             *
             * The optimization of this algorithm is the variable
             * remainder_quotient_chain. It is used to store both the
             * remainder and quotient in a way that can be accessed
             * after loop for the final answer.
             *
             * remainder_quotient_chain is stored in the following form:
             *   MSB                    LSB
             *    | remainder | quotient |
             */
            if (remainder_quotient_chain >= aligned_denominator) {
                remainder_quotient_chain = \
                    ((remainder_quotient_chain - aligned_denominator) << 1) | 1;
            } else {
                remainder_quotient_chain <<= 1;
            }
        }

        if (remainder != NULL) {
            *remainder = remainder_quotient_chain >> (shift + 1);
        }
        /*
         * in order to extract the quotient we need to mask
         * the bits that the quotient will be in
         */
        return remainder_quotient_chain & (((1 << shift) - 1) | 1 << shift);
    }
}

u64 modulus_u64(u64 numerator, u64 denominator) {
    u64 remainder;
    divmod_u64(numerator, denominator, &remainder);
    return remainder;
}

u64 divide_u64(u64 numerator, u64 denominator) {
    return divmod_u64(numerator, denominator, NULL);
}
