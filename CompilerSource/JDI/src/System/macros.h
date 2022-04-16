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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _MACROS__H
#define _MACROS__H

namespace jdi {
  struct macro_type;
}

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <General/llreader.h>
#include <API/error_reporting.h>
#include <System/token.h>

namespace jdi {
  using std::string;
  using std::string_view;
  using std::vector;
  
  /**
    @struct macro_type
    @brief  A generic structure representing a macro in memory.
    
    This is the base class beneath \c macro_scalar and \c macro_function.
    It should not be instantiated purely, as it does not contain any accessible
    value; its only purpose is for storing different types of macros together.
  **/
  struct macro_type {
    /** 
      Argument count, or -1 if not a function.
      Hence, a value of -1 implies that this is an instance of macro_scalar.
      Otherwise, this is an instance of macro_function.
      If this value is greater than the size of the argument vector in the
      macro_function, then the function is variadic.
    **/
    const bool is_function, is_variadic;
    /// A copy of the name of this macro.
    string name;
    /// A copy of the parameter list of this macro.
    vector<string> params;

    /// The definiens of this macro, as a series of preprocessor tokens.
    token_vector raw_value;
    /// For object-like macros, a copy of raw_value with any CONCATs evaluated.
    token_vector optimized_value;

    /// Caches meaning for chunks of the replacement list of macro functions.
    struct FuncComponent {
      enum TAG {
        TOKEN_SPAN = 1,
        RAW_ARGUMENT,
        EXPANDED_ARGUMENT,
        PASTE,
        STRINGIFY,
        VA_OPT
      };
      struct TokenSpan {
        size_t begin, end;
      };
      struct Argument {
        size_t index;
      };
      struct RawArgument : Argument {};
      struct ExpandedArgument : Argument {};
      struct Stringify : Argument {};
      struct Paste {};
      struct VAOpt {};

      TAG tag;
      union {
        TokenSpan token_span;
        Argument raw_expanded_or_stringify_argument;
        RawArgument raw_argument;
        ExpandedArgument expanded_argument;
        Stringify stringify;
      };

      FuncComponent(TokenSpan span):
          tag(TOKEN_SPAN), token_span(span) {}
      FuncComponent(RawArgument arg_index):
          tag(RAW_ARGUMENT), raw_argument(arg_index) {}
      FuncComponent(ExpandedArgument arg_index):
          tag(EXPANDED_ARGUMENT), expanded_argument(arg_index) {}
      FuncComponent(Stringify arg_index):
          tag(STRINGIFY), stringify(arg_index) {}
      FuncComponent(Paste):  tag(PASTE)   {}
      FuncComponent(VAOpt):  tag(VA_OPT)  {}
    };

    /// Semantic cache of the replacement list of our function-like macro.
    vector<FuncComponent> parts;

    /// Build the components vector from the given value vector.
    static vector<FuncComponent>
        componentize(const token_vector &tokens, const vector<string> &params,
                     ErrorHandler *herr);

    /// Expand this macro function, given arguments.
    token_vector substitute_and_unroll(const vector<token_vector> &args,
                                       const vector<token_vector> &args_evald,
                                       ErrorContext errc) const;

    /// Handle concatenations (##) in replacement lists for object-like macros.
    static token_vector evaluate_concats(const token_vector &replacement_list,
                                         ErrorHandler *herr);

    /// Convert this macro to a string
    string toString() const;
    /// Returns the name of this macro, including the parameter list for
    /// function-like macros.
    string NameAndPrototype() const;
    
    /// Default constructor; defines an object-like macro with the given value.
    macro_type(const string &n, vector<token_t> &&definiens, ErrorHandler *h):
        is_function(false), is_variadic(false), name(n), params(),
        raw_value(std::move(definiens)),
        optimized_value(evaluate_concats(raw_value, h)) {}

    /** Construct a macro function taking the arguments in arg_list.
        This function parses the given value based on the argument list.
        @param name_     The name of this macro.
        @param arg_list  Contains the arguments to be copied in.
        @param variadic  Determines if an additional parameter should be created
                         to store all excess arguments.
        @param definiens The value that will be assigned to this macro function. 
                         The constructor will automatically parse and expand
                         this value according to convention.
        @param herr      The error handler to receive any errors.
        @note
          If \p arg_list is empty, and \p variadic is false, the behavior is the
          same as the default constructor. 
    **/
    macro_type(string_view name_, vector<string> &&arg_list, bool variadic,
               vector<token_t> &&definiens, ErrorHandler *herr):
        is_function(true), is_variadic(variadic), name(name_),
        params(std::move(arg_list)), raw_value(std::move(definiens)),
        parts(componentize(raw_value, params, herr)) {}
    
    ~macro_type() {}
  };
  
  /** Map type used for storing macros. Sharing reduces copy times when cloning
      the base context. It also makes destruction automatic. */
  typedef std::map<string, std::shared_ptr<const jdi::macro_type>> macro_map;
  typedef macro_map::iterator macro_iter; ///< Iterator type for macro maps.
  typedef macro_map::const_iterator macro_iter_c; ///< Const iterator type for macro maps.
}
#endif
