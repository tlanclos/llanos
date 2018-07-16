#include <testsuite.h>
#include <x86/paging.h>

static void test_page_directory_set_present__should__set_present_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x000;
    page_directory_set_present(&entry, true);
    TEST_ASSERT_EQUAL(0x001, entry.config);
}

static void test_page_directory_set_present__should_clear_present_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x001;
    page_directory_set_present(&entry, false);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_directory_set_permissions__should__set_lowest_paging_access_bits_paging_supervisor_read_only(void) {
    page_directory_entry_t entry;
    entry.config = 0x006;
    page_directory_set_permissions(&entry, PAGING_SUPERVISOR_READ_ONLY);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_directory_set_permissions__should__set_highest_paging_access_bits_paging_user_read_write(void) {
    page_directory_entry_t entry;
    entry.config = 0x000;
    page_directory_set_permissions(&entry, PAGING_USER_READ_WRITE);
    TEST_ASSERT_EQUAL(0x006, entry.config);
}

static void test_page_directory_set_write_type__should__set_write_back(void) {
    page_directory_entry_t entry;
    entry.config = 0x008;
    page_directory_set_write_type(&entry, PAGING_WRITE_TYPE_WRITE_BACK);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_directory_set_write_type__should__set_write_through(void) {
    page_directory_entry_t entry;
    entry.config = 0x000;
    page_directory_set_write_type(&entry, PAGING_WRITE_TYPE_WRITE_THROUGH);
    TEST_ASSERT_EQUAL(0x008, entry.config);
}

static void test_page_directory_enable_caching__should__set_caching_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x010;
    page_directory_enable_caching(&entry, true);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_directory_enable_caching__should__clear_caching_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x000;
    page_directory_enable_caching(&entry, false);
    TEST_ASSERT_EQUAL(0x010, entry.config);
}

static void test_page_directory_set_accessed__should__set_accessed_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x000;
    page_directory_set_accessed(&entry, true);
    TEST_ASSERT_EQUAL(0x020, entry.config);
}

static void test_page_directory_set_accessed__should__clear_accessed_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x020;
    page_directory_set_accessed(&entry, false);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_directory_set_size__should__set_4k_page_size(void) {
    page_directory_entry_t entry;
    entry.config = 0x080;
    page_directory_set_size(&entry, PAGING_PAGE_SIZE_4K);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_directory_set_size__should__set_4m_page_size(void) {
    page_directory_entry_t entry;
    entry.config = 0x000;
    page_directory_set_size(&entry, PAGING_PAGE_SIZE_4M);
    TEST_ASSERT_EQUAL(0x080, entry.config);
}

static void test_page_directory_set_custom__should__set_custom_bits(void) {
    page_directory_entry_t entry;
    entry.custom = 0x0;
    page_directory_set_custom(&entry, 0x5);
    TEST_ASSERT_EQUAL(0x5, entry.custom);
}

static void test_page_directory_set_custom__should__not_overflow_bits(void) {
    page_directory_entry_t entry;
    entry.custom = 0x0;
    entry.page_table_base = 0x0;
    page_directory_set_custom(&entry, 0xff);
    TEST_ASSERT_EQUAL(0x7, entry.custom);
    TEST_ASSERT_EQUAL(0x0, entry.page_table_base);
}

static void test_page_directory_set_page_table_base__should__set_page_table_base(void) {
    page_directory_entry_t entry;
    entry.page_table_base = 0x0;
    page_directory_set_page_table_base(&entry, 0x12345678);
    TEST_ASSERT_EQUAL(0x12345, entry.page_table_base);
}

static void test_page_directory_get_present__should__get_present_bit(void) {
    page_directory_entry_t entry;
    entry.config = 0x0;
    page_directory_set_present(&entry, true);
    TEST_ASSERT_TRUE(page_directory_get_present(&entry));
}

static void test_page_directory_get_permissions__should__get_permissions(void) {
    page_directory_entry_t entry;
    entry.config = 0x0;
    page_directory_set_permissions(&entry, PAGING_USER_READ_ONLY);
    TEST_ASSERT_EQUAL(PAGING_USER_READ_ONLY, page_directory_get_permissions(&entry));
}

static void test_page_directory_get_write_type__should__get_write_type(void) {
    page_directory_entry_t entry;
    entry.config = 0x0;
    page_directory_set_write_type(&entry, PAGING_WRITE_TYPE_WRITE_THROUGH);
    TEST_ASSERT_EQUAL(PAGING_WRITE_TYPE_WRITE_THROUGH, page_directory_get_write_type(&entry));
}

static void test_page_directory_is_caching_enabled__should__get_is_caching_enabled(void) {
    page_directory_entry_t entry;
    entry.config = 0x0;
    page_directory_enable_caching(&entry, true);
    TEST_ASSERT_TRUE(page_directory_is_caching_enabled(&entry));
}

static void test_page_directory_has_been_accessed__should__get_has_been_accessed(void) {
    page_directory_entry_t entry;
    entry.config = 0x0;
    page_directory_set_accessed(&entry, true);
    TEST_ASSERT_TRUE(page_directory_has_been_accessed(&entry));
}

static void test_page_directory_get_size__should__get_page_size(void) {
    page_directory_entry_t entry;
    entry.config = 0x0;
    page_directory_set_size(&entry, PAGING_PAGE_SIZE_4M);
    TEST_ASSERT_EQUAL(PAGING_PAGE_SIZE_4M, page_directory_get_size(&entry));
}

static void test_page_directory_get_custom__should__get_custom_bits(void) {
    page_directory_entry_t entry;
    entry.custom = 0x0;
    page_directory_set_custom(&entry, 0xe);
    TEST_ASSERT_EQUAL(PAGING_PAGE_SIZE_4M, page_directory_get_size(&entry));
}

static void test_page_directory_get_page_table_base__should__get_page_table_base(void) {
    page_directory_entry_t entry;
    entry.custom = 0x0;
    page_directory_set_page_table_base(&entry, 0x12345678);
    TEST_ASSERT_EQUAL(0x12345000, page_directory_get_page_table_base(&entry));
}

static void test_page_table_set_present__should__set_present_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_set_present(&entry, true);
    TEST_ASSERT_EQUAL(0x001, entry.config);
}

