/** Copyright (C) 2020 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef ENIGMA_COMPILER_PARSING_MACROS_h
#define ENIGMA_COMPILER_PARSING_MACROS_h

#include "tokens.h"
#include "error_reporting.h"

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace enigma {
namespace parsing {

typedef std::vector<Token> TokenVector;

/**
  @struct macro_type
  @brief  A generic structure representing a macro in memory.
  
  This is the base class beneath \c macro_scalar and \c macro_function.
  It should not be instantiated purely, as it does not contain any accessible
  value; its only purpose is for storing different types of macros together.
**/
struct Macro {
  std::string name;

 private:
  // For macros constructed by the system (namely, JDI translations), this is
  // the value string from which tokens were originally parsed.
  // For other macros, this will be null.
  std::shared_ptr<const std::string> owned_raw_string;

 public:
  std::vector<Token> value;
  std::optional<std::vector<std::string>> parameters;
  bool is_variadic;

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
  std::vector<FuncComponent> parts;

  /// Build the components vector from the given value vector.
  static std::vector<FuncComponent> Componentize(
      const TokenVector &tokens, const std::vector<std::string> &params,
      ErrorHandler *herr);

  using StringifiedSet = std::unordered_set<std::string>;

  /// Expand this macro function, given arguments.
  TokenVector SubstituteAndUnroll(const std::vector<TokenVector> &args, const std::vector<TokenVector> &args_evald,
                                  ErrorContext errc, StringifiedSet &stringified_macros) const;

  /// Convert this macro to a string
  std::string ToString() const;
  /// Returns the name of this macro, including the parameter list for
  /// function-like macros.
  std::string NameAndPrototype() const;

  /// Most basic constructor; defines an object-like macro with the given value.
  Macro(std::string_view definiendum, TokenVector &&definiens):
      name(definiendum), value(std::move(definiens)), is_variadic(false) {}

  // Like the basic constructor, but tokenizes the given string, reporting any
  // errors to the given ErrorHandler.
  Macro(std::string_view definiendum, std::string definiens, ErrorHandler *herr)
      : name(definiendum),
        owned_raw_string(std::make_shared<std::string>(std::move(definiens))),
        value(ParseTokens(owned_raw_string, herr)), is_variadic(false) {}

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
  Macro(std::string_view definiendum, std::vector<std::string> &&arg_list,
        bool variadic, TokenVector &&definiens, ErrorHandler *herr):
      name(definiendum), value(std::move(definiens)),
      parameters(std::move(arg_list)), is_variadic(variadic),
      parts(Componentize(value, *parameters, herr)) {}
  /// Like the function-like constructor, but parses tokens from the
  /// given string.  NOTE: The string is lexed using C++ semantics.
  Macro(std::string_view definiendum, std::vector<std::string> &&arg_list,
        bool variadic, std::string definiens, ErrorHandler *herr):
      name(definiendum),
      owned_raw_string(std::make_shared<std::string>(std::move(definiens))),
      value(ParseTokens(owned_raw_string, herr)),
      parameters(std::move(arg_list)), is_variadic(variadic),
      parts(Componentize(value, *parameters, herr)) {}

  ~Macro() {}

 private:
  static TokenVector ParseTokens(
      std::shared_ptr<const std::string> owned_raw_string, ErrorHandler *herr);
};

typedef std::map<std::string, Macro, std::less<>> MacroMap;

}  // namespace parsing
}  // namespace enigma

#endif
