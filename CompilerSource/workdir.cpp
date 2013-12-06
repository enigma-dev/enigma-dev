#include "workdir.h"
#include "OS_Switchboard.h" //Tell us where the hell we are

#if CURRENT_PLATFORM_ID == OS_WINDOWS
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
std::string workdir = getenv("$HOME") + "/tmp/";
#endif