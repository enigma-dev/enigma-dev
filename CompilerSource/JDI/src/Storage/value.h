/**
 * @file value.h
 * @brief Header defining a structure for storing parsed values.
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

#ifndef _VALUE__H
#define _VALUE__H
#include <string>
#include <ostream>

namespace jdi {
  /**
    @enum VT
    Declares constants representing the possible types that \c value can hold.
  **/
  enum VT {
    VT_NONE, ///< No value has been assigned to this structure.
    VT_DEPENDENT, ///< The value stored depends on template parameters or other matters of context; a new value should be constructed ASAP.
    VT_DOUBLE,   ///< The value stored is a double.
    VT_INTEGER, ///< The value stored is an integer.
    VT_STRING  ///< The value stored is a string.
  };

  /**
    @struct jdi::value
    A structure for storing and communicating data of varying types.
    This structure can contain any value defined in \enum VT.
  **/
  struct value {
    long double real;  ///< Numeric values, including unsigned longs.
    std::string str;  ///< String values.
    
    VT type;
    
    bool operator==(const value& value) const; ///< Test for strict equality, including type.
    bool operator!=(const value& value) const; ///< Test against strict equality, including type.
    bool operator>=(const value& value) const; ///< Test a strict greater-than or equal inequality, including type.
    bool operator<=(const value& value) const; ///< Test a strict less-than or equal inequality, including type.
    bool operator>(const value& value) const; ///< Test a strict greater-than inequality, including type.
    bool operator<(const value& value) const; ///< Test a strict less-than inequality, including type.

    /// Construct a new, invalid value with no type (VT_NONE).
    value();
    /// Construct a new value representing a double.
    value(float v);
    value(double v);
    value(long double v);
    /// Construct a new value representing an integer.
    value(signed v);
    value(signed long v);
    value(signed long long v);
    /// Construct a new value representing an unsigned integer.
    value(unsigned v);
    value(unsigned long v);
    value(unsigned long long v);
    /// Construct a new value representing a copy of the passed string.
    /// This operates in O(N).
    value(std::string v);
    /// Copy a value. Handles allocation issues.
    value(const value& v);
    /// Construct with the default value of the given type.
    value(const VT& t);

    value &operator=(const value&) = delete;
    value &operator=(value&&);
    
    std::string toString() const; ///< Convert to a string, whatever the value is
    
    operator int() const; ///< Cast to an int, returning zero if no valid cast exists.
    operator long() const; ///< Cast to a long int, returning zero if no valid cast exists.
    operator double() const; ///< Cast to a double, returning zero if no valid cast exists.
    operator bool() const; ///< Cast to a boolean, returning false if no valid cast exists, true if this is a non-empty string.
    operator std::string_view() const; ///< Cast to a string_view, returning nullptr if no valid cast exists.
  };

  inline std::ostream &operator<<(std::ostream &stream, const value &val) {
    return stream << val.toString();
  }
}

#endif
