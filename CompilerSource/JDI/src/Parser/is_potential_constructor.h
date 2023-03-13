/**
 * @file is_potential_constructor.h
 * @brief Header with two inlined functions for checking if a name can be used as a constructor.
 *
 * This header only exists to prevent nontrivial duplicate code.
 * The code is only "nontrivial" because it has to consider templates; it's still pretty simple.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2014 Josh Ventura
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

#ifndef _IS_POTENTIAL_CONSTRUCTOR__H
#define _IS_POTENTIAL_CONSTRUCTOR__H

#include <string>
#include <Storage/definition.h>
#include <System/builtins.h>

#define constructor_name "(construct)"
inline bool ipc(jdi::definition_scope *scope, std::string dname) {
  if (scope->flags & (jdi::DEF_CLASS | jdi::DEF_UNION)) {
    if (dname == scope->name)
      return true;
    
    jdi::definition_class *sc = (jdi::definition_class*)scope;
    if (sc->instance_of && sc->instance_of->name == dname)
      return true;
  }
  return dname == constructor_name;
}
inline bool is_potential_constructor(jdi::definition_scope *scope, std::string dname) {
  return ipc(scope, dname) || ((scope->flags & jdi::DEF_TEMPLATE) && ipc(scope->parent, dname));
}
inline bool is_potential_constructor(jdi::definition_scope *scope, const jdi::full_type &tp) {
  const bool potentialc = is_potential_constructor(scope, tp.def->name);
  #define invalid_ctor_flags ~(jdi::builtin_flag__explicit | jdi::builtin_flag__virtual) // Virtual's a bit of a longshot, but we'll take it.
  return (potentialc and !(tp.flags & invalid_ctor_flags) and tp.refs.size() == 1 and tp.refs.top().type == jdi::ref_stack::RT_FUNCTION);
}

#endif
