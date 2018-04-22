#include <llanos/types.h>
#include <llanos/limits.h>

bool cast_s64_to_u64(u64* destination, s64 source) {
    if (source < 0) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (u64)source;
        }
        return true;
    }
}

bool cast_u64_to_s64(s64* destination, u64 source) {
    if (source > S64MAX) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (s64)source;
        }
        return true;
    }
}

bool cast_s32_to_u32(u32* destination, s32 source) {
    if (source < 0) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (u32)source;
        }
        return true;
    }
}

bool cast_u32_to_s32(s32* destination, u32 source) {
    if (source > S32MAX) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (s32)source;
        }
        return true;
    }
}

bool cast_s16_to_u16(u16* destination, s16 source) {
    if (source < 0) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (u16)source;
        }
        return true;
    }
}

bool cast_u16_to_s16(s16* destination, u16 source) {
    if (source > S16MAX) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (s16)source;
        }
        return true;
    }
}

bool cast_s8_to_u8(u8* destination, s8 source) {
    if (source < 0) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (u8)source;
        }
        return true;
    }
}

bool cast_u8_to_s8(s8* destination, u8 source) {
    if (source > S8MAX) {
        return false;
    } else {
        if (destination != NULL) {
            *destination = (s8)source;
        }
        return true;
    }
}
