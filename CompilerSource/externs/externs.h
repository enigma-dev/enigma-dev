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
#include "../general/darray.h"

enum
{
  EXTFLAG_VALUED =          1 <<  0,
  EXTFLAG_TEMPLATE =        1 <<  1,
  EXTFLAG_TYPENAME =        1 <<  2,
  EXTFLAG_MACRO =           1 <<  3,
  EXTFLAG_CLASS =           1 <<  4,
  EXTFLAG_ENUM =            1 <<  5,
  EXTFLAG_STRUCT =          1 <<  6,
  EXTFLAG_NAMESPACE =       1 <<  7,
  EXTFLAG_TYPEDEF =         1 <<  8,
  EXTFLAG_PENDING_TYPEDEF = 1 <<  9,
  EXTFLAG_DEFAULTED =       1 << 10,
  EXTFLAG_TEMPPARAM =       1 << 11,
  EXTFLAG_EXTERN =          1 << 12,
  EXTFLAG_HYPOTHETICAL =    1 << 13
};
//#define EXTFLAG_NAMESPACE 2048

#include "references.h"

struct externs
{
  unsigned int flags;
  string name;
  
  externs* type;
  externs* parent;
  long long  value_of;
  rf_stack refstack;
  
  varray<externs*> tempargs;
  darray_s<externs*> ancestors;
  map<string, externs*> members;
  
  typedef map<string,externs*>::iterator tempiter;
  
  bool is_function(); //test if this is a function
  int parameter_count_min(); //returns topmost function argument count
  int parameter_count_max(); //returns topmost function argument count
  void parameter_unify(rf_stack&); //Set parameter set to union of this and another
  
  externs();
  externs(string n,externs* p,unsigned int f);
  externs(string n,externs* t,externs* p,unsigned int f);
};

extern int tpc;

struct tpdata
{
  string name;
  externs* def;
  long long val;
  bool standalone;
  bool valdefd;
  
  tpdata();
  tpdata(string n,externs* d);
  tpdata(string n,externs* d, bool sa);
  tpdata(string n,externs* d, long long v, bool sa, bool vd);
};

extern int tpc;
extern varray<tpdata> tmplate_params;

struct ihdata
{
  externs *parent;
  enum heredtypes {
    s_private,
    s_public,
    s_protected
  } scopet;
  
  ihdata();
  ihdata(externs*,heredtypes);
};

extern int ihc;
extern varray<ihdata> inheritance_types;

struct macro_type
{
  int argc; //Argument count, or -1 if not a function.
  string val; //Definiens
  int args_uat; //Index of argument from which there can be infinitely many more
  bool recurse_danger; //True if it is possible for this macro to send the parser on an infinite loop
  varray<string> args; //Names of each argument
  
  operator string();
  macro_type();
  macro_type(string x);
  macro_type &operator= (string x);
  bool check_recurse_danger(string n);
  void assign_func(string n);
  void set_unltd_args(int x);
  void addarg(string x);
};

extern map<string,macro_type> macros;
typedef map<string,externs*>::iterator extiter;
typedef map<string,macro_type>::iterator maciter;
extern externs global_scope,*current_scope,*immediate_scope;

externs* scope_get_using(externs* scope);
externs* temp_get_specializations(externs* scope);
externs* temp_get_specializations_ie(externs* scope);
extiter scope_find_member(string name);
extern externs* ext_retriever_var;
bool find_extname(string name,unsigned int flags,bool expect_find = 1);
bool find_extname_global(string name,unsigned int flags = 0xFFFFFFFF);

#endif
