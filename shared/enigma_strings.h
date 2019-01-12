#ifndef ENIGMA_STRINGS
#define ENIGMA_STRINGS

#include <string>

inline std::string string_replace_all(std::string str, std::string substr, std::string nstr)
{
  pt pos=0;
  while ((pos=str.find(substr,pos)) != std::string::npos)
  {
    str.replace(pos,substr.length(),nstr);
    pos+=nstr.length();
  }
  return str;
}

#endif