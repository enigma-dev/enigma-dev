/**
 * @file  definition_forward.h
 * @brief Forward declarations for stuff specific to definition.h.
 * 
 * Include this file if you just need a definition pointer.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _DEFINITION_FORWARD__H
#define _DEFINITION_FORWARD__H

#include <map>
#include <string>
#include <memory>
#include <General/quickreference.h>

namespace jdi {

/** Flags given to a definition to describe its type simply and quickly. **/
enum DEF_FLAGS {
  DEF_TYPENAME =     1 <<  0, ///< This definition can be used as a typename. This does not imply that it has a valid type; see DEF_TYPED.
  DEF_NAMESPACE =    1 <<  1, ///< This definition is a namespace.
  DEF_CLASS =        1 <<  2, ///< This definition is a class or structure. 
  DEF_ENUM =         1 <<  3, ///< This definition is an enumeration of valued constants.
  DEF_UNION =        1 <<  4, ///< This definition is a union of multiple types.
  DEF_SCOPE =        1 <<  5, ///< This definition is a scope of some sort.
  DEF_TYPED =        1 <<  6, ///< This definition contains a type and referencer list. Used with DEF_TYPENAME to mean TYPEDEF.
  DEF_FUNCTION =     1 <<  7, ///< This definition is a function containing a list of zero or more overloads.
  DEF_OVERLOAD =     1 <<  8, ///< This definition is a function overload, containing a type and implementation.
  DEF_VALUED =       1 <<  9, ///< This definition has a default expression attached.
  DEF_EXTERN =       1 << 10, ///< This definition was declared with the "extern" flag.
  DEF_TEMPLATE =     1 << 11, ///< This definition has template parameters attached.
  DEF_TEMPPARAM =    1 << 12, ///< This definition belongs to a list of template parameters, and is therefore abstract.
  DEF_HYPOTHETICAL = 1 << 13, ///< This definition is a purely hypothetical template type, eg, template_param::typename type;
  DEF_DEPENDENT =    1 << 14, ///< This definition is dependent; maybe it's 
  DEF_PRIVATE =      1 << 15, ///< This definition was declared as a private member.
  DEF_PROTECTED =    1 << 16, ///< This definition was declared as a protected member.
  DEF_INCOMPLETE =   1 << 17, ///< This definition was declared but not implemented.
  DEF_ATOMIC =       1 << 18  ///< This is a global definition for objects of a fixed size, such as primitives.
};

struct definition;
struct definition_typed;
struct function_overload;
struct definition_function;
struct definition_valued;
struct definition_scope;
struct definition_class;
struct definition_union;
struct definition_enum;
struct definition_template;
struct definition_tempparam;
struct definition_atomic;
struct definition_hypothetical;

using std::pair;
using std::string;
using std::unique_ptr;

/// Structure returned when inserting declarations into a scope.
struct decpair {
  /// The definition that was there previously,
  /// or that was inserted if this entry did not exist.
  unique_ptr<definition> &def;
  /// True if the given definition was inserted, false otherwise.
  bool inserted;

  decpair(unique_ptr<definition> &def_ptr, bool inserted_):
      def(def_ptr), inserted(inserted_) {}
};

class typeflag;

/// Map type to contain definitions to remap along with the definition with which it will be replaced
typedef std::map<const definition*, definition*> remap_set;
typedef remap_set::const_iterator remap_citer;
typedef remap_set::iterator remap_iter;

std::string flagnames(unsigned flags);

}

#endif
