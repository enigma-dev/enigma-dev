#include "parser.h"

#include "ast.h"
#include "lexer.h"
#include "tokens.h"
#include "precedence.h"

#include <memory>

namespace enigma::parsing {

class AstBuilder {

  Lexer *lexer;
  ErrorHandler *herr;
  SyntaxMode mode;

  Token token;

  template <typename T1, typename T2>
  bool map_contains(const std::unordered_map<T1, T2> &map, const T1 &value) {
    return map.find(value) != map.end();
  }

  template <typename T2, typename T1, typename = std::enable_if_t<std::is_base_of_v<T1, T2>>>
  std::unique_ptr<T2> dynamic_unique_pointer_cast(std::unique_ptr<T1> value) {
    return std::unique_ptr<T2>(dynamic_cast<T2*>(value.release()));
  }

 public:

  AstBuilder(Lexer *lexer, ErrorHandler *herr, SyntaxMode mode): lexer{lexer}, herr{herr}, mode{mode} {
    token = lexer->ReadToken();
  }

  /// Parse an operand--this includes variables, literals, arrays, and
  /// unary expressions on these.
  std::unique_ptr<AST::Node> TryParseOperand() {
    switch (token.type) {
      case TT_BEGINBRACE: case TT_ENDBRACE:
      case TT_ENDPARENTH: case TT_ENDBRACKET:
      case TT_ENDOFCODE:
        return nullptr;

      case TT_SEMICOLON:
        return std::make_unique<AST::CodeBlock>();
      case TT_COLON:
        herr->ReportError(token, "Expected label or ternary expression before colon");
        token = lexer->ReadToken();
        return nullptr;
      case TT_COMMA:
        herr->ReportError(token, "Expected expression before comma");
        token = lexer->ReadToken();
        return nullptr;
      case TT_ASSIGN:
      case TT_ASSOP:
        herr->ReportError(token, "Expected assignable expression before assignment operator");
        token = lexer->ReadToken();
        return nullptr;
      case TT_DOT: case TT_ARROW:
        herr->ReportError(token, "Expected expression before member access");
        token = lexer->ReadToken();
        return nullptr;
      case TT_DOT_STAR: case TT_ARROW_STAR:
        herr->ReportError(token, "Expected expression before pointer-to-member");
        token = lexer->ReadToken();
        return nullptr;
      case TT_PERCENT: case TT_PIPE: case TT_CARET:
      case TT_AND: case TT_OR: case TT_XOR: case TT_DIV: case TT_MOD:
      case TT_EQUALS: case TT_SLASH: case TT_EQUALTO: case TT_NOTEQUAL:
      case TT_LESS: case TT_GREATER: case TT_LESSEQUAL: case TT_THREEWAY:
      case TT_GREATEREQUAL: case TT_LSH: case TT_RSH:
        herr->Error(token) << "Expected expression before binary operator `" << token.content << '`';
        token = lexer->ReadToken();
        return nullptr;

      case TT_QMARK:
        herr->Error(token) << "Expected expression before ternary operator ?";
        token = lexer->ReadToken();
        return nullptr;

      case TT_NOT: case TT_BANG: case TT_PLUS: case TT_MINUS:
      case TT_STAR: case TT_AMPERSAND: case TT_TILDE:
      case TT_INCREMENT: case TT_DECREMENT: {
        Token unary_op = token;
        token = lexer->ReadToken();
        if (auto exp = TryParseExpression(Precedence::kUnaryPrefix))
          return std::make_unique<AST::UnaryPrefixExpression>(std::move(exp), unary_op.type);
        herr->Error(unary_op) << "Expected expression following unary operator";
        break;
      }

      case TT_BEGINPARENTH: {
        token = lexer->ReadToken();
        auto exp = TryParseExpression(Precedence::kAll);
        if (token.type == TT_ENDPARENTH) {
          token = lexer->ReadToken();
        } else {
          herr->Error(token) << "Expected closing parenthesis before " << token;
        }
        return std::make_unique<AST::Parenthetical>(std::move(exp));
      }
      case TT_BEGINBRACKET: {
        token = lexer->ReadToken();
        std::vector<std::unique_ptr<AST::Node>> elements;
        while (std::unique_ptr<AST::Node> element = TryParseExpression(Precedence::kComma)) {
          elements.push_back(std::move(element));
          if (token.type != TT_COMMA) break;
          token = lexer->ReadToken();
        }
        if (token.type == TT_ENDBRACKET) {
          token = lexer->ReadToken();
        } else {
          herr->Error(token) << "Expected closing `]` for array";
        }
        return std::make_unique<AST::Array>(std::move(elements));
      }

      case TT_IDENTIFIER:
      case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
      case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT: {
        Token res = token;
        token = lexer->ReadToken();
        return std::make_unique<AST::Literal>(std::move(res));
      }

      case TT_SCOPEACCESS:

      case TT_TYPE_NAME:

      case TT_LOCAL:
      case TT_GLOBAL:

      case TT_RETURN:   case TT_EXIT:   case TT_BREAK:   case TT_CONTINUE:
      case TT_S_SWITCH: case TT_S_CASE: case TT_S_DEFAULT:
      case TT_S_FOR:    case TT_S_DO:   case TT_S_WHILE: case TT_S_UNTIL:
      case TT_S_REPEAT: case TT_S_IF:   case TT_S_THEN:  case TT_S_ELSE:
      case TT_S_WITH:   case TT_S_TRY:  case TT_S_CATCH:
      case TT_S_NEW:    case TT_S_DELETE:

      case TT_CLASS:    case TT_STRUCT:
      case TTM_WHITESPACE: case TTM_CONCAT: case TTM_STRINGIFY:

      case TT_ERROR:
        return nullptr;
    }
  }

