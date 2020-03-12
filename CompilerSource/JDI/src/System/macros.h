/**
 * @file  macros.h
 * @brief System header for parsing macros.
 * 
 * This header defines a few classes for representing macros in memory, both
 * macro functions and simple preprocessor substitutions.
 * 
 * Some of the constructors in this file do some heavy lifting to make the general
 * interface more pleasant in parser functions, as well as ultimately to make the
 * code more efficient.
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

#ifndef _MACROS__H
#define _MACROS__H

namespace jdip {
  struct macro_type;
  struct macro_scalar;
  struct macro_function;
}

#include <string>
#include <vector>
#include <General/llreader.h>
#include <API/error_reporting.h>
#include <System/token.h>

#if (__cplusplus - 0) >= 201703L
  #define __REGISTER
#else
  #define __REGISTER                             register
#endif

namespace jdip {
  using std::string;
  using std::vector;
  using namespace jdi;
  
  /**
    @struct macro_type
    @brief  A generic structure representing a macro in memory.
    
    This is the base class beneath \c macro_scalar and \c macro_function. It should not
    be instantiated purely, as it does not contain any accessible value; its only purpose
    is for storing different types of macros together. \
  **/
  struct macro_type {
    /** 
      Argument count, or -1 if not a function.
      Hence, a value of -1 implies that this is an instance of macro_scalar.
      Otherwise, this is an instance of macro_function.
      If this value is greater than the size of the argument vector in the macro_function, then the function is variadic.
    **/
    const int argc;
    /// Macros should not be edited, only replaced, and therefore are easy to copy by reference; this tells how many references are made to this macro.
    mutable unsigned refc;
    /// A copy of the name of this macro; std::string will take care of the aliasing.
    string name;
    
    /// Release a macro
    static void free(const macro_type* whom);
    
    /// Convert this macro to a string
    string toString() const;
    
    protected:
      /**
        The macro_type constructor is used to set the argument count or flag it inactive.
        As macro_type should never be instantiated purely, its constructor is protected. Modifying
        the parameter count could cause failure to recognize the type of the current macro.
        @see argc
      **/
      macro_type(string n, int argc);
      /// The base destructor of macro_type does not do anything to free memebers: DO NOT INVOKE IT!
      /// Use macro_type::free instead.
      ~macro_type();
  };
  
  /**
    @struct macro_scalar
    @brief  A structure representing a macro substitution in memory.
  **/
  struct macro_scalar: macro_type {
    string value; ///< The definiens of this macro.
    macro_scalar(string n, string val=""); ///< The default constructor, taking an optional value parameter.
    ~macro_scalar(); ///< The macro_scalar destructor is only for debugging purposes.
  };
  
  /**
    @struct macro_function
    @brief  A structure representing a macro function in memory.
  **/
  struct macro_function: macro_type {
    /// A structure, short for "macro value chunk," which contains either a string or an argument number.
    struct mv_chunk {
      char* data; ///< A pointer to the string value of this chunk, if it has one.
      size_t metric; ///< The argument number if we are an argument, or the length of data otherwise.
      bool is_arg; ///< True if we are an argument number.
      mv_chunk(const char* str, size_t start, size_t lenth); ///< Construct a new macro value chunk from a substring
      mv_chunk(char* data_buf, size_t length); ///< Construct a new macro value chunk as data, with a length, consuming data.
      mv_chunk(size_t argnum); ///< Construct a new macro value chunk as a reference to the value of a parameter with the given index
      mv_chunk(); ///< Default constructor; do not use. Created to appease the STL.
      string toString(macro_function *mf); ///< For debugging purposes, convert to a string.
    };
    
    /** The expanded value of this macro.
        
        @section Convention
        
        The definiens of this macro must be stored expanded such that each used parameter is given its own element.
        For example, given #define vertex_x(a,b,c) ((-(b))/(2*(a))), the value member will be expanded as follows:
        
          {   "((-(",   "b",   "))/(2*(",   "a",   ")))"   }.
        
        Use of the concatenation operator, "##", will be automatically accounted for, such that the definiens "a##b=c"
        will be expanded as such:
        
          {   "a",    "b",    "=",   "c"   }
        
        The string operator, #, will be handled by appending the singleton string "#" to the vector, followed by
        the name of the parameter. This is the only special case which the imploding function must implement.
        
        By this convention, evaluating a macro function is as simple as unloading the argument-value pairs into a
        map and iterating the value vector, substituting value[i] with map[value[i]] where defined.
    **/
    vector<mv_chunk> value;
    vector<string> args; //!< The names of each argument.
    
    /// Default constructor; construct a zero-parameter macro function with the given value, or an empty value if none is specified.
    macro_function(string n, string val="");
    /** Construct a macro function taking the arguments in arg_list.
        This function parses the given value based on the argument list.
        @param arg_list  Contains the arguments to be copied in.
        @param value     The value that will be assigned to this macro function. 
                         The constructor will automatically parse and expand this value according to convention.
        @param variadic  Determines if an additional parameter should be created to store all excess arguments.
        @param herr      The error handler to receive any errors.
        @note
          If \p arg_list is empty, and \p variadic is false, the behavior is the same as the default constructor. 
    **/
    macro_function(string n, const vector<string> &arg_list, string value="", bool variadic=0, error_handler *herr = def_error_handler);
    
    /** An internal function used to parse the definiens of a macro into a vector for collapse at eval-time.
        Saves big on CPU when evaluating a function many times.
        @see jdi::macro_function::value
    **/
    void preparse(string definiens, error_handler *herr);
    
    /** Parse an argument vector into a string. 
        @param arg_list  Contains the arguments to be copied in.
        @param dest      The char* to direct at the buffer. You are responsible for freeing this buffer. [out]
        @param destend   The char* to direct at the end of the buffer. [out]
        @param errtok    Dummy token for error reporting coordination.
        @param herr      The error handler to receive any errors.
    **/
    bool parse(vector<string> &arg_list, char* &dest, char* &destend, token_t errtok, error_handler *herr = def_error_handler) const;
    
    /// Big surprise: The macro_function destructor also does nothing.
    ~macro_function();
  };
  
}
#endif
