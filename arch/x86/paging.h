#pragma once

#include <llanos/types.h>

typedef struct page_directory_entry_s page_directory_entry_t;
typedef struct page_table_entry_s page_table_entry_t;
typedef struct page_location_s page_location_t;
typedef struct page_config_s page_config_t;
typedef enum paging_access_e paging_access_t;
typedef enum paging_write_type_e paging_write_type_t;
typedef enum paging_page_size_e paging_page_size_t;

struct page_directory_entry_s {
    u16 config : 9;
    u8 custom : 3;
    u32 page_table_base : 20;
};

struct page_table_entry_s {
    u16 config : 9;
    u8 custom : 3;
    u32 phys_page_addr : 20;
};

struct page_location_s {
    u32 directory_num;
    u32 table_num;
    u32 page_num;
    u32 page_base_addr;
};

struct page_config_s {
    u32 page_size;
    u32 page_table_size;
    u32 page_directory_size;
};

enum paging_access_e {
    PAGING_SUPERVISOR_READ_ONLY = 0,
    PAGING_SUPERVISOR_READ_WRITE = 1,
    PAGING_USER_READ_ONLY = 2,
    PAGING_USER_READ_WRITE = 3
};

enum paging_write_type_e {
    PAGING_WRITE_TYPE_WRITE_BACK = 0,
    PAGING_WRITE_TYPE_WRITE_THROUGH = 1
};

enum paging_page_size_e {
    PAGING_PAGE_SIZE_4K = 0,
    PAGING_PAGE_SIZE_4M = 1
};


/**
 * @brief Setup paging directory base address.
 *
 * @param directory page directory to use.
 */
extern void paging_set_page_directory(page_directory_entry_t* directory);


/**
 * @brief Set the present bit in a page table.
 *
 * The present bit specifies whether or not the page table is available in memory.
 *
 * @param entry entry to configure.
 * @param present true if the page table associated with this entry is in memory, false if it is not.
 */
extern void page_directory_set_present(page_directory_entry_t* entry, bool present);


/**
 * @brief Set the permissions/access for a given page table.
 *
 * The permissions will set whether or not a the page table is read/write or read-only and
 * also whether the page table has supervisor permissions or just user permissions.
 *
 * @param entry entry to configure.
 * @param access paging access type.
 */
extern void page_directory_set_permissions(page_directory_entry_t* entry, paging_access_t access);


/**
 * @brief Set the paging write type for a given page table.
 *
 * @param entry entry to configure.
 * @param write_type paging write type to configure.
 */
extern void page_directory_set_write_type(page_directory_entry_t* entry, paging_write_type_t write_type);


/**
 * @brief Enable/Disable cachine of this page table.
 *
 * @param entry entry to configure.
 * @param enable set true to enable caching, false will disable caching.
 */
extern void page_directory_enable_caching(page_directory_entry_t* entry, bool enable);


/**
 * @brief Set whether or not this page has been accessed.
 *
 * @param entry entry to configure.
 * @param accessed set true if the page has been accessed, false if the page has not been accessed.
 */
extern void page_directory_set_accessed(page_directory_entry_t* entry, bool accessed);


/**
 * @brief Set page size for this page table.
 *
 * Page size can be configured to 4MB or 4KB
 *
 * @param entry entry to configure.
 * @param page_size page size configuration for this page table.
 */
extern void page_directory_set_size(page_directory_entry_t* entry, paging_page_size_t page_size);


/**
 * @brief Set custom bits for OS specific use.
 *
 * There are 3 bits that can be used for custom use. The first 3 bits of the parameter custom will be used.
 *
 * @param entry entry to configure.
 * @param custom custom bits to set (only first 3 bits are used).
 */
extern void page_directory_set_custom(page_directory_entry_t* entry, u8 custom);


/**
 * @brief Set page table base address.
 *
 * The first 20 bits are used, all other bits are ignored.
 *
 * @param entry entry to configure.
 * @param base base address of page table.
 */
extern void page_directory_set_page_table_base(page_directory_entry_t* entry, u32 base);


/**
 * @brief Get whether or not the page table is present.
 *
 * @param entry page directory entry.
 * @return true if page table is present in memory, false otherwise. 
 */
extern bool page_directory_get_present(page_directory_entry_t* entry);


/**
 * @brief Get the page table permissions.
 *
 * @param entry page directory entry.
 * @return page table permissions.
 */
extern paging_access_t page_directory_get_permissions(page_directory_entry_t* entry);


/**
 * @brief Get the page table's write type.
 *
 * @param entry page directory entry.
 * @return page table's write type.
 */
extern paging_write_type_t page_directory_get_write_type(page_directory_entry_t* entry);


/**
 * @brief Get whether or not this page table has caching enabled.
 *
 * @param entry page directory entry.
 * @return true if page table has caching enabled, false otherwise. 
 */
extern bool page_directory_is_caching_enabled(page_directory_entry_t* entry);


/**
 * @brief Get whether or not the page table has been accessed.
 *
 * @param entry page directory entry.
 * @return true if page table has been accessed, false otherwise. 
 */
extern bool page_directory_has_been_accessed(page_directory_entry_t* entry);


/**
 * @brief Get the page table's page size.
 *
 * @param entry page directory entry.
 * @return page table's page size.
 */
extern paging_page_size_t page_directory_get_size(page_directory_entry_t* entry);


/**
 * @brief Get custom bits set by user.
 *
 * @param entry page directory entry.
 * @return custom bits set by user (lower 4 bits only).
 */
extern u8 page_directory_get_custom(page_directory_entry_t* entry);


