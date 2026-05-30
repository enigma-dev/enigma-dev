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


#include "parser/parser.h"

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
    const string& pmember = dait->first;
    wto << "  " << dait->second.type << " " << dait->second.prefix << REFERENCE_POSTFIX(dait->second.suffix) << " &varaccess_" << pmember << "(int x)" << endl;
    wto << "  {" << endl;

    wto << "    object_basic *inst = fetch_instance_by_int(x);" << endl;
    wto << "    if (inst) switch (inst->object_index)" << endl << "    {" << endl;

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

    if (global->globals.find(pmember) != global->globals.end())
      wto << "      case global: return " << pmember << ";" << endl;
    else
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
