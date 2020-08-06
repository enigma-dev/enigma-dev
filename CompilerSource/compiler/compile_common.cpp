/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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
using namespace std;

#include "parser/object_storage.h"
#include "compile_organization.h"

namespace used_funcs
{
  // TODO(JoshDreamland): This is hideous, and was never implemented.
  // Replace with map of functions of interest
  bool object_set_sprite = 0;
  void zero() {
    object_set_sprite = 0;
  }
}
map<string, ParsedScript*> scr_lookup;

map<string, vector<ParsedScript*> > tline_lookup;

//string event_get_function_name(int mid, int id) // Implemented in event_reader/event_parser.cpp


//Hey, it's that license from above!
const char* license = 
"/********************************************************************************\\\n"
"**                                                                              **\n"
"**  Copyright (C) 2008 Josh Ventura                                             **\n"
"**                                                                              **\n"
"**  This file is a part of the ENIGMA Development Environment.                  **\n"
"**                                                                              **\n"
"**                                                                              **\n"
"**  ENIGMA is free software: you can redistribute it and/or modify it under the **\n"
"**  terms of the GNU General Public License as published by the Free Software   **\n"
"**  Foundation, version 3 of the license or any later version.                  **\n"
"**                                                                              **\n"
"**  This application and its source code is distributed AS-IS, WITHOUT ANY      **\n"
"**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **\n"
"**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **\n"
"**  details.                                                                    **\n"
"**                                                                              **\n"
"**  You should have recieved a copy of the GNU General Public License along     **\n"
"**  with this code. If not, see <http://www.gnu.org/licenses/>                  **\n"
"**                                                                              **\n"
"**  ENIGMA is an environment designed to create games and other programs with a **\n"
"**  high-level, fully compilable language. Developers of ENIGMA or anything     **\n"
"**  associated with ENIGMA are in no way responsible for its users or           **\n"
"**  applications created by its users, or damages caused by the environment     **\n"
"**  or programs made in the environment.                                        **\n"                   
"**                                                                              **\n"
"\\********************************************************************************/\n"
"\n//This file was generated by the ENIGMA Development Environment."
"\n//Editing it is a sign of a certain medical condition. We're not sure which one.\n\n";
