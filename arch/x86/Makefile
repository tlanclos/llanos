CROSS_COMPILE ?= 
export CROSS_COMPILE

ASM_SOURCES := $(wildcard *.S)
C_SOURCES := $(wildcard *.c)

CFLAGS += -I../../include
CFLAGS += -ffreestanding
CFLAGS += -nostdlib
CFLAGS += -g

all: build

build: $(ASM_SOURCES:.S=.o) $(C_SOURCES:.c=.o)

$(ASM_SOURCES:.S=.o):%.o: %.S linker.ld
	@echo "	ASM	$@"
	@$(CROSS_COMPILE)as -msyntax=intel -mmnemonic=intel --32 -g -o $@ $<

$(C_SOURCES:.c=.o):%.o: %.c linker.ld
	@echo "	CC	$@"
	@$(CROSS_COMPILE)gcc -c -o $@ $(CFLAGS) $<

clean: FORCE
	@-rm -f *.o

FORCE:
