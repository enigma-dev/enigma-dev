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
#include <iostream>
#include <fstream>

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../compile_common.h"

int compile_writeGlobals(EnigmaStruct* es,parsed_object* global)
{
  ofstream wto;
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_globals.h",ios_base::out);
    wto << license;
    for (parsed_object::globit i = global->globals.begin(); i != global->globals.end(); i++)
      wto << i->second.type << " " << i->second.prefix << i->first << i->second.suffix << ";" << endl;
    //This part needs written into a global object_parent class instance elsewhere.
    //for (globit i = global->dots.begin(); i != global->globals.end(); i++)
    //  wto << i->second->type << " " << i->second->prefixes << i->second->name << i->second->suffixes << ";" << endl;
    wto << endl;
  return 0;
}