  static bool ShouldAcceptPrecedence(const OperatorPrecedence &prec,
                                     int target_prec) {
    return target_prec >= prec.precedence ||
              (target_prec == prec.precedence &&
                  prec.associativity == Associativity::RTL);
  }

  std::unique_ptr<AST::Node> TryParseExpression(int precedence) {
    if (auto operand = TryParseOperand()) {
      // TODO: Handle binary operators, unary postfix operators
      // (including function calls, array indexing, etc).
      // XXX: Maybe handle TT_IDENTIFIER here when `operand` names a type
      // to parse a declaration as an expression. This is a bold move, but
      // more robust than handling it in TryParseExpression.
      while (token.type != TT_ENDOFCODE) {
        if (auto find_binop = Precedence::kBinaryPrec.find(token.type); find_binop != Precedence::kBinaryPrec.end()) {
          if (!ShouldAcceptPrecedence(find_binop->second, precedence)) {
            break;
          }
          operand = TryParseBinaryExpression(precedence, std::move(operand));
        } else if (map_contains(Precedence::kUnaryPostfixPrec, token.type)) {
          if (precedence < Precedence::kUnaryPostfix) {
            break;
          }
          operand = TryParseBinaryExpression(precedence, std::move(operand));
        } else if (map_contains(Precedence::kTernaryPrec, token.type)) {
          if (precedence < Precedence::kTernary) {
            break;
          }
          operand = TryParseTernaryExpression(precedence, std::move(operand));
        } else if (token.type == TT_BEGINBRACKET) {
          if (precedence < Precedence::kUnaryPostfix) {
            break;
          }
          operand = TryParseSubscriptExpression(precedence, std::move(operand));
        } else if (token.type == TT_BEGINPARENTH) {
          operand = TryParseFunctionCallExpression(precedence, std::move(operand));
        } else {
          // If we reach this point, then the token that we are at is not an operator, otherwise it would have been picked
          // up by one of the branches, thus we need to break from the loop
          break;
        }
      }
      return operand;
    }
    return nullptr;
  }

  std::unique_ptr<AST::BinaryExpression> TryParseBinaryExpression(int precedence, std::unique_ptr<AST::Node> operand) {
    while (map_contains(Precedence::kBinaryPrec, token.type) &&
           precedence >= Precedence::kBinaryPrec[token.type].precedence && token.type != TT_ENDOFCODE) {
      Token oper = token;
      OperatorPrecedence rule = Precedence::kBinaryPrec[token.type];
      token = lexer->ReadToken(); // Consume the operator

      auto right = (rule.associativity == Associativity::LTR)
          ? TryParseExpression(rule.precedence - 1)
          : (rule.associativity == Associativity::RTL)
              ? TryParseExpression(rule.precedence)
              : nullptr;

      operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), oper.type);
    }

