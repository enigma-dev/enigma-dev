#include "workdir.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
	#include <cstdlib>
	
std::string myReplace(std::string str, const std::string& oldStr, const std::string& newStr)
{
  std::string nstr = str;
  size_t pos = 0;
  while((pos = nstr.find(oldStr, pos)) != std::string::npos)
  {
     nstr.replace(pos, oldStr.length(), newStr);
     pos += newStr.length();
  }
  return nstr;
}
	
std::string workdir = myReplace(getenv("APPDATA"), "\\","/") + std::string("/ENIGMA/");
#else
std::string workdir = myReplace(getenv("$HOME"), "\\","/") + "/tmp/";
#endif
