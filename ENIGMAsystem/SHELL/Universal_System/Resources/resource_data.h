/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef RESOURCE_DATA_H
#define RESOURCE_DATA_H

#include <string>
#include <vector>

namespace enigma {
  struct callable_script {
    evariant (*base)();
    int argnum;
  };
  struct nameid_pair {
    std::string name; int id;
  };
  void map_resource_ids(nameid_pair* n);
  extern std::vector<callable_script> callable_scripts;
  extern nameid_pair resource_nameids[];
}

namespace enigma_user
{

int resource_get_id(std::string name);
evariant script_execute(int scr, evariant arg0 = 0, evariant arg1 = 0, evariant arg2 = 0, evariant arg3 = 0, evariant arg4 = 0, evariant arg5 = 0, evariant arg6 = 0, evariant arg7 = 0, evariant arg8 = 0, evariant arg9 = 0, evariant arg10 = 0, evariant arg11 = 0, evariant arg12 = 0, evariant arg13 = 0, evariant arg14 = 14, evariant arg15 = 0);
bool script_exists(int script);

std::string object_get_name(int i);
std::string sprite_get_name(int i);
std::string background_get_name(int i);
std::string font_get_name(int i);
std::string timeline_get_name(int i);
std::string path_get_name(int i);
std::string sound_get_name(int i);
std::string script_get_name(int i);
std::string shader_get_name(int i);

}

#endif //RESOURCE_DATA_H
