#pragma once

namespace log {

enum class Level {
  Info,
  Warn,
  Debug,
  Error,
};

// Logging functions
auto info(const char *msg) -> void;
auto warn(const char *msg) -> void;
auto debug(const char *msg) -> void;
auto error(const char *msg) -> void;

} // namespace log
