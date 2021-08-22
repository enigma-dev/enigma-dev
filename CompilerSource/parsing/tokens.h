/** Copyright (C) 2020 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef ENIGMA_COMPILER_PARSING_TOKENS_h
#define ENIGMA_COMPILER_PARSING_TOKENS_h

#include "error_reporting.h"

#include <iosfwd>
#include <string>
#include <string_view>

// TODO: Delete. Replace with ENIGMA-specific wrapper class.
namespace jdi { struct definition; }

namespace enigma {
namespace parsing {

enum TokenType {
  TT_VARNAME,         // somevar
  TT_SEMICOLON,       // A semicolon.
  TT_COLON,           // A colon.
  TT_COMMA,           // A comma.
  TT_ASSIGN,          // The strict := assignment operator. Not to be confused with EQUALS.
  TT_ASSOP,           // Relative assignment operators += -= *= /= %= <<= >>=
  TT_EQUALS,          // The = operator, which raises warnings in conditional expressions.
  TT_DECIMAL,         // A dot. '.'. Whether for number or for access is determined in the second pass.
  TT_PLUS,            // The + operator.
  TT_MINUS,           // The - operator.
  TT_STAR,            // The * operator.
  TT_SLASH,           // The / operator.
  TT_PERCENT,         // The % operator.
  TT_AMPERSAND,       // The & operator.
  TT_PIPE,            // The | operator.
  TT_CARET,           // The ^ operator.
  TT_AND,             // The and/&& operator.
  TT_OR,              // The or/|| operator.
  TT_XOR,             // The xor/^^ operator.
  TT_DIV,             // The div operator.
  TT_MOD,             // The mod (modulo) operator.
  TT_NOT,             // The not operator. This refers to the keyword, not the bang symbol (!).
  TT_EQUALTO,         // The == operator. Always checks equality.
  TT_NOTEQUAL,        // The != or <> operator.
  TT_BANG,            // The ! operator.
  TT_TILDE,           // The ~ operator.
  TT_INCREMENT,       // The ++ operator.
  TT_DECREMENT,       // The -- operator.
  TT_LESS,            // The < operator (or opening angle bracket).
  TT_GREATER,         // The > operator (or closing angle bracket).
  TT_LESSEQUAL,       // The <= operator.
  TT_GREATEREQUAL,    // The >= operator.
  TT_LSH,             // The << operator.
  TT_RSH,             // The >> operator (or double closing angle bracket).
  TT_QMARK,           // ?
  TT_BEGINPARENTH,    // (
  TT_ENDPARENTH,      // )
  TT_BEGINBRACKET,    // [
  TT_ENDBRACKET,      // ]
  TT_BEGINBRACE,      // {
  TT_ENDBRACE,        // }
  TT_BEGINTRIANGLE,   // <
  TT_ENDTRIANGLE,     // >
  TT_DECLITERAL,      // 0 1 2... 9 10 11... 9876543210...
  TT_BINLITERAL,      // 0b0 0b1 0b10 0b11 0b100 0b101 0b110 ...
  TT_OCTLITERAL,      // 0o0 0o1 0o2... 0o6 0o7 0o10 0o11... 0o76543210 ...
  TT_HEXLITERAL,      // 0x0 $1 0x2... 0x9 0xA... $F $10... $FEDCBA9876543210...
  TT_STRINGLIT,       // "", ''
  TT_CHARLIT,         // '' when in C++ Strings mode (or translating a macro).
  TT_SCOPEACCESS,     // ::
  TT_FUNCTION,        // game_end
  TT_TYPE_NAME,       // int, double, whatever
  TT_NAMESPACE,       // std, enigma, sf
  TT_LOCAL,           // `local` storage specifier
  TT_GLOBAL,          // `global` storage specifier
  TT_RETURN,          // The `return` keyword
  TT_EXIT,            // Legacy `exit` keyword (return void)
  TT_BREAK,           // The `break` keyword
  TT_CONTINUE,        // The `continue` keyword
  TT_S_SWITCH,        // switch
  TT_S_REPEAT,        // repeat
  TT_S_CASE,          // case
  TT_S_DEFAULT,       // default
  TT_S_FOR,           // for
  TT_S_IF,            // if
  TT_S_THEN,          // legacy "then" keyword; some idiots use this
  TT_S_ELSE,          // else
  TT_S_DO,            // do
  TT_S_WHILE,         // while
  TT_S_UNTIL,         // until - means while not
  TT_S_WITH,          // with
  TT_S_TRY,           // try
  TT_S_CATCH,         // catch
  TT_S_NEW,           // C++ memory management `new` keyword
  TT_S_DELETE,        // C++ memory management `delete` keyword
  TT_CLASS,           // class declaration keyword
  TT_STRUCT,          // struct declaration keyword

  TTM_WHITESPACE,     // Preprocessing token: whitespace
  TTM_CONCAT,         // Preprocessing token: ##
  TTM_STRINGIFY,      // Preprocessing token: #

  TT_ERROR,
  TT_ENDOFCODE
};

namespace internal {
namespace useless {

// Allows Token to be a composite of two structs, with access to `type`
// requiring no pointer arithmetic. This struct in and of itself is useless.
struct TokenTypeWrapperStruct {
  TokenType type;
  TokenTypeWrapperStruct(TokenType t): type(t) {}
};

}  // namespace useless
}  // namespace internal

struct Token:
    internal::useless::TokenTypeWrapperStruct,
    CodeSnippet {
  jdi::definition *ext = nullptr;

  bool PreprocessesAway() const { return type == TTM_WHITESPACE; }

  Token(): TokenTypeWrapperStruct(TT_ERROR) {}
  Token(TokenType t, CodeSnippet snippet):
      internal::useless::TokenTypeWrapperStruct(t),
      CodeSnippet(snippet) {}
  Token(TokenType t, jdi::definition *ex, CodeSnippet snippet):
      internal::useless::TokenTypeWrapperStruct(t),
      CodeSnippet(snippet),
      ext(ex) {}
  std::string ToString() const;
};

std::string ToString(TokenType token);
std::ostream &operator<<(std::ostream &os, TokenType tt);
std::ostream &operator<<(std::ostream &os, const Token &t);

}  // namespace parsing
}  // namespace enigma

#endif
