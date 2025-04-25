#include <cstdint>

extern "C" void kernel_main(uint32_t a, uint32_t b) {

  while (true) {
    asm volatile("hlt");
  }
}
