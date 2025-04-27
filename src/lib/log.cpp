#include "log.h"
#include "../core/vga/vga_console.h"

namespace log {

namespace {
auto prefix(Level level) -> const char * {
  switch (level) {
  case Level::Info:
    return "[INFO] ";
  case Level::Warn:
    return "[WARN] ";
  case Level::Debug:
    return "[DEBUG]";
  case Level::Error:
    return "[ERROR]";
  default:
    return "[LOG]  ";
  }
}

auto set_color(Level level) -> void {
  using namespace vga;
  switch (level) {
  case Level::Info:
    set_color(Color::White, Color::Black);
    break;
  case Level::Warn:
    set_color(Color::Brown, Color::Black);
    break;
  case Level::Debug:
    set_color(Color::Green, Color::Black);
    break;
  case Level::Error:
    set_color(Color::Red, Color::Black);
    break;
  }
}

auto log(Level level, const char *message) -> void {
  set_color(level);
  vga::write(prefix(level));
  vga::write_line(message);
}
} // namespace

auto info(const char *msg) -> void { log(Level::Info, msg); }
auto warn(const char *msg) -> void { log(Level::Warn, msg); }
auto debug(const char *msg) -> void { log(Level::Debug, msg); }
auto error(const char *msg) -> void { log(Level::Error, msg); }

} // namespace log
