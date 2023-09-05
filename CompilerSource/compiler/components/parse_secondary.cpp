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
    for (auto &pev : oto->all_events)
      parser_secondary(state, &pev);
  }

  // Build an inheritance tree
  for (parsed_object *obj : state.parsed_objects) {
    auto parent_it = po_by_name.find(obj->parent_name);
    if (parent_it != po_by_name.end()) {
      obj->parent = parent_it->second;
      parent_it->second->children.push_back(obj);
      printf("Object %s (%d) is a child of %s (%d)\n", obj->name.c_str(),
             obj->id, obj->parent->name.c_str(), obj->parent->id);
    }
  }

  // Give all scripts a second pass
  for (ParsedScript *scr : scripts) {
    parser_secondary(state, &scr->code);
    if (scr->global_code) parser_secondary(state, scr->global_code);
  }

  // Give all timelines a second pass
  // Apparently they're still individual moments, at this point(?)
  for (ParsedScript *tline : tlines) {
    parser_secondary(state, &tline->code);
    if (tline->global_code) parser_secondary(state, tline->global_code);
  }

  // Give all room creation codes a second pass
  for (auto room : state.parsed_rooms) {
    if (room->creation_code) parser_secondary(state, room->creation_code);
    for (const auto &icc : room->instance_create_codes) {
      parser_secondary(state, icc.second.code);
    }
    for (const auto &icc : room->instance_precreate_codes) {
      parser_secondary(state, icc.second.code);
    }
  }

  return 0;
}
