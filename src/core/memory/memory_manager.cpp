#include "memory_manager.h"
#include "../../lib/hex_util.h"
#include "../../lib/log.h"
#include "../vga/vga_console.h"
#include <cstddef>

namespace memory {

namespace {

constexpr size_t PAGE_SIZE = 4096; // 4 KiB
constexpr size_t MAX_PAGES =
    32768; // Example: enough for 128 MiB if each page is 4 KiB

// Simple page bitmap (1 bit = 1 page)
uint8_t page_bitmap[MAX_PAGES / 8] = {0};

inline auto page_index(uintptr_t addr) -> size_t { return addr / PAGE_SIZE; }

inline auto set_bit(size_t idx) -> void {
  page_bitmap[idx / 8] |= (1 << (idx % 8));
}

inline auto clear_bit(size_t idx) -> void {
  page_bitmap[idx / 8] &= ~(1 << (idx % 8));
}

inline auto is_bit_set(size_t idx) -> bool {
  return page_bitmap[idx / 8] & (1 << (idx % 8));
}

} // namespace

auto alloc_page() -> uintptr_t {
  for (size_t idx = 0; idx < MAX_PAGES; ++idx) {
    if (!is_bit_set(idx)) {
      set_bit(idx);
      uintptr_t addr = idx * PAGE_SIZE;
      log::debug("Allocated page:");
      char buf[32];
      hex::u64_to_hex(addr, buf);
      log::debug(buf);
      return addr;
    }
  }

  log::error("Out of memory!");
  return 0;
}

auto free_page(uintptr_t addr) -> void {
  size_t idx = page_index(addr);
  clear_bit(idx);
  log::debug("Freed page:");
  char buf[32];
  hex::u64_to_hex(addr, buf);
  log::debug(buf);
}

auto mark_region_used(uintptr_t addr, size_t size) -> void {
  uintptr_t end = addr + size;
  for (uintptr_t page = addr; page < end; page += PAGE_SIZE) {
    set_bit(page_index(page));
  }
  log::debug("Marked region used.");
}

auto mark_region_free(uintptr_t addr, size_t size) -> void {
  uintptr_t end = addr + size;
  for (uintptr_t page = addr; page < end; page += PAGE_SIZE) {
    clear_bit(page_index(page));
  }
  log::debug("Marked region free.");
}

} // namespace memory
