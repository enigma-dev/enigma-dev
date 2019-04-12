/**
 * @file  full_type.cpp
 * @brief Source implementing functions for moving around a three-component type class.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2013 Josh Ventura
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

#include "full_type.h"
#include <System/builtins.h>
#include <iostream>

using namespace std;

namespace jdi {
  full_type::full_type(): def(NULL), flags(0) {}
  full_type::full_type(jdi::definition* d): def(d), flags(0) {}
  full_type::full_type(jdi::definition* d, int f): def(d), refs(), flags(f) {}
  full_type::full_type(jdi::definition* d, const jdi::ref_stack &r, int f): def(d), refs(r), flags(f) {}
  full_type::full_type(const full_type& ft): def(ft.def), refs(ft.refs), flags(ft.flags) { /* cerr << "DUPLICATED FULLTYPE" << endl; */ }
  
  void full_type::swap(full_type& ft) { 
    jdi::definition* ds = ft.def;
    ft.def = def, def = ds;
    int fs = ft.flags;
    ft.flags = flags, flags = fs;
    refs.swap(ft.refs);
  }
  void full_type::copy(const full_type& ft) {
    def = ft.def;
    flags = ft.flags;
    refs.copy(ft.refs);
  }
  
  bool full_type::operator==(const full_type& other) const {
    return def == other.def && refs == other.refs && flags == other.flags;
  }
  bool full_type::operator!=(const full_type& other) const {
    return def != other.def || refs != other.refs || flags != other.flags;
  }
  bool full_type::operator< (const full_type& other) const {
    ptrdiff_t nd = definition::defcmp(def, other.def);
    if (nd < 0) return true;
    if (nd > 0) return false;
    if (refs < other.refs) return true;
    if (refs > other.refs) return false;
    return flags < other.flags; 
  }
  bool full_type::operator> (const full_type& other) const {
    ptrdiff_t nd = definition::defcmp(def, other.def);
    if (nd > 0) return true;
    if (nd < 0) return false;
    if (refs > other.refs) return true;
    if (refs < other.refs) return false;
    return flags > other.flags; 
  }
  bool full_type::operator<= (const full_type& other) const {
    if (definition::defcmp(other.def, def) < 0) return false;
    if (other.refs < refs) return false;
    return flags <= other.flags; 
  }
  bool full_type::operator>= (const full_type& other) const {
    if (definition::defcmp(other.def, def) > 0) return false;
    if (other.refs > refs) return false;
    return flags >= other.flags; 
  }
  
  string full_type::toString() const {
    string res = typeflags_string(def, flags);
    string app = refs.toString();
    if (app.length()) res += " " + app;
    return res;
  }
  
  string full_type::toEnglish() const {
    return refs.toEnglish() + typeflags_string(def, flags);
  }
  
  full_type &full_type::reduce() {
    definition *type = def; definition_typed *typed;
    if (!type)
      return *this;
    while ((type->flags & (DEF_TYPED | DEF_TYPENAME)) == (DEF_TYPED | DEF_TYPENAME) and ((definition_typed*)type)->type) {
      typed = (definition_typed*)type;
      type = typed->type;
      ref_stack app = typed->referencers;
      refs.append_c(app);
      flags |= typed->modifiers;
    }
    def = type;
    return *this;
  }
  
  bool full_type::synonymous_with(const full_type& x) const {
    full_type a = *this, b = x;
    return a.reduce() == b.reduce();
  }
}
