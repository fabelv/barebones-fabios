CROSS_PREFIX = i686-elf
AS = $(CROSS_PREFIX)-as
CC = $(CROSS_PREFIX)-g++
LD = $(CROSS_PREFIX)-g++

GCC_ROOT := $(shell echo $$GCC_ROOT)
NEWLIB   := $(shell echo $$NEW_LIB)

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
GRUB_CFG = config/grub.cfg

SOURCES = \
  $(SRC_DIR)/arch/boot.s \
  $(SRC_DIR)/kernel.cpp \
  $(SRC_DIR)/core/vga.cpp
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter %.cpp,$(SOURCES))) \
          $(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(filter %.s,$(SOURCES)))

# Rule for compiling C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for assembling .s files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

# Link everything into the kernel binary
all: $(BUILD_DIR)/$(ISO_NAME).bin

$(BUILD_DIR)/$(ISO_NAME).bin: $(OBJECTS) $(SRC_DIR)/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# Create the ISO image
iso: all
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/$(ISO_NAME).bin $(ISO_DIR)/boot/$(ISO_NAME).bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/$(ISO_NAME).iso $(ISO_DIR)

# Run the ISO in QEMU
run: iso
	qemu-system-i386 -cdrom $(BUILD_DIR)/$(ISO_NAME).iso

# Clean everything
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)

.PHONY: all iso run clean

