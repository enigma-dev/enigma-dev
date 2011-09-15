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

#include <string>
#include <cstdlib>
using namespace std;
#include "parse_basics.h"
#include "macro_integration.h"

void macro_push_info::grab(string id, string c, pt p) {
  name = id;
  code = c; pos = p;
}
void macro_push_info::release(string &c, pt &p) {
  c = code; p = pos;
}

bool macro_recurses(string name, macro_stack_t &mymacrostack, unsigned mymacroind) {
  for (unsigned int iii = 0; iii < mymacroind; iii++)
  if (mymacrostack[iii].name == name)
    return 1;
  return 0;
}

pt skip_comments(const string& code, pt cwp) {
  while (is_useless(code[cwp]) or (code[cwp]=='/' and (code[cwp+1]=='/' or code[cwp+1]=='*')))
  {
    if (code[cwp++] == '*') {
      if (code[cwp] == '/') while (cwp < code.length() and  code[cwp] != '\r' and code[cwp]   != '\n') cwp++;
      else      { cwp += 2; while (cwp < code.length() and (code[cwp] != '/'  or  code[cwp-1] != '*')) cwp++; }
    }
  }
  return cwp;
}
