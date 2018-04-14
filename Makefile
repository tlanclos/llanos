ARCH ?= x86
CROSS_COMPILE ?=
export CROSS_COMPILE
export ARCH

CFLAGS := -g
LDFLAGS := -Tarch/$(ARCH)/linker.ld

SUBDIRS := arch/$(ARCH)
SUBDIRS += os

LINKER := arch/$(ARCH)/linker.ld
TARGET := vm-llanos
ISO := llanos.iso

all: build

build: $(TARGET) $(ISO)

$(ISO): $(TARGET) boot/grub.cfg
	@echo "	ISO	$@"
	@install -dv .isobuild/boot/grub
	@install -v $(TARGET) .isobuild/boot/.
	@install -v boot/grub.cfg .isobuild/boot/grub/.
	@grub-mkrescue -o $(ISO) .isobuild
	@rm -rf .isobuild

$(TARGET): $(SUBDIRS)
	@echo "	LD	$@"
	@$(CROSS_COMPILE)ld -o $@ $(CFLAGS) $(LDFLAGS) `for dir in $(SUBDIRS); do find $${dir} -name *.o; done`

$(SUBDIRS):%: FORCE
	@$(MAKE) -C $@ build

$(SUBDIRS:%=clean-%):%: FORCE
	@$(MAKE) -C `echo $@ | cut -d- -f2-` clean

clean: $(SUBDIRS:%=clean-%)
	@-rm -f $(TARGET)
	@-rm -f $(ISO)

FORCE:
