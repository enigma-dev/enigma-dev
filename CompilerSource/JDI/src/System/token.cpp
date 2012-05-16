/**
 * @file token.cpp
 * @brief Source implementing token type constructors.
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

#include "token.h"
#include <API/context.h>
using namespace jdip;

#ifdef DEBUG_MODE
namespace jdip {
  /// A debug listing of token descriptions by value ID
  const char* TOKEN_TYPE_NAME[TT_INVALID+1] = {
    "TT_DECLARATOR","TT_DECFLAG","TT_CLASS","TT_STRUCT","TT_ENUM","TT_UNION","TT_NAMESPACE","TT_EXTERN",
    "TT_ASM",
    "TT_IDENTIFIER",
    "TT_TEMPLATE","TT_TYPENAME",
    "TT_TYPEDEF","TT_USING",
    "TT_PUBLIC","TT_PRIVATE","TT_PROTECTED",
    "TT_COLON","TT_SCOPE",
    "TT_LEFTPARENTH","TT_RIGHTPARENTH","TT_LEFTBRACKET","TT_RIGHTBRACKET","TT_LEFTBRACE","TT_RIGHTBRACE","TT_LESSTHAN","TT_GREATERTHAN",
    "TT_TILDE","TT_OPERATOR","TT_COMMA","TT_SEMICOLON",
    "TT_STRINGLITERAL","TT_DECLITERAL","TT_HEXLITERAL","TT_OCTLITERAL",
    "TTM_CONCAT", "TTM_TOSTRING",
    "TT_ENDOFCODE","TT_INVALID"
    //,"error"
  };
}
#endif

token_t::token_t(): token_basics(type(TT_INVALID), file(""), linenum(), pos()) {}
token_t::token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p)): token_basics(type(t), file(fn), linenum(l), pos(p)) {}
token_t::token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p), const char* ct, int ctl): token_basics(type(t), file(fn), linenum(l), pos(p)), extra(ct, ctl) {}
token_t::token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p), definition* def): token_basics(type(t), file(fn), linenum(l), pos(p)), extra(def) {}

token_t::extra_::extra_() {}
token_t::extra_::extra_(const char* ct, int ctl) { content.str = ct; content.len = ctl; }
token_t::extra_::extra_(definition* d): def(d) {}

void token_t::report_error(error_handler *herr, std::string error)
{
  string fn; // Default values for non-existing info members
  int l = -1, p = -1;
  
  // Overwrite those which exist
  token_basics(p = -1,
    fn = (const char*)file,
    l = linenum,
    p = pos
  );
  
  herr->error(error, fn, l, p);
}

void token_t::report_warning(error_handler *herr, std::string error)
{
  string fn; // Default values for non-existing info members
  int l = -1, p = -1;
  
  // Overwrite those which exist
  token_basics(p = -1,
    fn = (const char*)file,
    l = linenum,
    p = pos
  );
  
  herr->error(error, fn, l, p);
}
