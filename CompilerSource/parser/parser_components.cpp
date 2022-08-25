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


#include <string>
#include <iostream>
#include <cstdio>
using namespace std;
#include "darray.h"

#include "general/parse_basics_old.h"
#include "compiler/output_locals.h"
#include "languages/language_adapter.h"

#include "settings.h"


typedef size_t pt; //Use size_t as our pos type; helps on 64bit systems.
map<string,char> edl_tokens; // Logarithmic lookup, with token.
typedef map<string,char>::iterator tokiter;

int scope_braceid = 0;
extern string tostring(int);

#include <Storage/definition.h>
static jdi::definition_scope *current_scope;

int dropscope()
{
  if (current_scope != main_context->get_global())
  current_scope = current_scope->parent;
  return 0;
}
int quickscope()
{
  jdi::definition_scope* ns = new jdi::definition_scope("{}",current_scope,jdi::DEF_NAMESPACE);
  current_scope->members["{}"+tostring(scope_braceid++)].reset(ns);
  current_scope = ns;
  return 0;
}
int initscope(string name)
{
  scope_braceid = 0;
  main_context->get_global()->members[name] = std::make_unique<jdi::definition_scope>(name,main_context->get_global(),jdi::DEF_NAMESPACE);
  current_scope = reinterpret_cast<jdi::definition_scope*>(main_context->get_global()->members[name].get());
  return 0;
}
int quicktype(unsigned flags, string name)
{
  current_scope->members[name] = std::make_unique<jdi::definition>(name,current_scope,flags | jdi::DEF_TYPENAME);
  return 0;
}
