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

#ifndef ENIGMA_COMPILER_PARSING_LEXER_h
#define ENIGMA_COMPILER_PARSING_LEXER_h

#include "tokens.h"
#include "macros.h"
#include "error_reporting.h"
#include "language_frontend.h"
#include "settings.h"

#include <cstddef>
#include <deque>
#include <map>
#include <string>

namespace enigma {
namespace parsing {

struct ParseContext {
  /// Compilation target language. Source language is EDL. Target language
  /// provides actual definitions for globals, functions, macros, etc, and
  /// implements pretty-printing.
  const LanguageFrontend *language_fe;
  /// All macros built-in or declared in user's Definitions pages.
  const MacroMap macro_map;
  /// Variables that are inherited by all objects.
  const NameSet *shared_locals;
  /// Scripts that are available for execution.
  const NameSet script_names;
  /// Options controlling how code is interpreted.
  setting::CompatibilityOptions compatibility_opts;

  static const ParseContext &ForTesting(bool use_cpp);
  static const ParseContext &ForPreprocessorEvaluation();

  /// Disallow null construction.
  ParseContext(nullptr_t) = delete;
  /// Used by everyone else.
  ParseContext(const LanguageFrontend *lang, NameSet script_names):
      language_fe(lang),
      macro_map(lang->builtin_macros()),
      shared_locals(&lang->shared_object_locals()),
      script_names(std::move(script_names)),
      compatibility_opts(lang->compatibility_opts()) {}
};

class Lexer {
 public:
  Token ReadToken();
  Token ReadRawToken();
  TokenVector PreprocessBuffer(const TokenVector &tokens);

  const std::string &GetCode() const { return code; }
  const ParseContext &GetContext() const { return *context; }
  size_t LineNumber() const { return line_number; }

  // Looks up an operator string in the default operator trie.
  // The entire given string must match the operator, or TT_ERROR is returned.
  static TokenType LookUpOperator(std::string_view op);

  void UseCppOptions() { options.SetAsCpp(); }

  Lexer(std::string code_, const ParseContext *ctx, ErrorHandler *herr_):
      owned_code(std::make_shared<std::string>(std::move(code_))),
      code(*owned_code), context(ctx), herr(herr_), options(ctx) {}
  Lexer(std::shared_ptr<const std::string> code_, const ParseContext *ctx,
        ErrorHandler *herr_):
      owned_code(code_), code(*owned_code), context(ctx), herr(herr_),
      options(ctx) {}
  Lexer(TokenVector tokens, const ParseContext *ctx, ErrorHandler *herr_);

 private:
  struct OpenMacro {
    TokenVector tokens;
    size_t index = 0;
    std::string_view name;

    OpenMacro(std::string_view macro_name, TokenVector macro_value):
        tokens(std::move(macro_value)), name(macro_name) {}
  };

  bool MacroRecurses(std::string_view name) const;

  // Enters a macro.
  void PushMacro(std::string_view name, TokenVector value);
  // Exits the most-recently-entered macro.
  void PopMacro();

  // Checks if a token that's about to be returned is a macro, and if so,
  // attempts to unroll it.
  bool HandleMacro(std::string_view name);

  // Looks up any keyword or global name behind the given token and updates its
  // token type to match (e.g. to TT_TYPE_NAME or TT_FUNCTION).
  // Returns the given token, with modifications, by reference.
  Token &TranslateNameToken(Token &token);

  size_t ComputeLineNumber(size_t lpos);
  CodeSnippet Mark(size_t pos, size_t length);

  // Allows this lexer to own the code it and its output tokens point into.
  // May be null if the code is stack-alloc'd and owned by another entity.
  // Always prefer `code`.
  std::shared_ptr<const std::string> owned_code;

  const std::string &code;
  size_t pos = 0;
  size_t line_number = 1;
  size_t line_last_evaluated_at_ = 0;
  size_t last_line_position = (size_t) -1;
  const ParseContext *context;
  std::deque<OpenMacro> open_macros;
  ErrorHandler *herr;

  /// Store the stringified versions of macros in a set so that they can be safely referred to by `std::string_view`.
  ///
  /// As `CodeSnippet`s point inside the source itself, it is not possible to make one that points to the `std::string`
  /// created on stringifying a macro as that string doesn't exist within the source and can only be referred to within
  /// the function it is created in and therefore when trying to access this string outside, it can cause a segfault.
  ///
  /// Thus, this set "extends" the lifetime of the stringified forms of macros by promoting their lifetimes to the
  /// lifetime of the lexer itself. The reason a `std::unordered_set` is used is to deduplicate macros which stringify
  /// to the same string.
  Macro::StringifiedSet stringified_macros;
  
  struct Options {
    bool use_escapes;  ///< Use C++-like escape sequences.
    bool use_char_literals;  ///< Treat 'l' as a char literal rather than a string literal.
    bool use_hex_literals;  ///< Treat 0x1AB as a hexadecimal literal.
    bool use_oct_literals;  ///< Treat 0755 as an octal literal.
    bool use_bin_literals;  ///< Treat 0b1AB as a hexadecimal literal.
    bool use_gml_style_hex;  ///< Treat $ABC123 as hex (otherwise, assume identifier char).
    bool use_preprocessor_tokens;  ///< Treat # and ## as preprocessing tokens.
    
    void SetAsCpp();  ///< Behave more like a C++ lexer.
    Options(const ParseContext *pc);  ///< Construct from parse context.
  } options;
};

}  // namespace parsing
}  // namespace enigma

#endif
