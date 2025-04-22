
#include "./kernel_string.h"

namespace lib {

KernelString::KernelString(char *buffer, size_t capacity)
    : buffer_(buffer), capacity_(capacity), length_(0) {
  buffer_[0] = '\0';
}

void KernelString::append(const char *str) {
  while (*str && length_ < capacity_ - 1) {
    buffer_[length_++] = *str++;
  }
  buffer_[length_] = '\0';
}

const char *KernelString::c_str() const { return buffer_; }

size_t KernelString::length() const { return length_; }
} // namespace lib
