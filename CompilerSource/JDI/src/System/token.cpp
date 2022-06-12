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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 *
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "token.h"
#include <API/context.h>
using namespace jdi;

#ifdef DEBUG_MODE
  #define cdebuginit(x,y) ,x(y)
#else
  #define cdebuginit(x,y)
#endif

#include <cstdio>
static struct token_info_c {
  string description[TT_INVALID+1];
  string spelling[TT_INVALID+1];
  token_info_c() {
    for (int i = 0; i <= TT_INVALID; ++i) {
      char buf[128];
      sprintf(buf, "GLITCH: No name data affiliated with token %d. Oops.", i);
      description[i] = buf;
      spelling[i] = buf;
    }
    TOKEN_TYPE a(TT_INVALID);
#   define TKD(tname, desc)         \
      [[fallthrough]]; case tname:  \
        spelling[tname] = #tname;   \
        description[tname] = desc;
    switch (a) {
      default:
      TKD(TT_INVALID,          "invalid token");

      TKD(TT_DECLARATOR,       "declarator");
      TKD(TT_DECFLAG,          "`%f' declarator");
      TKD(TT_TYPENAME,         "`typename' token");
      TKD(TT_INLINE,           "`inline' token");
      TKD(TT_EXTERN,           "`extern' token");
      TKD(TT_TYPEDEF,          "`typedef' token");
      TKD(TT_AUTO,             "`autp' token");
      TKD(TT_ALIGNAS,          "`alignas' token");
      TKD(TT_ATTRIBUTE,        "`attribute' token");
      TKD(TT_CONSTEXPR,        "`constexpr' token");
      TKD(TT_NOEXCEPT,         "`noexcept' token");

      TKD(TT_CLASS,            "`class' token");
      TKD(TT_STRUCT,           "`struct' token");
      TKD(TT_ENUM,             "`enum' token");
      TKD(TT_UNION,            "`union' token");
      TKD(TT_NAMESPACE,        "`namespace' token");

      TKD(TT_USING,            "`using' token");
      TKD(TT_TEMPLATE,         "`template' token");
      TKD(TT_ASM,              "`asm' token");
      TKD(TT_OPERATORKW,       "`operator' token");

      TKD(TT_SIZEOF,           "`sizeof' token");
      TKD(TT_ALIGNOF,          "`alignof' token");
      TKD(TT_DECLTYPE,         "`decltype' token");
      TKD(TT_TYPEOF,           "`typeof' token");
      TKD(TT_ISEMPTY,          "`is_empty' token");
      TKD(TT_TYPEID,           "`typeid' token");
      TKD(TT_STATIC_ASSERT,    "`static_assert' keyword");

      TKD(TT_IDENTIFIER,       "identifier (\"%s\")");
      TKD(TT_DEFINITION,       "identifier (\"%s\")");

      TKD(TT_PUBLIC,           "`public' token");
      TKD(TT_PRIVATE,          "`private' token");
      TKD(TT_PROTECTED,        "`protected' token");
      TKD(TT_FRIEND,           "`friend' token");

      TKD(TT_PLUS,              "'+' operator");
      TKD(TT_MINUS,             "'-' operator");
      TKD(TT_STAR,              "'*' operator");
      TKD(TT_SLASH,             "'/' operator");
      TKD(TT_MODULO,            "'%' operator");
      TKD(TT_EQUAL_TO,         "`==' operator");
      TKD(TT_NOT_EQUAL_TO,     "`!=' operator");
      TKD(TT_LESS_EQUAL,       "`<=' operator");
      TKD(TT_GREATER_EQUAL,    "`>=' operator");
      TKD(TT_AMPERSAND,         "'&' operator");
      TKD(TT_AMPERSANDS,       "`&&' operator");
      TKD(TT_PIPE,              "'|' operator");
      TKD(TT_PIPES,            "`||' operator");
      TKD(TT_CARET,             "'^' operator");
      TKD(TT_INCREMENT,        "`++' operator");
      TKD(TT_DECREMENT,        "`--' operator");
      TKD(TT_ARROW,            "`->' operator");
      TKD(TT_DOT,               "`.' operator");
      TKD(TT_ARROW_STAR,      "`->*' operator");
      TKD(TT_DOT_STAR,         "`.*' operator");
      TKD(TT_QUESTIONMARK,      "'?' operator");

      TKD(TT_EQUAL,             "`=' operator");

      TKD(TT_ADD_ASSIGN,       "`+=' operator");
      TKD(TT_SUBTRACT_ASSIGN,  "`-=' operator");
      TKD(TT_MULTIPLY_ASSIGN,  "`*=' operator");
      TKD(TT_DIVIDE_ASSIGN,    "`/=' operator");
      TKD(TT_MODULO_ASSIGN,    "`%=' operator");
      TKD(TT_LSHIFT_ASSIGN,   "`<<=' operator");
      TKD(TT_RSHIFT_ASSIGN,   "`>>=' operator");
      TKD(TT_AND_ASSIGN,       "`&=' operator");
      TKD(TT_OR_ASSIGN,        "`|=' operator");
      TKD(TT_XOR_ASSIGN,       "`^=' operator");
      TKD(TT_NEGATE_ASSIGN,    "`~=' operator");

      TKD(TT_LESSTHAN,         "'<' token");
      TKD(TT_GREATERTHAN,      "'>' token");
      TKD(TT_LSHIFT,           "`<<' operator");
      TKD(TT_RSHIFT,           "`>>' operator");

      TKD(TT_COLON,            "`:' token");
      TKD(TT_SCOPE,            "`::' token");
      TKD(TT_MEMBER,           "`::*' token");
      TKD(TT_TILDE,             "'~' token");
      TKD(TT_NOT,               "`!' operator");
      TKD(TT_ELLIPSIS,        "`...' token");
      TKD(TT_COMMA,            "',' token");
      TKD(TT_SEMICOLON,        "';' token");

      TKD(TT_LEFTPARENTH,      "'(' token");
      TKD(TT_RIGHTPARENTH,     "')' token");
      TKD(TT_LEFTBRACKET,      "'[' token");
      TKD(TT_RIGHTBRACKET,     "']' token");
      TKD(TT_LEFTBRACE,        "'{' token");
      TKD(TT_RIGHTBRACE,       "'}' token");

      TKD(TT_STRINGLITERAL,    "string literal %s");
      TKD(TT_CHARLITERAL,      "character literal %s");
      TKD(TT_DECLITERAL,       "decimal literal %s");
      TKD(TT_HEXLITERAL,       "hexadecimal literal %s");
      TKD(TT_OCTLITERAL,       "octal literal %s");
      TKD(TT_BINLITERAL,       "binary literal %s");
      TKD(TT_TRUE,             "`true' keyword");
      TKD(TT_FALSE,            "`false' keyword");

      TKD(TT_NEW,              "`new' keyword");
      TKD(TT_DELETE,           "`delete' keyword");
      TKD(TT_CONST_CAST,       "`const_cast' keyword");
      TKD(TT_STATIC_CAST,      "`static_cast' keyword");
      TKD(TT_DYNAMIC_CAST,     "`dynamic_cast' keyword");
      TKD(TT_REINTERPRET_CAST, "`reinterpret_cast' keyword");
      TKD(TT_EXTENSION,        "`extension' token");

      TKD(TT_THROW,            "`throw' keyword");

      TKD(TTM_CONCAT,          "`##' token");
      TKD(TTM_TOSTRING,        "`#' token");
      TKD(TTM_COMMENT,         "...comment(?)");
      TKD(TTM_NEWLINE,         "...newline(?)");
      TKD(TTM_WHITESPACE,      "...whitespace(?)");

      TKD(TT_ENDOFCODE, "end of code");
      #define handle_user_token(tname, desc) TKD(tname, desc);
      #include <User/token_cases.h>
    }
  }
} token_info;

