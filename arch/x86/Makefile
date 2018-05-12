CROSS_COMPILE ?= 
export CROSS_COMPILE

ASM_SOURCES := $(wildcard *.S)
C_SOURCES := $(wildcard *.c)

CFLAGS += -I../../include
CFLAGS += -ffreestanding
CFLAGS += -nostdlib
CFLAGS += -g
CFLAGS += -std=c11

all: build

build: $(ASM_SOURCES:.S=.S.o) $(C_SOURCES:.c=.c.o)

$(ASM_SOURCES:.S=.S.o):%.S.o: %.S linker.ld
	@echo "	ASM	$@"
	@$(CROSS_COMPILE)gcc -masm=intel $(CFLAGS) -D__ASM__ -c -o $@ $<

$(C_SOURCES:.c=.c.o):%.c.o: %.c linker.ld
	@echo "	CC	$@"
	@$(CROSS_COMPILE)gcc -c -o $@ $(CFLAGS) $<

clean: FORCE
	@-rm -f *.o

FORCE:
