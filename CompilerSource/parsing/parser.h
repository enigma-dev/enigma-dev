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
class AstBuilder {
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

  template <typename T1, typename T2>
  bool map_contains(const std::unordered_map<T1, T2> &map, const T1 &value);

  template <typename T2, typename T1, typename = std::enable_if_t<std::is_base_of_v<T1, T2>>>
  std::unique_ptr<T2> dynamic_unique_pointer_cast(std::unique_ptr<T1> value);

  template <typename... Messages>
  bool require_any_of(std::initializer_list<TokenType> tt, Messages &&...messages);

  template <typename... Messages>
  bool require_token(TokenType tt, Messages &&...messages);

  bool is_class_key(const Token &tok);

  bool next_is_class_key();

  bool is_cv_qualifier(const Token &tok);

  bool next_is_cv_qualifier();

  bool is_ref_qualifier(const Token &tok);

  bool next_is_ref_qualifier();

  std::string read_required_operatorkw();

  bool is_operatorkw(const Token &tok);

  bool next_is_operatorkw();

  bool is_user_defined_type(const Token &tok);

  bool next_is_user_defined_type();

  bool is_type_specifier(const Token &tok);

  bool next_is_type_specifier();

  bool maybe_nested_name(const Token &tok);

  bool next_maybe_nested_name();

  bool is_template_type(const Token &tok);

  bool is_template_type(jdi::definition *def);

  bool next_is_template_type();

  bool maybe_ptr_decl_operator(const Token &tok);

  bool next_maybe_ptr_decl_operator();

  bool is_decl_specifier(const Token &tok);

  std::size_t sizeof_builtin_type(std::string_view type);

  std::pair<std::size_t, std::size_t> jdi_decflag_bitmask(std::string_view tok);

  jdi::definition *require_defined_type(const Token &tok);

  jdi::definition *require_scope_type(const Token &tok);

  jdi::definition_scope *require_scope_type(jdi::definition *def, const Token &tok);

  void MaybeConsumeSemicolon();

  bool is_start_of_initializer(const Token &tok);

  bool next_is_start_of_initializer();

  bool is_start_of_id_expression(const Token &tok);

  bool next_is_start_of_id_expression();

  bool maybe_functional_cast(const Token &tok);

  bool next_maybe_functional_cast();

  std::unique_ptr<AST::DeclarationStatement> parse_declarations(
      AST::DeclarationStatement::StorageClass sc, FullType &ft, AST::DeclaratorType decl_type, bool parse_unbounded,
      std::vector<AST::DeclarationStatement::Declaration> decls, bool already_parsed_first = false);

  void maybe_infer_int(FullType &type);

  const Token &current_token();

  std::unique_ptr<AST::Node> TryParseConstantExpression();

  std::unique_ptr<AST::Node> TryParseArrayBoundsExpression(Declarator *decl, bool outside_nested);

  jdi::definition *TryParseNoexceptSpecifier();

  void TryParseParametersAndQualifiers(Declarator *decl, bool outside_nested, bool did_consume_paren,
                                       bool maybe_expression);

  jdi::definition *TryParseTypeName();

  AstBuilder(Lexer *lexer, ErrorHandler *herr);

  jdi::definition *TryParseIdExpression(Declarator *decl);

  jdi::definition *TryParseDecltype();

  void TryParseTemplateArgs(jdi::definition *def);

  jdi::definition *TryParseTypenameSpecifier();

  jdi::definition *TryParsePrefixIdentifier(Declarator *decl = nullptr, bool is_declarator = false);

  jdi::definition *TryParseNestedNameSpecifier(jdi::definition *scope, Declarator *decl = nullptr,
                                               bool is_declarator = false);

  bool matches_token_type(jdi::definition *def, const Token &tok);

  void TryParseElaboratedName(FullType *type);

  bool contains_decflag_bitmask(std::size_t combined, std::string_view name);

  void maybe_assign_full_type(FullType *type, jdi::definition *def, Token token);

  jdi::definition *get_builtin(std::string_view name);

  void TryParseTypeSpecifier(FullType *type, bool &first_signed);

  void TryParseTypeSpecifierSeq(FullType *type);

  void TryParsePtrOperator(FullType *type);

  void TryParseMaybeNestedPtrOperator(FullType *type);

  FullType TryParseTypeID();

  void TryParseDeclSpecifier(FullType *type, bool &first_signed);

