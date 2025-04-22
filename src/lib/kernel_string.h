#pragma once

#include <cstddef>
namespace lib {

class KernelString {
public:
  KernelString(char *buffer, size_t capacity);

  void append(const char *str);
  const char *c_str() const;
  size_t length() const;

private:
  char *buffer_;
  size_t capacity_;
  size_t length_;
};

} // namespace lib
