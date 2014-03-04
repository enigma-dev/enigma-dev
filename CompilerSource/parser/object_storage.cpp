/**
  @file  object_storage.cpp
  @brief The kind of bland backend to object_storage.h.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <map>
#include <string>
#include <stdio.h>
#include <iostream>

using namespace std;

#include "general/darray.h"

#include <Storage/definition.h>
#include "object_storage.h"
#include "settings-parse/crawler.h"
#include "compiler/compile_common.h"

#include "languages/lang_CPP.h"

parsed_code::parsed_code(jdi::context *ctex, definition_scope *oscp, definition_scope *gscp): code_scope(), ast(ctex, &code_scope, oscp, gscp) {}
parsed_event::parsed_event(jdi::context *ctex, int m, int s, parsed_object* po, definition_scope *global, const char *oc): id(s), main_id(m), origcode(oc), code(ctex, &po->self, global), my_obj(po) {}
parsed_object::parsed_object(string cn, GmObject *gmo): properties(gmo), class_name(cn) { self.name = class_name; }
parsed_object::parsed_object(const parsed_object& other): properties(other.properties), class_name(other.class_name) {}
parsed_script::parsed_script(jdi::context *ctex, Script *scr, definition_scope *global): properties(scr), locals(), code(ctex, &locals, global) {}
parsed_room::parsed_icreatecode::parsed_icreatecode(jdi::context *ctex, parsed_object *obj, definition_scope *gscp): my_obj(obj), code(ctex, &obj->self, gscp) {}
parsed_room::parsed_room(Room *rm): properties(rm), creation_code(NULL) {}

int parsed_script::parse() { return code.parse(properties->code); }
int parsed_event::parse() { return code.parse(origcode); }
int parsed_code::parse(string code) { return ast.parse_edl(code); }
int parsed_room::parsed_icreatecode::parse(string c) { return code.parse(c); }

compile_context::compile_context(EnigmaStruct *es1, error_handler *herr1, compile_mode mode1): es(es1), herr(herr1), mode(mode1), global(new definition_scope("global", NULL, 0)) {}
/// Construct with everything but maps of parsed data.
compile_context::~compile_context() { delete global; }

void scrcall::consolidate(const scrcall &x) {
  arg_max = max(arg_max, x.arg_max);
}

int scope_harvest::copy_scripts_from(const scope_harvest &sh) {
  for (script_map::const_iterator sic = sh.scripts.begin(); sic != sh.scripts.end(); ++sic) {
    pair<script_it, bool> ins = scripts.insert(*sic);
    if (!ins.second) ins.first->second.consolidate(sic->second);
  }
  return 0;
}



int scope_harvest::copy_from(const scope_harvest &sh, string dst, string src) {
  // NEWPARSER: TODO: WRITEME
}

