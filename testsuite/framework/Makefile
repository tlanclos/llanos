SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%=%.o)

DEPENDENCIES := $(SOURCES:%=%.d)

CFLAGS += -I../include

all: build

build: $(OBJECTS)

$(DEPENDENCIES):%.d: %
	@echo "	DEP	$@"
	@$(CC) $(CFLAGS) -MM $< > $@

$(OBJECTS):%.c.o: %.c
	@echo "	CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean: FORCE
	@-rm -f *.c.o
	@-rm -f *.c.d

FORCE:

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDENCIES)
endif