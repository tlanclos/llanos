SUBDIRS := $(shell find . -mindepth 1 -maxdepth 1 -type d -exec basename {} \;)
SOURCES := $(wildcard *.c)

CFLAGS := -I../include
CFLAGS += -ffreestanding
CFLAGS += -nostdlib
CFLAGS += -g

include Makefile.in
