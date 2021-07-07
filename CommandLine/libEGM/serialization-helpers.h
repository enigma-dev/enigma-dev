
#ifndef EGM_SERIAL_H
#define EGM_SERIAL_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

namespace egm {
namespace serialization {

/// Escapes a string for inclusion in a C++/YAML context.
std::string EscapeString(const std::string &str);

/// Returns the name or hex value of the color with the given integer value.
std::string ColorToStr(int color);
/// Returns the integer value of the given color, or -1 on failure.
int32_t ColorFromStr(std::string color);

inline bool IsHex(char c) {
  return (c >= '0' && c <= '9')
      || (c >= 'a' && c <= 'f')
      || (c >= 'A' && c <= 'F');
}

int ParseHex(const std::string &hex);

std::pair<std::string, size_t>
ReadQuotedString(const std::string &data, size_t i);

}  // namespace serialization
}  // namespace egm

#endif
