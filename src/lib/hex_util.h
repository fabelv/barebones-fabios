#pragma once

#include <cstdint>

namespace hex {

auto u32_to_hex(uint32_t value, char *buffer) -> void;
auto u64_to_hex(uint64_t value, char *buffer) -> void;

} // namespace hex
