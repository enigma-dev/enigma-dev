/** Copyright (C) 2025
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

#ifndef ENIGMA_COMPILER_PARSING_CLANG_PARSER_BRIDGE_H
#define ENIGMA_COMPILER_PARSING_CLANG_PARSER_BRIDGE_H

#include "tokens.h"
#include "full_type.h"
#include "ast.h"
#include "language_frontend.h"
#include <clang-c/Index.h>
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace enigma::parsing {

class ClangParserBridge {
public:
  ClangParserBridge(LanguageFrontend* frontend);
  ~ClangParserBridge();

  // Parse a declaration using Clang
  // Converts tokens to C++ code, parses with Clang, converts back to ENIGMA AST
  AST::PNode parse_declaration_with_clang(const std::vector<Token>& tokens);

  // Parse an expression using Clang
  AST::PNode parse_expression_with_clang(const std::vector<Token>& tokens);

  // Parse a type specifier and declarator using Clang
  // Returns a properly constructed FullType with components in correct order
  bool parse_type_with_clang(const std::vector<Token>& tokens, FullType& out_type);

private:
  LanguageFrontend* frontend_;
  CXIndex index_;
  
  // Cache for parsed results
  struct CacheEntry {
    std::string code;
    AST::PNode node;
    FullType type;
  };
  std::map<std::string, CacheEntry> cache_;

  // Convert tokens to C++ code string
  std::string tokens_to_cpp_code(const std::vector<Token>& tokens);

  // Convert Clang AST to ENIGMA AST
  AST::PNode convert_cursor_to_ast(CXCursor cursor);

  // Convert Clang type to FullType
  bool convert_type_to_fulltype(CXType type, CXCursor cursor, FullType& out_type);

  // Helper: Convert CXCursor to ENIGMA binary expression
  AST::PNode convert_binary_operator(CXCursor cursor);

  // Helper: Convert CXCursor to ENIGMA unary expression
  AST::PNode convert_unary_operator(CXCursor cursor);

  // Helper: Convert CXCursor to ENIGMA array subscript (binary expression)
  AST::PNode convert_array_subscript(CXCursor cursor);

  // Helper: Get token type from Clang binary operator
  TokenType get_binary_operator_token_type(CXCursor cursor);

  // Helper: Get token type from Clang unary operator
  TokenType get_unary_operator_token_type(CXCursor cursor);

  // Helper: Extract declarator components from Clang type
  void extract_declarator_components(CXType type, Declarator& decl);
};

} // namespace enigma::parsing

#endif // ENIGMA_COMPILER_PARSING_CLANG_PARSER_BRIDGE_H
