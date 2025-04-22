#include "vga.h"
#include <cstdint>

// unnamed (anonymous) namespace to limit the visibility of the contents to the
// current translation unit
namespace {

// VGA implements several text modes:
// 80 × 25, rendered with a 9 × 16 pixel font, with an effective resolution of
// 720 × 400[20] 40 × 25, with a 9 × 16 font, with an effective resolution of
// 360 × 400 80 × 43 or 80 × 50, with an 8 × 8 font grid, with an effective
// resolution of 640 × 344 or 640 × 400 pixels.
constexpr size_t WIDTH = 80;
constexpr size_t HEIGHT = 25;

// The video memory of the VGA is mapped to the PC's memory via a window in the
// range between segments 0xA0000 and 0xBFFFF in the PC's real mode address
// space (A000:0000 and B000:FFFF in segment:offset notation). Typically, these
// starting segments are:
// 0xA0000 for EGA/VGA graphics modes (64 KB)
// 0xB0000 for monochrome text mode (32 KB)
// 0xB8000 for color text mode and CGA-compatible graphics modes (32 KB)
constexpr uintptr_t VGA_MEMORY = 0xB8000;

// starting position
size_t row = 0;
size_t col = 0;

// color
uint8_t color =
    (uint8_t)vga::Color::LightGrey | ((uint8_t)vga::Color::Black << 4);

// cast VGA memory address to pointer
uint16_t *buffer = reinterpret_cast<uint16_t *>(VGA_MEMORY);

// vga entry 16 bit where the first 8 bit represent the color and the last 8 bit
// the ascii character
uint16_t vga_entry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}
} // namespace

extern "C" uint8_t __bss_start;
extern "C" uint8_t __bss_end;

// fill the screen with spaces to draw the background
void vga::initialize() {
  for (size_t y = 0; y < HEIGHT; ++y) {
    for (size_t x = 0; x < WIDTH; ++x) {
      buffer[y * WIDTH + x] = vga_entry(' ', color);
    }
  }
}

// shift all rows up by one
void vga::scroll_one_row() {
  for (size_t y = 1; y < HEIGHT - 1; ++y) {
    for (size_t x = 0; x < WIDTH; ++x) {
      buffer[(y - 1) * WIDTH + x] = buffer[y * WIDTH + x];
    }
  }

  row -= 1;
}

// put a single char at the current position
void vga::put_char(char c) {
  if (c == '\n') {
    col = 0;
    if (++row == HEIGHT)
      scroll_one_row();
    return;
  }

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

void vga::write_string_nl(const char *str) {
  write_string(str);
  put_char('\n');
}

void vga::set_color(vga::Color fg, vga::Color bg) {
  // 8 bit where the first 4 bits represent bg and the last 4 bits the fg
  ::color = (uint8_t)fg | ((uint8_t)bg << 4);
}
