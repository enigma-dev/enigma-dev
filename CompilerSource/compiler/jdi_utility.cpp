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

#include <Storage/definition.h>
#include <languages/lang_CPP.h>

using namespace jdip;

/*
 * Visit a function overload and change minimum argument count and maximum
 * argument count based on the number of arguments in the overload.
 */
static void visit_overload(
    definition_function* d, unsigned &min, unsigned &max, definition *varargs_t) {
  bool variadic = false;
  unsigned int local_min=0,local_max=0;
  
  const ref_stack &refs = ((definition_function*)d)->referencers;
  const ref_stack::parameter_ct& params = ((ref_stack::node_func*)&refs.top())->params;
  for (size_t i = 0; i < params.size(); ++i)
      if (params[i].variadic or params[i].def == varargs_t) variadic = true;
      else if (params[i].default_value) ++local_max; else ++local_min, ++local_max;
  if (variadic) max = -1;
  if (min > local_min) min = local_min;
  if (max < local_max) max = local_max;
}

/*
 * Iterate overloads of a function and change minimum argument count and maximum
 * argument count based on the number of arguments in the overload.
 */
static void iterate_overloads(
    definition_function* d, unsigned &min, unsigned &max, definition *varargs_t) {
  for (map<arg_key, definition_function*>::iterator iter = d->overloads.begin();
       iter != ((definition_function*)d)->overloads.end(); iter++) {
    visit_overload(iter->second, min, max, varargs_t);
  }
}

int lang_CPP::referencers_varargs_at(ref_stack &refs) {
  if (refs.empty() || refs.top().type != ref_stack::RT_FUNCTION)
    return -1;
  ref_stack::parameter_ct &params = ((ref_stack::node_func*)&refs.top())->params;
  for (size_t i = 0; i < params.size(); ++i)
    if (params[i].def == enigma_type__varargs)
      return i;
  return -1;
}

void lang_CPP::definition_parameter_bounds(definition *d, unsigned &min, unsigned &max) {
  min = (unsigned) SIZE_MAX;
  max = 0;
  
  if (!(d->flags & DEF_FUNCTION)) {
    if (d->flags & DEF_TEMPLATE) {
      definition_template *dt = (definition_template*) d;
      if (dt->def && (dt->def->flags & DEF_FUNCTION)) {
        iterate_overloads((definition_function*) dt->def, min, max, enigma_type__varargs);
        for (definition_template::specmap::iterator it = dt->specializations.begin();
             it != dt->specializations.end(); ++it) {
          definition_template *spec = it->second;
          if (spec->def && spec->def->flags & DEF_FUNCTION)
            iterate_overloads((definition_function*) spec->def, min, max, enigma_type__varargs);
        }
        return;
      }
    }
    cout << "Attempt to use " << d->toString() << " as function" << endl;
    return;
  }
  
  iterate_overloads((definition_function*) d, min, max, enigma_type__varargs);
}

bool lang_CPP::definition_is_function(definition *d) {
  if (d->flags & DEF_FUNCTION) return true;
  if (d->flags & DEF_TEMPLATE) {
    definition_template *dt = (definition_template*) d;
    if (dt->def && (dt->def->flags & DEF_FUNCTION)) return true;
  }
  return false;
}

size_t lang_CPP::definition_overload_count(jdi::definition *d) {
  if (!(d->flags & DEF_FUNCTION)) {
    if (d->flags & DEF_TEMPLATE) {
      definition_template *dt = (definition_template*) d;
      if (dt->def && (dt->def->flags & DEF_FUNCTION)) {
        size_t res = definition_overload_count(dt->def);
        for (definition_template::specmap::iterator it = dt->specializations.begin();
             it != dt->specializations.end(); ++it) {
          res += definition_overload_count(it->second);
        }
        return res;
      }
    }
    return 0;
  }
  definition_function *df = (definition_function*) d;
  return df->overloads.size() + df->template_overloads.size();
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


void lang_CPP::quickmember_variable(jdi::definition_scope* scope, jdi::definition* type, string name) {
  scope->members[name] = new jdi::definition_typed(name,scope,type,0);
}
void lang_CPP::quickmember_script(jdi::definition_scope* scope, string name) {
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
