/**
 * @file jdi_utility.cpp
 * @brief File implementing utility functions to pluck extra information from
 *        JDI constructs.
 * 
 * This file was written as a helper set in plugging JDI into the old parser.
 * 
 * @section License
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "jdi_utility.h"

using namespace jdip;

definition* enigma_type__var;
definition* enigma_type__variant;
definition* enigma_type__varargs;

int referencers_varargs_at(ref_stack &refs) {
  if (refs.top().type != ref_stack::RT_FUNCTION)
    return -1;
  ref_stack::parameter_ct &params = ((ref_stack::node_func*)&refs.top())->params;
  for (size_t i = 0; i < params.size(); ++i)
    if (params[i].def == enigma_type__varargs)
      return i;
  return -1;
}

/*
 Iterate over function overloads and change minimum argument count and maximum argument count based on the number
 of arguments in the overload.
 */
void iterate_overloads(definition_function* d, unsigned &min, unsigned &max) {
    bool variadic = false;
    int local_min=0,local_max=0;
    
    const ref_stack &refs = ((definition_function*)d)->referencers;
    const ref_stack::parameter_ct& params = ((ref_stack::node_func*)&refs.top())->params;
    for (size_t i = 0; i < params.size(); ++i)
        if (params[i].variadic or params[i].def == enigma_type__varargs) variadic = true;
        else if (params[i].default_value) ++local_max; else ++local_min, ++local_max;
    if (variadic) max = -1;
    if (min > local_min) min = local_min;
    if (max < local_max) max = local_max;
}

void definition_parameter_bounds(definition *d, unsigned &min, unsigned &max) {
  min = max = 0;
  
  if (!(d->flags & DEF_FUNCTION)) { cout << "Attempt to use " << d->toString() << " as function" << endl; return; }
    
  map<arg_key, definition_function*>::iterator iter=((definition_function*)d)->overloads.begin();
  for (; iter!=((definition_function*)d)->overloads.end(); iter++) {
        iterate_overloads(iter->second,min,max);
  }
}


#include "languages/lang_CPP.h"
definition* lang_CPP::find_typename(string n) {
  definition* d = main_context->get_global()->look_up(n);
  if (!d) return NULL;
  if (d->flags & DEF_TYPENAME) return d;
  return NULL;
}

bool lang_CPP::global_exists(string n) {
  definition* d = main_context->get_global()->look_up(n);
  return d;
}


void quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name) {
  scope->members[name] = new jdi::definition_typed(name,scope,type,0);
}
void quickmember_script(jdi::definition_scope* scope, string name) {
  jdi::ref_stack rfs;
  jdi::ref_stack::parameter_ct params;
  for (int i = 0; i < 16; ++i) {
    jdi::ref_stack::parameter p;
    p.def = enigma_type__variant;
    p.default_value = new jdi::AST();
    params.throw_on(p);
  }
  rfs.push_func(params);
  scope->members[name] = new jdi::definition_function(name,enigma_type__var,scope,rfs,0,0);
}
