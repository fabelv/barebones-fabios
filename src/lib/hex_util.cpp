#include "hex_util.h"

namespace hex {

namespace {
constexpr char HEX_CHARS[] = "0123456789ABCDEF";

auto to_hex(uint64_t value, int digits, char *buffer) -> void {
  buffer[0] = '0';
  buffer[1] = 'x';
  for (int i = 0; i < digits; ++i) {
    buffer[2 + i] = HEX_CHARS[(value >> (4 * (digits - 1 - i))) & 0xF];
  }
  buffer[2 + digits] = '\0';
}
} // namespace

auto u32_to_hex(uint32_t value, char *buffer) -> void {
  to_hex(value, 8, buffer);
}

auto u64_to_hex(uint64_t value, char *buffer) -> void {
  to_hex(value, 16, buffer);
}

} // namespace hex