    return dynamic_unique_pointer_cast<AST::BinaryExpression>(std::move(operand));
  }

  std::unique_ptr<AST::UnaryPostfixExpression> TryParseUnaryPostfixExpression(int precedence, std::unique_ptr<AST::Node> operand) {
    while (Precedence::kUnaryPostfixPrec.find(token.type) != Precedence::kUnaryPostfixPrec.end() &&
           precedence >= Precedence::kUnaryPostfixPrec[token.type].precedence && token.type != TT_ENDOFCODE) {
      Token oper = token;
      OperatorPrecedence rule = Precedence::kBinaryPrec[token.type];
      token = lexer->ReadToken(); // Consume the operator

      operand = std::make_unique<AST::UnaryPostfixExpression>(std::move(operand), oper.type);
    }

    return dynamic_unique_pointer_cast<AST::UnaryPostfixExpression>(std::move(operand));
  }

  std::unique_ptr<AST::TernaryExpression> TryParseTernaryExpression(int precedence, std::unique_ptr<AST::Node> operand) {
    Token oper = token;
    token = lexer->ReadToken(); // Consume the operator

    auto middle = TryParseExpression(Precedence::kBoolOr);

    if (token.type != TT_COLON) {
      herr->Error(token) << "Expected colon after expression in conditional operator";
    } else {
      token = lexer->ReadToken();
    }

    auto right = TryParseExpression(Precedence::kTernary);
    operand = std::make_unique<AST::TernaryExpression>(std::move(operand), std::move(middle), std::move(right));

    return dynamic_unique_pointer_cast<AST::TernaryExpression>(std::move(operand));
  }

  std::unique_ptr<AST::BinaryExpression> TryParseSubscriptExpression(int precedence, std::unique_ptr<AST::Node> operand) {
    while (token.type == TT_BEGINBRACKET) {
      Token oper = token;
      token = lexer->ReadToken(); // Consume the operator

      auto right = TryParseExpression(Precedence::kMin);
      operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), oper.type);
      if (token.type != TT_ENDBRACKET) {
        herr->Error(token) << "Expected closing bracket (']') at the end of array subscript";
      } else {
        token = lexer->ReadToken();
      }
    }

    return dynamic_unique_pointer_cast<AST::BinaryExpression>(std::move(operand));
  }
  std::unique_ptr<AST::FunctionCallExpression> TryParseFunctionCallExpression(int precedence, std::unique_ptr<AST::Node> operand) {
    while (token.type == TT_BEGINPARENTH) {
      Token oper = token;
      token = lexer->ReadToken(); // Consume the operator

      std::vector<std::unique_ptr<AST::Node>> arguments{};
      while (token.type != TT_ENDPARENTH && token.type != TT_ENDOFCODE) {
        arguments.emplace_back(TryParseExpression(Precedence::kTernary));
        if (token.type != TT_COMMA && token.type != TT_ENDPARENTH) {
          herr->Error(token) << "Expected ',' or ')' after function argument";
        } else if (token.type == TT_COMMA) {
          token = lexer->ReadToken();
        }
      }

      if (token.type != TT_ENDPARENTH) {
        herr->Error(token) << "Expected ')' after function call";
      } else {
        token = lexer->ReadToken();
      }

      operand = std::make_unique<AST::FunctionCallExpression>(std::move(operand), std::move(arguments));
    }

    return dynamic_unique_pointer_cast<AST::FunctionCallExpression>(std::move(operand));
  }

  std::unique_ptr<AST::Node> TryParseControlExpression() {
    switch (mode) {
      case SyntaxMode::STRICT: {
        if (token.type == TT_BEGINPARENTH) {
          token = lexer->ReadToken();
        } else {
          herr->Error(token) << "Expected '(' before control expression";
        }
        auto expr = TryParseExpression(Precedence::kAll);
        if (token.type == TT_ENDPARENTH) {
          token = lexer->ReadToken();
        } else {
          herr->Error(token) << "Expected ')' after control expression";
        }
        return expr;
      }
      case SyntaxMode::QUIRKS: {
        auto operand = TryParseOperand();
        if (map_contains(Precedence::kBinaryPrec, token.type) && token.type != TT_STAR) {
          Token oper = token;
          token = lexer->ReadToken(); // Consume the token
          auto right = TryParseControlExpression();
          operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), oper.type);
        }
        // TODO: handle [] for array access and () for direct func call
        return operand;
      }
      case SyntaxMode::GML:
        return TryParseExpression(Precedence::kAll);
    }
  }

  std::unique_ptr<AST::Node> TryReadStatement() {
    switch (token.type) {
      case TTM_WHITESPACE:
      case TTM_CONCAT:
      case TTM_STRINGIFY:
        herr->ReportError(token, "Internal error: Unhandled preprocessing token");
        token = lexer->ReadToken();
        return nullptr;
      case TT_ERROR:
        herr->ReportError(token, "Internal error: Bad token");
        token = lexer->ReadToken();
        return nullptr;

      case TT_COMMA:
        herr->ReportError(token, "Expected expression before comma");
        token = lexer->ReadToken();
        return nullptr;
      case TT_ENDPARENTH:
        herr->ReportError(token, "Unmatched closing parenthesis");
        token = lexer->ReadToken();
        return nullptr;
      case TT_ENDBRACKET:
        herr->ReportError(token, "Unmatched closing bracket");
        token = lexer->ReadToken();
        return nullptr;

      case TT_SEMICOLON:
        herr->ReportWarning(token, "Statement doesn't do anything (consider using `{}` instead of `;`)");
        token = lexer->ReadToken();
        return std::make_unique<AST::CodeBlock>();

      case TT_COLON: case TT_ASSIGN: case TT_ASSOP:
      case TT_DOT: case TT_ARROW: case TT_DOT_STAR: case TT_ARROW_STAR:
      case TT_PERCENT: case TT_PIPE: case TT_CARET:
      case TT_AND: case TT_OR: case TT_XOR:
      case TT_DIV: case TT_MOD: case TT_SLASH:
      case TT_EQUALS: case TT_EQUALTO: case TT_NOTEQUAL: case TT_THREEWAY:
      case TT_LESS: case TT_GREATER: case TT_LSH: case TT_RSH:
      case TT_LESSEQUAL: case TT_GREATEREQUAL:
      case TT_QMARK:
        // Allow TryParseExpression to handle errors.
        // (Fall through.)
      case TT_PLUS: case TT_MINUS: case TT_STAR: case TT_AMPERSAND:
      case TT_NOT: case TT_BANG: case TT_TILDE:
      case TT_INCREMENT: case TT_DECREMENT:
      case TT_BEGINPARENTH: case TT_BEGINBRACKET:
      case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
      case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT:
      case TT_SCOPEACCESS:
        return TryParseExpression(Precedence::kAll);

      case TT_ENDBRACE:
        return nullptr;

      case TT_BEGINBRACE: {
        auto code = ParseCodeBlock();
        if (token.type != TT_ENDBRACE) {
          herr->ReportError(token, "Expected closing brace");
        }
        return code;
      }

      case TT_IDENTIFIER: {
        // TODO: Verify that template instantiations are covered by kScope
        auto name = TryParseExpression(Precedence::kScope);
        // TODO:
        // if (name.IsTypeIdentifier()) {
        //   // Handle reading decl-specifier-seq and init-declarator-list
        //   return ReadDeclaration(name);
        // } else {
        //   return TryParseBinaryExpression(name, Precedence::kAll);
        // }
        return name;
      }

      case TT_TYPE_NAME:  // TODO: rename TT_DECLARATOR, exclude var/variant/C++ classes,
                          // include cv-qualifiers, storage-specifiers, etc
      case TT_LOCAL:      // XXX: Treat as storage-specifiers?
      case TT_GLOBAL:

      case TT_RETURN: return ParseReturnStatement();
      case TT_EXIT: return ParseExitStatement();
      case TT_BREAK: return ParseBreakStatement();
      case TT_CONTINUE: return ParseContinueStatement();
      case TT_S_SWITCH: return ParseSwitchStatement();
      case TT_S_REPEAT: return ParseRepeatStatement();
      case TT_S_CASE:   return ParseCaseStatement();
      case TT_S_DEFAULT: return ParseDefaultStatement();
      case TT_S_FOR: return ParseForLoop();
      case TT_S_IF: return ParseIfStatement();
      case TT_S_DO: return ParseDoLoop();
      case TT_S_WHILE: return ParseWhileLoop();
      case TT_S_UNTIL: return ParseUntilLoop();
      case TT_S_WITH: return ParseWithStatement();

      case TT_S_THEN:
        herr->ReportError(token, "`then` statement not paired with an `if`");
        token = lexer->ReadToken();
        return nullptr;
      case TT_S_ELSE:
        herr->ReportError(token, "`else` statement not paired with an `if`");
        token = lexer->ReadToken();
        return nullptr;

      case TT_S_TRY: case TT_S_CATCH:
      case TT_S_NEW: case TT_S_DELETE:


      case TT_ENDOFCODE: return nullptr;
        ;
    }
  }

  // Parse control flow statement body
  std::unique_ptr<AST::Node> ParseCFStmtBody() {
    return TryReadStatement();
  }

  // TODO: the following.
  std::unique_ptr<AST::CodeBlock> ParseCodeBlock() {
  }

  std::unique_ptr<AST::IfStatement> ParseIfStatement() {
    token = lexer->ReadToken();
    auto condition = TryParseControlExpression();
    if (token.type == TT_S_THEN) {
      if (mode == SyntaxMode::STRICT) {
        herr->Warning(token) << "Use of `then` keyword in if statement";
      }
      token = lexer->ReadToken();
    }

    auto true_branch = ParseCFStmtBody();

    if (token.type == TT_S_ELSE) {
      token = lexer->ReadToken();
      auto false_branch = ParseCFStmtBody();
      return std::make_unique<AST::IfStatement>(std::move(condition), std::move(true_branch), std::move(false_branch));
    } else {
      return std::make_unique<AST::IfStatement>(std::move(condition), std::move(true_branch), nullptr);
    }
  }

  std::unique_ptr<AST::ForLoop> ParseForLoop() {

  }

  std::unique_ptr<AST::WhileLoop> ParseWhileLoop() {
    token = lexer->ReadToken();
    auto condition = TryParseControlExpression();
    auto body = ParseCFStmtBody();

    return std::make_unique<AST::WhileLoop>(std::move(condition), std::move(body), AST::WhileLoop::Kind::WHILE);
  }

  std::unique_ptr<AST::WhileLoop> ParseUntilLoop() {
    token = lexer->ReadToken();
    auto condition = TryParseControlExpression();
    auto body = ParseCFStmtBody();

    return std::make_unique<AST::WhileLoop>(std::move(condition), std::move(body), AST::WhileLoop::Kind::UNTIL);
  }

  std::unique_ptr<AST::DoLoop> ParseDoLoop() {
    token = lexer->ReadToken();
    auto body = ParseCFStmtBody();

    Token kind = token;
    if (token.type == TT_S_WHILE || token.type == TT_S_UNTIL) {
      token = lexer->ReadToken();
    } else {
      herr->Error(token) << "Expected `while` or `until` after do loop body";
    }

    auto condition = TryParseControlExpression();

    return std::make_unique<AST::DoLoop>(std::move(body), std::move(condition), kind.type == TT_S_UNTIL);
  }

  std::unique_ptr<AST::WhileLoop> ParseRepeatStatement() {
    token = lexer->ReadToken();
    auto condition = TryParseControlExpression();
    auto body = ParseCFStmtBody();

    return std::make_unique<AST::WhileLoop>(std::move(condition), std::move(body), AST::WhileLoop::Kind::REPEAT);
  }

  std::unique_ptr<AST::ReturnStatement> ParseReturnStatement() {
    token = lexer->ReadToken();
    auto value = TryParseExpression(Precedence::kAll);

    return std::make_unique<AST::ReturnStatement>(std::move(value), false);
  }

  std::unique_ptr<AST::BreakStatement> ParseBreakStatement() {
    token = lexer->ReadToken(); // Consume the break
    if (token.type != TT_DECLITERAL && token.type != TT_BINLITERAL &&
        token.type != TT_OCTLITERAL && token.type != TT_HEXLITERAL) {
      return std::make_unique<AST::BreakStatement>(nullptr);
    } else {
      return std::make_unique<AST::BreakStatement>(TryParseOperand());
    }
  }

  std::unique_ptr<AST::ContinueStatement> ParseContinueStatement() {
    token = lexer->ReadToken(); // Consume the break
    if (token.type != TT_DECLITERAL && token.type != TT_BINLITERAL &&
        token.type != TT_OCTLITERAL && token.type != TT_HEXLITERAL) {
      return std::make_unique<AST::ContinueStatement>(nullptr);
    } else {
      return std::make_unique<AST::ContinueStatement>(TryParseOperand());
    }
  }

  std::unique_ptr<AST::ReturnStatement> ParseExitStatement() {
    token = lexer->ReadToken();
    return std::make_unique<AST::ReturnStatement>(nullptr, true);
  }

  std::unique_ptr<AST::SwitchStatement> ParseSwitchStatement() {

  }

  std::unique_ptr<AST::CaseStatement> ParseCaseStatement() {

  }

  std::unique_ptr<AST::CaseStatement> ParseDefaultStatement() {

  }

  std::unique_ptr<AST::WithStatement> ParseWithStatement() {
    token = lexer->ReadToken();
    auto object = TryParseControlExpression();
    auto body = ParseCFStmtBody();

    return std::make_unique<AST::WithStatement>(std::move(object), std::move(body));
  }

  };  // class AstBuilder
}  // namespace enigma::parsing