static void test_page_table_set_present__should__clear_present_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x001;
    page_table_set_present(&entry, false);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_set_permissions__should__set_lowest_paging_access_bits_paging_supervisor_read_only(void) {
    page_table_entry_t entry;
    entry.config = 0x006;
    page_table_set_permissions(&entry, PAGING_SUPERVISOR_READ_ONLY);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_set_permissions__should__set_highest_paging_access_bits_paging_user_read_write(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_set_permissions(&entry, PAGING_USER_READ_WRITE);
    TEST_ASSERT_EQUAL(0x006, entry.config);
}

static void test_page_table_set_write_type__should__set_write_through(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_set_write_type(&entry, PAGING_WRITE_TYPE_WRITE_THROUGH);
    TEST_ASSERT_EQUAL(0x008, entry.config);
}

static void test_page_table_set_write_type__should__set_write_back(void) {
    page_table_entry_t entry;
    entry.config = 0x008;
    page_table_set_write_type(&entry, PAGING_WRITE_TYPE_WRITE_BACK);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_enable_caching__should__enable_caching_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x010;
    page_table_enable_caching(&entry, true);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_enable_caching__should__disable_caching_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_enable_caching(&entry, false);
    TEST_ASSERT_EQUAL(0x010, entry.config);
}

static void test_page_table_set_accessed__should__set_accessed_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_set_accessed(&entry, true);
    TEST_ASSERT_EQUAL(0x020, entry.config);
}

static void test_page_table_set_accessed__should__clear_accessed_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x020;
    page_table_set_accessed(&entry, false);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_set_dirty__should__set_dirty_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_set_dirty(&entry, true);
    TEST_ASSERT_EQUAL(0x040, entry.config);
}

static void test_page_table_set_dirty__should__clear_dirty_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x040;
    page_table_set_dirty(&entry, false);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_set_global__should__set_global_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x000;
    page_table_set_global(&entry, true);
    TEST_ASSERT_EQUAL(0x100, entry.config);
}

static void test_page_table_set_global__should__clear_global_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x100;
    page_table_set_global(&entry, false);
    TEST_ASSERT_EQUAL(0x000, entry.config);
}

static void test_page_table_set_custom__should__set_custom_data(void) {
    page_table_entry_t entry;
    entry.custom = 0x0;
    page_table_set_custom(&entry, 0x5);
    TEST_ASSERT_EQUAL(0x5, entry.custom);
}

static void test_page_table_set_custom__should__not_overflow_bits(void) {
    page_table_entry_t entry;
    entry.custom = 0x0;
    entry.phys_page_addr = 0x0;
    page_table_set_custom(&entry, 0xff);
    TEST_ASSERT_EQUAL(0x7, entry.custom);
    TEST_ASSERT_EQUAL(0x0, entry.phys_page_addr);
}

