/**
 * @file  parse_context.h
 * @brief A file defining variables needed by the parser as it runs--Not to be
 *        confused with context.h!
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

#ifndef _PARSE_CONTEXT__H
#define _PARSE_CONTEXT__H

#include <General/llreader.h>
#include <Storage/definition.h>
#include <API/error_reporting.h>

namespace jdip {
  using namespace jdi;
  
  /**
    @struct jdip::parse_context
    @brief  A structure storing info needed by the parser as it runs.
    
    This structure contains any information which cannot be discerned based on
    a position in a handler function. For instance, this file contains information
    regarding what source file is currently being read and which source file it came from.
  **/
  struct parse_context {
    definition* global; ///< The global scope in this context
    bool active; ///< True if a parse is still in action (implying a second thread exists).
    error_handler *herr; ///< The \c error_handler which will receive warnings and errors.
    
    /// Default constructor, taking an optional pointer to an error handler.
    parse_context(error_handler *h_err = def_error_handler);
  };
}

#endif
