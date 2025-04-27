#pragma once

#include <cstddef>
#include <cstdint>

namespace memory {

auto alloc_page() -> uintptr_t;
auto free_page(uintptr_t addr) -> void;
auto mark_region_used(uintptr_t addr, size_t size) -> void;
auto mark_region_free(uintptr_t addr, size_t size) -> void;

} // namespace memory
