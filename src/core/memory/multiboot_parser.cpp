#include "multiboot_parser.h"
#include "../../lib/hex_util.h"
#include "../../lib/log.h"

namespace multiboot {

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

  char addr_buf[32];
  char len_buf[32];

  char line_buf[128];

  while (entry < end) {
    hex::u64_to_hex(entry->addr, addr_buf);
    hex::u64_to_hex(entry->len, len_buf);

    const char *type_str = "Unknown";
    switch (entry->type) {
    case 1:
      type_str = "Available";
      break;
    case 2:
      type_str = "Reserved";
      break;
    case 3:
      type_str = "ACPI Reclaimable";
      break;
    case 4:
      type_str = "NVS";
      break;
    case 5:
      type_str = "Bad RAM";
      break;
    }

    // Compose full line manually
    int n = 0;
    for (int i = 0; addr_buf[i]; ++i)
      line_buf[n++] = addr_buf[i];
    line_buf[n++] = ' ';
    for (int i = 0; len_buf[i]; ++i)
      line_buf[n++] = len_buf[i];
    line_buf[n++] = ' ';
    for (int i = 0; type_str[i]; ++i)
      line_buf[n++] = type_str[i];
    line_buf[n] = '\0';

    log::info(line_buf);

    entry = reinterpret_cast<const MultibootMMapEntry *>(
        reinterpret_cast<uintptr_t>(entry) + entry->size + sizeof(entry->size));
  }
}

} // namespace multiboot
