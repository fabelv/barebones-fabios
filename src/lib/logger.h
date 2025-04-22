#pragma once

namespace log {

enum class Level { Info, Warn, Error, Debug };

// You can change this to log with different prefixes/colors later
void log(Level level, const char *msg);

// Shorthand versions
void info(const char *msg);
void warn(const char *msg);
void debug(const char *msg);
void error(const char *msg);

} // namespace log
