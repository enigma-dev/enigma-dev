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

#include <map>
#include "compile_organization.h"

#define flushl (fflush(stdout), "\n")
#define flushs (fflush(stdout), " ")

namespace used_funcs
{
  extern bool object_set_sprite;
  void zero();
}
extern std::map<string,parsed_script*> scr_lookup;

extern const char* license;
extern string format_error(string code,string err,int pos);


inline string tdefault(string t) {
  return (t != "" ? t : "var");
}
inline void* lgmRoomBGColor(int c) {
  return (void*)((c & 0xFF)?(((c & 0x00FF0000) >> 8) | ((c & 0x0000FF00) << 8) | ((c & 0xFF000000) >> 24)):0xFFFFFFFF);
}

inline string system_get_uppermost_tier() {
  return "object_collisions";
}
