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

#include <stdio.h>
#include <string>
#include <list>

using namespace std;

#include "../filesystem/file_find.h"
#include "eyaml.h"

namespace settings
{
  list<string> systems;
  
}

int scour_settings()
{
  string bdir = "ENIGMAsystem/SHELL/";
  for (string mdir = file_find_first("ENIGMAsystem/SHELL/*",fa_directory | fa_nofiles); mdir != ""; mdir = file_find_next())
  {
    FILE *module_descriptor = fopen((bdir+mdir).c_str(),"rb");
    if (module_descriptor)
    {
      
    }
  }
  return 0;
}
