#ifndef ENIGMA_COMPILER_PARSING_PARSER_h
#define ENIGMA_COMPILER_PARSING_PARSER_h

#include "ast.h"
#include "full_type.h"
#include "language_frontend.h"
#include "lexer.h"
#include "precedence.h"
#include "settings.h"
#include "tokens.h"

#include <JDI/src/System/builtins.h>
#include <memory>

namespace enigma::parsing {

class AstBuilderTestAPI {
 public:
  using SyntaxMode = setting::SyntaxMode;

  Lexer *lexer;
  ErrorHandler *herr;
  SyntaxMode mode;
  const LanguageFrontend *frontend;
  Token token;

  /**
 * @brief Store a mapping from variable name to the @c FullType of its definition
 *
 * This is designed around the assumption that EDL does not yet support namespaces, so there is no need to consider
 * stacks here. If EDL were to support namespaces, this would have to be changed to a <tt> std::stack<...> </tt> and the
 * namespace or nested scope parser would have to push a new map onto the stack.
 */
  std::unordered_map<std::string_view, FullType *> declarations;

  AstBuilderTestAPI() = default;

  void initialize(Lexer *lexer, ErrorHandler *herr) {
    this->lexer = lexer;
    this->herr = herr;
    this->mode = lexer->GetContext().compatibility_opts.syntax_mode;
    this->frontend = lexer->GetContext().language_fe;
    token = lexer->ReadToken();
  }

  virtual std::unique_ptr<AST::Node> TryParseStatement() = 0;
  virtual std::unique_ptr<AST::CodeBlock> ParseCode() = 0;
  virtual const Token &current_token() = 0;
  virtual std::unique_ptr<AST::CodeBlock> ParseCodeBlock() = 0;
  virtual FullType TryParseTypeID() = 0;

  virtual ~AstBuilderTestAPI() = default;
};

AstBuilderTestAPI *CreateBuilder();
std::unique_ptr<AST::Node> Parse(Lexer *lexer, ErrorHandler *herr);

}  // namespace enigma::parsing

#endif  // ENIGMA_COMPILER_PARSING_PARSER_h
