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

#include "settings.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "backend/GameData.h"
#include "compiler/compile_common.h"
#include "event_reader/event_parser.h"
#include "parser/object_storage.h"

string REFERENCE_POSTFIX(string ref) {
  unsigned pos, spos = 0;
  bool makeItConst = false;
  int lvl = 0;
  for (pos = 0; pos < ref.length(); pos++)
  {
    if (ref[pos] == '[')
    {
      if (!lvl) spos = pos;
      makeItConst = true;
      lvl++;
    }
    else if (ref[pos] == ']')
    {
      if (--lvl == 0)
        ref.replace(spos,pos-spos+1,"*");
    }
  }
  if (makeItConst)
    ref += " const ";
  return ref;
}
#include "languages/lang_CPP.h"

struct usedtype { int uc; dectrip original; usedtype(): uc(0) {} }; // uc is the use count, then after polling, the dummy number.
int lang_CPP::compile_writeObjAccess(const ParsedObjectVec &parsed_objects, const DotLocalMap &dot_accessed_locals, const ParsedScope *global, bool treatUninitAs0)
{
  ofstream wto;
  wto.open((codegen_directory/"Preprocessor_Environment_Editable/IDE_EDIT_objectaccess.h").u8string().c_str(),ios_base::out);
  wto << license;
  wto << "// Depending on how many times your game accesses variables via OBJECT.varname, this file may be empty." << endl << endl;
  wto << "namespace enigma" << endl << "{" << endl;

  wto <<
  "  object_locals ldummy;" << endl <<
  "  object_locals *glaccess(int x)" << endl <<
  "  {" << endl << "    object_locals* ri = (object_locals*)fetch_instance_by_int(x);" << endl << "    return ri ? ri : &ldummy;" << endl << "  }" << endl << endl;

  wto <<
  "  var &map_var(std::map<string, var> **vmap, string str)" << endl <<
  "  {" << endl <<
  "      if (*vmap == NULL)" << endl <<
  "        *vmap = new std::map<string, var>();" << endl <<
  "      if ((*vmap)->find(str) == (*vmap)->end())" << endl <<
  "        (*vmap)->insert(std::pair<string, var>(str, 0));" << endl <<
  "      return ((*vmap)->find(str))->second;" << endl <<
  "  }" << endl << endl;


  map<string,usedtype> usedtypes;
  for (auto dait = dot_accessed_locals.begin(); dait != dot_accessed_locals.end(); dait++) {
    usedtype &ut = usedtypes[dait->second.type + " " + dait->second.prefix + dait->second.suffix];
    if (!ut.uc) ut.original = dait->second;
    ut.uc++;
  }
  int dummynumber = 0;
  for (map<string,usedtype>::iterator i = usedtypes.begin(); i != usedtypes.end(); i++)
  {
    int uc = i->second.uc;
    i->second.uc = dummynumber++;
    wto << "  " << i->second.original.type << " " << i->second.original.prefix << "dummy_" << i->second.uc << i->second.original.suffix << "; // Referenced by " << uc << " accessors" << endl;
  }

  for (auto dait = dot_accessed_locals.begin(); dait != dot_accessed_locals.end(); dait++) {
    // Note: __VA_ARGS__ is a preprocessor macro, but if user code incorrectly uses it,
    // it will be in object->locals and renamed to __va_args_var__ in write_object_data.cpp.
    // If it's only in dot_accessed_locals (not in object->locals), skip it.
    // But if it's in object->locals, it will be declared there, so we don't need varaccess here.
    // For now, skip __VA_ARGS__ in dot_accessed_locals - if it's actually used, it will be in object->locals.
    if (dait->first == "__VA_ARGS__") {
      continue;
    }
    
    // Skip built-in instance variables (discovered by clang parser) - they're accessible as member variables
    // These should not have varaccess functions generated since they're part of the object hierarchy
    if (this->is_shared_local(dait->first)) {
      continue;
    }
    
    // Also skip if it's a built-in constant from enigma_user namespace (like self, c_blue, c_white, etc.)
    // Use is_enigma_user_constant() which directly checks enigma_user namespace
    if (this->is_enigma_user_constant(dait->first)) {
      // It's a built-in constant in enigma_user namespace, don't generate varaccess function for it
      continue;
    }
    
    // Use the variable name as-is (no rename needed since we filtered __VA_ARGS__ above)
    string pmember = dait->first;
    wto << "  " << dait->second.type << " " << dait->second.prefix << REFERENCE_POSTFIX(dait->second.suffix) << " &varaccess_" << pmember << "(int x)" << endl;
    wto << "  {" << endl;

    wto << "    object_basic *inst = fetch_instance_by_int(x);" << endl;
    wto << "    if (inst) switch (inst->object_index)" << endl << "    {" << endl;

    // Only generate object-specific cases if the variable is actually declared in that object's locals
    // Variables in dot_accessed_locals should only be accessed via ENIGMA_global_instance (case global:)
    // Only generate object-specific cases if the variable is actually declared in that object's locals
    // AND it's not in dot_accessed_locals (variables in dot_accessed_locals are global, not object members)
    // Note: We check object->locals, but if the variable is in dot_accessed_locals, it won't be declared
    // in the object struct (we filter it in write_object_data.cpp), so we shouldn't generate these cases.
    // However, we still check object->locals here because the parser may have added it, but we filter it
    // during declaration. So we need to check if it would actually be declared (not filtered).
    // For now, we'll generate the cases - if the variable isn't declared, the compiler will error,
    // but that's better than generating incorrect code. Actually, let's be smarter:
    // If a variable is in dot_accessed_locals, it's a global variable and should only be accessed via
    // ENIGMA_global_instance, not as object members. So skip object-specific cases for dot_accessed_locals.
    bool is_dot_accessed = (dot_accessed_locals.find(pmember) != dot_accessed_locals.end());
    
    if (!is_dot_accessed) {
      // Only generate object-specific cases if variable is NOT in dot_accessed_locals
      // (variables in dot_accessed_locals are global, accessed via ENIGMA_global_instance)
      for (parsed_object *const obj : parsed_objects) {
        for (parsed_object *parent = obj; parent;) {
          map<string,dectrip>::iterator x = parent->locals.find(pmember);
          if (x != parent->locals.end())
          {
            string tot = x->second.type != "" ? x->second.type : "var";
            if (tot == dait->second.type and x->second.prefix == dait->second.prefix and x->second.suffix == dait->second.suffix)
            {
              wto << "      case " << obj->name << ": return ((OBJ_" << obj->name << "*)inst)->" << pmember << ";" << endl;
              break;
            }
          }
          parent = parent->parent;
        }
      }
    }

    // Check if variable is declared in any object (in object->locals but not in object->globals).
    // Only generate case global: for ENIGMA_global_structure if variable is not declared in objects.
    bool declared_in_any_object = false;
    for (parsed_object *const obj : parsed_objects) {
      bool in_locals = (obj->locals.find(pmember) != obj->locals.end());
      bool in_globals = (obj->globals.find(pmember) != obj->globals.end());
      if (in_locals && !in_globals) {
        declared_in_any_object = true;
        break;
      }
    }
    
    // Generate case global: for standalone globals or variables in ENIGMA_global_structure.
    if (global->globals.find(pmember) != global->globals.end() && 
        dot_accessed_locals.find(pmember) == dot_accessed_locals.end())
      wto << "      case global: return " << pmember << ";" << endl;
    else if (!declared_in_any_object)
      wto << "      case global: return ((ENIGMA_global_structure*)ENIGMA_global_instance)->" << pmember << ";" << endl;
    if (dait->second.type == "var")
      wto << "      default: return map_var(&(((enigma::object_locals*)inst)->vmap), \"" << pmember << "\");"  << endl;
    wto << "    }" << endl;
    if (treatUninitAs0) { //Can't keep re-using the same dummy variable.
      wto << "    dummy_" <<(usedtypes[dait->second.type + " " + dait->second.prefix + dait->second.suffix].uc) <<" = var();" << endl;
    }
    wto << "    return dummy_" << usedtypes[dait->second.type + " " + dait->second.prefix + dait->second.suffix].uc << ";" << endl;
    wto << "  }" << endl;
  }
  wto << "} // namespace enigma" << endl;
  wto.close();
  return 0;
}
