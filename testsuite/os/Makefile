SUBDIRS := $(shell find . -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
TEST_SOURCES := $(wildcard test_*.c)
TEST_DEP_SOURCES := ../../os/math.c
TEST_DEP_SOURCES += ../../os/types.c
TEST_DEP_SOURCES += ../../os/llanos-service.c
TEST_DEP_SOURCES += ../../os/video/vga.c
TEST_DEP_SOURCES += ../../os/util/memory.c
TEST_DEP_SOURCES += ../../os/util/crypt-crc32.c
TEST_DEP_SOURCES += ../../os/util/string.c

include ../Makefile.in
