/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "precedence.h"
#include "../parsing/tokens.h"
#include "object_storage.h"
#include <set>

void parser_init();
string file_parse(string filename,string outname);
void parser_main(enigma::parsing::AST *ast, const enigma::parsing::ParseContext &ctex);
int parser_secondary(CompileState &state, ParsedCode *pev);
void print_to_file(const enigma::parsing::ParseContext &ctex,string,string,const unsigned int,const varray<string>&,int,ofstream&);

namespace enigma::parsing {
enum class SyntaxMode {
  STRICT, QUIRKS, GML
};

/// @brief Parser class for the EDL language
///
/// Each parser in the class follows the contract that each callee's caller would have updated @c token to point to the
/// next token, and the callee would then decide if its necessary to consume that token or not. A callee will never
/// consume a token before considering the type of the token. Thus, it is necessary in the constructor that @c token be
/// set to the first token in the sequence.
///
class AstBuilder {
  Lexer *lexer;
  ErrorHandler *herr;
  Token token;

  SyntaxMode mode;

  AstBuilder(SyntaxMode mode = SyntaxMode::STRICT);
  AstBuilder(Lexer *lexer, ErrorHandler *herr, SyntaxMode mode = SyntaxMode::STRICT);

  template <typename T1, typename T2>
  bool map_contains(const std::unordered_map<T1, T2> &map, const T1 &value) {
    return map.find(value) != map.end();
  }

  template <typename T2, typename T1, typename = std::enable_if_t<std::is_base_of_v<T1, T2>>>
  std::unique_ptr<T2> dynamic_unique_pointer_cast(std::unique_ptr<T1> value) {
    return std::unique_ptr<T2>(dynamic_cast<T2*>(value.release()));
  }

  int GetPrecedence(TokenType token);

 // public for now, so that tests can be done
 public:
  /// <!-- Expression parsers -->
  std::unique_ptr<AST::Node> TryParseOperand();
  std::unique_ptr<AST::Node> TryParseExpression(int precedence);
  std::unique_ptr<AST::BinaryExpression> TryParseBinaryExpression(int precedence, std::unique_ptr<AST::Node> operand);
  std::unique_ptr<AST::UnaryPostfixExpression> TryParseUnaryPostfixExpression(int precedence, std::unique_ptr<AST::Node> operand);
  std::unique_ptr<AST::TernaryExpression> TryParseTernaryExpression(int precedence, std::unique_ptr<AST::Node> operand);
  std::unique_ptr<AST::BinaryExpression> TryParseSubscriptExpression(int precedence, std::unique_ptr<AST::Node> operand);
  std::unique_ptr<AST::FunctionCallExpression> TryParseFunctionCallExpression(int precedence, std::unique_ptr<AST::Node> operand);

  std::unique_ptr<AST::Node> TryParseControlExpression();

  /// <!-- Statement parsers -->
  std::unique_ptr<AST::Node> TryReadStatement();
  std::unique_ptr<AST::Node> ParseCFStmtBody();
  std::unique_ptr<AST::CodeBlock> ParseCodeBlock();
  std::unique_ptr<AST::IfStatement> ParseIfStatement();
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
  std::unique_ptr<AST::CaseStatement> ParseCaseStatement();
  std::unique_ptr<AST::CaseStatement> ParseDefaultStatement();
  std::unique_ptr<AST::WithStatement> ParseWithStatement();

  /// Reads if()/for()/while()/with()/switch() statements.
  ///
  /// Syntax rule: In quirks mode, all binary operators are presumed to
  /// extend a parenthetical expression, EXCLUDING the star operator but
  /// INCLUDING the ampersand operator. The rationale for this is that
  /// `if (expr) *stmt = ...;` is far more likely to appear in code than
  /// `if (expr) &expr...;`, and `if (expr1) & (expr2)` is far more likely
  /// to appear than `if (expr2) * (expr2)`. In quirks mode, use of either
  /// token will result in a warning.
  ///
  /// In strict mode, only the first parenthesized expression is taken.
  ///
  /// In GML mode, a complete expression is read; unary * and & do not
  /// exist and so are not ambiguous (same for prefix ++ and --).
  template<typename ExpNode>
  std::unique_ptr<ExpNode> ReadConditionalStatement() {

  }
};

} // namespace enigma::parsing