#include <llanos/video/vga.h>
#include <llanos/management/abort.h>
#include <llanos/types.h>
#include <llanos/limits.h>
#include <llanos/util/memory.h>
#include <llanos/util/crypt.h>
#include <stdarg.h>

/**
 * @brief Get a VGA entry from a forground color, background color, and character.
 *
 * @param color_fg foreground color (chosen from a color in vga_color_t).
 * @param color_bg background color (chosen from a color in vga_color_t).
 * @param c character for the VGA entry.
 * @return a VGA entry representing the character with foreground and background color.
 */
static inline u16 __vga_get_entry(vga_color_t color_fg, vga_color_t color_bg, char c) {
    return (((u16)(color_fg) | ((u16)(color_bg) << 4)) << 8) | (u16)(c);
}

/**
 * @brief Advance the cursor on a VGA type by 1 character.
 *
 * This will advance the VGA cursor by 1. If the character reaches the width
 * of the buffer, then it will move to the farthest left character of the next line.
 * If the cursor reaches the last character on the last line, then it will advance to the 
 * very first position (0, 0).
 *
 * @param vga the vga struct to advance by 1.
 */
static void __vga_advance_cursor_by_1(vga_t* vga) {
    /* move to the next column */
    vga->cursor_col++;

    /* if we have reached the terminal width, then we need to advance to the next row */
    if (vga->cursor_col >= vga->terminal_width) {
        vga->cursor_col = 0;
        vga->cursor_row++;

        /* if we have reached the terminal height, then we need to go back to the top */
        if (vga->cursor_row >= vga->terminal_height) {
            vga->cursor_row = 0;
        }
    }
}

static void __vga_print_unsigned_integer_base(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, u64 value, u8 base) {
    /* 64 is the longest string of characters that can be used specifically for base 2 */
    char strvalue[64];
    unsigned tmpvalue;
    int indexer;

    /* clear string value */
    memory_set_value((u8*)strvalue, 0, sizeof(strvalue));
    indexer = 0;

    if (value != 0) {
        /* build the string backwards in strvalue */
        while (value > 0) {
            tmpvalue = value % base;
            if (tmpvalue < 10) {
                strvalue[indexer] = '0' + tmpvalue;
            } else {
                strvalue[indexer] = 'a' + (tmpvalue - 10);
            }
            indexer++;
            value /= base;
        }
    } else {
        strvalue[indexer] = '0';
        indexer++;
    }

    while (indexer != 0) {
        indexer--;
        vga_put_character(vga, color_fg, color_bg, strvalue[indexer]);
    }
}

/**
 * @brief Print a unsigned integer to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_unsigned_integer(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, u64 value) {
    __vga_print_unsigned_integer_base(vga, color_fg, color_bg, value, 10);
}

/**
 * @brief Print a signed integer to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_signed_integer(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, s64 value) {
    if (value < 0) {
        vga_put_character(vga, color_fg, color_bg, '-');
        __vga_print_unsigned_integer(vga, color_fg, color_bg, (u64)(-value));
    } else {
        __vga_print_unsigned_integer(vga, color_fg, color_bg, (u64)value);
    }
}

/**
 * @brief Print a unsigned integer as an octal number to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_unsigned_octal(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, u64 value) {
    __vga_print_unsigned_integer_base(vga, color_fg, color_bg, value, 8);
}

/**
 * @brief Print a unsigned integer as a hex number to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_unsigned_hex(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, u64 value) {
    __vga_print_unsigned_integer_base(vga, color_fg, color_bg, value, 16);
}

/**
 * @brief Print a unsigned integer as a binary number to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_unsigned_binary(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, u64 value) {
    __vga_print_unsigned_integer_base(vga, color_fg, color_bg, value, 2);
}

/**
 * @brief Print a floating point number number as scientific notation to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_scientific_notation(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, double value) {
    u32 exp = 0;

    if (value == 0.0) {
        vga_put_string(vga, color_fg, color_bg, "0.0e+0");
    } else if (value < 0.0) {
        vga_put_character(vga, color_fg, color_bg, '-');
        __vga_print_scientific_notation(vga, color_fg, color_bg, -value);
    } else if (value < 1.0) {
        while (value < 1.0) {
            value *= 10;
            exp++;
        }

        vga_put_character(vga, color_fg, color_bg, '0' + (u64)value);
        vga_put_character(vga, color_fg, color_bg, '.');
            
        value -= (u64)value;
        do {
            value *= 10.0;
            vga_put_character(vga, color_fg, color_bg, (char)('0' + (u8)value));
            value -= (u64)value;
        } while (value > 0.0);

        vga_put_character(vga, color_fg, color_bg, 'e');
        vga_put_character(vga, color_fg, color_bg, '-');
        __vga_print_unsigned_integer(vga, color_fg, color_bg, exp);
    } else {
        while (value >= 10.0) {
            value /= 10;
            exp++;
        }

        vga_put_character(vga, color_fg, color_bg, '0' + (u64)value);
        vga_put_character(vga, color_fg, color_bg, '.');
            
        value -= (u64)value;
        do {
            value *= 10.0;
            vga_put_character(vga, color_fg, color_bg, (char)('0' + (u8)value));
            value -= (u64)value;
        } while (value > 0.0);

        vga_put_character(vga, color_fg, color_bg, 'e');
        vga_put_character(vga, color_fg, color_bg, '+');
        __vga_print_unsigned_integer(vga, color_fg, color_bg, exp);
    }
}

/**
 * @brief Print a floating point number number to a vga without padding.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param value value to print.
 */
