#pragma once

#include "multiboot_info.h"

namespace multiboot {

// Parses the multiboot memory map
auto parse_memory_map(const MultibootInfo *mbi) -> void;

} // namespace multiboot
