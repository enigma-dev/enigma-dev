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

#ifndef _INCLUDED_EXTERNS
#define _INCLUDED_EXTERNS 1

#include <map>

#define EXTFLAG_TEMPLATE 2
#define EXTFLAG_TYPENAME 4
#define EXTFLAG_MACRO 8
#define EXTFLAG_CLASS 16
#define EXTFLAG_ENUM 32
#define EXTFLAG_STRUCT 64
#define EXTFLAG_NAMESPACE 128
#define EXTFLAG_TYPEDEF 256
#define EXTFLAG_PENDING_TYPEDEF 512
//#define EXTFLAG_NAMESPACE 1024

#include "references.h"

struct externs;

struct tpdata
{
  string name;
  externs* def;
  tpdata(): name(""), def(NULL) {}
  tpdata(string n,externs* d): name(n), def(d) {}
};

struct externs
{
  unsigned int flags;
  string name;
  
  externs* type;
  externs* parent;
  rf_stack refstack;
  
  darray<externs*> ancestors;
  map<string,externs*> tempargs;
  map<string, externs*> members;
  
  typedef map<string,externs*>::iterator tempiter;
  
  bool is_function(); //test if this is a function
  int parameter_count(); //returns topmost function argument count
  
  externs();
  externs(string n,externs* p,unsigned int f);
  externs(string n,externs* t,externs* p,unsigned int f);
};

struct macro_type
{
  int argc;
  string val;
  bool recurse_danger;
  varray<string> args;
  operator string();
  macro_type();
  macro_type(string x);
  macro_type &operator= (string x);
  bool check_recurse_danger(string n);
  void assign_func(string n);
  void addarg(string x);
};

extern map<string,macro_type> macros;
typedef map<string,externs*>::iterator extiter;
typedef map<string,macro_type>::iterator maciter;
extern externs global_scope,*current_scope,using_scope;

extiter scope_find_member(string name);
extern externs* ext_retriever_var;
bool find_extname(string name,unsigned int flags);

#endif
