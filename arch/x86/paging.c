#include "paging.h"
#include <llanos/management/abort.h>
#include <llanos/util/crypt.h>
#include <llanos/llanos.h>

void page_directory_set_present(page_directory_entry_t* entry, bool present) {
    if (present) {
        entry->config |= (1 << 0);
    } else {
        entry->config &= ~(1 << 0);
    }
}

void page_directory_set_permissions(page_directory_entry_t* entry, paging_access_t access) {
    switch (access) {
        case PAGING_SUPERVISOR_READ_ONLY:
        case PAGING_SUPERVISOR_READ_WRITE:
        case PAGING_USER_READ_ONLY:
        case PAGING_USER_READ_WRITE:
            entry->config = (entry->config & ~(3 << 1)) | ((u8)access) << 1;
            break;
        default:
            break;
    }
}

void page_directory_set_write_type(page_directory_entry_t* entry, paging_write_type_t write_type) {
    switch (write_type) {
        case PAGING_WRITE_TYPE_WRITE_BACK:
        case PAGING_WRITE_TYPE_WRITE_THROUGH:
            entry->config = (entry->config & ~(1 << 3)) | ((u8)write_type) << 3;
            break;
        default:
            break;
    }
}

void page_directory_enable_caching(page_directory_entry_t* entry, bool enable) {
    if (enable) {
        entry->config &= ~(1 << 4);
    } else {
        entry->config |= (1 << 4);
    }
}

void page_directory_set_accessed(page_directory_entry_t* entry, bool accessed) {
    if (accessed) {
        entry->config |= (1 << 5);
    } else {
        entry->config &= ~(1 << 5);
    }
}

void page_directory_set_size(page_directory_entry_t* entry, paging_page_size_t page_size) {
    switch (page_size) {
        case PAGING_PAGE_SIZE_4K:
        case PAGING_PAGE_SIZE_4M:
            entry->config = (entry->config & ~(1 << 7)) | ((u8)page_size) << 7;
            break;
        default:
            break;
    }
}

void page_directory_set_custom(page_directory_entry_t* entry, u8 custom) {
    entry->custom = custom;
}

void page_directory_set_page_table_base(page_directory_entry_t* entry, u32 base) {
    entry->page_table_base = base >> 12;
}

bool page_directory_get_present(page_directory_entry_t* entry) {
    return (bool)(entry->config & 0x1);
}

paging_access_t page_directory_get_permissions(page_directory_entry_t* entry) {
    return (paging_access_t)((entry->config >> 1) & 0x3);
}

paging_write_type_t page_directory_get_write_type(page_directory_entry_t* entry) {
    return (paging_write_type_t)((entry->config >> 3) & 0x1);
}

bool page_directory_is_caching_enabled(page_directory_entry_t* entry) {
    return !(bool)((entry->config >> 4) & 0x1);
}

bool page_directory_has_been_accessed(page_directory_entry_t* entry) {
    return (bool)((entry->config >> 5) & 0x1);
}

paging_page_size_t page_directory_get_size(page_directory_entry_t* entry) {
    return (paging_page_size_t)((entry->config >> 7) & 0x1);
}

u8 page_directory_get_custom(page_directory_entry_t* entry) {
    return entry->custom;
}

u32 page_directory_get_page_table_base(page_directory_entry_t* entry) {
    return entry->page_table_base << 12;
}

void page_table_set_present(page_table_entry_t* entry, bool present) {
    if (present) {
        entry->config |= (1 << 0);
    } else {
        entry->config &= ~(1 << 0);
    }
}

void page_table_set_permissions(page_table_entry_t* entry, paging_access_t access) {
    switch (access) {
        case PAGING_SUPERVISOR_READ_ONLY:
        case PAGING_SUPERVISOR_READ_WRITE:
        case PAGING_USER_READ_ONLY:
        case PAGING_USER_READ_WRITE:
            entry->config = (entry->config & ~(3 << 1)) | ((u8)access) << 1;
            break;
        default:
            break;
    }
}

void page_table_set_write_type(page_table_entry_t* entry, paging_write_type_t write_type) {
    switch (write_type) {
        case PAGING_WRITE_TYPE_WRITE_BACK:
        case PAGING_WRITE_TYPE_WRITE_THROUGH:
            entry->config = (entry->config & ~(1 << 3)) | ((u8)write_type) << 3;
            break;
        default:
            break;
    }
}

void page_table_enable_caching(page_table_entry_t* entry, bool enable){
    if (enable) {
        entry->config &= ~(1 << 4);
    } else {
        entry->config |= (1 << 4);
    }
}

void page_table_set_accessed(page_table_entry_t* entry, bool accessed) {
    if (accessed) {
        entry->config |= (1 << 5);
    } else {
        entry->config &= ~(1 << 5);
    }
}

void page_table_set_dirty(page_table_entry_t* entry, bool dirty) {
    if (dirty) {
        entry->config |= (1 << 6);
    } else {
        entry->config &= ~(1 << 6);
    }
}

void page_table_set_global(page_table_entry_t* entry, bool global) {
    if (global) {
        entry->config |= (1 << 8);
    } else {
        entry->config &= ~(1 << 8);
    }
}

void page_table_set_custom(page_table_entry_t* entry, u8 custom) {
    entry->custom = custom;
}

void page_table_set_physical_page_address(page_table_entry_t* entry, u32 address) {
    entry->phys_page_addr = address >> 12;
}

bool page_table_get_present(page_table_entry_t* entry){
    return (bool)(entry->config & 0x1);
}

paging_access_t page_table_get_permissions(page_table_entry_t* entry) {
    return (paging_access_t)((entry->config >> 1) & 0x3);
}

paging_write_type_t page_table_get_write_type(page_table_entry_t* entry) {
    return (paging_write_type_t)((entry->config >> 3) & 0x1);
}

bool page_table_is_caching_enabled(page_table_entry_t* entry) {
    return !(bool)((entry->config >> 4) & 0x1);
}

bool page_table_has_been_accessed(page_table_entry_t* entry) {
    return (bool)((entry->config >> 5) & 0x1);
}

bool page_table_get_dirty(page_table_entry_t* entry) {
    return (bool)((entry->config >> 6) & 0x1);
}

bool page_table_get_global(page_table_entry_t* entry) {
    return (bool)((entry->config >> 8) & 0x1);
}

u8 page_table_get_custom(page_table_entry_t* entry) {
    return entry->custom;
}

u32 page_table_get_physical_page_address(page_table_entry_t* entry) {
    return entry->phys_page_addr << 12;
}

void paging_address_location(page_location_t* dest, page_config_t* config, u64 address) {
    u32 page = ((address & ~0xfff) / config->page_size);
    dest->page_num = page % config->page_table_size;
    dest->table_num = page / config->page_table_size;
    dest->directory_num = dest->table_num / config->page_directory_size;
    dest->page_base_addr = address & ~0xfff;
}
