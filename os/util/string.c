#include <llanos/util/string.h>


size_t string_length(const char* str) {
    size_t count = 0;
    while (*str++ != '\0') {
        count++;
    }
    return count;
}