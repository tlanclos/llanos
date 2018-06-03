SOURCES := $(wildcard *.c)
GCC_ARCH := $(shell $(CC) -dumpmachine)
export GCC_ARCH

OBJECTS := $(SOURCES:%=%.$(GCC_ARCH).o)
DEPENDENCIES := $(SOURCES:%=%.$(GCC_ARCH).d)

CFLAGS += -I../include
CFLAGS += -g

all: build

build: $(OBJECTS)

$(DEPENDENCIES):%.$(GCC_ARCH).d: %
	@echo "	DEP	$@"
	@$(CC) $(CFLAGS) -MM $< > $@

$(OBJECTS):%.c.$(GCC_ARCH).o: %.c
	@echo "	CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean: FORCE
	@-rm -f *.o
	@-rm -f *.d

FORCE:

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDENCIES)
endif