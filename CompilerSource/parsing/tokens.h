/*  Copyright (C) 2020 Josh Ventura
**
**  This file is a part of the ENIGMA Development Environment.
**
**
**  ENIGMA is free software: you can redistribute it and/or modify it under the
**  terms of the GNU General Public License as published by the Free Software
**  Foundation, version 3 of the license or any later version.
**
**  This application and its source code is distributed AS-IS, WITHOUT ANY
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
**  details.
**
**  You should have recieved a copy of the GNU General Public License along
**  with this code. If not, see <http://www.gnu.org/licenses/>
**
**  ENIGMA is an environment designed to create games and other programs with a
**  high-level, fully compilable language. Developers of ENIGMA or anything
**  associated with ENIGMA are in no way responsible for its users or
**  applications created by its users, or damages caused by the environment
**  or programs made in the environment.
*/

#include <string>
#include <string_view>

namespace enigma {
namespace parsing {

enum TokenType {
  TT_VARNAME,         // somevar
  TT_SEMICOLON,       // A semicolon.
  TT_COLON,           // A colon.
  TT_COMMA,           // A comma.
  TT_ASSOP,           // = += -= *= /= ...
  TT_DECIMAL,         // A dot. '.'. Whether for number or for access is determined in the second pass.
  TT_OPERATOR,        // Basic operators. = + - *...
  TT_UNARYPRE,        // Unary prefix operators. + - ! ++(int) --(int)
  TT_UNARYPOST,       // Unary postfix operators. ++ --
  TT_TERNARY,         // ?
  TT_BEGINPARENTH,    // (
  TT_ENDPARENTH,      // )
  TT_BEGINBRACKET,    // [
  TT_ENDBRACKET,      // ]
  TT_BEGINBRACE,      // {
  TT_ENDBRACE,        // }
  TT_BEGINTRIANGLE,   // <
  TT_ENDTRIANGLE,     // >
  TT_DIGIT,           // 0 1 2... (...)
  TT_STRING,          // "", ''
  TT_SCOPEACCESS,     // ::
  TT_FUNCTION,        // game_end
  TT_TYPE_NAME,       // int, double, whatever
  TT_NAMESPACE,       // std, enigma, sf
  TT_LOCGLOBAL,       // global/local
  TT_SHORTSTATEMENT,  // Short statements; return, mostly
  TT_TINYSTATEMENT,   // break, continue, exit...
  TT_S_SWITCH,        // switch
  TT_S_CASE,          // case
  TT_S_DEFAULT,       // default
  TT_S_FOR,           // for
  TT_S_IF,            // if
  TT_S_ELSE,          // else
  TT_S_DO,            // do
  TT_S_WHILE,         // while
  TT_S_UNTIL,         // while
  TT_S_WITH,          // with
  TT_S_TRY,           // try
  TT_S_CATCH,         // catch
  TT_S_NEW,           // new

  TT_IMPLICIT_SEMICOLON,
  TT_ERROR = -1
};

// TODO: Delete. Replace with ENIGMA-specific wrapper class.
namespace jdi { struct definition; }

struct Token {
  TokenType type;
  std::string_view content;
  size_t pos, length;

  struct {
    bool separator,      // Separator is true for, ie, any of "; , { ( [  }".
         breakandfollow, // Break and follow is true for "] )" and strings/varnames/digits.
         operatorlike;   // Operator like is true for OPERATOR, ASSOP, (, [, and statements expecting an expression.
    int macrolevel;
  } antiquated;

  unsigned match; // The index of the matching parenthesis/bracket/brace in the lex
  jdi::definition* ext;

  Token(): type(TT_ERROR) {}
  Token(TokenType t,                      std::string_view ct, size_t p, size_t l): type(t), content(ct), pos(p), length(l), match(0),     ext(nullptr) {}
  Token(TokenType t, unsigned match,      std::string_view ct, size_t p, size_t l): type(t), content(ct), pos(p), length(l), match(match), ext(nullptr) {}
  Token(TokenType t, jdi::definition *ex, std::string_view ct, size_t p, size_t l): type(t), content(ct), pos(p), length(l), match(0),     ext(ex) {}
  std::string ToString() const;
};

}  // namespace parsing
}  // namespace enigma
