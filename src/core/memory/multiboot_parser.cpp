#include "multiboot_parser.h"
#include "../../lib/log.h"

namespace multiboot {

// Static storage for memory regions
static MemoryRegion available_memory[MAX_MEMORY_REGIONS];
static uint32_t available_memory_count = 0;

auto parse_memory_map(const MultibootInfo *mbi) -> void {
  if (!(mbi->flags & (1 << 6))) {
    log::error("No memory map provided");
    return;
  }

  log::info("Parsing memory map:");
  const auto *entry =
      reinterpret_cast<const MultibootMMapEntry *>(mbi->mmap_addr);
  const auto *end = reinterpret_cast<const MultibootMMapEntry *>(
      mbi->mmap_addr + mbi->mmap_length);

  while (entry < end) {
    if (entry->type == 1) { // 1 = Available
      if (available_memory_count < MAX_MEMORY_REGIONS) {
        available_memory[available_memory_count++] = MemoryRegion{
            .addr = entry->addr,
            .len = entry->len,
        };
      } else {
        log::error("Too many memory regions, increase MAX_MEMORY_REGIONS");
      }
    }

    entry = reinterpret_cast<const MultibootMMapEntry *>(
        reinterpret_cast<uintptr_t>(entry) + entry->size + sizeof(entry->size));
  }
}

auto get_available_memory() -> const MemoryRegion * { return available_memory; }

auto get_available_memory_count() -> uint32_t { return available_memory_count; }

} // namespace multiboot
