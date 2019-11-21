#ifndef STRINGS_UTIL_H
#define STRINGS_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>

inline std::string strtolower(std::string r) {
  for (size_t i = 0; i < r.length(); ++i)
    if (r[i] >= 'A' && r[i] <= 'Z') r[i] += 'a' - 'A';
  return r;
}

inline bool ParseBool(const std::string &b) {
  const std::string bl = strtolower(b);
  if (bl == "yes" || bl == "true" || bl == "y") return true;
  return std::stod(bl);
}


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

inline std::string FileToString(const std::string &fName) {
  std::ifstream t(fName.c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}



#endif