namespace std {
  string to_string(jdi::TOKEN_TYPE type) {
    return token_info.spelling[type];
  }
  string to_string(const jdi::token_t &token) {
    return token.to_string();
  }
}

#include <cstdio>

void token_t::report_error(ErrorHandler *herr, std::string error) const {
  if (herr) {
    herr->error(error, *this);
  } else {
    perror("nullptr ERROR HANDLER PASSED TO REPORT METHOD\n");
    perror((error + "\n").c_str());
  }
}

void token_t::report_errorf(ErrorHandler *herr, std::string error) const {
  string str = to_string();
  for (size_t f = 0; (f = error.find("%s", f)) != string::npos; f += str.length())
    error.replace(f, 2, str);
  report_error(herr, error);
}
std::string token_t::to_string() const {
  size_t f;
  string str = token_info.description[type];
  for (f = 0; (f = str.find("%s", f)) != string::npos; f += content.len)
    str.replace(f, 2, string((const char*) content.str, content.len));
  for (f = 0; (f = str.find("%s", f)) != string::npos; f += content.len)
    str.replace(f, 2, string((const char*) content.str, content.len));
  return str + " (" + (content.toString()) + ")";
}
void token_t::report_warning(ErrorHandler *herr, std::string error) const {
  if (herr) {
    herr->warning(error, *this);
  } else {
    perror("nullptr ERROR HANDLER PASSED TO WARNING METHOD\n");
    perror((error + "\n").c_str());
  }
}

std::string token_t::get_name(TOKEN_TYPE tt) {
  size_t f;
  string str = token_info.description[tt];
  f = str.find("%s");
  while (f != string::npos) {
    str.replace(f, 2, "(content)");
    f = str.find("%s");
  }
  return str;
}

void token_t::validate() const {
  if (file.empty() && type != TT_ENDOFCODE) throw std::range_error("You bastard!");
  if (content.toString().empty() && type != TT_ENDOFCODE && type != TTM_NEWLINE)
    throw std::range_error("You slut!");
}

void token_t::content::copy(const content &other) {
  if (other.cached_str == &other.owned_str) {
    // The other token already owned its data.
    owned_str = other.owned_str;
    cached_str = &owned_str;
    str = owned_str.data();
    len = owned_str.length();
  } else if (other.cached_str) {
    // Static or non-owned string data.
    cached_str = other.cached_str;
    str = other.str;
    len = other.len;
  } else {
    // Volatile data, probably from the file buffer. Persist it.
    owned_str = string((const char*) other.str, other.len);
    cached_str = &owned_str;
    str = owned_str.data();
    len = owned_str.length();
  }
}

void token_t::content::consume(content &&other) {
  if (other.cached_str == &other.owned_str) {
    // The other token already owned its data.
    owned_str = std::move(other.owned_str);
    cached_str = &owned_str;
    str = owned_str.data();
    len = owned_str.length();
    other.cached_str = nullptr;
  } else {
    str = other.str;
    len = other.len;
    // May be null. Don't bother persisting string view, here.
    cached_str = other.cached_str;
  }
}
