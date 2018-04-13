CROSS_COMPILE ?= 
export CROSS_COMPILE

ASM_SOURCES := $(wildcard *.S)
OBJECTS := $(ASM_SOURCES:.S=.o)

all: build

build: $(OBJECTS)

$(OBJECTS):%.o: %.S
	@echo "	ASM	$@"
	@$(CROSS_COMPILE)as -msyntax=intel -mmnemonic=intel --32 -g -o $@ $<

clean: FORCE
	@-rm -f *.o

FORCE:
