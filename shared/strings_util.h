#ifndef STRINGS_UTIL_H
#define STRINGS_UTIL_H

#include <string>
#include <vector>
#include <sstream>

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr) {
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos)
  {
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


#endif
