#include <string>

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr)
{
  size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos)
  {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}
