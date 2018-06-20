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

#ifndef ENIGMA_WORKDIR_H
#define ENIGMA_WORKDIR_H

#include <cstdlib>
#include <string>

#include "makedir.h"
#include "settings.h"
#include "OS_Switchboard.h" //Tell us where the hell we are

extern std::string eobjs_directory;
extern std::string codegen_directory;

std::string myReplace(std::string str, const std::string& oldStr, const std::string& newStr);
std::string escapeEnv(std::string str, std::string env);
std::string escapeEnv(std::string str);

#endif // ENIGMA_WORKDIR_H
