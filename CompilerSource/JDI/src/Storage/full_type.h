/**
 * @file  full_type.h
 * @brief Header declaring a type storing a \c definition, \c ref_stack, and flag word.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2013 Josh Ventura
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

#ifndef _FULL_TYPE__H
#define _FULL_TYPE__H

namespace jdi {
  struct full_type;
}

#endif

#include <Storage/definition_forward.h>
#include <Storage/references.h>

#ifndef _FULL_TYPE__H__DETAIL
#define _FULL_TYPE__H__DETAIL

#include <string>

namespace jdi {
  /**
    @struct jdi::full_type
    A structure noting the \c definition associated with a type along with a
    set of modifier flags.
  **/
  struct full_type {
    jdi::definition *def; ///< The \c definition associated with the type
    jdi::ref_stack refs; ///< Any referencers affecting this type, such as the pointer-to asterisk (*) or ampersand reference (&).
    int flags; ///< Any flags, such as unsigned, signed, or const, associated with us, as a bitmask.
    
    void swap(full_type& ft); ///< Trade contents with another full_type.
    void copy(const full_type& ft); ///< Copy a full_type without warning; this is for when copy is inevitable.
    
    std::string toString() const; ///< Represent as a string.
    std::string toEnglish() const; ///< Represent as a string giving a plain-English description.
    
    bool operator==(const full_type& other) const; ///< Compare for STRICT equality across all three attributes; to factor in synonyms, use \c synonymous_with.
    bool operator!=(const full_type& other) const; ///< Compare against equality across all three attributes.
    bool operator< (const full_type& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator> (const full_type& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator<= (const full_type& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    bool operator>= (const full_type& other) const; ///< Inequality comparison, just in case someone needs to shove these in a map.
    
    bool synonymous_with(const full_type& x) const; ///< Returns whether this reference stack is equal to another when you consider typedefs.
    full_type &reduce(); ///< Reduces this type by unrolling typedefs.
    
    full_type(); ///< Default constructor.
    full_type(jdi::definition*); ///< Construct with only a definition.
    full_type(jdi::definition* d, int f); ///< Construct with a definition and flags, but no refs.
    full_type(jdi::definition*, const jdi::ref_stack&, int); ///< Construct from individual components. Copies the given \c ref_stack, so may be slow.
    full_type(const full_type&); ///< Copy constructor. Makes a copy, so slowish.
  };
}

#endif
