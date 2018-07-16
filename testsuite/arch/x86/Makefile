REPO_ROOT := $(shell git rev-parse --show-toplevel)

TEST_SOURCES := $(wildcard test_*.c)
TEST_DEP_SOURCES := ../../../arch/x86/paging.c

CFLAGS += -I"$(REPO_ROOT)/arch"

include ../../Makefile.in
