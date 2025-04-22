#include "./logger.h"
#include "../core/vga.h"

namespace log {

void log(Level level, const char *message) {
  using namespace vga;

  // Simple colored log output
  switch (level) {
  case Level::Info:
    set_color(vga::Color::White, vga::Color::Black);
    write_string("[INFO] ");
    break;
  case Level::Warn:
    set_color(vga::Color::Brown, vga::Color::Black);
    write_string("[WARN] ");
    break;
  case Level::Debug:
    set_color(vga::Color::Green, vga::Color::Black);
    write_string("[DEBUG] ");
    break;
  case Level::Error:
    set_color(vga::Color::Red, vga::Color::Black);
    write_string("[ERR ] ");
    break;
  }
  write_string_nl(message);
}

void info(const char *msg) { log(Level::Info, msg); }
void warn(const char *msg) { log(Level::Warn, msg); }
void debug(const char *msg) { log(Level::Debug, msg); }
void error(const char *msg) { log(Level::Error, msg); }

} // namespace log
