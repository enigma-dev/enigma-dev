/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "precedence.h"

namespace enigma::parsing {
  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kBinaryPrec{
      {TT_SCOPEACCESS,  {Precedence::kScope,          Associativity::LTR}},
      {TT_DOT,          {Precedence::kMemberAccess,   Associativity::LTR}},
      {TT_ARROW,        {Precedence::kMemberAccess,   Associativity::LTR}},
      {TT_DOT_STAR,     {Precedence::kMemberPointer,  Associativity::LTR}},
      {TT_ARROW_STAR,   {Precedence::kMemberPointer,  Associativity::LTR}},
      {TT_STAR,         {Precedence::kMultiply,       Associativity::LTR}},
      {TT_SLASH,        {Precedence::kDivide,         Associativity::LTR}},
      {TT_DIV,          {Precedence::kDivide,         Associativity::LTR}},
      {TT_MOD,          {Precedence::kModulo,         Associativity::LTR}},
      {TT_PERCENT,      {Precedence::kModulo,         Associativity::LTR}},
      {TT_PLUS,         {Precedence::kAdd,            Associativity::LTR}},
      {TT_MINUS,        {Precedence::kSubtract,       Associativity::LTR}},
      {TT_LSH,          {Precedence::kShift,          Associativity::LTR}},
      {TT_RSH,          {Precedence::kShift,          Associativity::LTR}},
      {TT_THREEWAY,     {Precedence::kThreeWayComp,   Associativity::LTR}},
      {TT_LESS,         {Precedence::kRelational,     Associativity::LTR}},
      {TT_LESSEQUAL,    {Precedence::kRelational,     Associativity::LTR}},
      {TT_GREATER,      {Precedence::kRelational,     Associativity::LTR}},
      {TT_GREATEREQUAL, {Precedence::kRelational,     Associativity::LTR}},
      {TT_EQUALTO,      {Precedence::kEquality,       Associativity::LTR}},
      {TT_NOTEQUAL,     {Precedence::kEquality,       Associativity::LTR}},
      {TT_AMPERSAND,    {Precedence::kBitAnd,         Associativity::LTR}},
      {TT_CARET,        {Precedence::kBitXOr,         Associativity::LTR}},
      {TT_PIPE,         {Precedence::kBitOr,          Associativity::LTR}},
      {TT_AND,          {Precedence::kBoolAnd,        Associativity::LTR}},
      {TT_XOR,          {Precedence::kBoolXOr,        Associativity::LTR}},
      {TT_OR,           {Precedence::kBoolOr,         Associativity::LTR}},
      {TT_ASSIGN,       {Precedence::kAssign,         Associativity::RTL}},
      {TT_EQUALS,       {Precedence::kAssign,         Associativity::RTL}},
      {TT_ASSOP,        {Precedence::kCompoundAssign, Associativity::RTL}},
      {TT_COMMA,        {Precedence::kComma,          Associativity::LTR}},

      // TODO: TT_THROW as kThrow RTL
      // TODO: TT_ kYield RTL
  };

  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kUnaryPostfixPrec {
      {TT_INCREMENT,    {Precedence::kUnaryPostfix, Associativity::LTR}},
      {TT_DECREMENT,    {Precedence::kUnaryPostfix, Associativity::LTR}},
  };

  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kTernaryPrec {
      {TT_QMARK, {Precedence::kTernary, Associativity::RTL}},
  };

  std::unordered_map<TokenType, OperatorPrecedence> Precedence::kSpecialPrec {
      {TT_BEGINBRACKET, {Precedence::kSubscript, Associativity::LTR}},
      {TT_BEGINPARENTH, {Precedence::kFuncCall,  Associativity::LTR}},
  };

  std::unordered_set<TokenType> Precedence::kUnaryPrefixOps{
      TT_PLUS, TT_MINUS, TT_STAR, TT_AMPERSAND, TT_TILDE, TT_NOT, TT_BANG,
      TT_INCREMENT, TT_DECREMENT
  };
}