static void __vga_print_floating_point(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, double value) {
    if (value < 0) {
        vga_put_character(vga, color_fg, color_bg, '-');
        __vga_print_floating_point(vga, color_fg, color_bg, -value);
    } else {
        if (value >= S64MAX) {
            __vga_print_scientific_notation(vga, color_fg, color_bg, value);
        } else {
            __vga_print_unsigned_integer(vga, color_fg, color_bg, (u64)value);
            vga_put_character(vga, color_fg, color_bg, '.');
            
            value -= (u64)value;
            do {
                value *= 10.0;
                vga_put_character(vga, color_fg, color_bg, (char)('0' + (u8)value));
                value -= (u64)value;
            } while (value > 0.0);
        }
    }
}

/**
 * @brief Print a string of characters to the terminal, but only printer `length` characters.
 *
 * @param vga vga to print to.
 * @param color_fg color to set as character's forground.
 * @param color_bg color to set as character's background.
 * @param length number of characters to print.
 * @param value value to print.
 */
static void __vga_print_string_with_length(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, size_t length, const char* value) {
    size_t index;

    for (index = 0; index < length; index++) {
        vga_put_character(vga, color_fg, color_bg, value[index]);
    }
}

size_t vga_get_default_terminal_width(void) {
    return 80;
}

size_t vga_get_default_terminal_height(void) {
    return 25;
}

u16* vga_get_default_buffer_address(void) {
    return (u16*)0xb8000;
}

void vga_initialize(vga_t* vga, u16* buffer_address, size_t width, size_t height) {
    size_t row;
    size_t col;

    vga->buffer_address = buffer_address;
    vga->terminal_width = width;
    vga->terminal_height = height;
    vga->cursor_row = 0;
    vga->cursor_col = 0;

    for (col = 0; col < width; col++) {
        for (row = 0; row < height; row++) {
            *buffer_address++ = 0;
        }
    }
}

void vga_put_character(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, char c) {
    if (c == '\n') {
        vga->cursor_col = vga->terminal_width;
        __vga_advance_cursor_by_1(vga);
    } else {
        /* insert the wanted vga entry at the current cursor address */
        vga->buffer_address[vga->cursor_col + (vga->cursor_row * vga->terminal_width)] = \
            __vga_get_entry(color_fg, color_bg, c);

        /* advance the cursor on this vga by 1 */
        __vga_advance_cursor_by_1(vga);
    }
}

void vga_put_string(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, const char* str) {
    while (*str != '\0') {
        vga_put_character(vga, color_fg, color_bg, *str);
        str++;
    }
}

void vga_printf(vga_t* vga, vga_color_t color_fg, vga_color_t color_bg, const char* format, ...) {
    char* format_ptr;
    va_list vl;
    size_t length;

    va_start(vl, format);
    format_ptr = (char*)format;
    while (*format_ptr != '\0') {
        if (*format_ptr == '%') {
            format_ptr++;

            switch (*format_ptr) {
                case 'd':
                    __vga_print_signed_integer(vga, color_fg, color_bg, (s64)va_arg(vl, s32));
                    break;
                case 'u':
                    __vga_print_unsigned_integer(vga, color_fg, color_bg, (u64)va_arg(vl, u32));
                    break;
                case 'o':
                    __vga_print_unsigned_octal(vga, color_fg, color_bg, (u64)va_arg(vl, u32));
                    break;
                case 'x':
                    __vga_print_unsigned_hex(vga, color_fg, color_bg, (u64)va_arg(vl, u32));
                    break;
                case 'b':
                    __vga_print_unsigned_binary(vga, color_fg, color_bg, (u64)va_arg(vl, u32));
                    break;
                case 'f':
                    __vga_print_floating_point(vga, color_fg, color_bg, va_arg(vl, double));
                    break;
                case 'e':
                    __vga_print_scientific_notation(vga, color_fg, color_bg, va_arg(vl, double));
                    break;
                case 'c':
                    vga_put_character(vga, color_fg, color_bg, (char)va_arg(vl, int));
                    break;
                case 's':
                    vga_put_string(vga, color_fg, color_bg, va_arg(vl, char*));
                    break;
                case 'S':
                    length = va_arg(vl, size_t);
                    __vga_print_string_with_length(vga, color_fg, color_bg, length, va_arg(vl, char*));
                    break;
                case '%':
                    vga_put_character(vga, color_fg, color_bg, (char)*format_ptr);
                    break;
                default:
                    abort(crc32str("vga_printf"), vga);
                    break;
            }
        } else {
            vga_put_character(vga, color_fg, color_bg, (char)*format_ptr);
        }
        
        format_ptr++;
    }
    va_end(vl);
}


bool vga_equal(vga_t* first, vga_t* second) {
    return first->buffer_address == second->buffer_address && \
        first->cursor_row == second->cursor_row && \
        first->cursor_col == second->cursor_col && \
        first->terminal_width == second->terminal_width && \
        first->terminal_height == second->terminal_height;
}

void vga_copy(vga_t* dest, vga_t* source) {
    dest->buffer_address = source->buffer_address;
    dest->cursor_row = source->cursor_row;
    dest->cursor_col = source->cursor_col;
    dest->terminal_width = source->terminal_width;
    dest->terminal_height = source->terminal_height;
}