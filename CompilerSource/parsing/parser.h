#ifndef ENIGMA_COMPILER_PARSING_PARSER_h
#define ENIGMA_COMPILER_PARSING_PARSER_h

#include "ast.h"
#include "language_frontend.h"
#include "lexer.h"

#include <memory>

namespace enigma::parsing {

enum class SyntaxMode {
  STRICT, QUIRKS, GML
};

std::unique_ptr<AST::Node> Parse(Lexer *lexer, ErrorHandler *herr);

} // namespace enigma::parsing

#endif  // ENIGMA_COMPILER_PARSING_PARSER_h