/**
 * @brief Get page table base address.
 *
 * @param entry page directory entry.
 * @return page table base address (lower 20 bits only).
 */
extern u32 page_directory_get_page_table_base(page_directory_entry_t* entry);


/**
 * @brief Set the present bit in a page table.
 *
 * The present bit specifies whether or not the page table is available in memory.
 *
 * @param entry entry to configure.
 * @param present true if the page table associated with this entry is in memory, false if it is not.
 */
extern void page_table_set_present(page_table_entry_t* entry, bool present);


/**
 * @brief Set the permissions/access for a given page table.
 *
 * The permissions will set whether or not a the page table is read/write or read-only and
 * also whether the page table has supervisor permissions or just user permissions.
 *
 * @param entry entry to configure.
 * @param access paging access type.
 */
extern void page_table_set_permissions(page_table_entry_t* entry, paging_access_t access);


/**
 * @brief Set the paging write type for a given page table.
 *
 * @param entry entry to configure.
 * @param write_type paging write type to configure.
 */
extern void page_table_set_write_type(page_table_entry_t* entry, paging_write_type_t write_type);


/**
 * @brief Enable/Disable cachine of this page table.
 *
 * @param entry entry to configure.
 * @param enable set true to enable caching, false will disable caching.
 */
extern void page_table_enable_caching(page_table_entry_t* entry, bool enable);


/**
 * @brief Set whether or not this page has been accessed.
 *
 * @param entry entry to configure.
 * @param accessed set true if the page has been accessed, false if the page has not been accessed.
 */
extern void page_table_set_accessed(page_table_entry_t* entry, bool accessed);


/**
 * @brief Set this page's dirty bit.
 *
 * When a page is dirty (1), it means that the data within the page is different
 * than when it was originally loaded. When the page is clean (0), the page
 * contents are the same as when they were originally loaded. This means that 
 * if a page is dirty, the page should be saved back onto the disk before loading
 * another page.
 *
 * @param entry entry to configure.
 * @param dirty is the page dirty (true) or clean (false).
 */
extern void page_table_set_dirty(page_table_entry_t* entry, bool dirty);


/**
 * @brief Set this page's global bit.
 *
 * If this page is marked as global, then it will prevent the TLB
 * from updating the address in its cache if cr3 is reset.
 *
 * @param entry entry to configure.
 * @param global set to true to enable global on this page, false to disable global.
 */
extern void page_table_set_global(page_table_entry_t* entry, bool global);


/**
 * @brief Set custom bits for OS specific use.
 *
 * There are 3 bits that can be used for custom use. The first 3 bits of the parameter custom will be used.
 *
 * @param entry entry to configure.
 * @param custom custom bits to set (only first 3 bits are used).
 */
extern void page_table_set_custom(page_table_entry_t* entry, u8 custom);


/**
 * @brief Set page table base address.
 *
 * The first 20 bits are used, all other bits are ignored.
 *
 * @param entry entry to configure.
 * @param address base address of page table.
 */
extern void page_table_set_physical_page_address(page_table_entry_t* entry, u32 address);


/**
 * @brief Get whether or not the page table is present.
 *
 * @param entry page directory entry.
 * @return true if page table is present in memory, false otherwise. 
 */
extern bool page_table_get_present(page_table_entry_t* entry);


/**
 * @brief Get the page table permissions.
 *
 * @param entry page directory entry.
 * @return page table permissions.
 */
extern paging_access_t page_table_get_permissions(page_table_entry_t* entry);


/**
 * @brief Get the page table's write type.
 *
 * @param entry page directory entry.
 * @return page table's write type.
 */
extern paging_write_type_t page_table_get_write_type(page_table_entry_t* entry);


/**
 * @brief Get whether or not this page table has caching enabled.
 *
 * @param entry page directory entry.
 * @return true if page table has caching enabled, false otherwise. 
 */
extern bool page_table_is_caching_enabled(page_table_entry_t* entry);


/**
 * @brief Get whether or not the page table has been accessed.
 *
 * @param entry page directory entry.
 * @return true if page table has been accessed, false otherwise. 
 */
extern bool page_table_has_been_accessed(page_table_entry_t* entry);


/**
 * @brief Get the page table's dirty status.
 *
 * @param entry page directory entry.
 * @return true if the page is dirty, false if the page is clean.
 */
extern bool page_table_get_dirty(page_table_entry_t* entry);


/**
 * @brief Get the page table's global status.
 *
 * @param entry page directory entry.
 * @return true if the page is global, false if the page is not global.
 */
extern bool page_table_get_global(page_table_entry_t* entry);


/**
 * @brief Get custom bits set by user.
 *
 * @param entry page directory entry.
 * @return custom bits set by user (lower 4 bits only).
 */
extern u8 page_table_get_custom(page_table_entry_t* entry);


/**
 * @brief Get page table base address.
 *
 * @param entry page directory entry.
 * @return page table base address (lower 20 bits only).
 */
extern u32 page_table_get_physical_page_address(page_table_entry_t* entry);


/**
 * @brief Enable paging.
 */
extern void paging_enable(void);


/**
 * @brief Disable paging.
 */
extern void paging_disable(void);


/**
 * @brief Get page location indicies.
 *
 * @param dest a location structure representing the location of the page. This function does
 *      not adjust for whether or not the page exists.
 * @param config page configuration structure specifying the page sizes/table sizes/directory sizes.
 * @param address memory address to calculate the location of.
 */
extern void paging_address_location(page_location_t* dest, page_config_t* config, u64 address);
