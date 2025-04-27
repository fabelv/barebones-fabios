# === Paths ===
CMAKE_BUILD_DIR := build/kernel
OUT_DIR := build/output
LINKER := src/linker.ld
BOOT := src/arch/boot.s

# === Output Files ===
KERNEL_ELF := $(OUT_DIR)/kernel.elf
ISO := $(OUT_DIR)/fabios.iso

# === Targets ===
.PHONY: all run clean

all: $(KERNEL_ELF)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	@echo "[+] Cleaning..."
	rm -rf build

# === Build Kernel ===
$(KERNEL_ELF): build_kernel_objs $(OUT_DIR)/boot.o
	@echo "[+] Linking kernel.elf..."
	i686-elf-gcc -T $(LINKER) -o $@ -ffreestanding -O2 -nostdlib \
	  $(OUT_DIR)/boot.o $(shell find $(CMAKE_BUILD_DIR)/CMakeFiles/kernel_objs.dir -name '*.o') -lgcc

build_kernel_objs:
	@echo "[+] Building kernel objects..."
	cmake -B $(CMAKE_BUILD_DIR) -S .
	cmake --build $(CMAKE_BUILD_DIR) --target kernel_objs

# === Assemble Boot ===
$(OUT_DIR)/boot.o: $(BOOT)
	@echo "[+] Assembling boot.s..."
	mkdir -p $(OUT_DIR)
	i686-elf-gcc -c $< -o $@ -ffreestanding

# === Create ISO ===
$(ISO): $(KERNEL_ELF)
	@echo "[+] Creating ISO..."
	mkdir -p $(OUT_DIR)/isodir/boot/grub
	cp $(KERNEL_ELF) $(OUT_DIR)/isodir/boot/fabios.bin
	cp config/grub.cfg $(OUT_DIR)/isodir/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(OUT_DIR)/isodir

