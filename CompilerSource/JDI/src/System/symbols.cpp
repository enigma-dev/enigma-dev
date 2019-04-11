/**
 * @file symbols.cpp
 * @brief Source populating symbol tables for AST building.
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

#include <cstdio>
#include "symbols.h"
#include <Storage/value_funcs.h>

namespace jdip {
  symbol_table symbols;

  symbol::symbol() {}
  symbol::symbol(unsigned char t, unsigned char p):
    type(t), prec_binary(t&(ST_BINARY|ST_TERNARY)? p:0), prec_unary_pre(t&ST_UNARY_PRE? p:0), prec_unary_post(t&ST_UNARY_POST? p:0),
    operate(NULL), operate_unary_pre(NULL), operate_unary_post(NULL) {}
  symbol::symbol(unsigned char t, unsigned char p, value(*o)(const value&, const value&)):
    type(t), prec_binary(p), prec_unary_pre(0), prec_unary_post(0),
    operate(o), operate_unary_pre(NULL), operate_unary_post(NULL) {}
  symbol::symbol(unsigned char t, unsigned char p, value(*ou)(const value&)):
    type(t), prec_binary(0), prec_unary_pre(t&ST_UNARY_PRE? p:0), prec_unary_post(t&ST_UNARY_POST? p:0),
    operate(NULL), operate_unary_pre(t&ST_UNARY_PRE? ou:NULL), operate_unary_post(t&ST_UNARY_POST? ou:NULL) {}
  symbol& symbol::operator|=(const symbol& other) {
    type |= other.type;
    prec_binary |= other.prec_binary;
    prec_unary_pre |= other.prec_unary_pre;
    prec_unary_post |= other.prec_unary_post;
    if (!operate) operate = other.operate;
    if (!operate_unary_pre) operate_unary_pre = other.operate_unary_pre;
    if (!operate_unary_post) operate_unary_post = other.operate_unary_post;
    return *this;
  }
}

using namespace jdip;

/// Constructor designed to circumvent C++'s lack of static initializer blocks; will be run at program start to populate symbols.
/// Simply maps all the symbols with their AST generation and evaluation information.
symbol_table::symbol_table()
{
  symbols["::"] = symbol(ST_BINARY, precedence::scope);
  
  symbols["++"] = symbol(ST_UNARY_POST,precedence::unary_post,value_unary_increment);
  symbols["--"] = symbol(ST_UNARY_POST,precedence::unary_post,value_unary_decrement);
  symbols["("]  = symbol(ST_BINARY,precedence::unary_post);
  symbols["["]  = symbol(ST_BINARY,precedence::unary_post);
  symbols["."]  = symbol(ST_BINARY,precedence::unary_post);
  symbols["->"] = symbol(ST_BINARY,precedence::unary_post);
  
  symbols["++"] |= symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_increment);
  symbols["--"] |= symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_decrement);
  symbols["+"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_positive);
  symbols["-"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_negative);
  symbols["!"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_not);
  symbols["~"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_negate);
  symbols["*"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_dereference);
  symbols["&"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_reference);
  symbols["(cast)"]   = symbol(ST_UNARY_PRE,precedence::unary_pre,value_unary_reference);
  symbols["sizeof"] = symbol(ST_UNARY_PRE,precedence::unary_pre);
  symbols["new"]    = symbol(ST_UNARY_PRE,precedence::unary_pre);
  symbols["delete"] = symbol(ST_UNARY_PRE,precedence::unary_pre);
  
  symbols[".*"]  = symbol(ST_BINARY, precedence::ptr_member);
  symbols["->*"] = symbol(ST_BINARY, precedence::ptr_member);
  
  symbols["*"] |= symbol(ST_BINARY,precedence::multiplication,values_multiply);
  symbols["/"]  = symbol(ST_BINARY,precedence::multiplication,values_divide);
  symbols["%"]  = symbol(ST_BINARY,precedence::multiplication,values_modulo);
  
  symbols["+"] |= symbol(ST_BINARY,precedence::addition,values_add);
  symbols["-"] |= symbol(ST_BINARY,precedence::addition,values_subtract);
  
  symbols["<<"] = symbol(ST_BINARY,precedence::shift,values_lshift);
  symbols[">>"] = symbol(ST_BINARY,precedence::shift,values_rshift);
  
  symbols["<"]  = symbol(ST_BINARY,precedence::comparison,values_less);
  symbols[">"]  = symbol(ST_BINARY,precedence::comparison,values_greater);
  symbols["<="] = symbol(ST_BINARY,precedence::comparison,values_less_or_equal);
  symbols[">="] = symbol(ST_BINARY,precedence::comparison,values_greater_or_equal);
  
  symbols["=="]  = symbol(ST_BINARY,precedence::equivalence,values_equal);
  symbols["!="]  = symbol(ST_BINARY,precedence::equivalence,values_notequal);
  
  symbols["&"] |= symbol(ST_BINARY,precedence::bit_and,values_bitand);
  symbols["^"]  = symbol(ST_BINARY,precedence::bit_xor,values_bitxor);
  symbols["|"]  = symbol(ST_BINARY,precedence::bit_or,values_bitor);
  
  symbols["&&"] = symbol(ST_BINARY,precedence::logical_and,values_booland);
  symbols["^^"] = symbol(ST_BINARY,precedence::logical_or,values_boolxor);
  symbols["||"] = symbol(ST_BINARY,precedence::logical_or,values_boolor);
  
  symbols["?"]  = symbol(ST_TERNARY | ST_RTL_PARSED,precedence::ternary);
  
  symbols["="]   = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign, values_latter);
  symbols["+="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["-="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["*="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["%="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["/="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["&="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["^="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["|="]  = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols["<<="] = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  symbols[">>="] = symbol(ST_BINARY | ST_RTL_PARSED,precedence::assign);
  
  symbols[","]  = symbol(ST_BINARY,precedence::comma,values_latter);
}
