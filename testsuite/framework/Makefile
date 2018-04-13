SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)

DEPENDENCIES := $(SOURCES:%=%.d)

CFLAGS += -I../include

all: build

build: $(OBJECTS)

$(DEPENDENCIES):%.d: %
	@echo "	DEP	$@"
	@$(CC) $(CFLAGS) -MM $< > $@

$(OBJECTS):%.o: %.c
	@echo "	CC	$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

clean: FORCE
	@-rm -f $(OBJECTS)
	@-rm -f $(DEPENDENCIES)

FORCE:

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDENCIES)
endif