#pragma once

#include "multiboot.h"
namespace mem {
void parse_memory_map(const multiboot_info_t *mbi);
}