  void TryParseDeclSpecifierSeq(FullType *type);

  std::unique_ptr<AST::Node> TryParsePtrDeclarator(FullType *type, AST::DeclaratorType is_abstract,
                                                   bool maybe_expression = false);

  std::unique_ptr<AST::Node> TryParseNoPtrDeclarator(FullType *type, AST::DeclaratorType is_abstract,
                                                     bool maybe_expression = false);

  void TryParseDeclarator(FullType *type, AST::DeclaratorType is_abstract = AST::DeclaratorType::NON_ABSTRACT);

  AST::InitializerNode TryParseExprOrBracedInitList(bool is_init_clause, bool in_init_list);

  AST::BraceOrParenInitNode TryParseInitializerList(TokenType closing);

  AST::BraceOrParenInitNode TryParseBraceInitializer();

  AST::InitializerNode TryParseInitializer(bool allow_paren_init = true);

  std::unique_ptr<AST::Node> TryParseDeclarations(bool parse_unbounded);

  std::unique_ptr<AST::Node> TryParseNewExpression(bool is_global);

  std::unique_ptr<AST::Node> TryParseDeleteExpression(bool is_global);

  std::unique_ptr<AST::Node> TryParseIdExpression();

  std::unique_ptr<AST::Node> TryParseOperand();

  static bool ShouldAcceptPrecedence(const OperatorPrecedence &prec, int target_prec);

  std::unique_ptr<AST::Node> TryParseExpression(int precedence, std::unique_ptr<AST::Node> operand = nullptr);

  std::unique_ptr<AST::BinaryExpression> TryParseBinaryExpression(int precedence, std::unique_ptr<AST::Node> operand);

  std::unique_ptr<AST::UnaryPostfixExpression> TryParseUnaryPostfixExpression(int precedence,
                                                                              std::unique_ptr<AST::Node> operand);

  std::unique_ptr<AST::TernaryExpression> TryParseTernaryExpression(int precedence, std::unique_ptr<AST::Node> operand);

  std::unique_ptr<AST::BinaryExpression> TryParseSubscriptExpression(int precedence,
                                                                     std::unique_ptr<AST::Node> operand);

  std::unique_ptr<AST::FunctionCallExpression> TryParseFunctionCallExpression(int precedence,
                                                                              std::unique_ptr<AST::Node> operand);

  std::unique_ptr<AST::Node> TryParseControlExpression(SyntaxMode mode_);

  std::unique_ptr<AST::Node> TryParseDeclOrTypeExpression();

  std::unique_ptr<AST::Node> TryParseStatement();

  std::unique_ptr<AST::Node> ParseCFStmtBody();

  bool next_is_decl_specifier();

  std::unique_ptr<AST::Node> ParseStatementOrBlock();

  std::unique_ptr<AST::CodeBlock> ParseCode();

  std::unique_ptr<AST::CodeBlock> ParseCodeBlock();

  std::unique_ptr<AST::IfStatement> ParseIfStatement();

  std::unique_ptr<AST::Node> TryParseEitherFunctionalCastOrDeclaration(AST::DeclaratorType decl_type,
                                                                       bool parse_unbounded, bool maybe_c_style_cast,
                                                                       AST::DeclarationStatement::StorageClass sc);

  std::unique_ptr<AST::ForLoop> ParseForLoop();

  std::unique_ptr<AST::WhileLoop> ParseWhileLoop();

  std::unique_ptr<AST::WhileLoop> ParseUntilLoop();

  std::unique_ptr<AST::DoLoop> ParseDoLoop();

  std::unique_ptr<AST::WhileLoop> ParseRepeatStatement();

  std::unique_ptr<AST::ReturnStatement> ParseReturnStatement();

  std::unique_ptr<AST::BreakStatement> ParseBreakStatement();

  std::unique_ptr<AST::ContinueStatement> ParseContinueStatement();

  std::unique_ptr<AST::ReturnStatement> ParseExitStatement();

  std::unique_ptr<AST::SwitchStatement> ParseSwitchStatement();

  std::unique_ptr<AST::Node> ParseCaseOrDefaultStatement(bool is_default);

  std::unique_ptr<AST::WithStatement> ParseWithStatement();
};

std::unique_ptr<AST::Node> Parse(Lexer *lexer, ErrorHandler *herr);
}  // namespace enigma::parsing

#endif  // ENIGMA_COMPILER_PARSING_PARSER_h