static void test_page_table_set_physical_page_address__should__set_physical_page_address(void) {
    page_table_entry_t entry;
    entry.phys_page_addr = 0x0;
    page_table_set_physical_page_address(&entry, 0x12345678);
    TEST_ASSERT_EQUAL(0x12345, entry.phys_page_addr);
}

static void test_page_table_get_present__should__get_present_bit(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_set_present(&entry, true);
    TEST_ASSERT_TRUE(page_table_get_present(&entry));
}

static void test_page_table_get_permissions__should__get_permissions(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_set_permissions(&entry, PAGING_USER_READ_ONLY);
    TEST_ASSERT_EQUAL(PAGING_USER_READ_ONLY, page_table_get_permissions(&entry));
}

static void test_page_table_get_write_type__should__get_write_type(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_set_write_type(&entry, PAGING_WRITE_TYPE_WRITE_THROUGH);
    TEST_ASSERT_EQUAL(PAGING_WRITE_TYPE_WRITE_THROUGH, page_table_get_write_type(&entry));
}

static void test_page_table_is_caching_enabled__should__get_is_caching_enabled(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_enable_caching(&entry, true);
    TEST_ASSERT_TRUE(page_table_is_caching_enabled(&entry));
}

static void test_page_table_has_been_accessed__should__get_has_been_accessed(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_set_accessed(&entry, true);
    TEST_ASSERT_TRUE(page_table_has_been_accessed(&entry));
}

static void test_page_table_get_dirty__should__get_is_dirty(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_set_dirty(&entry, true);
    TEST_ASSERT_TRUE(page_table_get_dirty(&entry));
}

static void test_page_table_get_global__should__get_is_global(void) {
    page_table_entry_t entry;
    entry.config = 0x0;
    page_table_set_global(&entry, true);
    TEST_ASSERT_TRUE(page_table_get_global(&entry));
}

static void test_page_table_get_custom__should__get_custom_bits(void) {
    page_table_entry_t entry;
    entry.custom = 0x0;
    page_table_set_custom(&entry, 0x5);
    TEST_ASSERT_EQUAL(0x5, page_table_get_custom(&entry));
}

static void test_page_table_get_physical_page_address__should__get_physical_page_address(void) {
    page_table_entry_t entry;
    entry.custom = 0x0;
    page_table_set_physical_page_address(&entry, 0x12345678);
    TEST_ASSERT_EQUAL(0x12345000, page_table_get_physical_page_address(&entry));
}

static void test_paging_address_location__should__get_page_directory_num(void) {

}

static void test_paging_address_location__should__get_page_table_num(void) {

}

static void test_paging_address_location__should__get_page_num(void) {

}

static void test_paging_address_location__should__get_page_base_addr(void) {

}

