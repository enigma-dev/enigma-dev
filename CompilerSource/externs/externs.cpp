/********************************************************************************\
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
\********************************************************************************/

#include <map>
#include <string>
#include <iostream>
#include "../general/darray.h"
using namespace std;
#include "externs.h"

extern externs* builtin_type__int;

bool externs::is_function()
{
  return refstack.is_function();
}

//This thing was really designed with 32bit architecture in mind
int externs::parameter_count_min()
{
  return refstack.parameter_count_min();
}
int externs::parameter_count_max()
{
  return refstack.parameter_count_max();
}
void externs::parameter_unify(rf_stack& x)
{
  const unsigned short n1 = parameter_count_min(), n2 = x.parameter_count_min();
  const unsigned short x1 = parameter_count_max(), x2 = x.parameter_count_max();
  const int nn = n1<n2?n1:n2, nx = x1>x2?x1:x2;
  refstack.parameter_count_set(nn,nx);
}

externs::externs()
{
  type = NULL;
  parent = NULL;
}
externs::externs(string n,externs* p,unsigned int f)
{
  name = n;
  type = NULL;
  parent = p;
  flags = f;
}
externs::externs(string n,externs* t,externs* p,unsigned int f)
{
  name = n;
  type = NULL;
  parent = p;
  flags = f;
}

macro_type::operator string() { return val; }
macro_type::macro_type(): argc(-1), args_uat(-1), recurse_danger(0) {}
macro_type::macro_type(string x): argc(-1), val(x), args_uat(-1), recurse_danger(0) {}

macro_type &macro_type::operator= (string x) { val = x; return *this; }
void macro_type::assign_func(string n) { if (argc==-1) argc=0; recurse_danger = check_recurse_danger(n); }
void macro_type::set_unltd_args(int x) { args_uat = x; }
void macro_type::addarg(string x) { args[argc++] = x; }

#include "../general/parse_basics.h"
bool macro_type::check_recurse_danger(string n)
{
  register bool dgr = 0;
  for (pt i = 0; i < val.length(); i++)
  {
    if (is_letter(val[i]))
    {
      const pt si = i;
      while (is_letterd(val[i])) i++;
      if (val.substr(si,i-si) == n)
      { dgr = 1; break; }
    }
  }
  return dgr;
}



//Map to sort, darray for polymorphic things
map<string, varray<externs> > extarray;
externs global_scope,*current_scope = &global_scope,*immediate_scope=NULL;
map<string,macro_type> macros;



extiter scope_find_member(string name)
{
  for (externs *s = current_scope; s != NULL; s = s->parent)
  {
    extiter it = s->members.find(name);
    if (it != s->members.end() or s == &global_scope)
      return it;
  }
  return global_scope.members.end();
}

externs* scope_get_using(externs* scope)
{
  extiter u = scope->members.find("<using>");
  if (u != scope->members.end())
    return u->second;
  
  externs* rv = new externs;
  scope->members["<using>"] = rv;
  rv->name = "<using>";
  rv->flags = EXTFLAG_NAMESPACE;
  return rv;
}

externs* temp_get_specializations(externs* scope)
{
  extiter u = scope->members.find("<specializations>");
  if (u != scope->members.end())
    return u->second;
  
  externs* rv = new externs;
  scope->members["<specializations>"] = rv;
  rv->name = "<specializations>";
  rv->flags = EXTFLAG_NAMESPACE;
  return rv;
}
externs* temp_get_specializations_ie(externs* scope)
{
  extiter u = scope->members.find("<specializations>");
  if (u != scope->members.end())
    return u->second;
  return NULL;
}

externs* scope_get_using_ie(externs* scope)
{
  extiter u = scope->members.find("<using>");
  if (u != scope->members.end())
    return u->second;
  return NULL;
}

string strace(externs *f);
externs* ext_retriever_var = NULL;
void print_scope_members(externs*, int);




//Implement TpData's constructors

tpdata::tpdata(): name(""), valdefd(0)
{
  def = new externs;
  def->name = name = "";
  def->flags = EXTFLAG_TEMPPARAM | EXTFLAG_TYPEDEF;
  def->value_of = 0;
  def->type = NULL;
}
tpdata::tpdata(string n,externs* d): name(n), val(0), standalone(false), valdefd(0)
{
  def = new externs;
  def->name = name = n;
  def->flags = EXTFLAG_TYPEDEF | EXTFLAG_TEMPPARAM | EXTFLAG_TYPENAME | (d?EXTFLAG_DEFAULTED:0);
  def->value_of = 0;
  def->type = d;
}
tpdata::tpdata(string n,externs* d, bool sa): name(n), val(0), standalone(sa), valdefd(0)
{
  def = new externs;
  def->name = name = n;
  def->flags = (sa?0:EXTFLAG_TYPEDEF) | EXTFLAG_TEMPPARAM | EXTFLAG_TYPENAME | ((d and !sa)?EXTFLAG_DEFAULTED:0);
  def->value_of = 0;
  def->type = d;
}
tpdata::tpdata(string n,externs* d, long long v, bool sa, bool vd): name(n), standalone(sa), valdefd(vd)
{
  def = new externs;
  def->name = name = n;
  def->flags = (sa?0:EXTFLAG_TYPEDEF) | EXTFLAG_TEMPPARAM | EXTFLAG_TYPENAME | ((d and !sa)?EXTFLAG_DEFAULTED:0) | (vd?EXTFLAG_VALUED:0);
  def->value_of = v;
  def->type = d;
}
int tpc = -1;
varray<tpdata> tmplate_params;


