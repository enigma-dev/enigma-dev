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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _VALUE__H
#define _VALUE__H
#include <string>

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
    /** This union contains the actual data for each type.
    **/
    union {
      double d; ///< Any data stored as a floating point.
      long i; ///< Any data stored as an integer.
      const char* s; ///< Any data stored as a string. This value must be deleted if replaced or destructed.
    } val; ///< The value storage for this structure.
    
    VT type; ///< The type of our value. This little bugger is the reason we can't just be a big union.
    
    bool operator==(const value& value) const; ///< Test for strict equality, including type.
    bool operator!=(const value& value) const; ///< Test against strict equality, including type.
    bool operator>=(const value& value) const; ///< Test a strict greater-than or equal inequality, including type.
    bool operator<=(const value& value) const; ///< Test a strict less-than or equal inequality, including type.
    bool operator>(const value& value) const; ///< Test a strict greater-than inequality, including type.
    bool operator<(const value& value) const; ///< Test a strict less-than inequality, including type.
    
    value(); ///< Construct a new, invalid value with no type (VT_NONE).
    value(double v); ///< Construct a new value representing a double.
    value(long v); ///< Construct a new value representing an integer.
    value(const char* v); ///< Construct a new value consuming and representing a const char*. Once you pass a const char*, it belongs to this class.
    value(std::string v); ///< Construct a new value representing a copy of the passed string. This operates in O(N).
    value(const value& v); ///< Copy a value. Handles allocation issues.
    value(const VT& t); ///< Construct with only a value type
    ~value(); ///< Default destructor; handles freeing any strings.
    
    std::string toString() const; ///< Convert to a string, whatever the value is
    
    operator int() const; ///< Cast to an int, returning zero if no valid cast exists.
    operator long() const; ///< Cast to a long int, returning zero if no valid cast exists.
    operator double() const; ///< Cast to a double, returning zero if no valid cast exists.
    operator bool() const; ///< Cast to a boolean, returning false if no valid cast exists, true if this is a non-empty string.
    operator const char* () const; ///< Cast to a const char*, returning NULL if no valid cast exists. @warning May be null. @warning Do not free.
  };
}

#endif