testfunc_container_t test_function_containers[] = {
    {"page_directory_set_present should set present bit", test_page_directory_set_present__should__set_present_bit},
    {"page_directory_set_present should clear present bit", test_page_directory_set_present__should_clear_present_bit},
    
    {"page_directory_set_permissions should set lowest paging access bits (paging supervisor read only)", test_page_directory_set_permissions__should__set_lowest_paging_access_bits_paging_supervisor_read_only},
    {"page_directory_set_permissions should set highest paging access bits (paging user read write)", test_page_directory_set_permissions__should__set_highest_paging_access_bits_paging_user_read_write},
    
    {"page_directory_set_write_type should set write-back", test_page_directory_set_write_type__should__set_write_back},
    {"page_directory_set_write_type should set write-through", test_page_directory_set_write_type__should__set_write_through},
    
    {"page_directory_enable_caching should set caching bit", test_page_directory_enable_caching__should__set_caching_bit},
    {"page_directory_enable_caching should clear caching bit", test_page_directory_enable_caching__should__clear_caching_bit},
    
    {"page_directory_set_accessed should set accessed bit", test_page_directory_set_accessed__should__set_accessed_bit},
    {"page_directory_set_accessed should clear accessed bit", test_page_directory_set_accessed__should__clear_accessed_bit},

    {"page_directory_set_size should set 4k page size", test_page_directory_set_size__should__set_4k_page_size},
    {"page_directory_set_size should set 4m page size", test_page_directory_set_size__should__set_4m_page_size},

    {"page_directory_set_custom should set custom bits", test_page_directory_set_custom__should__set_custom_bits},
    {"page_directory_set_custom should not overflow bits", test_page_directory_set_custom__should__not_overflow_bits},

    {"page_directory_set_page_table_base should set page table base", test_page_directory_set_page_table_base__should__set_page_table_base},

    {"page_directory_get_present should get present bit", test_page_directory_get_present__should__get_present_bit},
    {"page_directory_get_permissions should get permissions", test_page_directory_get_permissions__should__get_permissions},
    {"page_directory_get_write_type should get write type", test_page_directory_get_write_type__should__get_write_type},
    {"page_directory_is_caching_enabled should get is caching enabled", test_page_directory_is_caching_enabled__should__get_is_caching_enabled},
    {"page_directory_has_been_accessed should get has been accessed", test_page_directory_has_been_accessed__should__get_has_been_accessed},
    {"page_directory_get_size should get page size", test_page_directory_get_size__should__get_page_size},
    {"page_directory_get_custom should get custom bits", test_page_directory_get_custom__should__get_custom_bits},
    {"page_directory_get_page_table_base should get page table base", test_page_directory_get_page_table_base__should__get_page_table_base},

    {"page_table_set_present should set present bit", test_page_table_set_present__should__set_present_bit},
    {"page_table_set_present should clear present bit", test_page_table_set_present__should__clear_present_bit},

    {"page_table_set_permissions should set lowest paging access bits (paging supervisor read only)", test_page_table_set_permissions__should__set_lowest_paging_access_bits_paging_supervisor_read_only},
    {"page_table_set_permissions should set highest paging access bits (paging user read write)", test_page_table_set_permissions__should__set_highest_paging_access_bits_paging_user_read_write},

    {"page_table_set_write_type should set write-through", test_page_table_set_write_type__should__set_write_through},
    {"page_table_set_write_type should set write-back", test_page_table_set_write_type__should__set_write_back},

    {"page_table_enable_caching should enable caching bit", test_page_table_enable_caching__should__enable_caching_bit},
    {"page_table_enable_caching should disable caching bit", test_page_table_enable_caching__should__disable_caching_bit},

    {"page_table_set_accessed should set accessed bit", test_page_table_set_accessed__should__set_accessed_bit},
    {"page_table_set_accessed should clear accessed bit", test_page_table_set_accessed__should__clear_accessed_bit},

    {"page_table_set_dirty should set dirty bit", test_page_table_set_dirty__should__set_dirty_bit},
    {"page_table_set_dirty should clear dirty bit", test_page_table_set_dirty__should__clear_dirty_bit},

    {"page_table_set_global should set global bit", test_page_table_set_global__should__set_global_bit},
    {"page_table_set_global should clear global bit", test_page_table_set_global__should__clear_global_bit},

    {"page_table_set_custom should set custom data", test_page_table_set_custom__should__set_custom_data},
    {"page_table_set_custom should not overflow bits", test_page_table_set_custom__should__not_overflow_bits},

    {"page_table_set_physical_page_address should set page table base", test_page_table_set_physical_page_address__should__set_physical_page_address},

    {"page_table_get_present should get present bit", test_page_table_get_present__should__get_present_bit},
    {"page_table_get_permissions should get permissions", test_page_table_get_permissions__should__get_permissions},
    {"page_table_get_write_type should get write type", test_page_table_get_write_type__should__get_write_type},
    {"page_table_is_caching_enabled should get is caching enabled", test_page_table_is_caching_enabled__should__get_is_caching_enabled},
    {"page_table_has_been_accessed should get has been accessed", test_page_table_has_been_accessed__should__get_has_been_accessed},
    {"page_table_get_dirty should get is dirty", test_page_table_get_dirty__should__get_is_dirty},
    {"page_table_get_global should get is global", test_page_table_get_global__should__get_is_global},
    {"page_table_get_custom should get custom bits", test_page_table_get_custom__should__get_custom_bits},
    {"page_table_get_physical_page_address should get page table base", test_page_table_get_physical_page_address__should__get_physical_page_address},

    {"paging_address_location should get page directory number", test_paging_address_location__should__get_page_directory_num},
    {"paging_address_location should get page table number", test_paging_address_location__should__get_page_table_num},
    {"paging_address_location should get page number", test_paging_address_location__should__get_page_num},
    {"paging_address_location should get page base address", test_paging_address_location__should__get_page_base_addr},
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
    return 0;
}