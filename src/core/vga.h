#pragma once
#include <cstddef>
#include <cstdint>

namespace vga {
enum class Color : uint8_t {
  Black = 0,
  Blue = 1,
  Green = 2,
  Cyan = 3,
  Red = 4,
  Magenta = 5,
  Brown = 6,
  LightGrey = 7,
  DarkGrey = 8,
  LightBlue = 9,
  LightGreen = 10,
  LightCyan = 11,
  LightRed = 12,
  LightMagenta = 13,
  LightBrown = 14,
  White = 15,
};

void initialize();
void write_string(const char *str);
void put_char(char c);
} // namespace vga
