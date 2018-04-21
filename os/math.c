#include <llanos/math.h>

bool in_range(s64 value, const range_t* range) {
    return value >= range->start && value < range->end;
}
