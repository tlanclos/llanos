#pragma once

#include <llanos/types.h>
#include <llanos/video/vga.h>

/**
 * Abort the kernel operations immediately.
 * 
 * @param errorcode causing the abort.
 * @param vga vga device to print the error message on with the errorcode (NULL if not needed).
 */
extern void abort(u32 errorcode, vga_t* vga);
