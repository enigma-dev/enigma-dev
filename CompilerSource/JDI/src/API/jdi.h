/**
 * @file  jdi.h
 * @brief Main header for interfacing with JDI.
 * 
 * The sole purpose of this header is to document and include other headers.
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

#ifndef _PARSE_CONFIG__H
#define _PARSE_CONFIG__H

#include <string>

/**
  @namespace jdi
  @brief The main namespace for JDI functions meant for the user.
  
  The jdi namespace contains the most basic functions, such as \c parse_C_stream(),
  as well as important utility functions and classes, such as \c jdi::context.
**/
#include <System/builtins.h>

/**
  @namespace jdip
  @brief A private namespace containing classes meant for use by the system, or
         projects extending the system.
  
  The jdip namespace contains token information and other data that does not concern
  the typical user. Users wishing to create structures that are compatible with JDI
  can use this namespace, but for typical applications, it is recommended that only
  the jdi namespace be used.
  
  Functions in this namespace are to be regarded only as mostly stable; the existence
  of most material in this namespace is not official and not guaranteed.
**/

namespace jdi {
  /** Initialize JustDefineIt */
  void initialize();
  /** Clean up (uninitialize) JustDefineIt */
  void clean_up();
}

#endif
