/**
 * @file  builtins.cpp
 * @brief A small source implementing the \c jdi::builtin context and functions
 *        for manipulating other builtins.
 * 
 * This file is used for configuring the parse environment ahead of time. Contents
 * of the builtin context are inherited by user-created contexts, and the contents
 * of \c builtin_flags determines accepted modifiers when parsing code in general,
 * regardless of the enclosing context.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <map>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include "builtins.h"

namespace jdip {
  tf_map builtin_declarators;
  prim_map builtin_primitives;
  tf_flag_map builtin_decls_byflag;
  typeflag::typeflag(): usage(), def(NULL) {}
  typeflag::typeflag(string n, USAGE_FLAG u): name(n), usage(u), def(NULL) {}
  typeflag::~typeflag() { }
}

using namespace jdip;
namespace jdi {
  jdi::context builtin(0);
  
  void read_declarators(const char* filename, USAGE_FLAG usage_flags)
  {
    string tname; // The name of this type, as it appears in code.
    ifstream in(filename);
    if (!in.is_open())
      return;
    while (!in.eof()) {
      in >> tname;
      add_declarator(tname, usage_flags);
    }
    in.close();
  }
  void add_declarator(string type_name, USAGE_FLAG usage_flags, string prim_name)
  {
    pair<tf_iter, bool> insit = builtin_declarators.insert(pair<string,typeflag*>(type_name,NULL));
    if (insit.second) {
      insit.first->second = new typeflag(type_name, usage_flags);
      if (usage_flags & (UF_PRIMITIVE | UF_STANDALONE)) {
        if (prim_name.empty())
          prim_name = type_name;
        pair<prim_iter, bool> ntit = builtin_primitives.insert(pair<string,definition*>(prim_name,NULL));
        if (ntit.second)
          ntit.first->second = new definition(prim_name, NULL, DEF_TYPENAME);
        insit.first->second->def = ntit.first->second;
      }
      if (usage_flags & UF_FLAG)
      {
        const unsigned long fb = 1 << builtin_decls_byflag.size();
        cout << type_name << "=>" << fb << endl;
        builtin_decls_byflag[fb] = insit.first->second;
        insit.first->second->flagbit = fb;
        if (type_name[0] != '_')
        {
          pair<tf_iter, bool> redit = builtin_declarators.insert(pair<string,typeflag*>("__" + type_name,NULL));
          if (redit.second) redit.first->second = new typeflag(*insit.first->second);
        }
      }
    }
    else
      insit.first->second->usage = USAGE_FLAG(insit.first->second->usage | usage_flags);
  }
  
  void add_gnu_declarators() {
    add_declarator("volatile", UF_FLAG);
    add_declarator("static",   UF_FLAG);
    add_declarator("const",    UF_FLAG);
    add_declarator("register", UF_FLAG);
    add_declarator("inline",   UF_FLAG);
    add_declarator("throw",    UF_FLAG);
    
    add_declarator("unsigned", UF_STANDALONE_FLAG, "int");
    add_declarator("signed",   UF_STANDALONE_FLAG, "int");
    add_declarator("long",     UF_PRIMITIVE_FLAG);
    add_declarator("short",    UF_PRIMITIVE_FLAG);
    
    add_declarator("void",     UF_PRIMITIVE);
    add_declarator("char",     UF_PRIMITIVE);
    add_declarator("int",      UF_PRIMITIVE);
    add_declarator("float",    UF_PRIMITIVE);
    add_declarator("double",   UF_PRIMITIVE);
  }
  
  void cleanup_declarators() {
    for (tf_iter tfit = builtin_declarators.begin(); tfit != builtin_declarators.end(); tfit++)
      delete tfit->second;
    for (prim_iter prit = builtin_primitives.begin(); prit != builtin_primitives.end(); prit++)
      delete prit->second;
  }
}
