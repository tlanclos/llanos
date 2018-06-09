#pragma once

#include <llanos/video/vga.h>

/**
 * @brief Get the current llanos global VGA.
 *
 * @return the current llanos global VGA.
 */
extern vga_t* get_llanos_vga(void);

/**
 * @brief Set the current llanos global VGA.
 * 
 * @param vga vga to configure as the current llanos global VGA.
 */
extern void set_llanos_vga(vga_t* vga);


/**
 * @brief Reset the current llanos global VGA the default configuration.
 */
extern void reset_llanos_vga(void);
