#include "vga.h"

namespace {
constexpr size_t WIDTH = 80;
constexpr size_t HEIGHT = 25;
constexpr uintptr_t VGA_MEMORY = 0xB8000;

size_t row = 0;
size_t col = 0;
uint8_t color =
    (uint8_t)vga::Color::LightGrey | ((uint8_t)vga::Color::Black << 4);
uint16_t *buffer = reinterpret_cast<uint16_t *>(VGA_MEMORY);

uint16_t vga_entry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}
} // namespace

extern "C" uint8_t __bss_start;
extern "C" uint8_t __bss_end;

void vga::initialize() {
  for (size_t y = 0; y < HEIGHT; ++y) {
    for (size_t x = 0; x < WIDTH; ++x) {
      buffer[y * WIDTH + x] = vga_entry(' ', color);
    }
  }
}

void vga::put_char(char c) {
  buffer[row * WIDTH + col] = vga_entry(c, color);
  if (++col == WIDTH) {
    col = 0;
    if (++row == HEIGHT)
      row = 0;
  }
}

void vga::write_string(const char *str) {
  for (size_t i = 0; str[i]; ++i)
    put_char(str[i]);
}
