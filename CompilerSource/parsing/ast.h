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

#ifndef ENIGMA_COMPILER_PARSING_AST_h
#define ENIGMA_COMPILER_PARSING_AST_h

#include "error_reporting.h"
#include "lexer.h"
#include "tokens.h"

#include <string>
#include <memory>
#include <vector>

namespace enigma {
namespace parsing {

struct AST {
  // Used to adapt to current single-error syntax checking interface.
  ErrorCollector herr;
  // A lexed (tokenized) view of the code.
  const std::unique_ptr<Lexer> lexer;
  // The raw input code, owned by the lexer.
  const std::string &code;

  bool HasError() { return !herr.errors.empty(); }
  std::string ErrorString() {
    if (herr.errors.empty()) return "No error";
    return herr.errors.front().ToString();
  }

  // Utility routine: Apply this AST to a specified instance.
  void ApplyTo(int instance_id);

  // Parses the given code, returning an AST*. The resulting AST* is never null.
  // If syntax errors were encountered, they are stored within the AST.
  static AST Parse(std::string code, const ParseContext *ctex);

  // Disallow copy. Our tokens point into our code.
  AST(const AST &) = delete;
  AST(AST &&other);

 private:
  // Constructs an AST from the code it will parse. Does not initiate parse.
  AST(std::string &&code_, const ParseContext *ctex):
      lexer(std::make_unique<Lexer>(std::move(code_), ctex, &herr)),
      code(lexer->GetCode()) {}
};

}  // namespace parsing
}  // namespace enigma

#endif  // ENIGMA_COMPILER_PARSING_AST_h
