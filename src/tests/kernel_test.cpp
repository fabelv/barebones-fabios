#include "../core/memory/memory_map.h"
#include "../core/memory/multiboot.h"
#include "../core/vga.h"
#include "../lib/logger.h"
#include <cstdint>

static void u32_to_hex(uint32_t value, char *buffer) {
  const char *hex_chars = "0123456789ABCDEF";
  buffer[0] = '0';
  buffer[1] = 'x';
  for (int i = 0; i < 8; ++i) {
    buffer[2 + i] = hex_chars[(value >> (28 - 4 * i)) & 0xF];
  }
  buffer[10] = '\0';
}

static void u64_to_hex(uint64_t value, char *buffer) {
  const char *hex_chars = "0123456789ABCDEF";
  buffer[0] = '0';
  buffer[1] = 'x';
  for (int i = 0; i < 16; ++i) {
    buffer[2 + i] = hex_chars[(value >> (60 - 4 * i)) & 0xF];
  }
  buffer[18] = '\0';
}

void print_multiboot_info(const multiboot_info_t *mbi) {
  char buf[32];

  log::info("=== Multiboot Info ===");

  // --- Flags ---
  u32_to_hex(mbi->flags, buf);
  log::info("flags:");
  log::info(buf);

  // --- Basic Memory Info ---
  if (mbi->flags & (1 << 0)) {
    u32_to_hex(mbi->mem_lower, buf);
    log::info("mem_lower:");
    log::info(buf);

    u32_to_hex(mbi->mem_upper, buf);
    log::info("mem_upper:");
    log::info(buf);
  }

  // --- Boot Device ---
  if (mbi->flags & (1 << 1)) {
    u32_to_hex(mbi->boot_device, buf);
    log::info("boot_device:");
    log::info(buf);
  }

  // --- Kernel Command Line ---
  if (mbi->flags & (1 << 2)) {
    u32_to_hex(mbi->cmdline, buf);
    log::info("cmdline (address):");
    log::info(buf);
  }

  // --- Modules ---
  if (mbi->flags & (1 << 3)) {
    u32_to_hex(mbi->mods_count, buf);
    log::info("mods_count:");
    log::info(buf);

    u32_to_hex(mbi->mods_addr, buf);
    log::info("mods_addr:");
    log::info(buf);
  }

  // --- Symbol Tables ---
  if (mbi->flags & (1 << 4)) {
    log::info("a.out symbol table:");
    u32_to_hex(mbi->syms.aout_syms.tabsize, buf);
    log::info("  tabsize:");
    log::info(buf);

    u32_to_hex(mbi->syms.aout_syms.strsize, buf);
    log::info("  strsize:");
    log::info(buf);

    u32_to_hex(mbi->syms.aout_syms.addr, buf);
    log::info("  addr:");
    log::info(buf);
  } else if (mbi->flags & (1 << 5)) {
    log::info("ELF symbol table:");
    u32_to_hex(mbi->syms.elf_syms.num, buf);
    log::info("  num:");
    log::info(buf);

    u32_to_hex(mbi->syms.elf_syms.size, buf);
    log::info("  size:");
    log::info(buf);

    u32_to_hex(mbi->syms.elf_syms.addr, buf);
    log::info("  addr:");
    log::info(buf);

    u32_to_hex(mbi->syms.elf_syms.shndx, buf);
    log::info("  shndx:");
    log::info(buf);
  }

  // --- Memory Map ---
  if (mbi->flags & (1 << 6)) {
    u32_to_hex(mbi->mmap_length, buf);
    log::info("mmap_length:");
    log::info(buf);

    u32_to_hex(mbi->mmap_addr, buf);
    log::info("mmap_addr:");
    log::info(buf);
  }

  // --- Drives ---
  if (mbi->flags & (1 << 7)) {
    u32_to_hex(mbi->drives_length, buf);
    log::info("drives_length:");
    log::info(buf);

    u32_to_hex(mbi->drives_addr, buf);
    log::info("drives_addr:");
    log::info(buf);
  }

  // --- ROM Config Table ---
  if (mbi->flags & (1 << 8)) {
    u32_to_hex(mbi->config_table, buf);
    log::info("config_table:");
    log::info(buf);
  }

  // --- Boot Loader Name ---
  if (mbi->flags & (1 << 9)) {
    u32_to_hex(mbi->boot_loader_name, buf);
    log::info("boot_loader_name (address):");
    log::info(buf);
  }

  // --- APM Table ---
  if (mbi->flags & (1 << 10)) {
    u32_to_hex(mbi->apm_table, buf);
    log::info("apm_table:");
    log::info(buf);
  }

  // --- VBE (Video BIOS Extensions) ---
  if (mbi->flags & (1 << 11)) {
    log::info("VBE info:");

    u32_to_hex(mbi->vbe_control_info, buf);
    log::info("  vbe_control_info:");
    log::info(buf);

    u32_to_hex(mbi->vbe_mode_info, buf);
    log::info("  vbe_mode_info:");
    log::info(buf);

    u32_to_hex(mbi->vbe_mode, buf);
    log::info("  vbe_mode:");
    log::info(buf);
  }

  // --- Framebuffer Info ---
  if (mbi->flags & (1 << 12)) {
    log::info("Framebuffer info:");

    u64_to_hex(mbi->framebuffer_addr, buf);
    log::info("  framebuffer_addr:");
    log::info(buf);

    u32_to_hex(mbi->framebuffer_pitch, buf);
    log::info("  framebuffer_pitch:");
    log::info(buf);

    u32_to_hex(mbi->framebuffer_width, buf);
    log::info("  framebuffer_width:");
    log::info(buf);

    u32_to_hex(mbi->framebuffer_height, buf);
    log::info("  framebuffer_height:");
    log::info(buf);

    char fb_info[16];
    fb_info[0] = 'B';
    fb_info[1] = 'P';
    fb_info[2] = 'P';
    fb_info[3] = ':';
    fb_info[4] = ' ';
    fb_info[5] = '0' + (mbi->framebuffer_bpp / 10);
    fb_info[6] = '0' + (mbi->framebuffer_bpp % 10);
    fb_info[7] = '\0';
    log::info(fb_info);
  }

  log::info("=== End Multiboot Info ===");
}
extern "C" void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info);

extern "C" void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info) {

  log::info("should be hidden by scrolling");
  log::info("Booting test kernel...");
  log::warn("This is a warning");
  log::debug("This is a debug");
  log::error("This is an error!");

  vga::scroll_one_row();

  multiboot_info_t *mbi = reinterpret_cast<multiboot_info_t *>(multiboot_info);

  char buf[32];
  u32_to_hex(mbi->flags, buf);
  log::info("mbi->flags: ");
  log::info(buf);

  if (!(mbi->flags & (1 << 6))) {
    log::error("No memory map provided by bootloader!");
  } else {
    log::info("Memory map is available!");
  }

  mem::parse_memory_map(mbi);

  print_multiboot_info(mbi);
  log::info("end");

  while (true)
    asm volatile("hlt");
}
