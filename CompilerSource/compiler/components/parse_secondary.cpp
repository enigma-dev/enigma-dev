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
#include "backend/ideprint.h"

using namespace std;

#include "externs/externs.h"
#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "parser/object_storage.h"
#include "compiler/compile_common.h"
#include "compiler/event_reader/event_parser.h"

int compile_parseSecondary(map<int,parsed_object*> &parsed_objects, parsed_script* scripts[], int scrcount, map<int,parsed_room*> &parsed_rooms, parsed_object* EGMglobal)
{
  // Dump our list of dot-accessed locals
  dot_accessed_locals.clear();
  
  // Give all objects and events a second pass
  for (po_i it = parsed_objects.begin(); it != parsed_objects.end(); it++)
  {
    parsed_object *oto = it->second;
    for (unsigned iit = 0; iit < oto->events.size; iit++)
      parser_secondary(oto->events[iit].code,oto->events[iit].synt,EGMglobal,oto,&oto->events[iit]);
  }
  
  // Give all scripts a second pass
  for (int i = 0; i < scrcount; i++) {
    parser_secondary(scripts[i]->pev.code,scripts[i]->pev.synt,EGMglobal,&scripts[i]->obj,&scripts[i]->pev);
    if (scripts[i]->pev_global)
      parser_secondary(scripts[i]->pev_global->code,scripts[i]->pev_global->synt,EGMglobal,&scripts[i]->obj,scripts[i]->pev_global);
  }
  
  // Give all room creation codes a second pass
  for (pr_i it = parsed_rooms.begin(); it != parsed_rooms.end(); it++)
  {
    parsed_object *oto; // The object into which we will dump locals
    oto = it->second; // Start by dumping into this room
    if (it->second->events.size)
      parser_secondary(oto->events[0].code,oto->events[0].synt,EGMglobal,oto,&oto->events[0]);
    for (map<int,parsed_room::parsed_icreatecode>::iterator ici = it->second->instance_create_codes.begin(); ici != it->second->instance_create_codes.end(); ici++)
    {
      oto = parsed_objects[ici->second.object_index];
      parser_secondary(ici->second.pe->code,ici->second.pe->synt,EGMglobal,oto,ici->second.pe);
    }
  }
  
  return 0;
}

