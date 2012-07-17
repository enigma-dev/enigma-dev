/**
  @file  compile_includes.h
  @brief Contains general-purpose functions for use in compile.cpp.
  
  This file seems like a pile of shit and will likely be removed soon in favor of
  just sticking its few methods in the including source.
  
  @section License
    Copyright (C) 2008-2012 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef _COMPILE_INCLUDES__H
#define _COMPILE_INCLUDES__H

#include <JDI/src/Storage/definition.h>

extern jdi::definition *enigma_type__var;
extern jdi::definition *enigma_type__variant;
extern jdi::definition *enigma_type__varargs;

extern string tostring(int val);

inline void quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name) {
  scope->members[name] = new jdi::definition_typed(name,type,scope,0);
}
inline void quickmember_script(jdi::definition_scope* scope, string name) {
  jdi::ref_stack rfs;
  jdi::ref_stack::parameter_ct params;
  for (int i = 0; i < 16; ++i) {
    jdi::ref_stack::parameter p;
    p.def = enigma_type__variant;
    params.throw_on(p);
  }
  scope->members[name] = new jdi::definition_function(name,enigma_type__var,scope,rfs,0,0);
}

inline string format_error(string code,string err,int pos)
{
  if (pos == -1)
    return err;
  
  int line = 1, lp = 1;
  for (int i=0; i<pos; i++,lp++) {
    if (code[i] =='\n')
      line++, lp = 0, i += code[i+1] == '\n';
    else if (code[i] == '\n') line++, lp = 0;
  }
  return "Line " + tostring(line) + ", position " + tostring(lp) + " (absolute " + tostring(pos) + "): " + err;
}

#endif
