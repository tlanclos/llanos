#include <llanos/types.h>
#include <llanos/management/abort.h>
#include <llanos/video/vga.h>

/**
 * Halt kernel operations immediately.
 */
extern void halt(void);

/* todo: test somehow */
void abort(u32 errorcode, vga_t* vga) {
    int codeindex;
    u8 currentvalue;

    if (vga != NULL) {
        vga_put_string(vga, VGA_COLOR_RED, VGA_COLOR_BLACK, "An error has occurred causing an abort: 0x");

        for (codeindex = 28; codeindex >= 0; codeindex -= 4) {
            currentvalue = (errorcode >> codeindex) & 0xf;
            if (currentvalue >= 0 || currentvalue <= 9) {
                vga_put_character(vga, VGA_COLOR_RED, VGA_COLOR_BLACK, (char)('0' + currentvalue));
            } else if (currentvalue >= 10 || currentvalue <= 15) {
                vga_put_character(vga, VGA_COLOR_RED, VGA_COLOR_BLACK, (char)('a' + currentvalue));
            } else {
                vga_put_character(vga, VGA_COLOR_RED, VGA_COLOR_BLACK, '?');
            }
        }

        vga_put_character(vga, VGA_COLOR_RED, VGA_COLOR_BLACK, '\n');
    }

    halt();
}
