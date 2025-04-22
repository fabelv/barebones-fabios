#include "./core/vga.h"

extern "C" void kernel_main();

extern "C" void kernel_main() {
  vga::initialize();
  vga::write_string("Hello from kernel!\n");

  while (true) {
    asm volatile("hlt");
  }
}
