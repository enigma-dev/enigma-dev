#ifndef _WORKDIR_H
#define _WORKDIR_H
#include <cstdlib>
#include <string>
using std::string;

#include "workdir.h"
#include "settings.h"
#include "OS_Switchboard.h" //Tell us where the hell we are

extern string workdir;

string myReplace(string str, const string& oldStr, const string& newStr);
string escapeEnv(string str, string env);
string escapeEnv(string str);

#endif //_WORKDIR_H
