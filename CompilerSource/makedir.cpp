/** Copyright (C) 2013-2014 Robert B. Colton, Cheeseboy
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "makedir.h"

#include <iostream>
using namespace std;

// Only include the headers for mkdir() if we are not on Windows; on Windows we use CreateDirectory() from windows.h
#if CURRENT_PLATFORM_ID != OS_WINDOWS
#include <sys/stat.h>
#include <unistd.h>
#else
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte
#endif

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
	string escaped = escapeEnv(str, "LOCALAPPDATA");
	escaped = escapeEnv(escaped, "APPDATA");
	escaped = escapeEnv(escaped, "PROGRAMDATA");
	escaped = escapeEnv(escaped, "ALLUSERSPROFILE");
	escaped = escapeEnv(escaped, "HOME");
	return escaped;
}

std::string eobjs_directory;
std::string codegen_directory;
