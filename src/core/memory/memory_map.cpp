#include "memory_map.h"
#include "../../lib/logger.h"

namespace mem {

void parse_memory_map(const multiboot_info_t *mbi) {
  if (!(mbi->flags & (1 << 6))) {
    log::error("No memory map provided by bootloader!");
    return;
  }

  log::info("Parsing Multiboot Memory Map...");

  uint32_t mmap_addr = mbi->mmap_addr;
  uint32_t mmap_end = mmap_addr + mbi->mmap_length;

  while (mmap_addr < mmap_end) {
    auto *entry = reinterpret_cast<const multiboot_mmap_entry_t *>(mmap_addr);

    char buffer[64];
    // You can write a helper to convert each field to hex or decimal strings
    log::info("Memory Region:");
    // For now, just print types:
    switch (entry->type) {
    case AVAILABLE:
      log::info("  -> Available");
      break;
    case RESERVED:
      log::info("  -> Reserved");
      break;
    case ACPI_RECLAIMABLE:
      log::info("  -> ACPI Reclaimable");
      break;
    case NVS:
      log::info("  -> NVS");
      break;
    case BADRAM:
      log::info("  -> Bad RAM");
      break;
    default:
      log::warn("  -> Unknown memory type");
      break;
    }

    mmap_addr += entry->size + sizeof(entry->size);
  }

  log::info("Done parsing memory map.");
}

} // namespace mem
