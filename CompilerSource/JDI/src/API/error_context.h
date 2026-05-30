/**
 * @file  error_context.h
 * @brief Header declaring a structure for complete error handling, as used in the parser.
 * 
 * @section License
 * 
 * Copyright (C) 2014 Josh Ventura
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

#ifndef _ERROR_CONTEXT__H
#define _ERROR_CONTEXT__H

namespace jdi { class error_context; }

#include "error_reporting.h"
#include "System/token.h"

namespace jdi {
  class error_context {
    error_handler *herr;
    string fname;
    jdip::token_t token;
  public:
    /// Construct from an error_handler and a token
    error_context(error_handler *h, const jdip::token_t &tk): herr(h), token(tk) {}
    /// Construct from an error_handler and line info
    error_context(error_handler *h, string filename, int linenum, int pos): herr(h), fname(filename), token(token_basics(jdip::TT_INVALID, fname.c_str(), linenum, pos)) {}
    /// Report an error
    inline void report_error(string err) const { token.report_error(herr, err); }
    /// Report a warning
    inline void report_warning(string err) const { token.report_warning(herr, err); }
    /// Get a token representation of the line information
    inline error_handler *get_herr()  const { return herr; }
    inline const jdip::token_t &get_token() const { return token; }
  };
}

#endif
