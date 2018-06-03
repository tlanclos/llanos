CROSS_COMPILE ?= 
GCC_ARCH := $(shell $(CROSS_COMPILE)gcc -dumpmachine)
export CROSS_COMPILE
export GCC_ARCH

ASM_SOURCES := $(wildcard *.S)
C_SOURCES := $(wildcard *.c)

CFLAGS += -I../../include
CFLAGS += -ffreestanding
CFLAGS += -nostdlib
CFLAGS += -g
CFLAGS += -std=c11

all: build

build: $(ASM_SOURCES:.S=.S.$(GCC_ARCH).o) $(C_SOURCES:.c=.c.$(GCC_ARCH).o)

$(ASM_SOURCES:.S=.S.$(GCC_ARCH).o):%.S.$(GCC_ARCH).o: %.S linker.ld
	@echo "	ASM	$@"
	@$(CROSS_COMPILE)gcc -masm=intel $(CFLAGS) -D__ASM__ -c -o $@ $<

$(C_SOURCES:.c=.c.$(GCC_ARCH).o):%.c.$(GCC_ARCH).o: %.c linker.ld
	@echo "	CC	$@"
	@$(CROSS_COMPILE)gcc -c -o $@ $(CFLAGS) $<

clean: FORCE
	@-rm -f *.o

FORCE:
