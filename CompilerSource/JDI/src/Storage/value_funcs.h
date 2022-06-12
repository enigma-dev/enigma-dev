/**
 * @file value_funcs.h
 * @brief Header declaring functions for operating on values.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "value.h"

namespace jdi {
  /** The most complicated binary operator; returns sums or concatenations.
      * If the inputs are both real, their sum is returned.
      * If the inputs are both strings, the string formed by concatenating
        \p x and \p y is returned.
      * If one input is real, and the other string, the string input is
        treated as a pointer, and a substring of equal or lesser length
        is returned. For example, "Hello" + 1 = "ello". This is an O(N)
        operation.
  **/
  value values_add(const value& x, const value& y);
  value values_subtract(const value& x, const value& y); ///< Returns the difference of two real values.
  value values_multiply(const value& x, const value& y); ///< Returns the product of two real values.
  value values_divide(const value& x, const value& y);  ///< Returns the quotient of two real values.
  value values_modulo(const value& x, const value& y);  ///< Returns the modulo of two real values.
  value values_lshift(const value& x, const value& y);  ///< Returns the value given by x, multiplied by two to the power of y.
  value values_rshift(const value& x, const value& y);  ///< Returns the value given by x, divided by two to the power of y.
  value values_bitand(const value& x, const value& y);  ///< Returns the value given by the bitwise AND of x and y.
  value values_bitor(const value& x, const value& y);  ///< Returns the value given by the bitwise OR of x and y.
  value values_bitxor(const value& x, const value& y);  ///< Returns the value given by the bitwise XOR of x and y.
  value values_booland(const value& x, const value& y);  ///< Returns the value given by the bitwise AND of x and y.
  value values_boolor(const value& x, const value& y);  ///< Returns the value given by the bitwise OR of x and y.
  value values_boolxor(const value& x, const value& y);  ///< Returns the value given by the bitwise XOR of x and y.
  
  /** Compares two values, x and y.
      If x and y are both real, the result is true iff x > y.
      If x is real, but y is a string, the comparison is false.
      If x is a string, but y is real, the comparison is true.
      If x and y are both strings, the comparison is done per the specification of std::string::operator>().
      @return Returns a value containing the integer 1 if the comparison was true, 0 if it was false.
  **/
  value values_greater(const value& x, const value& y);
  /** Compares two values, x and y.
      If x and y are both real, the result is true iff x < y.
      If x is real, but y is a string, the comparison is true.
      If x is a string, but y is real, the comparison is false.
      If x and y are both strings, the comparison is done per the specification of std::string::operator<().
      @return Returns a value containing the integer 1 if the comparison was true, 0 if it was false.
  **/
  value values_less(const value& x, const value& y);
  /** Compares two values, x and y.
      If x and y are both real, the result is true iff x >= y.
      If x is real, but y is a string, the comparison is false.
      If x is a string, but y is real, the comparison is true.
      If x and y are both strings, the comparison is done per the specification of std::string::operator>=().
      @return Returns a value containing the integer 1 if the comparison was true, 0 if it was false.
  **/
  value values_greater_or_equal(const value& x, const value& y);
  /** Compares two values, x and y.
      If x and y are both real, the result is true iff x <= y.
      If x is real, but y is a string, the comparison is true.
      If x is a string, but y is real, the comparison is false.
      If x and y are both strings, the comparison is done per the specification of std::string::operator<=().
      @return Returns a value containing the integer 1 if the comparison was true, 0 if it was false.
  **/
  value values_less_or_equal(const value& x, const value& y);
  /** Compares two values for equality, x and y.
      If x and y are both real, the result is true iff x == y.
      If x and y are both strings, the result is true if the strings are identical.
      If x and y differ in type, the comparison is false.
      @return Returns a value containing the integer 1 if the comparison was true, 0 if it was false.
  **/
  value values_equal(const value& x, const value& y);
  /** Compares two values for equality, x and y.
      If x and y are both real, the result is true iff x != y.
      If x and y are both strings, the result is true if the strings are in any way different.
      If x and y differ in type, the comparison is true.
      @return Returns a value containing the integer 1 if the comparison was true, 0 if it was false.
  **/
  value values_notequal(const value& x, const value& y);
  
  /// Returns the value given by y, regardless of the value given by x. For use as a callback for comma operators.
  value values_latter(const value&, const value& y);
  
  /// If x is real, returns x + 1, or if it is a string, returns a substring containing all but the first character.
  /// The value of x is not modified.
  value value_unary_increment(const value& x);
  value value_unary_decrement(const value& x); ///< Returns x-1, if x is real. Does not change the value of x.
  value value_unary_positive(const value& x); ///< Returns x, if x is real, or an invalid value otherwise.
  value value_unary_negative(const value& x); ///< Returns -x, if x is real, or an invalid value otherwise.
  value value_unary_dereference(const value& x); ///< Returns the first character in x, if x is a string, or an invalid value otherwise.
  value value_unary_reference(const value&); ///< Returns zero, as an integer.
  value value_unary_negate(const value& v); ///< Returns the bitwise negation of the value, cast to an integer.
  value value_unary_not(const value& v); ///< Returns the integers one or zero, the former when value is not equal to zero.
  
  bool value_boolean(const value& v); ///< Returns true if v is real and v != 0 or if v is a string and v is not empty. If v is invalid, false is returned.
}
