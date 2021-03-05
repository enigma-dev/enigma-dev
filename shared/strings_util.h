#ifndef STRINGS_UTIL_H
#define STRINGS_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#ifdef _WIN32

#include <cstddef>
#include <cwchar>

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

inline std::wstring widen(const std::string &str) {
  // Number of shorts will be <= number of bytes; add one for null terminator
  const std::size_t wchar_count = str.size() + 1;
  std::vector<wchar_t> buf(wchar_count);
  return std::wstring { buf.data(), (std::size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count)};
}

inline std::string shorten(std::wstring str) {
  int nbytes = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0, NULL, NULL);
  std::vector<char> buf((std::size_t)nbytes);
  return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.length(), buf.data(), nbytes, NULL, NULL)};
}

#endif

inline std::string ToLower(std::string str) {
  for (char &c : str) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
  return str;
}

inline std::string Hyphenate(std::string snake) {
  for (char &c : snake) if (c == '_') c = '-';
  return snake;
}

inline std::string Spaceify(std::string snake) {
  for (char &c : snake) if (c == '_') c = ' ';
  return snake;
}

inline std::string ToCamelCase(std::string snake, bool upper = false) {
  size_t out = 0;
  for (char &c : snake) {
    if (c == '_') {
      upper = true;
      continue;
    }
    if (c >= 'a' && c <= 'z') {
      snake[out++] = upper ? c + 'A' - 'a' : c;
    } else if (c >= 'A' && c <= 'Z') {
      snake[out++] = upper ? c : c + 'a' - 'A';
    } else {
      snake[out++] = c;
    }
    upper = false;
  }
  snake.erase(out);
  return snake;
}

inline std::string ToPascalCase(std::string snake) {
  return ToCamelCase(snake, true);
}

inline std::string Capitalize(std::string str) {
  if (str[0] >= 'a' && str[0] <= 'z') str[0] -= 'a' - 'A';
  return str;
}

inline bool ParseBool(const std::string &b) {
  const std::string bl = ToLower(b);
  if (bl == "yes" || bl == "true" || bl == "y") return true;
  return std::stod(bl);
}

// Parses the given string as an integer, returning nullopt if any character in
// the given string is not a digit (Does not support negatives).
// TODO: Replace result type with optional<int>.
inline std::pair<bool, int> SafeAtoL(const std::string &str) {
  int res = 0;
  for (char c : str) {
    if (c < '0' || c > '9' || res > 429496729) return {false, 0};
    res = 10 * res + c - '0';
  }
  return {true, res};
}

// Returns the first argument that isn't empty, or empty if all are empty.
inline const std::string &FirstNotEmpty(const std::string &a,
                                        const std::string &b) {
  return a.empty() ? b : a;
}

// Removes all occurrences of the given character from the given string.
inline std::string StripChar(std::string str, char c) {
  size_t i = 0;
  for (size_t j = 0; j < str.length(); ++j) {
    if (str[j] != c) str[i++] = str[j];
  }
  str.resize(i);
  return str;
}

inline bool string_ends_with(std::string const &fullString, std::string const &ending) {
    if (fullString.length() < ending.length())
      return false;

    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
}


inline std::string string_replace_all(std::string str, std::string_view substr,
                                      std::string_view nstr) {
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos) {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

inline std::vector<std::string> split_string(const std::string &str, char delimiter) {
        std::vector<std::string> vec;
        std::stringstream sstr(str);
        std::string tmp;

        while (std::getline(sstr, tmp, delimiter))
                vec.push_back(tmp);

        return vec;
}

inline std::string StrTrim(const std::string &str) {
  size_t s = str.find_first_not_of(" \t\r\n");
  if (s == std::string::npos) return {};
  size_t e = str.find_last_not_of(" \t\r\n");
  return str.substr(s, e - s + 1);
}

inline std::string FileToString(const std::string &fName) {
  std::ifstream t(fName.c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

inline std::string FileToString(const std::filesystem::path &path) {
  return FileToString(path.string());
}

inline bool IsNumber(const std::string& s) {
  return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

#endif
