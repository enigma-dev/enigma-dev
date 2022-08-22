/**
 * @file token.cpp
 * @brief Source implementing token type constructors.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2014 Josh Ventura
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
  #define cdebuginit(x,y) ,x(y)
#else
  #define cdebuginit(x,y)
#endif
  
token_t::token_t(): token_basics(type(TT_INVALID), file(""), linenum(), pos()) {}
token_t::token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p)): token_basics(type(t), file(fn), linenum(l), pos(p)) cdebuginit(def,NULL) {}
token_t::token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p), const char* ct, int ctl): token_basics(type(t), file(fn), linenum(l), pos(p)), content(ct, ctl) cdebuginit(def,NULL) {}
token_t::token_t(token_basics(TOKEN_TYPE t, const char* fn, int l, int p), definition* d): token_basics(type(t), file(fn), linenum(l), pos(p)), def(d) {}

void token_t::report_error(error_handler *herr, std::string error) const
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

#include <cstdio>
static struct token_info_c {
  string name[TT_INVALID+1];
  token_info_c() {
    for (int i = 0; i <= TT_INVALID; ++i) {
      char buf[128];
      sprintf(buf, "GLITCH: No name data afilliated with token %d. Oops.", i);
      name[i] = buf;
    }
    TOKEN_TYPE a(TT_INVALID);
    switch (a) {
      default:
      case TT_INVALID: name[TT_INVALID] = "invalid token";
      
      case TT_DECLARATOR:name[TT_DECLARATOR] = "declarator";
      case TT_DECFLAG: name[TT_DECFLAG] = "declarator";
      case TT_CLASS: name[TT_CLASS] = "`class' token";
      case TT_STRUCT: name[TT_STRUCT] = "`struct' token";
      case TT_ENUM: name[TT_ENUM] = "`enum' token";
      case TT_UNION: name[TT_UNION] = "`union' token";
      case TT_NAMESPACE: name[TT_NAMESPACE] = "`namespace' token";
      case TT_EXTERN: name[TT_EXTERN] = "`extern' token";
      
      case TT_ASM: name[TT_ASM] = "`asm' token";
      case TT_OPERATORKW: name[TT_OPERATORKW] = "`operator' token";
      case TT_SIZEOF: name[TT_SIZEOF] = "`sizeof' token";
      case TT_ISEMPTY: name[TT_ISEMPTY] = "`is_empty' token";
      case TT_DECLTYPE: name[TT_DECLTYPE] = "`decltype' token";
      case TT_TYPEID: name[TT_TYPEID] = "`typeid' token";
      
      case TT_ALIGNAS: name[TT_ALIGNAS] = "`alignas' token";
      case TT_ALIGNOF: name[TT_ALIGNOF] = "`alignof' token";
      case TT_AUTO: name[TT_AUTO] = "`autp' token";
      case TT_CONSTEXPR: name[TT_CONSTEXPR] = "`constexpr' token";
      case TT_NOEXCEPT: name[TT_NOEXCEPT] = "`noexcept' token";
      case TT_STATIC_ASSERT: name[TT_STATIC_ASSERT] = "`static_assert' token";
      
      case TT_IDENTIFIER: name[TT_IDENTIFIER] = "identifier";
      case TT_DEFINITION: name[TT_DEFINITION] = "identifier";
      
      case TT_TEMPLATE: name[TT_TEMPLATE] = "`template' token";
      case TT_TYPENAME: name[TT_TYPENAME] = "`typename' token";
      
      case TT_TYPEDEF: name[TT_TYPEDEF] = "`typedef' token";
      case TT_USING: name[TT_USING] = "`using' token";
      
      case TT_PUBLIC: name[TT_PUBLIC] = "`public' token";
      case TT_PRIVATE: name[TT_PRIVATE] = "`private' token";
      case TT_PROTECTED: name[TT_PROTECTED] = "`protected' token";
      case TT_FRIEND: name[TT_FRIEND] = "`friend' token";
      
      case TT_COLON: name[TT_COLON] = "`:' token";
      case TT_SCOPE: name[TT_SCOPE] = "`::' token";
      case TT_MEMBEROF: name[TT_MEMBEROF] = "member specifier (class::*)";
      
      case TT_LEFTPARENTH: name[TT_LEFTPARENTH] = "'(' token";
      case TT_RIGHTPARENTH: name[TT_RIGHTPARENTH] = "')' token";
      case TT_LEFTBRACKET: name[TT_LEFTBRACKET] = "'[' token";
      case TT_RIGHTBRACKET: name[TT_RIGHTBRACKET] = "']' token";
      case TT_LEFTBRACE: name[TT_LEFTBRACE] = "'{' token";
      case TT_RIGHTBRACE: name[TT_RIGHTBRACE] = "'}' token";
      case TT_LESSTHAN: name[TT_LESSTHAN] = "'<' token";
      case TT_GREATERTHAN: name[TT_GREATERTHAN] = "'>' token";
      
      case TT_TILDE: name[TT_TILDE] = "'~' token";
      case TT_ELLIPSIS: name[TT_ELLIPSIS] = "`...' token";
      case TT_OPERATOR: name[TT_OPERATOR] = "`%s' operator";
      
      case TT_COMMA: name[TT_COMMA] = "',' token";
      case TT_SEMICOLON: name[TT_SEMICOLON] = "';' token";
      
      case TT_STRINGLITERAL: name[TT_STRINGLITERAL] = "string literal";
      case TT_CHARLITERAL: name[TT_CHARLITERAL] = "character literal";
      case TT_DECLITERAL: name[TT_DECLITERAL] = "decimal literal";
      case TT_HEXLITERAL: name[TT_HEXLITERAL] = "hexadecimal literal";
      case TT_OCTLITERAL: name[TT_OCTLITERAL] = "octal literal";
      
      case TTM_CONCAT:   name[TTM_CONCAT] = "`##' token";
      case TTM_TOSTRING: name[TTM_TOSTRING] = "`#' token";
      
      case TT_NEW: name[TT_NEW] = "`new' keyword";
      case TT_DELETE: name[TT_DELETE] = "`delete' keyword";
      
      case TT_CONST_CAST: name[TT_CONST_CAST] = "`const_cast' keyword";
      case TT_STATIC_CAST: name[TT_STATIC_CAST] = "`static_cast' keyword";
      case TT_DYNAMIC_CAST: name[TT_DYNAMIC_CAST] = "`dynamic_cast' keyword";
      case TT_REINTERPRET_CAST: name[TT_REINTERPRET_CAST] = "`reinterpret_cast' keyword";
      
      case TT_ENDOFCODE: name[TT_ENDOFCODE] = "end of code";
      #define handle_user_token(tname, desc) case tname: name[tname] = desc;
      #include <User/token_cases.h>
    }
  }
} token_info;

#include <cstdio>
void token_t::report_errorf(error_handler *herr, std::string error) const
{
  string fn; // Default values for non-existing info members
  int l = -1, p = -1;
  
  // Overwrite those which exist
  token_basics(p = -1,
    fn = (const char*)file,
    l = linenum,
    p = pos
  );
  
  size_t f;
  string str = token_info.name[type];
  f = str.find("%s");
  while (f != string::npos) {
    str.replace(f,2,string((const char*)content.str,content.len));
    f = str.find("%s");
  }
  
  f = error.find("%s");
  while (f != string::npos) {
    error.replace(f,2,str);
    f = error.find("%s");
  }
  
  #ifdef DEBUG_MODE
    if (herr)
  #endif
  herr->error(error, fn, l, p);
  #ifdef DEBUG_MODE
    else {
      perror("NULL ERROR HANDLER PASSED TO REPORT METHOD\n");
      perror((error + "\n").c_str());
    }
  #endif
}
void token_t::report_warning(error_handler *herr, std::string error) const
{
  string fn; // Default values for non-existing info members
  int l = -1, p = -1;
  
  // Overwrite those which exist
  token_basics(p = -1,
    fn = (const char*)file,
    l = linenum,
    p = pos
  );
  
  herr->warning(error, fn, l, p);
}
