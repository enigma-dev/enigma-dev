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

#include <time.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "../general/darray.h"

#include "externs.h"

string fc(const char* fn);

int m_prog_loop_cfp();

#include <sys/time.h>

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

namespace dll_ext_iteration {
  extiter it;
  externs*gus;
  string  its_name;
}
using namespace dll_ext_iteration;

dllexport const char* first_available_resource() //Returns the name of the first resource on the list, or "" otherwise.
{
  gus = scope_get_using_ie(&global_scope);
  if (!gus)
    gus = &global_scope;
  
  it = gus->members.begin();
  if (it == gus->members.end())
  {
    if (gus == &global_scope)
      return NULL;
    gus = &global_scope;
    it = gus->members.begin();
    if (it == gus->members.end())
      return NULL;
  }
  
  its_name = it->second->name;
  return its_name.c_str();
}
dllexport bool resource_isFunction() //Returns whether the resource can be called as a function
{
  return it->second->is_function();
}
dllexport int resource_argCountMin() //Returns the minimum number of arguments to the function
{
  return it->second->parameter_count_min();
}
dllexport int resource_argCountMax() //Returns the maximum number of arguments to the function
{
  return it->second->parameter_count_max();
}
dllexport int resource_overloadCount() //Returns the number of times the function was declared in the parsed sources
{
  return it->second->sparams.size;
}
dllexport const char* resource_parameters(int i) //Returns a simple string of parameters and defaults that would serve as the prototype of this function
{ // The returned pointer to the string is INVALIDATED upon the next call to definitionsModified().
  return it->second->sparams[i].c_str();
}
dllexport int resource_isTypeName() //Returns whether the resource can be used as a typename.
{
  return bool(it->second->flags & EXTFLAG_TYPENAME);
}
dllexport int resource_isGlobal()   //Returns whether the resource is nothing but a global variable.
{
  return bool(!it->second->flags and !it->second->is_function());
}

dllexport const char* next_available_resource() //Returns the name of the next resource on the list, or "" otherwise.
{
  it++;
  if (it == gus->members.end())
  {
    if (gus == &global_scope)
      return NULL;
    gus = &global_scope;
    it = gus->members.begin();
    if (it == gus->members.end())
      return NULL;
  }
  
  its_name = it->second->name;
  return its_name.c_str();
}
dllexport bool resources_atEnd() //Returns whether we're really done iterating the list
{
  return ((it == gus->members.end()) and (gus == &global_scope));
}