ihdata::ihdata()
{
  parent = NULL;
  scopet = s_public;
}
ihdata::ihdata(externs* p,heredtypes t)
{
  parent = p;
  scopet = t;
}
int ihc;
varray<ihdata> inheritance_types;

bool find_in_parents(externs* whose, string name,unsigned int flags);
bool find_in_using(externs* inscope,string name, unsigned flags);
bool find_in_specializations(externs* inscope,string name, unsigned flags);


bool find_in_parents(externs* whose, string name,unsigned int flags)
{
  if (whose->name == name and whose->flags & flags) {
    ext_retriever_var = whose;
    return 1;
  }
  
  extiter it = whose->members.find(name);
  if (it != whose->members.end())
  {
    if ((it->second->flags & flags) or flags == 0xFFFFFFFF) {
      ext_retriever_var = it->second;
      return 1;
    } //else cout << it->second->flags << " : " << flags << " of " << strace(it->second) << endl << endl;
  }
  
  if (find_in_specializations(whose,name,flags))
    return true;
  
  for (unsigned i=0; i<whose->ancestors.size; i++)
  {
    externs* a = whose->ancestors[i];
    for (unsigned ii = 0; ii < a->ancestors.size; ii++)
      if (find_in_parents(a->ancestors[ii],name,flags))
        return 1;
  }
  return 0;
}

