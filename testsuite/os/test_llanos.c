#include <testsuite.h>
#include <llanos/llanos.h>
#include <llanos/video/vga.h>


static void test_set_llanos_vga__should__set_a_copy_of_vga_structure(void) {
    vga_t vga;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&vga, buffer, width, height);
    set_llanos_vga(&vga);
    vga_put_character(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, 'a');

    TEST_ASSERT_FALSE(vga_equal(&vga, get_llanos_vga()));
}

static void test_get_llanos_vga__should__return_a_modifiable_vga(void) {
    vga_t vga;
    int width = 10;
    int height = 10;
    uint16_t buffer[width * height];

    vga_initialize(&vga, buffer, width, height);
    set_llanos_vga(&vga);

    vga_put_character(get_llanos_vga(), VGA_COLOR_BLACK, VGA_COLOR_BLACK, 'a');
    vga_put_character(get_llanos_vga(), VGA_COLOR_BLACK, VGA_COLOR_BLACK, 'a');
    vga_put_character(get_llanos_vga(), VGA_COLOR_BLACK, VGA_COLOR_BLACK, 'a');

    vga_put_string(&vga, VGA_COLOR_BLACK, VGA_COLOR_BLACK, "aaa");

    TEST_ASSERT_TRUE(vga_equal(&vga, get_llanos_vga()));
}


testfunc_container_t test_function_containers[] = {
    {"test_set_llanos_vga__should__set_a_copy_of_vga_structure", test_set_llanos_vga__should__set_a_copy_of_vga_structure},

    {"test_get_llanos_vga__should__return_a_modifiable_vga", test_get_llanos_vga__should__return_a_modifiable_vga}
};

int main(void) {
    const testsuite_t testsuite = {
        .test_function_containers = test_function_containers,
        .num_test_function_containers = sizeof(test_function_containers) / sizeof(testfunc_container_t)
    };

    testsuite_run_tests(&testsuite);
}