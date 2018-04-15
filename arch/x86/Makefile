CROSS_COMPILE ?= 
export CROSS_COMPILE

ASM_SOURCES := $(wildcard *.S)
C_SOURCES := $(wildcard *.c)

all: build

build: $(OBJECTS)

$(ASM_SOURCES:.S=.o):%.o: %.S
	@echo "	ASM	$@"
	@$(CROSS_COMPILE)as -msyntax=intel -mmnemonic=intel --32 -g -o $@ $<

$(C_SOURCES:.c=.o):%.o: %.c
	@echo "	CC	$@"
	@$(CROSS_COMPILE)gcc -o $@ -c $<

clean: FORCE
	@-rm -f *.o

FORCE:
