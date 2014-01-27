#include "workdir.h"

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
