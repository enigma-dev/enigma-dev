/**
 * @file  base.cpp
 * @brief The source implementing the top-level function of the parser.
 * 
 * Practically no work is done by this file; it's mostly the front-end, but such
 * is usually the case in a recursive-descent scheme. The job of this file is
 * simply to mark that we're parsing and hand the baton to the scope handler.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <fstream>
#include <API/context.h>
#include <System/lex_cpp.h>
#include <System/token.h>
#include <General/debug_macros.h>
#include "context_parser.h"
using namespace std;
using namespace jdi;

/** @section Implementation
  This function's task is to make a call to check if the parser is already running, then
  instantiate a token class and set a few members. The actual work is done by the next
  call, \c handle_scope(), and then the other members of the derived \c context_parser
  class in \c jdi, which will be called from handle_scope.
*/
int jdi::Context::parse_stream(llreader &cfile) {
  if (parse_open) { // Make sure we're not still parsing anything
    herr->error(cfile) <<
        "Attempted to invoke parser while parse is in progress in another thread";
    return -1;
  }
  
  if (!herr)
    herr = default_error_handler;
  
  int res;
  {
    context_parser cp(this, cfile);
    token_t eoc; // An invalid token to appease the parameter chain.
    res = cp.handle_scope(global.get(), eoc);
    while (eoc.type != TT_ENDOFCODE) {
      #ifdef FATAL_ERRORS
        eoc.report_errorf(herr, "Premature abort caused by %s here; aborting.");
      #else
        eoc.report_errorf(herr, "Premature abort caused by %s here; relaunching");
        while (eoc.type != TT_SEMICOLON && eoc.type != TT_LEFTBRACE && eoc.type != TT_RIGHTBRACE && eoc.type != TT_ENDOFCODE)
          eoc = cp.get_lex()->get_token_in_scope(global.get());
        if (eoc.type == TT_LEFTBRACE) {
          size_t depth = 1;
          while (eoc.type != TT_ENDOFCODE) {
            eoc = cp.get_lex()->get_token_in_scope(global.get());
            if (eoc.type == TT_LEFTBRACE) ++depth;
            else if (eoc.type == TT_RIGHTBRACE) if (!--depth) break;
          }
        }
        cp.handle_scope(global.get(), eoc);
      #endif
    }
  }
  
  return res;
}
