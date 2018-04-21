/* multiboot.h - Multiboot header file.
 *
 * Copyright (C) 1999,2003,2007,2008,2009,2010  Free Software Foundation, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
 * DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <llanos/types.h>

/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH                        8192
#define MULTIBOOT_HEADER_ALIGN                  4

/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC                  0x1BADB002

/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC              0x2BADB002

/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN                     0x00001000

/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN                    0x00000004

/* Flags set in the 'flags' member of the multiboot header. */

/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN                    0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO                   0x00000002

/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE                    0x00000004

/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE                   0x00010000

/* Flags to be set in the 'flags' member of the multiboot info structure. */

/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY                   0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV                  0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE                  0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS                     0x00000008

/* These next two are mutually exclusive */

/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS                0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR                 0X00000020

/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP                  0x00000040

/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO               0x00000080

/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE             0x00000100

/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME         0x00000200

/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE                0x00000400

/* Is there video information? */
#define MULTIBOOT_INFO_VBE_INFO                 0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO         0x00001000

/* Multiboot framebuffer type as part of multiboot_info_s */
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED      0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB          1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT     23

/* Multiboot memory type as part of multiboot_memory_map_s */
#define MULTIBOOT_MEMORY_AVAILABLE              1
#define MULTIBOOT_MEMORY_RESERVED               2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
#define MULTIBOOT_MEMORY_NVS                    4
#define MULTIBOOT_MEMORY_BADRAM                 5

typedef struct multiboot_header_s multiboot_header_t;
typedef struct multiboot_aout_symbol_type_s multiboot_aout_symbol_table_t;
typedef struct multiboot_elf_section_header_table_s multiboot_elf_section_header_table_t;
typedef struct multiboot_info_s multiboot_info_t;
typedef struct multiboot_memory_map_s multiboot_memory_map_t;
typedef struct multiboot_module_s multiboot_module_t;
typedef struct multiboot_apm_info_s multiboot_apm_info_t;

struct multiboot_header_s {
    /* Must be MULTIBOOT_MAGIC - see above. */
    u32 magic;

    /* Feature flags. */
    u32 flags;

    /* The above fields plus this one must equal 0 mod 2^32. */
    u32 checksum;

    /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
    u32 header_address;
    u32 load_address;
    u32 load_end_address;
    u32 bss_end_address;
    u32 entry_address;

    /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
    u32 mode_type;
    u32 width;
    u32 height;
    u32 depth;
};

/* The symbol table for a.out. */
struct multiboot_aout_symbol_type_s {
    u32 table_size;
    u32 string_size;
    u32 address;
    u32 reserved;
};

/* The section header table for ELF. */
struct multiboot_elf_section_header_table_s {
    u32 num;
    u32 size;
    u32 address;
    u32 shndx;
};

struct multiboot_info_s {
    /* Multiboot info version number */
    u32 flags;

    /* Available memory from BIOS */
    u32 memory_lower;
    u32 memory_upper;

    /* "root" partition */
    u32 boot_device;

    /* Kernel command line */
    u32 command_line;

    /* Boot-Module list */
    u32 modules_count;
    u32 modules_addr;

    union {
        multiboot_aout_symbol_table_t aout_symbols;
        multiboot_elf_section_header_table_t elf_sections;
    } table;

    /* Memory Mapping buffer */
    u32 mmap_length;
    u32 mmap_address;

    /* Drive Info buffer */
    u32 drives_length;
    u32 drives_address;

    /* ROM configuration table */
    u32 config_table;

    /* Boot Loader Name */
    u32 boot_loader_name;

    /* APM table */
    u32 apm_table;

    /* Video */
    u32 vbe_control_info;
    u32 vbe_mode_info;
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_offset;
    u16 vbe_interface_length;

    u64 framebuffer_address;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    union {
        struct {
            u32 framebuffer_palette_addr;
            u16 framebuffer_palette_num_colors;
        };  
        struct {
            u8 framebuffer_red_field_position;
            u8 framebuffer_red_mask_size;
            u8 framebuffer_green_field_position;
            u8 framebuffer_green_mask_size;
            u8 framebuffer_blue_field_position;
            u8 framebuffer_blue_mask_size;
        };
    };
};

struct multiboot_color_s {
    u8 red;
    u8 green;
    u8 blue;
};

struct multiboot_memory_map_s {
    u32 size;
    u64 address;
    u64 length;
    u32 type;
} __attribute__((packed));

struct multiboot_module_s {
    /* the memory used goes from bytes 'module_start' to 'module_end-1' inclusive */
    u32 module_start;
    u32 module_end;

    /* Module command line */
    u32 command_line;

    /* padding to take it to 16 bytes (must be zero) */
    u32 padding;
};

/* APM BIOS info. */
struct multiboot_apm_info_s {
    u16 version;
    u16 cseg;
    u32 offset;
    u16 cseg_16;
    u16 dseg;
    u16 flags;
    u16 cseg_length;
    u16 cseg_16_length;
    u16 dseg_length;
};

extern multiboot_info_t* multiboot_info;
