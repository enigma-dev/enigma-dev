#include <string>
#include <vector>

namespace egm {
namespace serialization {

/// Escapes a string for inclusion in a C++/YAML context.
std::string EscapeString(const std::string &str);

/// Returns the name or hex value of the color with the given integer value.
std::string ColorToStr(int color);
/// Returns the integer value of the given color, or -1 on failure.
int32_t ColorFromStr(std::string color);

std::vector<std::string> Tokenize(
    const std::string& str, const std::string& delimiters);

inline std::string unquote(const std::string& quotedStr) {
  std::string str = quotedStr;
  if (str.length() >= 2 && str.front() == '"' && str.back() == '"')
    str = str.substr(1, str.length()-2);

  return str;
}

inline std::string escape_hex(const std::string &hex) {
  if (hex[0] == '#') return "0x" + hex.substr(1);
  return hex;
}

}  // namespace serialization
}  // namespace egm
