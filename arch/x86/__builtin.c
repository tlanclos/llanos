#include <llanos/math.h>

u64 __udivmoddi4(u64 numerator, u64 denominator, u64* remainder) {
    return divmod_u64(numerator, denominator, remainder);
}

u64 __umoddi3(u64 numerator, u64 denominator) {
    return modulus_u64(numerator, denominator);
}

u64 __udivdi3(u64 numerator, u64 denominator) {
    return divide_u64(numerator, denominator);
}
