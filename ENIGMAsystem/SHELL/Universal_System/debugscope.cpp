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

#include "debugscope.h"

#include <vector>

namespace enigma {

  static std::vector<std::string> scope_stack;

  debug_scope::debug_scope(std::string x)
  {
    scope_stack.push_back(x);
  }

  debug_scope::~debug_scope() {
    scope_stack.pop_back();
  }

  std::string debug_scope::GetErrors()
  {
    if (scope_stack.empty())
      return "Debug error stack is empty. Error may stem from engine internals.";

    std::string str = "Stack trace (most recent frame first):\n";
    for (std::vector<std::string>::reverse_iterator it = scope_stack.rbegin(); it != scope_stack.rend(); it++)
      str += "  " + *it + "\n";

    return str;
  }

}
#endif //DEBUG_MODE
