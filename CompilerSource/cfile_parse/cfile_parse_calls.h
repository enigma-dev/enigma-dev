/*********************************************************************************\
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
\*********************************************************************************/


//Some things that only cause clutter when left in the same file

#include "../general/parse_basics.h"

bool is_tflag(string x)
{
  return 
     x=="unsigned" 
  or x=="signed" 
  or x=="register" 
  or x=="extern" 
  or x=="short" 
  or x=="long" 
  or x=="static" 
  or x=="mutable" 
  or x=="volatile";
}



inline externs* regt(string x)
{
  externs* t = current_scope->members[x] = new externs;
  t->flags = EXTFLAG_TYPENAME;
  t->name = x;
  return t;
}
inline void regmacro(string m)
{
  macros[m];
}
inline void regmacro(string m,string val)
{
  macros[m] = val;
}
inline void regmacro(string m,string val,string arg1)
{
  macro_type *mac = &macros[m];
  *mac = val;
  mac->assign_func();
  mac->addarg(arg1);
}

externs *builtin_type__int;
varray<string> include_directories;
unsigned int include_directory_count;

void cparse_init()
{
  current_scope = &global_scope;
  
  regt("bool");
  regt("char");
  builtin_type__int = regt("int");
  regt("float");
  regt("double");
  
  regt("void"); //this was only after careful consideration
  
  regt("long");
  regt("short");
  regt("signed");
  regt("unsigned");
  regt("const");
  regt("static");
  regt("volatile");
  regt("register");
  regt("auto");
  
  //lesser used types
  //regt("size_t"); //size_t doesn't need registered here as it is typdef'd in stdio somewhere.
  regt("wchar_t");
  
  //__builtin_ grabbage
  regt("__builtin_va_list");
  
  #undef regt
  
  //These are GCC things and must be hard coded in
  regmacro("__attribute__","","x"); //__attribute__(x) 
  regmacro("__extension__"); //__extension__
  
  #ifdef linux
  include_directories[0] = "/usr/include/";
  include_directories[1] = "/usr/lib/gcc/i486-linux-gnu/4.3.3/include/";
  #else
  include_directories[0] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\";
  include_directories[1] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\lib\\gcc\\mingw32\\3.4.5\\install-tools\\include\\";
  #endif
  
  include_directory_count = 2;
}


int tpc;
varray<tpdata> tmplate_params;

int anoncount = 0;

bool ExtRegister(unsigned int last,string name,rf_stack refs,externs *type = NULL,varray<tpdata> &tparams = tmplate_params, int tpc = 0)
{
  unsigned int is_tdef = last & LN_TYPEDEF;
  last &= ~LN_TYPEDEF;
  
  if (name != "")
  {
    extiter it = current_scope->members.find(name);
    
    //cout << "  Receiving " << (refs.empty()?"empty":"unempty") << " reference stack\r\n";
    
    if (it != current_scope->members.end())
    {
      if (last == LN_NAMESPACE and (it->second->flags & EXTFLAG_NAMESPACE))
      {
        ext_retriever_var = it->second;
        return 1;
      }
      else
      {
        if (it->second->is_function())
        {
          ext_retriever_var = it->second;
          return 1;
        }
        else
        {
          cferr = "Redeclaration of `"+name+"' at this point";
          return 0;
        }
      }
    }
  }
  else
  {
    if (last != LN_NAMESPACE)
      name = "<anonymous"+tostring(anoncount++)+">";
    else
    {
      cferr = "Identifier required for namespace definition";
      return 0;
    }
  }
  
  externs* e = new externs;
  current_scope->members[name] = e;
  ext_retriever_var = e;
  e->name = name;
  
  e->flags = 0;
  if (last == LN_CLASS)
    e->flags = EXTFLAG_CLASS | EXTFLAG_TYPENAME;
  else if (last == LN_STRUCT)
    e->flags = EXTFLAG_STRUCT | EXTFLAG_TYPENAME;
  else if (last == LN_UNION) // Same as struct, for our purposes. 
    e->flags = EXTFLAG_STRUCT | EXTFLAG_TYPENAME; //sizeof() can't be used in Macros, you bunch of arses.
  else if (last == LN_ENUM)
    e->flags = EXTFLAG_ENUM | EXTFLAG_TYPENAME;
  else if (last == LN_NAMESPACE)
    e->flags = EXTFLAG_NAMESPACE;
  
  if (is_tdef)
    e->flags |= EXTFLAG_TYPEDEF; //If this is a new type being typedef'd, it will later be undone
  
  if (tpc != 0)
  {
    e->flags |= EXTFLAG_TEMPLATE;
    for (int i=0; i<tpc; i++)
      e->tempargs[tparams[i].name] = tparams[i].def;
  }
  
  e->type = type;
  e->parent = current_scope;
  e->refstack = refs;
  
  return 1;
}
