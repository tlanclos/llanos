ARCH ?= x86
CROSS_COMPILE ?=
export CROSS_COMPILE
export ARCH

GCC_ARCH := $(shell $(CROSS_COMPILE)gcc -dumpmachine)

CFLAGS := -g
LDFLAGS := -Tarch/$(ARCH)/linker.ld

SUBDIRS := arch/$(ARCH)
SUBDIRS += os

LINKER := arch/$(ARCH)/linker.ld
TARGET := vm-llanos
ISO := llanos.iso

all: build

build: built-with testsuite $(TARGET) $(ISO)

testsuite: FORCE
	@$(MAKE) -C testsuite

test: testsuite
	@$(MAKE) -C testsuite run

run-x86: FORCE
	@qemu-system-i386 -cdrom llanos.iso -m 1024

debug-x86: FORCE
	@qemu-system-i386 -cdrom llanos.iso -m 1024 -s -S &

run: $(TARGET) $(ISO)
	@$(MAKE) run-$(ARCH)

debug: $(TARGET) $(ISO)
	@$(MAKE) debug-$(ARCH)
	@gdb -x arch/$(ARCH)/gdbinit

$(ISO): $(TARGET) boot/grub.cfg
	@echo "	ISO	$@"
	@install -dv .isobuild/boot/grub
	@install -v $(TARGET) .isobuild/boot/.
	@install -v boot/grub.cfg .isobuild/boot/grub/.
	@grub-mkrescue -o $(ISO) .isobuild
	@rm -rf .isobuild

$(TARGET): $(SUBDIRS) FORCE
	@echo "	LD	$@"
	@$(CROSS_COMPILE)ld -o $@ $(CFLAGS) $(LDFLAGS) `for dir in $(SUBDIRS); do find $${dir} -name *.$(GCC_ARCH).o; done`
	@grub-file --is-x86-multiboot "$(TARGET)" || (echo "	MULTIBOOT CHECK FAILED!" && exit 1)

$(SUBDIRS):%: FORCE
	@$(MAKE) -C $@ build

$(SUBDIRS:%=clean-%):%: FORCE
	@$(MAKE) -C `echo $@ | cut -d- -f2-` clean

clean: $(SUBDIRS:%=clean-%)
	@$(MAKE) -C testsuite clean
	@-rm -f $(TARGET)
	@-rm -f $(ISO)

built-with: FORCE
	@echo "Cross Compile: $(CROSS_COMPILE)"

FORCE:
