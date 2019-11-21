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

string fc(const char* fn);

int m_prog_loop_cfp();

#ifdef _WIN32
 #include <windows.h>
 #define dllexport extern "C" __declspec(dllexport)
#else
 #define dllexport extern "C"
 #include <cstdio>
#endif

namespace dll_ext_iteration {
  jdi::definition_scope::defiter rit;
  jdi::definition_scope::defiter uit;
  jdi::definition_scope* searching_in;
  jdi::definition* current_resource;
  string its_name;
}
using namespace dll_ext_iteration;

dllexport const char* next_available_resource();
/// Returns the name of the first resource on the list, or "" otherwise.
dllexport const char* first_available_resource() {
  searching_in = (jdi::definition_scope*) main_context->get_global()->look_up("enigma_user");
  rit = searching_in->members.begin();
  uit = searching_in->using_general.begin();
  return next_available_resource();
}
/// Returns whether the resource can be called as a function
dllexport bool resource_isFunction() {
  return current_language->definition_is_function(current_resource);
}
/// Returns the minimum number of arguments to the function
dllexport int resource_argCountMin() {
  unsigned min, max;
  current_language->definition_parameter_bounds(current_resource, min, max);
  return min;
}
/// Returns the maximum number of arguments to the function
dllexport int resource_argCountMax() {
  unsigned min, max;
  current_language->definition_parameter_bounds(current_resource, min, max);
  return max;
}
/// Returns the number of times the function was declared in the parsed sources
dllexport int resource_overloadCount() {
  return current_language->definition_overload_count(current_resource);
}
/// Returns a simple string of parameters and defaults that would serve as the prototype of this function
/// The returned pointer to the string is INVALIDATED upon the next call to definitionsModified().
dllexport const char* resource_parameters(int i)  {
  static string res;
  res = current_resource->toString();
  return res.c_str();
}
/// Returns whether the resource can be used as a typename.
dllexport int resource_isTypeName() {
  return bool(current_resource->flags & jdi::DEF_TYPENAME);
}
/// Returns whether the resource is nothing but a global variable.
dllexport int resource_isGlobal() {
  return (current_resource->flags & (jdi::DEF_TYPED | jdi::DEF_TYPENAME | jdi::DEF_SCOPE | jdi::DEF_TEMPLATE | jdi::DEF_FUNCTION | jdi::DEF_PRIVATE)) == jdi::DEF_TYPED
     and !resource_isFunction();
}

/// Returns the name of the next resource on the list, or "" otherwise.
dllexport const char* next_available_resource() {
  if (rit == searching_in->members.end()) {
    if (uit == searching_in->using_general.end()) {
      return NULL;
    }
    current_resource = uit->second;
    ++uit;
  } else {
    current_resource = rit->second;
    ++rit;
  }
  
  its_name = current_resource->name;
  return its_name.c_str();
}
/// Returns whether we're really done iterating the list
dllexport bool resources_atEnd() {
  return (rit == searching_in->members.end() && uit == searching_in->using_general.end());
}
