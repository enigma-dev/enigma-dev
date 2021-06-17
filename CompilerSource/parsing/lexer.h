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

  Lexer(std::string &&code_, const ParseContext *ctx, ErrorHandler *herr_):
      code(code_), context(ctx), herr(herr_) {}
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

  std::string code;
  size_t pos = 0;
  size_t line_number = 1;
  size_t line_last_evaluated_at_ = 0;
  size_t last_line_position = (size_t) -1;
  const ParseContext *context;
  std::deque<OpenMacro> open_macros;
  ErrorHandler *herr;
};

}  // namespace parsing
}  // namespace enigma

#endif
