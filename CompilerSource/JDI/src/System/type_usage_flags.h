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
    Various declarators can be used in differing contexts. For instance, primitive types
    such as int and double can stand alone, and can only appear once. By contrast, modifiers
    such as static and const must be coupled with a primitive type, and modifiers such as
    unsigned and long can behave either way. This enumeration represents each of these.
  **/
  enum USAGE_FLAG {
    UF_PRIMITIVE = 1,      ///< The type name is a standard primitive type, such as int or double.
    UF_FLAG = 2,           ///< The type name is a modifier flag, such as volatile or const.
    UF_PRIMITIVE_FLAG = 3, ///< The type name is both a primitive and a modifier flag, such as long or short.
    UF_STANDALONE = 4,     ///< The type name is a modifier flag that can stand alone, such as unsigned and signed. These imply int.
    UF_STANDALONE_FLAG = 6 ///< This is a convenience OR of STANDALONE and FLAG, even though you can not have STANDALONE without FLAG.
  };
}
#endif
