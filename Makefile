CROSS_PREFIX = i686-elf
AS = $(CROSS_PREFIX)-as
CC = $(CROSS_PREFIX)-g++
LD = $(CROSS_PREFIX)-g++

GCC_ROOT ?= /nix/store/yanlwl2l0xvl299iv0zg37wsbz450snb-i686-elf-gcc-13.3.0
NEWLIB   ?= /nix/store/85ldw7bzhivglbsb6v8wcngcgigqbk8m-newlib-i686-elf-4.3.0.20230120

CFLAGS = \
  -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti \
  -fno-threadsafe-statics \
  -std=c++20 -D__NO_FLOAT128 \
  -nostdinc -nostdinc++ \
  -isystem $(NEWLIB)/i686-elf/include \
  -isystem $(GCC_ROOT)/lib/gcc/i686-elf/13.3.0/include \
  -isystem $(GCC_ROOT)/lib/gcc/i686-elf/13.3.0/include-fixed \
  -isystem $(GCC_ROOT)/include/c++/13.3.0 \
  -isystem $(GCC_ROOT)/include/c++/13.3.0/i686-elf

LDFLAGS = -T src/linker.ld -ffreestanding -O2 -nostdlib -lgcc

SRC_DIR = src
BUILD_DIR = build
ISO_NAME = fabios
ISO_DIR = isodir
GRUB_CFG = grub.cfg

SOURCES = $(SRC_DIR)/boot.s $(SRC_DIR)/kernel.cpp
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SOURCES))) \
          $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(filter %.s,$(SOURCES)))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(BUILD_DIR)
	$(AS) $< -o $@

all: $(BUILD_DIR)/$(ISO_NAME).bin

$(BUILD_DIR)/$(ISO_NAME).bin: $(OBJECTS) $(SRC_DIR)/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

iso: all
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/$(ISO_NAME).bin $(ISO_DIR)/boot/$(ISO_NAME).bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/$(ISO_NAME).iso $(ISO_DIR)

run: iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/$(ISO_NAME).iso

clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)

.PHONY: all iso run clean

