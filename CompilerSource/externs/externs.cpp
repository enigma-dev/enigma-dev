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
  if (refstack.topmostsymbol() == '(')
    return true;
  if (refstack.last != NULL and refstack.last->prev != NULL and refstack.last->prev->ref.symbol == '(')
    return true;
  return false;
}
int externs::parameter_count()
{
  if (refstack.topmostsymbol() == '(')
    return refstack.topmostcount();
  if (refstack.last != NULL and refstack.last->prev != NULL and refstack.last->prev->ref.symbol == '(')
    return refstack.last->prev->ref.count;
  return 0;
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

macro_type::operator string() { return name; }
macro_type::macro_type(): argc(-1) {}
macro_type::macro_type(string x): argc(-1), name(x) {}
macro_type& macro_type::operator= (string x) { name = x; return *this; }
void macro_type::assign_func() { if (argc==-1) argc=0; }
void macro_type::addarg(string x) { args[argc++] = x; }



//Map to sort, darray for polymorphic things
map<string, varray<externs> > extarray;
externs global_scope,*current_scope;
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


externs* ext_retriever_var = NULL;
bool find_extname(string name,unsigned int flags)
{
  externs* inscope=current_scope;
  externs::tempiter tit = inscope->tempargs.find(name);
  if (tit != inscope->tempargs.end())
  {
    ext_retriever_var = tit->second;
    return 1;
  }
  extiter it = inscope->members.find(name);
  while (it == inscope->members.end()) //Until we find it
  {
    if (inscope==&global_scope) //If we're at global scope, give up
      return 0;
    inscope=inscope->parent; //This must ALWAYS be nonzero when != global_scope
    
    tit = inscope->tempargs.find(name);
    if (tit != inscope->tempargs.end())
    {
      ext_retriever_var = tit->second;
      return 1;
    }
    
    it = inscope->members.find(name);
  } 
  ext_retriever_var = it->second;
  return ((it->second->flags & flags) != 0);
}
