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

#include <stdio.h>
#include <iostream>
#include "backend/ideprint.h"

using namespace std;

#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"
#include "event_reader/event_parser.h"

#include "languages/lang_CPP.h"
int lang_CPP::compile_parseSecondary(CompileState &state) {
  // Index things
  const auto &scripts = state.parsed_scripts;
  const auto &tlines = state.parsed_tlines;
  map<string, parsed_object*> po_by_name;
  for (parsed_object* po : state.parsed_objects) po_by_name[po->name] = po;

  // Give all objects and events a second pass
  for (parsed_object *oto : state.parsed_objects) {
    for (unsigned iit = 0; iit < oto->events.size; iit++)
      parser_secondary(state, oto->events[iit].code, oto->events[iit].synt, oto, &oto->events[iit]);
  }

  // Build an inheritance tree
  for (parsed_object *obj : state.parsed_objects) {
    auto parent_it = po_by_name.find(obj->parent_name);
    if (parent_it != po_by_name.end()) {
      obj->parent = parent_it->second;
      parent_it->second->children.push_back(obj);
      printf("Object %s (%d) is a child of %s (%d)\n", obj->name.c_str(), obj->id, obj->parent->name.c_str(), obj->parent->id);
    }
  }

  // Give all scripts a second pass
  for (size_t i = 0; i < scripts.size(); i++) {
    parser_secondary(state, scripts[i]->pev.code,scripts[i]->pev.synt,&scripts[i]->obj,&scripts[i]->pev);
    if (scripts[i]->pev_global)
      parser_secondary(state, scripts[i]->pev_global->code,scripts[i]->pev_global->synt,&scripts[i]->obj,scripts[i]->pev_global);
  }

  //Give all timelines a second pass
  for (size_t i=0; i < tlines.size(); i++) {
    parser_secondary(state, tlines[i]->pev.code,tlines[i]->pev.synt,&tlines[i]->obj,&tlines[i]->pev);
    if (tlines[i]->pev_global)
      parser_secondary(state, tlines[i]->pev_global->code,tlines[i]->pev_global->synt,&tlines[i]->obj,tlines[i]->pev_global);
  }

  // Give all room creation codes a second pass
  for (auto room : state.parsed_rooms){
    parsed_object *oto; // The object into which we will dump locals
    oto = room; // Start by dumping into this room; rooms are treated like objects, here
    if (room->events.size)
      parser_secondary(state, oto->events[0].code,oto->events[0].synt,oto,&oto->events[0]);
    for (map<int,parsed_room::parsed_icreatecode>::iterator ici = room->instance_create_codes.begin(); ici != room->instance_create_codes.end(); ici++)
    {
      oto = po_by_name[ici->second.object_name];
      parser_secondary(state, ici->second.pe->code,ici->second.pe->synt,oto,ici->second.pe);
    }
    for (map<int,parsed_room::parsed_icreatecode>::iterator ici = room->instance_precreate_codes.begin(); ici != room->instance_precreate_codes.end(); ici++)
    {
      oto = po_by_name[ici->second.object_name];
      parser_secondary(state, ici->second.pe->code,ici->second.pe->synt,oto,ici->second.pe);
    }
  }

  return 0;
}
