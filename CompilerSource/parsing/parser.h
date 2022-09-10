#ifndef ENIGMA_COMPILER_PARSING_PARSER_h
#define ENIGMA_COMPILER_PARSING_PARSER_h

#include "ast.h"
#include "language_frontend.h"
#include "settings.h"
#include "lexer.h"

namespace enigma::parsing {

std::unique_ptr<AST::Node> Parse(Lexer *lexer, ErrorHandler *herr);

} // namespace enigma::parsing

#endif  // ENIGMA_COMPILER_PARSING_PARSER_h
