#include "../core/vga.h"
#include "../lib/logger.h"

extern "C" void kernel_main();

extern "C" void kernel_main() {
  log::info("should be hidden by scrolling");
  log::info("Booting test kernel...");
  log::warn("This is a warning");
  log::debug("This is a debug");
  log::error("This is an error!");

  vga::scroll_one_row();

  while (true)
    asm volatile("hlt");
}
