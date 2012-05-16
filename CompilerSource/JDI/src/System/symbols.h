/**
 * @file symbols.h
 * @brief Header declaring symbol types for AST building.
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

#include <map>
#include <string>
#include <Storage/value.h>

namespace jdip {
  using namespace jdi;
  /**
    Enumeration of usage types of symbols used in this AST.
  **/
  enum symbol_type {
    ST_TERNARY = 1, ///< True if this can be used as a ternary operator
    ST_BINARY = 2, ///< True if this can be used as a ternary operator
    ST_UNARY_PRE = 4, ///< True if this can be used as a prefix unary operator
    ST_UNARY_POST = 8, ///< True if this can be used as a postfix unary operator
    ST_RTL_PARSED = 16 ///< For types parsed right-to-left, such as assignments
  };

  /**
    Structure containing information about a given symbol, including how it is
    used, its precedence, and methods for executing its operation on two values.
  **/
  struct symbol {
    short type; ///< Usage information, as declared in the \c symbol_type enum.
    short prec; ///< Precedence, where 1 is the highest precedence.
    value (*operate)(const value&, const value&); ///< Method to perform this operation on two values, if this is a binary operator.
    value (*operate_unary)(const value&); ///< Method to perform this operation on one value, if this is a unary prefix operator.
    symbol(); ///< Default constructor, so std::map doesn't have a conniption.
    symbol(short t, short p); ///< Operation-free constructor.
    symbol(short t, short p, value(*o)(const value&, const value&)); ///< Constructor for binary operators.
    symbol(short t, short p, value(*o)(const value&, const value&), value(*ou)(const value&)); ///< Constructor operators that can be binary or unary.
    symbol(short t, short p, value(*ou)(const value&)); ///< Constructor for unary operators.
  };
  
  /// Our "own map type" to circumvent the lack of static code blocks in C++.
  struct symbol_table: public std::map<std::string, symbol> {
    symbol_table(); ///< Default constructor. Populates map.
  };
  extern symbol_table symbols; ///< The symbol table which will be searched while building ASTs.
  typedef symbol_table::iterator symbol_iter; ///< Convenience typedef to the iterator type of the symbol table.
}
