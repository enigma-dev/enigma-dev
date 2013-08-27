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

namespace enigma {
  struct callable_script {
    variant (*base)();
    int argnum;
  };
  struct nameid_pair {
    string name; int id;
  };
  void map_resource_ids(nameid_pair* n);
  extern callable_script callable_scripts[];
  extern nameid_pair resource_nameids[];
}

namespace enigma_user
{

int resource_get_id(string name);
variant script_execute(int scr, variant arg0 = 0, variant arg1 = 0, variant arg2 = 0, variant arg3 = 0, variant arg4 = 0, variant arg5 = 0, variant arg6 = 0, variant arg7 = 0, variant arg8 = 0, variant arg9 = 0, variant arg10 = 0, variant arg11 = 0, variant arg12 = 0, variant arg13 = 0, variant arg14 = 14, variant arg15 = 0);
bool script_exists(int script);

string object_get_name(int i);
string sprite_get_name(int i);
string background_get_name(int i);
string font_get_name(int i);
string timeline_get_name(int i);
string path_get_name(int i);
string sound_get_name(int i);
string script_get_name(int i);
string shader_get_name(int i);

}

