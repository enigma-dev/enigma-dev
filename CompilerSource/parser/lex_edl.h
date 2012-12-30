/**
 * @file lex_edl.h
 * @brief Header extending the \c lexer base class for parsing C++.
 * 
 * This file defines two subclasses of \c jdi::lexer. The first is meant to lex
 * C++ definitions, and so returns a wide range of token types. It will also do
 * any needed preprocessing, handing the second lexer to \c jdi::AST to handle
 * #if expression evaluation. The second lexer is much simpler, and treats 
 * 
 * @section License
 * 
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

#ifndef _LEX_EDL__H
#define _LEX_EDL__H

#include <System/lex_cpp.h>

/**
  @brief An implementation of \c jdi::lexer for lexing EDL. Handles preprocessing
         seamlessly, returning only relevant tokens.
**/
struct lexer_edl: jdip::lexer_cpp {
  jdip::token_t get_token(jdi::error_handler *herr);
  
  /**
    Utility function designed to handle the preprocessor directive
    pointed to by \c pos upon invoking the function. Note that it should
    be the character directly after the brackets pointed to upon invoking
    the function, not the brackets themselves.
    @param herr  The error handler to use if the preprocessor doesn't
                 exist or is malformed.
  **/
  void handle_preprocessor(jdi::error_handler *herr);
  
  /// Utility function to skip a GML string; invoke with pos indicating the quotation mark: terminates indicating match.
  void skip_gml_string(jdi::error_handler *herr);
  /// Function used by the preprocessor to navigate to terminating double braces.
  string read_preprocessor_args(jdi::error_handler *herr);
  
  lexer_edl(llreader &input, jdi::macro_map &pmacros, const char *fname);
  ~lexer_edl();
};

#endif
