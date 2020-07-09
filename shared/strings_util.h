#ifndef STRINGS_UTIL_H
#define STRINGS_UTIL_H

#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

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

inline std::string tolower(const std::string &str) {
  std::string res = str;
  for (size_t i = 0; i < res.length(); ++i) {
    if (res[i] >= 'A' && res[i] <= 'Z') res[i] += 'a' - 'A';
  }
  return res;
}

// helper function to parse strings contain environment variables
// example: "${HOME}/fuck you bitch/i hate you bitch ass faggots"
inline std::string environment_expand_variables(std::string str) {
  if (str.find("${") == std::string::npos) return str;
  std::string pre = str.substr(0, str.find( "${" ));
  std::string post = str.substr(str.find( "${" ) + 2);
  if (post.find('}') == std::string::npos) return str;
  std::string variable = post.substr(0, post.find('}'));
  post = post.substr(post.find('}') + 1);
  std::string value = std::getenv(variable.c_str()) ? : "";
  return environment_expand_variables(pre + value + post);
}

#endif
