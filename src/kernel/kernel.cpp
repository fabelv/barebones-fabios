#include "../core/memory/memory_manager.h"
#include "../core/memory/multiboot_info.h"
#include "../core/memory/multiboot_parser.h"
#include "../core/vga/vga_console.h"
#include "../lib/hex_util.h"
#include "../lib/log.h"
#include <cstdint>

extern "C" auto kernel_main(uint32_t multiboot_magic,
                            uint32_t multiboot_info) -> void;

extern "C" auto kernel_main(uint32_t multiboot_magic,
                            uint32_t multiboot_info) -> void {

  // vga
  vga::clear();
  log::info("Booting fabios kernel...");

  // check magic number
  char magic_buf[32];
  hex::u32_to_hex(multiboot_magic, magic_buf);
  log::debug("Multiboot magic: ");
  log::debug(magic_buf);

  if (multiboot_magic != 0x2BADB002) {
    log::error("Invalid multiboot magic!");
    while (true)
      asm volatile("hlt");
  }

  // get multiboot information
  auto *mbi = reinterpret_cast<MultibootInfo *>(multiboot_info);

  char flags_buf[32];
  hex::u32_to_hex(mbi->flags, flags_buf);
  log::debug("Multiboot flags: ");
  log::debug(flags_buf);

  // parse memory map
  multiboot::parse_memory_map(mbi);

  // log available memory regions
  uint32_t count = multiboot::get_available_memory_count();
  const multiboot::MemoryRegion *availableRegions =
      multiboot::get_available_memory();

  char region_buf[64];
  for (uint32_t i = 0; i < count; ++i) {
    const multiboot::MemoryRegion region = availableRegions[i];
    hex::u64_to_hex(region.addr, region_buf);
    log::debug("Available region:");
    log::debug(region_buf);
    hex::u64_to_hex(region.len, region_buf);
    log::debug(region_buf);
  }

  // allocated pages
  auto page1 = memory::alloc_page();
  auto page2 = memory::alloc_page();

  if (page1 != 0 && page2 != 0) {
    log::info("Allocated two pages:");
    char buf1[32];
    char buf2[32];
    hex::u64_to_hex(page1, buf1);
    hex::u64_to_hex(page2, buf2);
    log::debug(buf1);
    log::debug(buf2);
  } else {
    log::error("Failed to allocate pages!");
  }

  log::info("Kernel initialized successfully.");

  while (true)
    asm volatile("hlt");
}
