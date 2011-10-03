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

#ifndef INSTANCE_SYSTEM__H
#define INSTANCE_SYSTEM__H

#include <string>
#include "var4.h"
#include "reflexive_types.h"

#include "object.h"

#include "instance_system_base.h"

#ifdef INCLUDED_FROM_SHELLMAIN
#error This file is high-impact and should not be included from SHELLmain.cpp.
#endif

#include <map>
#include <vector>

namespace enigma {
  typedef std::map<int,inst_iter*>::iterator instance_list_iterator;
  extern std::map<int,inst_iter*> instance_list;
  extern std::vector<inst_iter*> cleanups;
  
  void unlink_main(instance_list_iterator who);
}

#endif
