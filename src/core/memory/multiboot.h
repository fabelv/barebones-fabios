#pragma once

#include <stdint.h>

struct multiboot_info_t {
  uint32_t flags;

  // flags[0] - basic memory info
  uint32_t mem_lower;
  uint32_t mem_upper;

  // flags[1] - boot device
  uint32_t boot_device;

  // flags[2] - kernel command line
  uint32_t cmdline;

  // flags[3] - modules
  uint32_t mods_count;
  uint32_t mods_addr;

  // flags[4] or flags[5] - symbol table
  union {
    struct {
      uint32_t tabsize;
      uint32_t strsize;
      uint32_t addr;
      uint32_t reserved;
    } aout_syms;

    struct {
      uint32_t num;
      uint32_t size;
      uint32_t addr;
      uint32_t shndx;
    } elf_syms;
  } syms;

  // flags[6] - memory map
  uint32_t mmap_length;
  uint32_t mmap_addr;

  // flags[7] - drives
  uint32_t drives_length;
  uint32_t drives_addr;

  // flags[8] - ROM configuration table
  uint32_t config_table;

  // flags[9] - boot loader name
  uint32_t boot_loader_name;

  // flags[10] - APM table
  uint32_t apm_table;

  // flags[11] - VBE info
  uint32_t vbe_control_info;
  uint32_t vbe_mode_info;
  uint16_t vbe_mode;
  uint16_t vbe_interface_seg;
  uint16_t vbe_interface_off;
  uint16_t vbe_interface_len;

  // flags[12] - framebuffer info
  uint64_t framebuffer_addr;
  uint32_t framebuffer_pitch;
  uint32_t framebuffer_width;
  uint32_t framebuffer_height;
  uint8_t framebuffer_bpp;
  uint8_t framebuffer_type;
  uint8_t color_info[6]; // Enough to hold any color data
} __attribute__((packed));

struct multiboot_mmap_entry_t {
  uint32_t size;
  uint64_t addr;
  uint64_t len;
  uint32_t type;
} __attribute__((packed));

enum MemoryType {
  AVAILABLE = 1,
  RESERVED = 2,
  ACPI_RECLAIMABLE = 3,
  NVS = 4,
  BADRAM = 5
};

// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Specification
// The format of the Multiboot information structure (as defined so far)
// follows:
/*
        +-------------------+
0       | flags             |    (required)
        +-------------------+
4       | mem_lower         |    (present if flags[0] is set)
8       | mem_upper         |    (present if flags[0] is set)
        +-------------------+
12      | boot_device       |    (present if flags[1] is set)
        +-------------------+
16      | cmdline           |    (present if flags[2] is set)
        +-------------------+
20      | mods_count        |    (present if flags[3] is set)
24      | mods_addr         |    (present if flags[3] is set)
        +-------------------+
28 - 40 | syms              |    (present if flags[4] or
        |                   |                flags[5] is set)
        +-------------------+
44      | mmap_length       |    (present if flags[6] is set)
48      | mmap_addr         |    (present if flags[6] is set)
        +-------------------+
52      | drives_length     |    (present if flags[7] is set)
56      | drives_addr       |    (present if flags[7] is set)
        +-------------------+
60      | config_table      |    (present if flags[8] is set)
        +-------------------+
64      | boot_loader_name  |    (present if flags[9] is set)
        +-------------------+
68      | apm_table         |    (present if flags[10] is set)
        +-------------------+
72      | vbe_control_info  |    (present if flags[11] is set)
76      | vbe_mode_info     |
80      | vbe_mode          |
82      | vbe_interface_seg |
84      | vbe_interface_off |
86      | vbe_interface_len |
        +-------------------+
88      | framebuffer_addr  |    (present if flags[12] is set)
96      | framebuffer_pitch |
100     | framebuffer_width |
104     | framebuffer_height|
108     | framebuffer_bpp   |
109     | framebuffer_type  |
110-115 | color_info        |
        +-------------------+

*/
