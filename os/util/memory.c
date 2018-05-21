#include <llanos/types.h>
#include <llanos/util/memory.h>

void memory_set_value(u8* dest, u8 value, u32 length) {
    while (length--) {
        dest[length] = value;
    }
}