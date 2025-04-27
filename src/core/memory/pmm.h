#pragma once

#include "multiboot_info.h"

namespace pmm {

auto init(const MultibootInfo *mbi) -> void;
auto alloc_page() -> void *;
auto free_page(void *page) -> void;

} // namespace pmm
