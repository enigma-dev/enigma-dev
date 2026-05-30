#ifndef STRINGS_UTIL_H
#define STRINGS_UTIL_H

#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <filesystem>

inline std::string ToLower(std::string_view str_v) {
  std::string str(str_v);
  for (char &c : str) if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
  return str;
}

inline std::string Hyphenate(std::string_view snakev) {
  std::string snake{snakev};
  for (char &c : snake) if (c == '_') c = '-';
  return snake;
}

inline std::string Spaceify(std::string_view snakev) {
  std::string snake{snakev};
  for (char &c : snake) if (c == '_') c = ' ';
  return snake;
}

inline std::string ToCamelCase(std::string_view snakev, bool upper = false) {
  std::string snake{snakev};
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

inline std::string ToPascalCase(std::string_view snakev) {
  std::string snake{snakev};
  return ToCamelCase(snake, true);
}

inline std::string Capitalize(std::string_view strv) {
  std::string str{strv};
  if (str[0] >= 'a' && str[0] <= 'z') str[0] -= 'a' - 'A';
  return str;
}

inline bool ParseBool(std::string_view b) {
  const std::string bl = ToLower(b);
  if (bl == "yes" || bl == "true" || bl == "y") return true;
  return std::stod(bl);
}

// Parses the given string as an integer, returning nullopt if any character in
// the given string is not a digit (Does not support negatives).
// TODO: Replace result type with optional<int>.
inline std::pair<bool, int> SafeAtoL(std::string_view str) {
  int res = 0;
  for (char c : str) {
    if (c < '0' || c > '9' || res > 429496729) return {false, 0};
    res = 10 * res + c - '0';
  }
  return {true, res};
}

// Returns the first argument that isn't empty, or empty if all are empty.
inline std::string_view FirstNotEmpty(std::string_view a,
                                      std::string_view b) {
  return a.empty() ? b : a;
}

// Removes all occurrences of the given character from the given string.
inline std::string StripChar(std::string_view str, char c) {
  std::string res;
  res.reserve(str.length());
  for (size_t j = 0; j < str.length(); ++j) {
    if (str[j] != c) res += str[j];
  }
  return res;
}

inline bool string_ends_with(std::string_view fullString, std::string const &ending) {
  if (fullString.length() < ending.length())
    return false;

  return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
}


inline std::string string_replace_all(std::string_view strv, std::string_view substr,
                                      std::string_view nstr) {
  std::string str(strv);
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos) {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

inline std::string StrCat(std::string_view a, std::string_view b) {
  std::string res;
  res.reserve(a.length() + b.length());
  res.append(a);
  res.append(b);
  return res;
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

inline std::string remove_trailing_zeros(double numb) {
  std::string strnumb = std::to_string(numb);
  while (!strnumb.empty() && strnumb.find('.') != std::string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();
  return strnumb;
}

#endif
