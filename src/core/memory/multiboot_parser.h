#pragma once

#include "multiboot_info.h"
#include <cstdint>

constexpr uint32_t MAX_MEMORY_REGIONS = 32;

namespace multiboot {

struct MemoryRegion {
  uint64_t addr;
  uint64_t len;
};

auto parse_memory_map(const MultibootInfo *mbi) -> void;
auto get_available_memory() -> const MemoryRegion *;
auto get_available_memory_count() -> uint32_t;

} // namespace multiboot
