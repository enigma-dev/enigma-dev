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
 * Copyright (C) 2011-2012 Josh Ventura
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
  typeflag::typeflag(): usage(), flagbit(0), def(NULL) {}
  typeflag::typeflag(string n, USAGE_FLAG u): name(n), usage(u), flagbit(0), def(NULL) {}
  typeflag::~typeflag() { }
  
  typeflag* builtin_typeflag__throw;
}

using namespace jdip;
namespace jdi {
  jdi::context *builtin = NULL;
  
  unsigned long builtin_flag__volatile;
  unsigned long builtin_flag__static;
  unsigned long builtin_flag__const;
  unsigned long builtin_flag__mutable;
  unsigned long builtin_flag__register;
  unsigned long builtin_flag__inline;
  unsigned long builtin_flag__Complex;
  
  unsigned long builtin_flag__unsigned;
  unsigned long builtin_flag__signed;
  unsigned long builtin_flag__short;
  unsigned long builtin_flag__long;
  
  unsigned long builtin_flag__restrict;
  
  definition *builtin_type__unsigned;
  definition *builtin_type__signed;
  definition *builtin_type__short;
  definition *builtin_type__long;
  
  definition *builtin_type__void;
  definition *builtin_type__bool;
  definition *builtin_type__char;
  definition *builtin_type__int;
  definition *builtin_type__float;
  definition *builtin_type__double;
  
  definition *builtin_type__wchar_t;
  definition *builtin_type__va_list;
  
  unsigned long builtin_flag__virtual;
  unsigned long builtin_flag__explicit;
  
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
  add_decl_info add_declarator(string type_name, USAGE_FLAG usage_flags, size_t sz, string prim_name)
  {
    unsigned long flag;
    pair<tf_iter, bool> insit = builtin_declarators.insert(pair<string,typeflag*>(type_name,NULL));
    if (insit.second) {
      insit.first->second = new typeflag(type_name, usage_flags);
      if (usage_flags & (UF_PRIMITIVE | UF_STANDALONE)) {
        if (prim_name.empty())
          prim_name = type_name;
        pair<prim_iter, bool> ntit = builtin_primitives.insert(pair<string,definition*>(prim_name,NULL));
        if (ntit.second)
          ntit.first->second = new definition_atomic(prim_name, NULL, DEF_TYPENAME, sz);
        insit.first->second->def = ntit.first->second;
      }
      if (usage_flags & UF_FLAG)
      {
        flag = 1 << builtin_decls_byflag.size();
        builtin_decls_byflag[flag] = insit.first->second;
        insit.first->second->flagbit = flag;
        if (type_name[0] != '_') {
          pair<tf_iter, bool> redit = builtin_declarators.insert(pair<string,typeflag*>("__" + type_name,NULL));
          if (redit.second) redit.first->second = new typeflag(*insit.first->second);
          if (type_name[type_name.length() - 1] != '_') {
            redit = builtin_declarators.insert(pair<string,typeflag*>("__" + type_name + "__",NULL));
            if (redit.second) redit.first->second = new typeflag(*insit.first->second);
          }
        }
        else if (type_name[type_name.length() - 1] != '_') {
          pair<tf_iter, bool> redit = builtin_declarators.insert(pair<string,typeflag*>(type_name + "__",NULL));
          if (redit.second) redit.first->second = new typeflag(*insit.first->second);
        }
      }
      else flag = 0;
    }
    else
      insit.first->second->usage = USAGE_FLAG(insit.first->second->usage | usage_flags), flag = 0;
    return add_decl_info(insit.first->second->def, flag, insit.first->second);
  }
  
  add_decl_info::add_decl_info(definition *d, unsigned long f, typeflag *tf): def(d), flag(f), tf_struct(tf) {}
  
  void add_gnu_declarators() {
    builtin_flag__volatile = add_declarator("volatile", UF_FLAG).flag;
    builtin_flag__static   = add_declarator("static",   UF_FLAG).flag;
    builtin_flag__const    = add_declarator("const",    UF_FLAG).flag;
    builtin_flag__mutable  = add_declarator("mutable",  UF_FLAG).flag;
    builtin_flag__register = add_declarator("register", UF_FLAG).flag;
    builtin_flag__inline   = add_declarator("inline",   UF_FLAG).flag;
    builtin_flag__Complex  = add_declarator("_Complex", UF_FLAG).flag;
    
    builtin_typeflag__throw = add_declarator("throw", UF_FLAG).tf_struct;
    builtin_flag__restrict = add_declarator("__restrict", UF_FLAG).flag;
    
    jdi::add_decl_info
    c = add_declarator("unsigned", UF_STANDALONE_FLAG, 4, "int");
    builtin_type__unsigned = c.def, builtin_flag__unsigned = c.flag;
    c = add_declarator("signed",   UF_STANDALONE_FLAG, 4, "int");
    builtin_type__signed   = c.def, builtin_flag__signed   = c.flag;
    c = add_declarator("short",    UF_PRIMITIVE_FLAG, 4);
    builtin_type__short    = c.def, builtin_flag__short    = c.flag;
    c = add_declarator("long",     UF_PRIMITIVE_FLAG, 4);
    builtin_type__long     = c.def, builtin_flag__long     = c.flag;
    
    builtin_type__void   = add_declarator("void",    UF_PRIMITIVE).def;
    builtin_type__bool   = add_declarator("bool",    UF_PRIMITIVE, 1).def;
    builtin_type__char   = add_declarator("char",    UF_PRIMITIVE, 1).def;
    builtin_type__int    = add_declarator("int",     UF_PRIMITIVE, 4).def;
    builtin_type__float  = add_declarator("float",   UF_PRIMITIVE, 4).def;
    builtin_type__double = add_declarator("double",  UF_PRIMITIVE, 8).def;
    
    builtin_type__wchar_t = add_declarator("wchar_t",     UF_PRIMITIVE, 2).def;
    builtin_type__va_list = add_declarator("__int128",    UF_PRIMITIVE, 8).def; // GCC extension
    builtin_type__va_list = add_declarator("__float128",  UF_PRIMITIVE, 8).def; // GCC extension
    builtin_type__va_list = add_declarator("__builtin_va_list",   UF_PRIMITIVE, 8).def;
    
    builtin_flag__virtual = add_declarator("virtual", UF_FLAG).flag;
    builtin_flag__explicit = add_declarator("explicit", UF_FLAG).flag;
    
    builtin->variadics.insert(builtin_type__va_list);
    builtin->add_macro("JUST_DEFINE_IT_RUN", string());
  }
  
  void cleanup_declarators() {
    for (tf_iter tfit = builtin_declarators.begin(); tfit != builtin_declarators.end(); tfit++)
      delete tfit->second;
    for (prim_iter prit = builtin_primitives.begin(); prit != builtin_primitives.end(); prit++)
      delete prit->second;
  }
  
  static inline string tostr(int x) { char buf[16]; sprintf(buf, "%d", x); return buf; }
  string typeflags_string(definition *type, unsigned flags) {
    string res;
    for (int i = 1; i <= 0x10000; i <<= 1)
      if (flags & i) {
        jdip::tf_flag_map::iterator tfi = builtin_decls_byflag.find(i);
        if (tfi == builtin_decls_byflag.end()) res += "<ERROR:NOSUCHFLAG:" + tostr(i) + "> ";
        else res += tfi->second->name + " ";
      }
    if (type)
      res += type->name;
    else res += "<NULL>";
    return res;
  }
}
