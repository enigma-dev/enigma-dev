/**
 * @file  definition.cpp
 * @brief System source implementing constructor.
 * 
 * This file is likely used by absolutely everything in the parse system, as
 * it is the medium through which output definitions are created and manipulated.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
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

#include "definition.h"

namespace jdi {
  definition *definition::duplicate() {
    return new definition(name, parent, flags);
  }
  definition::definition(string n,definition* p,unsigned int f): flags(f), name(n), parent((definition_scope*)p) {}
  definition::definition(): flags(0), name(), parent(NULL) {}
  definition::~definition() {}
  
  definition_typed::definition_typed(string n, definition* p, definition* tp, unsigned int typeflags, int flgs): definition(n,p,flgs), type(tp), referencers(), modifiers(typeflags) {}
  definition_typed::definition_typed(string n, definition* p, definition* tp, ref_stack &rf, unsigned int typeflags, int flgs): definition(n,p,flgs), type(tp), referencers(rf), modifiers(typeflags) {}
  
  definition *definition_scope::look_up(string sname) {
    defiter it = members.find(sname);
    if (it != members.end())
      return it->second;
    if (parent == NULL)
      return NULL;
    return parent->look_up(sname);
  }
  void definition_scope::copy(const definition_scope* from) {
    for (defiter_c it = from->members.begin(); it != from->members.end(); it++) {
      pair<defiter,bool> dest = members.insert(pair<string,definition*>(it->first,NULL));
      if (dest.second)
        dest.first->second = it->second->duplicate();
    }
  }
  definition* definition_scope::duplicate() {
    definition_scope* res = new definition_scope(name, parent, flags);
    copy(res);
    return res;
  }
  definition_scope::definition_scope(): definition() { }
  definition_scope::definition_scope(const definition_scope&): definition() { }
  definition_scope::definition_scope(string name_, definition *parent_, unsigned int flags_): definition(name_,parent_,flags_) {}
  definition_scope::~definition_scope() {
    for (defiter it = members.begin(); it != members.end(); it++)
      delete it->second;
    members.clear();
  }
  
  definition_class::ancestor::ancestor(unsigned protection_level, definition* inherit_from): protection(protection_level), def(inherit_from) {}
  definition_class::ancestor::ancestor() {}
  definition_class::definition_class(string classname, definition_scope* prnt, unsigned flgs): definition_scope(classname, prnt, flgs) {}
  
  definition_valued::definition_valued(string vname, definition *parnt, definition* tp, unsigned int flgs, value &val): definition_typed(vname, parnt, tp, flgs), value_of(val) {}
  
  definition_enum::definition_enum(string classname, definition_scope* parnt, unsigned flgs): definition_typed(classname, parnt, NULL, 0, flgs) {}
}
