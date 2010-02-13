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

//Each of these flags implies that the type
//named cannot be a struct or the like:
//Picture any of these flags prefixed to string
bool tflag_atomic(string x)
{
  return 
     x=="unsigned" 
  or x=="signed"
  or x=="short" 
  or x=="long";
}



inline externs* regt(string x)
{
  externs* t = current_scope->members[x] = new externs;
  t->flags = EXTFLAG_TYPENAME;
  t->parent = &global_scope;
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
  mac->assign_func(m);
  mac->addarg(arg1);
}

externs *builtin_type__int, *builtin_type__void;
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
  
  builtin_type__void = regt("void"); //this was only after careful consideration
  
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
  regmacro("__typeof__","int","x"); //__typeof__(x) 
  regmacro("__extension__"); //__extension__
  regmacro("false","0"); //false
  regmacro("true","1"); //true
  
  //Cheap hacks
  
  regmacro("__typeof","int","x"); //__typeof(x) 
  regmacro("sizeof","4","x"); //sizeof(x)
  
  #ifdef linux
  include_directories[0] = "/usr/include/";
  include_directories[1] = "/usr/include/c++/4.3/";
  include_directories[2] = "/usr/include/c++/4.3/i486-linux-gnu/";
  include_directories[3] = "/usr/lib/gcc/i486-linux-gnu/4.3.3/include/";
  include_directory_count = 4;
  #else
  include_directories[0] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\";
  include_directories[1] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\lib\\gcc\\mingw32\\3.4.5\\install-tools\\include\\";
  include_directories[2] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\c++\\3.4.5\\";
  include_directories[3] = "C:\\Program Files (x86)\\CodeBlocks\\MinGW\\include\\c++\\3.4.5\\mingw32\\";
  include_directory_count = 4;
  #endif
  
}



int anoncount = 0;
extern void print_err_line_at(unsigned a);
bool ExtRegister(unsigned int last,unsigned phase,string name,rf_stack refs,externs *type = NULL,varray<tpdata> &tparams = tmplate_params, int tpc = -1,long long last_value = 0)
{
  unsigned int is_tdef = last & LN_TYPEDEF;
  last &= ~LN_TYPEDEF;
  
  externs *scope_to_use = immediate_scope ? immediate_scope : current_scope;
  if (name != "")
  {
    extiter it = scope_to_use->members.find(name);
    
    //cout << "  Receiving " << (refs.empty()?"empty":"unempty") << " reference stack\r\n";
    
    if (it != scope_to_use->members.end())
    {
      if (last == LN_NAMESPACE and (it->second->flags & EXTFLAG_NAMESPACE)) {
        ext_retriever_var = it->second;
        return 1;
      }
      else //Potentially redeclaring X, X is not a namespace
      {
        ext_retriever_var = it->second; //Assuming we don't error, this is what we will return
        
        if (it->second->is_function() and refs.is_function())
          it->second->parameter_unify(refs);
        else
        {
          if (!(it->second->flags & (EXTFLAG_CLASS | EXTFLAG_STRUCT)))
            // struct a {}; will declare 'a' at '{' and then again at the first ';' after '}'.
            // This is not because the type is not zeroed at '{', but rather because it is reenabled at '}'.
          {
            if (it->second->parent == current_scope) {
              cferr = "Redeclaration of `"+name+"' at this point";
              return 0;
            }
            else if (!(it->second->parent->flags & EXTFLAG_TEMPLATE)) {
              cferr = "Cannot declare that here.";
              return 0;
            }
            
            //Inheritance
            if (ext_retriever_var->ancestors.size != 0) {
              cferr = "Implementing structure that has already been given inheritance";
              return 0;
            }
          }
          //Inheritance (cont'd)
              for (int i=0; i<ihc; i++)
                ext_retriever_var->ancestors[i] = inheritance_types[i].parent;
              ihc = 0;
          //Template args
            if (tpc > 0)
            {
              if ((unsigned)tpc != ext_retriever_var->tempargs.size) {
                cferr = "Template parameter mismatch in implementation of `" + name + "' as `"+strace(ext_retriever_var)+"': Set from " + tostring(ext_retriever_var->tempargs.size) + " to " + tostring(tpc);
                return 0;
              }
              for (int i=0; i<tpc; i++)
              {
                ext_retriever_var->tempargs[i]->name = tmplate_params[i].name;
                if (tmplate_params[i].def) {                                     // cferr = "Implementing `"+ext_retriever_var->name+"'"; print_err_line_at(pos);
                  ext_retriever_var->tempargs[i]->type = tmplate_params[i].def;
                  ext_retriever_var->tempargs[i]->flags |= EXTFLAG_DEFAULTED;
                }
              }
            }
          tpc = -1;
        }        
        
        //Success
        immediate_scope = NULL;
        return 1;
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
  scope_to_use->members[name] = e;
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
    e->flags = (phase == EN_IDENTIFIER or phase == EN_NOTHING) ? EXTFLAG_ENUM | EXTFLAG_TYPENAME : EXTFLAG_VALUED;
  else if (last == LN_NAMESPACE)
    e->flags = EXTFLAG_NAMESPACE;
  
  if (is_tdef)
    e->flags |= EXTFLAG_PENDING_TYPEDEF; //If this is a new type being typedef'd, it will later be undone
  
  if (tpc > 0)
  {
    e->flags |= EXTFLAG_TEMPLATE;
    for (int i=0; i<tpc; i++)
    {
      if (!tparams[i].standalone)
        tparams[i].def->parent = e;
      else
      {
        if (tparams[i].def == NULL) {
          cferr = "Parse error: Template parameter is marked as definite but defined as NULL";
          return 0;
        }
      }
      e->tempargs[e->tempargs.size] = tparams[i].def;
    }
    tpc = 0;
  }
  
  e->type = type;
  e->parent = scope_to_use;
  e->value_of = last_value;
  e->refstack = refs;
  
  if (ihc)
  {
    if (last != LN_STRUCT and last != LN_CLASS) {
      cferr = "Mishandling of inheritance. This is probably my fault. Please report the concerned code.";
      return 0;
    }
    for (int i = 0; i < ihc; i++) {
      e->ancestors[e->ancestors.size] = inheritance_types[i].parent;
      if (cfile_debug) cout << "Transferring " << inheritance_types[i].parent->name << endl;
    }
    ihc = 0;
  }
  
  immediate_scope = NULL;
  return 1;
}

//#include "expression_evaluator.h"


unsigned int handle_comments()
{
  pos++;
  if (cfile[pos]=='/')
  {
    while (cfile[pos] != '\r' and cfile[pos] != '\n' and (pos++)<len);
    return unsigned(-2);
  }
  if (cfile[pos]=='*')
  {
    int spos=pos;
    pos+=2;
    
    while ((cfile[pos] != '/' or cfile[pos-1] != '*') and (pos++)<len);
    if (pos>=len)
    {
      cferr="Unterminating comment";
      return spos;
    }
    
    pos++;
    return unsigned(-2);
  }
  pos--;
  return unsigned(-1);
}
