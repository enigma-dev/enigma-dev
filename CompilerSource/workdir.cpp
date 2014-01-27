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

string escapeEnv(string str, string env) {
	char* val = getenv(env.c_str());
	if (val != NULL)
		return myReplace(str, "%" + env + "%", val);
	return str;
}

string escapeEnv(string str) {
	string escaped = escapeEnv(str, "PROGRAMDATA");
	escaped = escapeEnv(escaped, "ALLUSERSPROFILE");
	escaped = escapeEnv(escaped, "HOME");
	return escaped;
}

#if CURRENT_PLATFORM_ID == OS_WINDOWS
string workdir = myReplace(escapeEnv(setting::make_directory), "\\","/");
#else
string workdir = escapeEnv(setting::make_directory);
#endif
