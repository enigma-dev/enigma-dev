/**
 * @file  error_reporting.h
 * @brief Header declaring a base class for error and warning reporting.
 *
 * Also defines a default error handling class, which shall write all warnings
 * and errors to stderr.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef STRINGS__H
#define STRINGS__H

#include <string>
#include <string_view>
#include <ostream>

namespace jdi {

template<typename T> std::string to_string(T x) { return std::to_string(x); }
static inline std::string to_string(const std::string &x) { return x; }
static inline std::string to_string(std::string_view x) {
  return std::string(x);
}

template <typename... Args>
std::string format(std::string_view text, Args... args) {
  std::string res;
  std::string strs[] = { jdi::to_string(args)... };

  int cap = text.length();
  for (const std::string str : strs) { cap += str.length() - 2; }
  if (cap > 0) res.reserve(cap);  // Should always be true if format str is correct.

  size_t p = 0;
  for (const std::string str : strs) {
    const size_t pn = text.find("%s", p);
    if (pn == std::string::npos) break;
    res += text.substr(p, pn - p);
    res += str;
    p = pn + 2;
  }
  if (p != std::string::npos) res += text.substr(p);
  return res;
}

/// Does a printf-like string format operation.
static inline std::string format(std::string_view text) {
  return std::string(text);
}

/// Quotes a string for inclusion in C++ source code.
static inline std::string quote(std::string_view str) {
  std::string res;
  res.reserve(str.length() * 2 + 2);
  res += '"';
  for (char c : str) switch (c) {
    case '\r': res += "\\r"; break;
    case '\n': res += "\\n"; break;
    case '\\': res += "\\\\"; break;
    case '\"': res += "\\\""; break;
    case '\'': res += "\\\'"; break;
    default: res += c;
  }
  res += '"';
  return res;
}


// =============================================================================
// == Introspection logic for string conversion ================================
// =============================================================================
namespace string_detection {

/// Always contains true; SFINAE happens elsewhere.
template<typename T> struct WhenValid { enum { v = 1 }; typedef T Type; };

// Explicit acceptance of known string types.
// =============================================================================
template<typename T> struct IsString { enum { v = 0 }; };
template<> struct IsString<char*> { enum { v = 1 }; };
template<> struct IsString<const char*> { enum { v = 1 }; };
template<size_t n> struct IsString<char[n]> { enum { v = 1 }; };
template<size_t n> struct IsString<const char[n]> { enum { v = 1 }; };
template<> struct IsString<std::string_view> { enum { v = 1 }; };
template<> struct IsString<std::string> { enum { v = 1 }; };


// ToString() detector
// =============================================================================
template<typename T, bool B> struct KHas_toString {
  enum { v = 0 };
};

template<typename T>
struct KHas_toString<T, IsString<decltype(std::declval<T>().toString())>::v> {
  enum { v = 1 };
};

template<typename T> using Has_toString = KHas_toString<T, true>;


// to_string() detector
// =============================================================================
template<typename T, bool B> struct KHas_to_string { enum { v = 0 }; };

template<typename T>
struct KHas_to_string<T, IsString<decltype(std::declval<T>().to_string())>::v> {
  enum { v = 1 };
};

template<typename T> using Has_to_string = KHas_to_string<T, true>;


// Stream-put detector
// =============================================================================
template<typename T, bool U> struct KHasStreamPut { enum { v = 0 }; };

template<typename T> struct KHasStreamPut<
    T,
    WhenValid<decltype(std::declval<std::ostream>() << std::declval<T>())>::v> {
  enum { v = 1 };
};

template<typename T> using HasStreamPut = KHasStreamPut<T, true>;


// Converting to string
// =============================================================================
template<typename T,
         bool is_str = IsString<T>::v,
         bool has_ToString = Has_toString<T>::v,
         bool has_to_string = Has_to_string<T>::v,
         bool has_stream_put = HasStreamPut<T>::v>
class Stringifier {};

template<typename T, bool a, bool b, bool c>
class Stringifier<T, true, a, b, c> {  // Is a string. Rest are don't care.
 public:
  static std::string str(const T &s) { return s; };
  static std::string_view str_or_view(const T &s) { return s; };
};

template<typename T, bool a, bool b>
class Stringifier<T, false, true, a, b> {  // Not string; has toString.
 public:
  static std::string str(const T &s) { return s.toString(); };
  static std::string str_or_view(const T &s) { return s.toString(); };
};

template<typename T, bool b>
class Stringifier<T, false, false, true, b> {  // Use to_string.
 public:
  static std::string str(const T &s) { return s.to_string(); };
  static std::string str_or_view(const T &s) { return s.to_string(); };
};

template<typename T>
class Stringifier<T, false, false, false, true> {  // Use stringsteam.
 public:
  static std::string str(const T &s) {
    std::stringstream ss;
    ss << s;
    return ss.str();
  };
  static std::string str_or_view(const T &s) { return str(s); };
};

// Put to stream
// =============================================================================
template<typename T,
         bool is_str = IsString<T>::v,
         bool has_toString = Has_toString<T>::v,
         bool has_to_string = Has_to_string<T>::v,
         bool has_stream_put = HasStreamPut<T>::v>
class StringPutter {};

template<typename T, bool a, bool b, bool c>
class StringPutter<T, a, b, c, true> {
 public:
  template<typename OS> static std::ostream &put(OS &os, const T &t) {
    return os << t;
  }
};

template<typename T, bool a, bool b, bool c>
class StringPutter<T, a, b, c, false> {
 public:
  template<typename OS> static std::ostream &put(OS &os, const T &t) {
    return os << Stringifier<T>::str(t);
  }
};

// Main methods
// =============================================================================
template<typename T> std::string to_string(const T &x) {
  return Stringifier<T>::str(x);
}

template<class OS, typename T> std::ostream &put(OS &os, const T &x) {
  return StringPutter<T>::put(os, x);
}

}  // namespace string_detection


// =============================================================================
// == Return to code that does stuff ===========================================
// =============================================================================

template<typename T> struct PrettyQuote {
  const T &obj;
  operator std::string() {
    return "`" + string_detection::to_string(obj) + "`";
  }
};
template<typename T>
std::ostream &operator<<(std::ostream &stream, const PrettyQuote<T> &q) {
  stream << "`";
  string_detection::put(stream, q.obj);
  return stream << "`";
}

template<typename T> PrettyQuote<T> PQuote(T x) { return {x}; }

}  // namespace jdi

#endif
