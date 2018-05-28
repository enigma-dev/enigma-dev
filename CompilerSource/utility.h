#ifndef COMPILER_UTILITY_H
#define COMPILER_UTILITY_H

#include <string>

static inline std::string toUpper(std::string x) {
  std::string res = x;
  for (size_t i = 0; i < res.length(); i++)
    res[i] = res[i] >= 'a' and res[i] <= 'z' ? res[i] + 'A' - 'a' : res[i];
  return res;
}

#endif  // COMPILER_UTILITY_H