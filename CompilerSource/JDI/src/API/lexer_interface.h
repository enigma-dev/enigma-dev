/**
 * @file lexer_interface.h
 * @brief Header abstracting an interface by which lexers can be thrown around.
 * 
 * This file will be used to implement a lexer for C++ definitions and for
 * preprocessor expressions, as in #if. It will also allow users to pass in
 * their own lexers to stable components such as the expression evaluator.
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

#ifndef LEXER_INTERFACE__H
#define LEXER_INTERFACE__H

namespace jdi {
  struct lexer;
}

#include <System/token.h>
#include <API/error_reporting.h>
#include <Storage/definition.h>

namespace jdi {
  /**
    Base lexer class.
    This class contains a virtual method for reading a token from the stream, and a
    virtual destructor to enable correctly freeing implementing classes with non-POD
    members.
  **/
  struct lexer {
    /** Read in a token from the open stream.
        @param herr  The error handler which will receive any lexing errors.
    **/
    virtual jdip::token_t get_token(error_handler *herr = def_error_handler) = 0;
    jdip::token_t get_token_in_scope(jdi::definition_scope *scope, error_handler *herr = def_error_handler);
    virtual ~lexer(); ///< Destruct and free any non-POD or pointer members.
  };
}

#endif
