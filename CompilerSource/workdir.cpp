#include "workdir.h"
#include "settings.h"
#include "OS_Switchboard.h" //Tell us where the hell we are
#include <cstdlib>
#include <string.h>

using std::string;

string myReplace(string str, const string& oldStr, const string& newStr)
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

string escapeEnv(string str) {
	string escaped = myReplace(str, "%PROGRAMDATA%", getenv("PROGRAMDATA"));
	escaped = myReplace(str, "%ALLUSERSPROFILE%", getenv("ALLUSERSPROFILE"));
	return myReplace(str, "%HOME%", getenv("HOME"));
}

#if CURRENT_PLATFORM_ID == OS_WINDOWS
string workdir = myReplace(escapeEnv(make_directory), "\\","/");
#else
string workdir = mescapeEnv(make_directory);
#endif
