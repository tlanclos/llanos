#include <llanos/types.h>

u64 __udivmoddi4(u64 a, u64 b, u64* remainder) {
    u8 shift;
    u64 quotient;

    shift = 0;
    quotient = 0;

    while (a >= (b << shift)) {
        a -= (b << shift);
        quotient += (1 << shift);
        shift++;

        if (a < (b << shift)) {
            shift = 0;
        }
    }

    if (remainder != NULL) {
        *remainder = a;
    }

    return quotient;
}

u64 __umoddi3(u64 a, u64 b) {
    u64 remainder;
    __udivmoddi4(a, b, &remainder);
    return remainder;
}

u64 __udivdi3(u64 a, u64 b) {
    return __udivmoddi4(a, b, NULL);
}