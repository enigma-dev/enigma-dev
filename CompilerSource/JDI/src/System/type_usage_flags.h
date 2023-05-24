/* Copyright (C) 2011 Josh Ventura
 * This file is part of JustDefineIt.
 *
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 *
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
*/

/**
  @file type_usage_flags.h
  @brief API header declaring flags for describing declarator usage.
  @see macros.h
**/

#ifndef _TYPE_USAGE_FLAGS__H
#define _TYPE_USAGE_FLAGS__H

namespace jdi {
  /**
    @enum  USAGE_FLAG
    @brief Describes how a declarator is used in code.
    Various declarators can be used in differing contexts. For instance,
    primitive types such as `int` and `double` specify a primitive and can stand
    alone. These can only appear once. By contrast, cv-qualifiers and storage
    specifiers, such as `static` and `const` must be coupled with a primitive
    type, as they do not imply a type on their own. Modifiers such as `unsigned`
    and `long` can behave either way. This enumeration is meant to generalize
    how these are handled during parsing.
  **/
  enum USAGE_FLAG {
    /// The type name is a standard primitive type, such as int or double.
    UF_PRIMITIVE = 1,
    /// The type name is a modifier flag, such as volatile or const.
    UF_FLAG = 2,
    /// OR of PRIMITIVE and FLAG. This modifier flag changes the type signature.
    /// That is, its presence constitutes a different type than the bare type.
    /// This includes long, short, signed and unsigned (note that signed is the
    /// default). Primitive flags, when standing alone, imply `int` as the type.
    UF_PRIMITIVE_FLAG = 3
  };
}
#endif
