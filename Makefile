CMAKE_BUILD_DIR := build/kernel
OUT_DIR := build/output
LINKER := src/linker.ld
BOOT := src/arch/boot.s

# Output binaries
KERNEL_BIN := $(OUT_DIR)/kernel.elf
KERNEL_TEST_BIN := $(OUT_DIR)/kernel_test.elf
ISO := $(OUT_DIR)/fabios.iso
ISO_TEST := $(OUT_DIR)/fabios-test.iso

.PHONY: all iso run test kernel_test.iso run_tests clean

all:
	cmake -B $(CMAKE_BUILD_DIR) -S .
	cmake --build $(CMAKE_BUILD_DIR) --target kernel_objs
	$(MAKE) $(KERNEL_BIN)

$(KERNEL_BIN): $(BOOT)
	mkdir -p $(OUT_DIR)
	i686-elf-gcc -c $(BOOT) -o $(OUT_DIR)/boot.o -ffreestanding
	i686-elf-gcc -T $(LINKER) -o $@ -ffreestanding -O2 -nostdlib \
	  $(OUT_DIR)/boot.o $(shell find $(CMAKE_BUILD_DIR) -name '*.o' -and -path '*kernel_objs*') -lgcc

iso: $(KERNEL_BIN)
	mkdir -p $(OUT_DIR)/isodir/boot/grub
	cp $(KERNEL_BIN) $(OUT_DIR)/isodir/boot/fabios.bin
	cp config/grub.cfg $(OUT_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(OUT_DIR)/isodir

run: iso
	qemu-system-i386 -cdrom $(ISO)

test:
	cmake -B build/test -S .
	cmake --build build/test --target unit_tests
	./build/test/unit_tests

kernel_test:
	cmake -B $(CMAKE_BUILD_DIR) -S .
	cmake --build $(CMAKE_BUILD_DIR) --target kernel_test_objs
	$(MAKE) $(KERNEL_TEST_BIN)

$(KERNEL_TEST_BIN): $(BOOT)
	mkdir -p $(OUT_DIR)
	i686-elf-gcc -c $(BOOT) -o $(OUT_DIR)/boot.o -ffreestanding
	i686-elf-gcc -T $(LINKER) -o $@ -ffreestanding -O2 -nostdlib \
	  $(OUT_DIR)/boot.o $(shell find $(CMAKE_BUILD_DIR) -name '*.o' -and -path '*kernel_test_objs*') -lgcc

kernel_test.iso: kernel_test
	mkdir -p $(OUT_DIR)/isodir/boot/grub
	cp $(KERNEL_TEST_BIN) $(OUT_DIR)/isodir/boot/fabios.bin
	cp config/grub.cfg $(OUT_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_TEST) $(OUT_DIR)/isodir

run_tests: kernel_test.iso
	qemu-system-i386 -cdrom $(ISO_TEST)

clean:
	rm -rf build