bool find_in_using(externs* inscope,string name, unsigned flags)
{
  externs* using_scope = scope_get_using_ie(inscope);
  if (using_scope != NULL)
  {
    extiter it = using_scope->members.find(name);
    if (it != using_scope->members.end())
      if ((((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF))) {
        ext_retriever_var = it->second;
        return 1;
    }
    for (it = using_scope->members.begin(); it != using_scope->members.end(); it++)
      if (it->second->flags & EXTFLAG_NAMESPACE)
      {
        extiter sit = it->second->members.find(name);
        if (sit != it->second->members.end()
        and  (((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF)))
        {
          ext_retriever_var = sit->second;
          return 1;
        }
      }
  }
  return 0;
}

bool find_in_specializations(externs* inscope,string name, unsigned flags)
{
  externs* spec_scope = temp_get_specializations(inscope);
  if (spec_scope != NULL)
  {
    for (extiter it = spec_scope->members.begin(); it != spec_scope->members.end(); it++)
    {
      extiter sit = it->second->members.find(name);
      if (sit != it->second->members.end()
      and (((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF)))
      {
        ext_retriever_var = sit->second;
        return 1;
      }
    }
  }
  return 0;
}

//This function is used a lot; it traverses scopes, checks 
//template parameters and scope names, and filters by type.

extern string cferr;
  #include "../cfile_parse/cparse_shared.h"
  #include "../cfile_parse/cfile_parse_constants.h"
void print_err_line_at(pt a = pos);
bool find_extname(string name,unsigned int flags,bool expect_find)
{
  //If we've been given a qualified id, check in the path or give up
  if (immediate_scope != NULL)
  {
    externs* iscope = immediate_scope;
    while ((iscope->flags & EXTFLAG_TYPEDEF) and !(iscope->flags & EXTFLAG_HYPOTHETICAL) and iscope->type)
      iscope = iscope->type;
    
    extiter f = iscope->members.find(name);
    
    if (f == iscope->members.end())
    {
      if (iscope->flags & EXTFLAG_HYPOTHETICAL)
      {
        //We know it doesn't exist to our immediate findings; create it.
        ext_retriever_var = new externs;
        ext_retriever_var->name = name;
        ext_retriever_var->type = builtin_type__int;
        ext_retriever_var->flags = EXTFLAG_TYPENAME
          | EXTFLAG_TYPEDEF         // Not a base type, but typedef to NULL.
          | EXTFLAG_HYPOTHETICAL;   // HYPOTHETICAL is viral.
        ext_retriever_var->parent = iscope;
        ext_retriever_var->value_of = 0;
        
        if ((last_named & ~LN_TYPEDEF) == LN_TYPENAME and last_named_phase == TN_TEMPLATE) {
          ext_retriever_var->flags |= EXTFLAG_TEMPLATE;
          ext_retriever_var->tempargs[0] = tpdata().def;
        }
        
        iscope->members[name] = ext_retriever_var;
        immediate_scope = NULL;
        return true;
      }
      
      //Check ancestors
      for (unsigned i = 0; i < iscope->ancestors.size; i++)
        if (find_in_parents(iscope->ancestors[i],name,flags))
          return immediate_scope = NULL, true;
      
      if (find_in_using(iscope,name,flags))
        return immediate_scope = NULL, true;
        
      if (find_in_specializations(iscope,name,flags))
        return immediate_scope = NULL, true;
      
      if (expect_find) {
        cferr = "Unable to locate member `" + name + "' in scope `" + strace(iscope) + "' (" + strace(immediate_scope) + ")";
        print_err_line_at();
      }
      return false;
    }
    ext_retriever_var = f->second;
    
    const bool ret = ((f->second->flags & flags) != 0) or (flags == 0xFFFFFFFF);
    if (ret) immediate_scope = NULL;
    return ret;
  }
  
  //Start looking in this scope
  externs* inscope = current_scope;
  
  //If we're looking for a type name, try the template args
  if (flags & (EXTFLAG_TYPENAME | EXTFLAG_TEMPPARAM))
  {
    //The actual scope we're in should get search precedence, otherwise constructors will flop
    if (inscope->flags & EXTFLAG_TYPENAME)
    { 
      if (inscope->name == name) {
        ext_retriever_var = inscope;
        return true;
      }
      if (inscope->ancestors.size and inscope->ancestors[0]->name == name) //Templates...
      {
        externs* a = temp_get_specializations_ie(inscope->ancestors[0]);
        //If we're in "a__int" (inscope), stored in parent is '__int', and name (our search) is 'a'
        if (a and name.length() < inscope->name.length()) //TODO: find this comment's twin and replace both with a CONSTRUCTOR extflag.
        {
          //Find templated constructor by this name in the parent class
          extiter it = a->members.find(inscope->name.substr(name.length()));
          if (it != a->members.end() and it->second == inscope) {
            ext_retriever_var = inscope;
            return true;
          }
        }
      }
    }
    
    //Check new templates
    for (int ti=0; ti<tpc; ti++)
    {
      if (tmplate_params[ti].name == name)
      {
        ext_retriever_var = tmplate_params[ti].def;
        if (ext_retriever_var == NULL)
          return 0;
        return 1;
      }
    }
    
    //Check scope's own templates
    for (unsigned ti=0; ti<current_scope->tempargs.size; ti++)
    {
      //cout << "if ("<<current_scope->tempargs[ti]->name<<" == "<<name<<") {\n";
      if (current_scope->tempargs[ti]->name == name) {
        ext_retriever_var = current_scope->tempargs[ti];
        return 1;
      }
    }
  }
  
  //Check all scopes here and above.
  extiter it = inscope->members.find(name);
  while (it == inscope->members.end()) //Until we find it
  {
    if (find_in_using(inscope,name,flags))
      return true;
    
    //Check ancestors
    for (unsigned i = 0; i < inscope->ancestors.size; i++)
      if (find_in_parents(inscope->ancestors[i],name,flags))
        return 1;
    
    if (inscope != &global_scope) //If we're not at global scope, move up
      inscope = inscope->parent; //This must ALWAYS be nonzero when != global_scope
    else return 0;
    
    //Check template params of new scope first
    if (flags & EXTFLAG_TYPENAME)
    {
      for (unsigned ti=0; ti<inscope->tempargs.size; ti++)
      {
        if (inscope->tempargs[ti]->name == name) {
          ext_retriever_var = inscope->tempargs[ti];
          return 1;
        }
      }
      
      //The actual scope we're in should then get search precedence, otherwise constructors will flop
      if (inscope->flags & EXTFLAG_TYPENAME and inscope->name == name) {
        ext_retriever_var = inscope;
        return true;
      }
    }
    
    
    //Try to find it as a member of this scope
    it = inscope->members.find(name);
    if (cfile_debug)
    {
      cout << "Found '" << name << "' in " << strace(inscope) << ": " << (it != inscope->members.end()) << endl;
    //tree_scope_members(inscope,1);
    //cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl << endl << endl << endl << endl;
    }
  }
  ext_retriever_var = it->second;
  return ((it->second->flags & flags) != 0 or flags == 0xFFFFFFFF);
  
  return 0;
}


bool find_extname_global(string name,unsigned int flags)
{
  extiter it = global_scope.members.find(name);
  if (it != global_scope.members.end()) {
    ext_retriever_var = it->second;
    return (it->second->flags & flags) or (flags == 0xFFFFFFFF);
  }
  
  externs *using_scope = scope_get_using_ie(&global_scope);
  if (using_scope)
  for (it = using_scope->members.begin(); it != using_scope->members.end(); it++)
    if (it->second->name == name){
      ext_retriever_var = it->second;
      return (it->second->flags & flags) or (flags == 0xFFFFFFFF);
    }
    else if (it->second->flags & EXTFLAG_NAMESPACE)
    {
      extiter sit = it->second->members.find(name);
      if (sit != it->second->members.end()
      and  (((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF))) {
        ext_retriever_var = sit->second;
        return 1;
      }
    }
  
  return 0;
}
