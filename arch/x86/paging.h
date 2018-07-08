#pragma once

#include <llanos/types.h>

typedef struct page_directory_entry_s page_directory_entry_t;
typedef enum paging_access_e paging_access_t;
typedef enum paging_write_type_e paging_write_type_t;
typedef enum paging_page_size_e paging_page_size_t;

struct page_directory_entry_s {
    u8 config;
    u8 custom : 4;
    u32 page_table_base : 20;
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
 * There are 4 bits that can be used for custom use. The first 4 bits of the parameter custom will be used.
 *
 * @param entry entry to configure.
 * @param custom custom bits to set (only first 4 bits are used).
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
 * @param entry entry to get custom bits from.
 * @return true if page table is present in memory, false otherwise. 
 */
extern bool page_directory_get_present(page_directory_entry_t* entry);


/**
 * @brief Get the page table permissions.
 *
 * @param entry entry to get custom bits from.
 * @return page table permissions.
 */
extern paging_access_t page_directory_get_permissions(page_directory_entry_t* entry);


/**
 * @brief Get the page table's write type.
 *
 * @param entry entry to get custom bits from.
 * @return page table's write type.
 */
extern paging_write_type_t page_directory_get_write_type(page_directory_entry_t* entry);


/**
 * @brief Get whether or not this page table has caching enabled.
 *
 * @param entry entry to get custom bits from.
 * @return true if page table has caching enabled, false otherwise. 
 */
extern bool page_directory_is_caching_enabled(page_directory_entry_t* entry);


/**
 * @brief Get whether or not the page table has been accessed.
 *
 * @param entry entry to get custom bits from.
 * @return true if page table has been accessed, false otherwise. 
 */
extern bool page_directory_has_been_accessed(page_directory_entry_t* entry);


/**
 * @brief Get the page table's page size.
 *
 * @param entry entry to get custom bits from.
 * @return page table's page size.
 */
extern paging_page_size_t page_directory_get_size(page_directory_entry_t* entry);


/**
 * @brief Get custom bits set by user.
 *
 * @param entry entry to get custom bits from.
 * @return custom bits set by user (lower 4 bits only).
 */
extern u8 page_directory_get_custom(page_directory_entry_t* entry);


/**
 * @brief Get page table base address.
 *
 * @param entry entry to get custom bits from.
 * @return page table base address (lower 20 bits only).
 */
extern u32 page_directory_get_page_table_base(page_directory_entry_t* entry);


/**
 * @brief Setup paging directory base address.
 *
 * @param directory page directory to use.
 */
extern void paging_set_page_directory(page_directory_entry_t* directory);

/**
 * @brief Enable paging.
 */
extern void paging_enable(void);

/**
 * @brief Disable paging.
 */
extern void paging_disable(void);
