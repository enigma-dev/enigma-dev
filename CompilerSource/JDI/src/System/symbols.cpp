/**
 * @file symbols.cpp
 * @brief Source populating symbol tables for AST building.
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

#include <cstdio>
#include "symbols.h"
#include <Storage/value_funcs.h>

namespace jdip {
  symbol_table symbols;

  symbol::symbol() {}
  symbol::symbol(short t, short p): type(t), prec(p), operate(NULL), operate_unary(NULL) {} ///< Operation-free constructor.
  symbol::symbol(short t, short p, value(*o)(const value&, const value&)): type(t), prec(p), operate(o), operate_unary(NULL) {} ///< 
  symbol::symbol(short t, short p, value(*o)(const value&, const value&), value(*ou)(const value&)): type(t), prec(p), operate(o), operate_unary(ou) {}
  symbol::symbol(short t, short p, value(*ou)(const value&)): type(t), prec(p), operate(NULL), operate_unary(ou) {}
}

using namespace jdip;

/// Constructor designed to circumvent C++'s lack of static initializer blocks; will be run at program start to populate symbols.
/// Simply maps all the symbols with their AST generation and evaluation information.
symbol_table::symbol_table()
{
  int prec = 18; // Count down precedence levels to avoid accidental decrements.
  symbols["::"] = symbol(ST_BINARY, prec);
  
  prec--;
  symbols["["]  = symbol(ST_BINARY,prec);
  symbols["("]  = symbol(ST_BINARY | ST_UNARY_PRE,prec);
  symbols["."]  = symbol(ST_BINARY,prec);
  symbols["->"] = symbol(ST_BINARY,prec);
  
  prec--;
  symbols["++"] = symbol(ST_UNARY_PRE | ST_UNARY_POST,prec,value_unary_increment);
  symbols["--"] = symbol(ST_UNARY_PRE | ST_UNARY_POST,prec,value_unary_decrement);
  symbols["!"] = symbol(ST_UNARY_PRE | ST_UNARY_POST,prec,value_unary_not);
  symbols["~"] = symbol(ST_UNARY_PRE | ST_UNARY_POST,prec,value_unary_negate);
  
  prec--;
  symbols[".*"] = symbol(ST_UNARY_PRE | ST_UNARY_POST,  prec);
  symbols["->*"] = symbol(ST_UNARY_PRE | ST_UNARY_POST, prec);
  
  prec--;
  symbols["*"]  = symbol(ST_UNARY_PRE | ST_BINARY,prec,values_multiply,value_unary_dereference);
  symbols["/"]  = symbol(ST_BINARY,prec,values_divide);
  symbols["%"]  = symbol(ST_BINARY,prec,values_modulo);
  
  prec--;
  symbols["+"]  = symbol(ST_UNARY_PRE | ST_BINARY,prec,values_add,value_unary_positive);
  symbols["-"]  = symbol(ST_UNARY_PRE | ST_BINARY,prec,values_subtract,value_unary_negative);
  
  prec--;
  symbols["<<"]  = symbol(ST_BINARY,prec,values_lshift);
  symbols[">>"]  = symbol(ST_BINARY,prec,values_rshift);
  
  prec--;
  symbols["<"]  = symbol(ST_BINARY,prec,values_less);
  symbols[">"]  = symbol(ST_BINARY,prec,values_greater);
  symbols["<="]  = symbol(ST_BINARY,prec,values_less_or_equal);
  symbols[">="]  = symbol(ST_BINARY,prec,values_greater_or_equal);
  
  prec--;
  symbols["=="]  = symbol(ST_BINARY,prec,values_equal);
  symbols["!="]  = symbol(ST_BINARY,prec,values_notequal);
  
  prec--; symbols["&"]  = symbol(ST_UNARY_PRE | ST_BINARY,prec,values_bitand,value_unary_reference);
  prec--; symbols["^"]  = symbol(ST_BINARY,prec,values_bitxor);
  prec--; symbols["|"]  = symbol(ST_BINARY,prec,values_bitor);
  
  prec--; symbols["&&"] = symbol(ST_UNARY_PRE | ST_BINARY,prec,values_booland);
  prec--; symbols["^^"] = symbol(ST_BINARY,prec,values_boolxor);
  prec--; symbols["||"] = symbol(ST_BINARY,prec,values_boolor);
  
  prec--; symbols["?"]  = symbol(ST_TERNARY | ST_RTL_PARSED,prec);
  
  prec--;
  symbols["="]   = symbol(ST_BINARY | ST_RTL_PARSED,prec, values_latter);
  symbols["+="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["-="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["*="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["%="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["/="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["&="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["^="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["|="]  = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols["<<="] = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  symbols[">>="] = symbol(ST_BINARY | ST_RTL_PARSED,prec);
  
  prec--;
  symbols[","]  = symbol(ST_BINARY,prec,values_latter);
  
  if (prec != 1)
    printf("INTERNAL ERROR. INCORRECT PRECENDENCE COUNT GIVEN.\n");
}
