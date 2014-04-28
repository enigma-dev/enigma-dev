/** Copyright (C) 2014 Josh Ventura, Robert B. Colton
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

#ifdef DEBUG_MODE
#ifndef DEBUG_SCOPE_H
#define DEBUG_SCOPE_H

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace enigma {

extern vector<string> scope_stack;

struct debug_scope { 
  vector<string>::iterator it;
  
  debug_scope(string x) 
  { 
    it = scope_stack.insert(scope_stack.end(), x);
  } 
  
  ~debug_scope() { 
    scope_stack.erase(it);
  } 
   
};

}

#endif
#endif