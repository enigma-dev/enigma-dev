/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

#ifndef ENIGMA_COMPILE_COMMON_H
#define ENIGMA_COMPILE_COMMON_H

#include <map>
#include <vector>
#include "compile_organization.h"
#include "parser/object_storage.h"

// modes: 0=run, 1=debug, 2=design, 3=compile
enum compile_modes {
  emode_run,
  emode_debug,
  emode_design,
  emode_compile,
  emode_rebuild
};

namespace used_funcs
{
  extern bool object_set_sprite;
  void zero();
}

extern const char* license;


inline string tdefault(string t) {
  return (t != "" ? t : "var");
}

inline string system_get_uppermost_tier() {
  return "object_collisions";
}

#endif
