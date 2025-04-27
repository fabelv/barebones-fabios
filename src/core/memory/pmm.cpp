#include "pmm.h"
#include "../../lib/log.h"
#include "multiboot_info.h"
#include <cstddef>

namespace memory {

namespace {

// Constants
constexpr size_t PAGE_SIZE = 4096;
constexpr size_t MAX_PAGES = 32768; // 128 MiB for now

// Bitmap and counters
uint8_t page_bitmap[MAX_PAGES / 8] = {0};
size_t total_pages = 0;
size_t used_pages = 0;

// Bitmap manipulation
auto set_bit(size_t idx) -> void { page_bitmap[idx / 8] |= (1 << (idx % 8)); }

auto clear_bit(size_t idx) -> void {
  page_bitmap[idx / 8] &= ~(1 << (idx % 8));
}

auto test_bit(size_t idx) -> bool {
  return (page_bitmap[idx / 8] & (1 << (idx % 8))) != 0;
}

// New: Page bitmap printing
auto print_page_bitmap() -> void {
  log::info("Page Bitmap:");
  for (size_t i = 0; i < total_pages; ++i) {
    if (test_bit(i)) {
      log::debug("#");
    } else {
      log::debug(".");
    }
    if ((i + 1) % 64 == 0) {
      log::debug("\n");
    }
  }
  log::debug("\n");
}

} // namespace

auto initialize(const MultibootInfo *mbi) -> void {
  log::info("Initializing Physical Memory Manager...");

  const auto *entry =
      reinterpret_cast<const MultibootMMapEntry *>(mbi->mmap_addr);
  const uintptr_t end = mbi->mmap_addr + mbi->mmap_length;

  while (reinterpret_cast<uintptr_t>(entry) < end) {
    if (entry->type == 1) { // Available
      uintptr_t region_start = entry->addr;
      uintptr_t region_end = entry->addr + entry->len;

      for (uintptr_t addr = region_start; addr < region_end;
           addr += PAGE_SIZE) {
        if (total_pages < MAX_PAGES) {
          clear_bit(total_pages);
          total_pages++;
        }
      }
    }

    entry = reinterpret_cast<const MultibootMMapEntry *>(
        reinterpret_cast<uintptr_t>(entry) + entry->size + sizeof(entry->size));
  }

  log::info("PMM initialization complete.");
  print_page_bitmap();
}

auto allocate_page() -> void * {
  for (size_t i = 0; i < total_pages; ++i) {
    if (!test_bit(i)) {
      set_bit(i);
      used_pages++;
      log::debug("Allocated page.");
      print_page_bitmap();
      return reinterpret_cast<void *>(i * PAGE_SIZE);
    }
  }

  log::error("Out of memory: no free pages available.");
  return nullptr;
}

auto free_page(void *addr) -> void {
  size_t idx = reinterpret_cast<uintptr_t>(addr) / PAGE_SIZE;
  if (idx < total_pages) {
    clear_bit(idx);
    used_pages--;
    log::debug("Freed page.");
    print_page_bitmap();
  } else {
    log::error("Invalid free_page call: address out of range.");
  }
}

auto get_total_memory() -> size_t { return total_pages * PAGE_SIZE; }

auto get_used_memory() -> size_t { return used_pages * PAGE_SIZE; }

auto get_free_memory() -> size_t {
  return (total_pages - used_pages) * PAGE_SIZE;
}

} // namespace memory
