#include "vga_console.h"
#include <cstddef>

namespace vga {

constexpr uint16_t VGA_WIDTH = 80;
constexpr uint16_t VGA_HEIGHT = 25;
constexpr uintptr_t VGA_ADDRESS = 0xB8000;

uint8_t *vga_buffer = reinterpret_cast<uint8_t *>(VGA_ADDRESS);
uint16_t cursor_row = 0;
uint16_t cursor_col = 0;
uint8_t color = (static_cast<uint8_t>(Color::LightGray) |
                 (static_cast<uint8_t>(Color::Black) << 4));

auto set_color(Color fg, Color bg) -> void {
  color = (static_cast<uint8_t>(fg) | (static_cast<uint8_t>(bg) << 4));
}

auto clear() -> void {
  for (size_t y = 0; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t idx = (y * VGA_WIDTH + x) * 2;
      vga_buffer[idx] = ' ';
      vga_buffer[idx + 1] = color;
    }
  }
  cursor_row = 0;
  cursor_col = 0;
}

auto scroll() -> void {
  for (size_t y = 1; y < VGA_HEIGHT; y++) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
      const size_t src = (y * VGA_WIDTH + x) * 2;
      const size_t dst = ((y - 1) * VGA_WIDTH + x) * 2;
      vga_buffer[dst] = vga_buffer[src];
      vga_buffer[dst + 1] = vga_buffer[src + 1];
    }
  }
  for (size_t x = 0; x < VGA_WIDTH; x++) {
    const size_t idx = ((VGA_HEIGHT - 1) * VGA_WIDTH + x) * 2;
    vga_buffer[idx] = ' ';
    vga_buffer[idx + 1] = color;
  }
  cursor_row = VGA_HEIGHT - 1;
  cursor_col = 0;
}

auto put_char(char c) -> void {
  if (c == '\n') {
    cursor_row++;
    cursor_col = 0;
    if (cursor_row >= VGA_HEIGHT)
      scroll();
    return;
  }

  const size_t idx = (cursor_row * VGA_WIDTH + cursor_col) * 2;
  vga_buffer[idx] = c;
  vga_buffer[idx + 1] = color;

  cursor_col++;
  if (cursor_col >= VGA_WIDTH) {
    cursor_col = 0;
    cursor_row++;
    if (cursor_row >= VGA_HEIGHT)
      scroll();
  }
}

auto write(const char *text) -> void {
  for (size_t i = 0; text[i]; i++)
    put_char(text[i]);
}

auto write_line(const char *text) -> void {
  write(text);
  put_char('\n');
}

} // namespace vga
