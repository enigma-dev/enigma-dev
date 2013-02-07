/** Copyright (C) 2008-2012 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "frontend.h"
void syntax_error::set(int x, int y, int a, string s)
{
  error_sstring = s;
  err_str = error_sstring.c_str();
  line = x, position = y;
  absolute_index = a;
}

string error_sstring;
syntax_error ide_passback_error;


#include <time.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <map>

using namespace std;
#define flushl '\n' << flush
#define flushs flush

#include "general/darray.h"

#include <API/context.h>
#include "languages/language_adapter.h"
#include "compiler/jdi_utility.h"

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
  jdi::definition_scope::defiter rit;
  jdi::definition_scope* searching_in;
  jdi::definition* current_resource;
  string its_name;
}
using namespace dll_ext_iteration;

dllexport const char* next_available_resource();
dllexport const char* first_available_resource() //Returns the name of the first resource on the list, or "" otherwise.
{
  searching_in = main_context->get_global();
  rit = searching_in->members.begin();
  return next_available_resource();
}
dllexport bool resource_isFunction() //Returns whether the resource can be called as a function
{
  return current_resource->flags & jdi::DEF_FUNCTION;
}
dllexport int resource_argCountMin() //Returns the minimum number of arguments to the function
{
  unsigned min, max;
  definition_parameter_bounds(current_resource, min, max);
  return min;
}
dllexport int resource_argCountMax() //Returns the maximum number of arguments to the function
{
  unsigned min, max;
  definition_parameter_bounds(current_resource, min, max);
  return max;
}
dllexport int resource_overloadCount() //Returns the number of times the function was declared in the parsed sources
{
  return ((jdi::definition_function*)current_resource)->overloads.size() + ((jdi::definition_function*)current_resource)->template_overloads.size();
}
dllexport const char* resource_parameters(int i) //Returns a simple string of parameters and defaults that would serve as the prototype of this function
{ // The returned pointer to the string is INVALIDATED upon the next call to definitionsModified().
  static string res; res = ((jdi::definition_function*)current_resource)->toString();
  return res.c_str();
}
dllexport int resource_isTypeName() //Returns whether the resource can be used as a typename.
{
  return bool(current_resource->flags & jdi::DEF_TYPENAME);
}
dllexport int resource_isGlobal() //Returns whether the resource is nothing but a global variable.
{
  return (current_resource->flags & (jdi::DEF_TYPED | jdi::DEF_TYPENAME | jdi::DEF_SCOPE | jdi::DEF_TEMPLATE | jdi::DEF_FUNCTION | jdi::DEF_PRIVATE)) == jdi::DEF_TYPED
     and !resource_isFunction();
}

dllexport const char* next_available_resource() //Returns the name of the next resource on the list, or "" otherwise.
{
  if (rit == searching_in->members.end())
    return NULL;
  
  current_resource = rit->second;
  its_name = current_resource->name;
  ++rit; return its_name.c_str();
}
dllexport bool resources_atEnd() //Returns whether we're really done iterating the list
{
  return (rit == searching_in->members.end());
}
