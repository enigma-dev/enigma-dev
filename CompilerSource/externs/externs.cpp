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

#include <map>
#include <string>
#include <iostream>
#include "../general/darray.h"
using namespace std;
#include "externs.h"


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
macro_type::macro_type(): argc(-1), recurse_danger(0) {}
macro_type::macro_type(string x): argc(-1), val(x), recurse_danger(0) {}
macro_type& macro_type::operator= (string x) { val = x; return *this; }
void macro_type::assign_func(string n) { if (argc==-1) argc=0; recurse_danger = check_recurse_danger(n); }
void macro_type::addarg(string x) { args[argc++] = x; }

#include "../general/parse_basics.h"
bool macro_type::check_recurse_danger(string n)
{
  register bool dgr = 0;
  for (unsigned i = 0; i < val.length(); i++)
  {
    if (is_letter(val[i]))
    {
      const unsigned si = i;
      while (is_letterd(val[i])) i++;
      if (val.substr(si,i-si) == n)
      { dgr = 1; break; }
    }
  }
  return dgr;
}



//Map to sort, darray for polymorphic things
map<string, varray<externs> > extarray;
externs global_scope,*current_scope,*immediate_scope=NULL;
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

externs* temp_get_specialization(externs* scope)
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

externs* scope_get_using_ie(externs* scope)
{
  extiter u = scope->members.find("<using>");
  if (u != scope->members.end())
    return u->second;
  return NULL;
}

externs* ext_retriever_var = NULL;
void print_scope_members(externs*, int);
bool find_extname(string name,unsigned int flags)
{
  //If we've been given a qualified id, check in the path or give up
  if (immediate_scope != NULL)
  {
    extiter f = immediate_scope->members.find(name);
      /*cout << "Find " << name << " in " << immediate_scope->name << endl;
      print_scope_members(immediate_scope,4);*/
    if (f == immediate_scope->members.end()) return false;
    ext_retriever_var = f->second;
    immediate_scope = NULL;
    
    return ((f->second->flags & flags) != 0) or (flags == 0xFFFFFFFF);
  }
  
  //Start looking in this scope
  externs* inscope=current_scope;
  
  //If we're looking for a type name, try the template args
  if (flags & EXTFLAG_TYPENAME)
  {
    for (unsigned ti=0; ti<current_scope->tempargs.size; ti++)
    {
      if (current_scope->tempargs[ti]->name == name)
      {
        ext_retriever_var = current_scope->tempargs[ti];
        return 1;
      }
    }
  }
  
  //Check all scopes here and above.
  extiter it = inscope->members.find(name);
  while (it == inscope->members.end()) //Until we find it
  {
    //Wasn't a member. Try the `using' scope.
    externs* using_scope = scope_get_using_ie(inscope);
    if (using_scope != NULL)
    {
      it = using_scope->members.find(name);
      if (it != using_scope->members.end() 
      and (((it->second->flags & flags) != 0) or (flags == 0xFFFFFFFF)))
      {
        ext_retriever_var = it->second;
        return 1;
      }
      for (it = using_scope->members.begin(); it != using_scope->members.end(); it++)
        if (it->second->flags & EXTFLAG_NAMESPACE)
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
    
    if (inscope != &global_scope) //If we're not at global scope, move up
      inscope = inscope->parent; //This must ALWAYS be nonzero when != global_scope
    else return 0;
    
    //Check template params of new scope first
    if (flags & EXTFLAG_TYPENAME)
    {
      for (unsigned ti=0; ti<inscope->tempargs.size; ti++)
      {
        if (inscope->tempargs[ti]->name == name)
          ext_retriever_var = inscope->tempargs[ti];
        return 1;
      }
    }
    
    //Try to find it as a member of this scope
    it = inscope->members.find(name);
  }
  ext_retriever_var = it->second;
  return ((it->second->flags & flags) != 0 or flags == 0xFFFFFFFF);
  
  return 0;
}
