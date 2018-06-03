SUBDIRS := $(shell find . -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
TEST_SOURCES := $(wildcard test_*.c)
TEST_DEP_SOURCES := ../../os/math.c
TEST_DEP_SOURCES += ../../os/types.c

include ../Makefile.in
