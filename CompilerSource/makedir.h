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

#ifndef _WORKDIR_H
#define _WORKDIR_H

#include <cstdlib>
#include <string>
using std::string;

#include "makedir.h"
#include "settings.h"
#include "OS_Switchboard.h" //Tell us where the hell we are

extern string makedir;

string myReplace(string str, const string& oldStr, const string& newStr);
string escapeEnv(string str, string env);
string escapeEnv(string str);
void setMakeDirectory(string dir);

#endif //_WORKDIR_H
