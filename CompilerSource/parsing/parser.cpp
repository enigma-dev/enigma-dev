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
LanguageFrontend *frontend;

Token token;

template <typename T1, typename T2>
bool map_contains(const std::unordered_map<T1, T2> &map, const T1 &value) {
  return map.find(value) != map.end();
}

template <typename T2, typename T1, typename = std::enable_if_t<std::is_base_of_v<T1, T2>>>
std::unique_ptr<T2> dynamic_unique_pointer_cast(std::unique_ptr<T1> value) {
  return std::unique_ptr<T2>(dynamic_cast<T2*>(value.release()));
}

template <typename... Messages>
bool require_any_of(std::initializer_list<TokenType> tt, Messages &&...messages) {
  if (std::any_of(tt.begin(), tt.end(), [this](TokenType tt) { return token.type == tt; })) {
    token = lexer->ReadToken();
    return true;
  } else {
    if constexpr (sizeof...(messages) > 0) {
      (herr->Error(token) << ... << messages);
    } else {
      herr->Error(token) << "Unexpected token: `" << token.ToString() << '`';
    }
    return false;
  }
}

template <typename ... Messages>
bool require_token(TokenType tt, Messages &&...messages) {
  return require_any_of({tt}, std::forward<Messages>(messages)...);
}

bool is_class_key(const Token &tok) {
  switch (tok.type) {
    case TT_CLASS:
    case TT_STRUCT:
    case TT_UNION:
      return true;

    default:
      return false;
  }
}

bool next_is_class_key() {
  return is_class_key(token);
}

bool is_cv_qualifier(const Token &tok) {
  return tok.type == TT_CONST || tok.type == TT_VOLATILE;
}

bool next_is_cv_qualifier() {
  return is_cv_qualifier(token);
}

bool is_ref_qualifier(const Token &tok) {
  return tok.type == TT_AMPERSAND || tok.type == TT_AND;
}

bool next_is_ref_qualifier() {
  return is_ref_qualifier(token);
}

bool require_operatorkw() {
  if (next_is_operatorkw()) {
    if (token.type == TT_S_NEW || token.type == TT_S_DELETE) {
      TokenType type = token.type;
      token = lexer->ReadToken();
      if (token.type == TT_BEGINBRACKET) {
        token = lexer->ReadToken();
        require_token(TT_ENDBRACKET, "Expected ']' after '[' in '", type == TT_S_NEW ? "new" : "delete", "[]'");
      }
    } else if (token.type == TT_BEGINPARENTH) {
      token = lexer->ReadToken();
      require_token(TT_ENDPARENTH, "Expected ')' after '('");
    } else if (token.type == TT_BEGINBRACKET) {
      token = lexer->ReadToken();
      require_token(TT_ENDBRACKET, "Expected ']' after '['");
    } else {
      token = lexer->ReadToken();
    }
  } else {
    herr->Error(token) << "Expected an overloadable operator";
  }
}

bool is_operatorkw(const Token &tok) {
  switch (tok.type) {
    case TT_S_NEW:
    case TT_S_DELETE:
    case TT_CO_AWAIT:
    case TT_BEGINPARENTH:
    case TT_BEGINBRACKET:
    case TT_ARROW:
    case TT_ARROW_STAR:
    case TT_TILDE:
    case TT_BANG:
    case TT_PLUS:
    case TT_MINUS:
    case TT_STAR:
    case TT_DIV:
    case TT_PERCENT:
    case TT_AMPERSAND:
    case TT_CARET:
    case TT_PIPE:
    case TT_EQUALS:
    case TT_EQUALTO:
    case TT_NOTEQUAL:
    case TT_LESS:
    case TT_LESSEQUAL:
    case TT_GREATER:
    case TT_GREATEREQUAL:
    case TT_THREEWAY:
    case TT_AND:
    case TT_OR:
    case TT_XOR:
    case TT_LSH:
    case TT_RSH:
    case TT_INCREMENT:
    case TT_DECREMENT:
    case TT_COMMA:
      // TODO: compound assignment operators
      return true;

    default:
      return false;
  }
}

bool next_is_operatorkw() {
  return is_operatorkw(token);
}

bool is_type_specifier(const Token &tok) {
  switch (tok.type) {
    case TT_TYPE_NAME:
    case TT_TYPENAME:
    case TT_SCOPEACCESS:
    case TT_DECLTYPE:
    case TT_ENUM:
    case TT_SIGNED:
    case TT_UNSIGNED:
      return true;

    case TT_IDENTIFIER: {
      if (auto def = frontend->look_up(tok.content); def != nullptr) {
        return def->flags & (jdi::DEF_TYPENAME | jdi::DEF_CLASS | jdi::DEF_ENUM | jdi::DEF_TYPED | jdi::DEF_TEMPLATE);
      } else {
        return false;
      }
    }

    default:
      return is_cv_qualifier(tok) || is_class_key(tok);
  }
}

bool next_is_type_specifier() {
  return is_type_specifier(token);
}

bool maybe_nested_name(const Token &tok) {
  switch (tok.type) {
    case TT_SCOPEACCESS:
    case TT_IDENTIFIER:
    case TT_DECLTYPE:
      return true;

    default:
      return false;
  }
}

bool next_maybe_nested_name() {
  return maybe_nested_name(token);
}

bool is_template_type(const Token &tok) {
  if (auto def = frontend->look_up(tok.content); def != nullptr) {
    return def->flags & jdi::DEF_TEMPLATE;
  }
  return false;
}

bool next_is_template_type() {
  return is_template_type(token);
}

bool maybe_ptr_operator(const Token &tok) {
  return tok.type == TT_STAR || is_ref_qualifier(tok) || maybe_nested_name(tok);
}

bool next_maybe_ptr_operator() {
  return maybe_ptr_operator(token);
}

bool is_decl_specifier(const Token &tok) {
  switch (tok.type) {
    case TT_TYPEDEF:
    case TT_CONSTEXPR:
    case TT_CONSTINIT:
    case TT_CONSTEVAL:
    case TT_MUTABLE:
    case TT_INLINE:
    case TT_STATIC:
    case TT_THREAD_LOCAL:
    case TT_EXTERN:
      return true;

    default:
      return is_type_specifier(tok);
  }
}

bool next_is_decl_specifier() {
  return is_decl_specifier(token);
}

public:

AstBuilder(Lexer *lexer, ErrorHandler *herr, SyntaxMode mode, LanguageFrontend *frontend): lexer{lexer}, herr{herr},
  mode{mode}, frontend{frontend} {
  frontend->definitionsModified(nullptr, "");
  token = lexer->ReadToken();
}

std::unique_ptr<AST::Node> TryParseConstantExpression() {
  return TryParseExpression(Precedence::kTernary);
}

std::unique_ptr<AST::Node> TryParseArrayBoundsExpression() {
  require_token(TT_BEGINBRACKET, "Expected '[' before array bounds expression");
  if (token.type != TT_ENDBRACKET) {
    TryParseConstantExpression();
  }
  require_token(TT_ENDBRACKET, "Expected ']' after array bounds expression");
  return nullptr;
}

std::unique_ptr<AST::Node> TryParseNoexceptSpecifier() {
  require_token(TT_NOEXCEPT, "Expected 'noexcept' in noexcept specifier");
  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    TryParseConstantExpression();
    require_token(TT_ENDPARENTH, "Expected ')' after noexcept expression");
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseParametersAndQualifiers(bool did_consume_paren = false) {
  if (!did_consume_paren) {
    require_token(TT_BEGINPARENTH, "Expected '(' before function parameters");
  }
  if (token.type != TT_ENDPARENTH) {
    while (token.type != TT_ENDPARENTH) {
      TryParseDeclSpecifierSeq();
      TryParseDeclarator();

      if (token.type == TT_EQUALS) {
        token = lexer->ReadToken();
        // TODO: Parse initializer clause
      }

      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
        if (token.type == TT_ELLIPSES) {
          token = lexer->ReadToken();
          if (token.type != TT_ENDPARENTH) {
            herr->Error(token) << "Extra parameters after ellipses in function parameter";
          }
          break;
        }
      } else {
        break;
      }
    }

    if (token.type == TT_ELLIPSES) {
      token = lexer->ReadToken();
    }
  }
  require_token(TT_ENDPARENTH, "Expected ')' after function parameters");

  if (next_is_cv_qualifier()) {
    token = lexer->ReadToken();
  }
  if (next_is_ref_qualifier()) {
    token = lexer->ReadToken();
  }
  if (token.type == TT_NOEXCEPT) {
    TryParseNoexceptSpecifier();
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseTypeName() {
  Token name = token;
  require_token(TT_IDENTIFIER, "Expected identifier in type name");
  if (is_template_type(name) && token.type == TT_LESS) {
    TryParseTemplateArgs(frontend->look_up(name.content));
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseIdExpression() {
  if (token.type == TT_IDENTIFIER) {
    TryParsePrefixIdentifier();
  } else if (token.type == TT_OPERATOR) {
    Token op = token;
    token = lexer->ReadToken();
    require_operatorkw();

    if (token.type == TT_LESS && is_template_type(op)) {
      TryParseTemplateArgs(frontend->look_up(op.content));
    }
  } else if (token.type == TT_TILDE) {
    if (token.type == TT_IDENTIFIER) {
      TryParseTypeName();
    } else if (token.type == TT_DECLTYPE) {
      TryParseDecltype();
    }
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseDecltype() {
  Token tok = token;
  require_token(TT_DECLTYPE, "Expected 'decltype' keyword");
  require_token(TT_BEGINPARENTH, "Expected '(' after 'decltype'");
  auto expr = TryParseExpression(Precedence::kAll);
  require_token(TT_ENDPARENTH, "Expected ')' after decltype expression");

  return nullptr;
}

std::vector<std::unique_ptr<AST::Node>> TryParseTemplateArgs(jdi::definition *def) {
  if (def->flags & jdi::DEF_TEMPLATE) {
    require_token(TT_LESS, "Expected '<' at start of template arguments");
    for (std::size_t i = 0; token.type != TT_GREATER && token.type != TT_ENDOFCODE;) {
      if (reinterpret_cast<jdi::definition_template *>(def)->params[i]->flags & jdi::DEF_TYPENAME) {
        TryParseTypeID();
      } else {
        TryParseConstantExpression();
      }

      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
        i++;
      } else {
        break;
      }
    }
    require_token(TT_GREATER, "Expected '>' after template arguments");
  }
  return {};
}

std::unique_ptr<AST::Node> TryParseTypenameSpecifier() {
  require_token(TT_TYPENAME, "Expected 'typename' in typename specifier");

  if (token.type == TT_IDENTIFIER) {
    TryParsePrefixIdentifier();
  } else if (token.type == TT_DECLTYPE) {
    TryParseDecltype();
    TryParseNestedNameSpecifier();
  } else if (token.type == TT_SCOPEACCESS) {
    TryParseNestedNameSpecifier();
  } else {
    herr->Error(token) << "Expected nested name specifier after 'typename'";
    return nullptr;
  }

  return nullptr;
}

std::vector<std::unique_ptr<AST::Node>> TryParsePrefixIdentifier() {
  Token id = token;
  require_token(TT_IDENTIFIER, "Expected identifier");

  if (token.type == TT_SCOPEACCESS) {
    TryParseNestedNameSpecifier();
  } else if (token.type == TT_LESS && is_template_type(id)) {
    TryParseTemplateArgs(frontend->look_up(id.content));
  }

  return {};
}

std::unique_ptr<AST::Node> TryParseNestedNameSpecifier() {
  if (token.type != TT_SCOPEACCESS) {
    herr->Error(token) << "Expected scope access '::' in nested name specifier";
    return nullptr;
  }

  while (token.type == TT_SCOPEACCESS) {
    token = lexer->ReadToken();
    if (token.type == TT_IDENTIFIER) {
      Token id = token;
      token = lexer->ReadToken();
      if (token.type == TT_LESS && is_template_type(id)) {
        TryParseTemplateArgs(frontend->look_up(id.content));
      }
    } else if (token.type == TT_DECLTYPE) {
      auto decl = TryParseDecltype();
    }
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseTypeSpecifier() {
  if (token.type == TT_TYPE_NAME) {
    token = lexer->ReadToken();
  } else if (token.type == TT_IDENTIFIER) {
    TryParsePrefixIdentifier();
  } else if (token.type == TT_SCOPEACCESS) {
    TryParseNestedNameSpecifier();
  } else if (token.type == TT_DECLTYPE) {
    auto decl = TryParseDecltype();

    if (token.type == TT_SCOPEACCESS) {
      TryParseNestedNameSpecifier();
    }
  } else if (token.type == TT_TYPENAME) {
    TryParseTypenameSpecifier();
  } else if (next_is_cv_qualifier()) {
    token = lexer->ReadToken();
  } else if (next_is_class_key()) {
    Token type = token;
    token = lexer->ReadToken();
    if (token.type == TT_IDENTIFIER) {
      TryParsePrefixIdentifier();
    } else {
      herr->Error(token) << "Expected identifier after '" << type << "'";
    }
  } else if (token.type == TT_ENUM) {
    token = lexer->ReadToken();
    require_token(TT_IDENTIFIER, "Expected identifier after 'enum'");
    if (token.type == TT_SCOPEACCESS) {
      TryParseNestedNameSpecifier();
    }
  } else if (token.type == TT_SIGNED || token.type == TT_UNSIGNED) {
    token = lexer->ReadToken();
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParsePtrOperator() {
  if (token.type == TT_STAR) {
    token = lexer->ReadToken();
    if (next_is_cv_qualifier()) {
      token = lexer->ReadToken();
    }
  } else if (next_is_ref_qualifier()) {
    token = lexer->ReadToken();
  } else if (next_maybe_nested_name()) {
    if (token.type == TT_IDENTIFIER) {
      token = lexer->ReadToken();
    } else if (token.type == TT_DECLTYPE) {
      TryParseDecltype();
    }

    TryParseNestedNameSpecifier();
    require_token(TT_STAR, "Expected '*' after nested name specifier");

    if (next_is_cv_qualifier()) {
      token = lexer->ReadToken();
    }
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseNoPtrAbstractDeclarator() {
  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    if (next_maybe_ptr_operator()) {
      while (next_maybe_ptr_operator()) {
        TryParsePtrOperator();
      }
      require_token(TT_ENDPARENTH, "Expected ')' after pointer declarator");
    } else {
      TryParseParametersAndQualifiers(true);
      if (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
        TryParseNoPtrAbstractDeclarator();
      }
    }
  } else if (token.type == TT_BEGINBRACKET) {
    TryParseArrayBoundsExpression();
    if (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
      TryParseNoPtrAbstractDeclarator();
    }
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseAbstractDeclarator() {
  if (next_maybe_ptr_operator()) {
    while(next_maybe_ptr_operator()) {
      TryParsePtrOperator();
    }

    while (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
      TryParseNoPtrAbstractDeclarator();
    }

    if (token.type == TT_ELLIPSES) {
      token = lexer->ReadToken();
      while (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
        if (token.type == TT_BEGINPARENTH) {
          TryParseParametersAndQualifiers();
        } else {
          TryParseArrayBoundsExpression();
        }
      }
    }
  } else if (token.type == TT_BEGINPARENTH) {
    TryParseParametersAndQualifiers();
    if (token.type == TT_ARROW) {
      token = lexer->ReadToken();
      TryParseTypeID();
    }
  } else {
    TryParseNoPtrAbstractDeclarator();
    if (token.type == TT_BEGINPARENTH) {
      TryParseParametersAndQualifiers();
      if (token.type == TT_ARROW) {
        token = lexer->ReadToken();
        TryParseTypeID();
      }
    }
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseTypeID() {
  while (next_is_type_specifier()) {
    TryParseTypeSpecifier();
  }
  if (next_maybe_ptr_operator() || token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
    TryParseAbstractDeclarator();
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseDeclSpecifier() {
  if (token.type == TT_TYPEDEF) {
    token = lexer->ReadToken();
  } else if (token.type == TT_CONSTEXPR) {
    token = lexer->ReadToken();
  } else if (token.type == TT_CONSTINIT) {
    token = lexer->ReadToken();
  } else if (token.type == TT_CONSTEVAL) {
    token = lexer->ReadToken();
  } else if (token.type == TT_MUTABLE) {
    token = lexer->ReadToken();
  } else if (token.type == TT_INLINE) {
    token = lexer->ReadToken();
  } else if (token.type == TT_STATIC) {
    token = lexer->ReadToken();
  } else if (token.type == TT_THREAD_LOCAL) {
    token = lexer->ReadToken();
  } else if (token.type == TT_EXTERN) {
    token = lexer->ReadToken();
  } else if (next_is_type_specifier()) {
    TryParseTypeSpecifier();
  }

  return nullptr;
}

std::unique_ptr<AST::Node> TryParseDeclSpecifierSeq() {
  while (next_is_decl_specifier()) {
    TryParseDeclSpecifier();
  }
}

std::unique_ptr<AST::Node> TryParsePtrDeclarator() {
  while (next_maybe_ptr_operator()) {
    TryParsePtrOperator();
  }

  TryParseNoPtrDeclarator();
}

std::unique_ptr<AST::Node> TryParseNoPtrDeclarator() {
  if (token.type == TT_BEGINPARENTH) {
    TryParsePtrDeclarator();
    require_token(TT_ENDPARENTH, "Expected ')' after pointer declarator");
  } else {
    if (token.type == TT_ELLIPSES) {
      token = lexer->ReadToken();
    }
    TryParseIdExpression();
  }

  while (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
    if (token.type == TT_BEGINPARENTH) {
      TryParseParametersAndQualifiers();
    } else {
      TryParseArrayBoundsExpression();
    }
  }
}

std::unique_ptr<AST::Node> TryParseDeclarator() {
  if (next_maybe_ptr_operator()) {
    TryParsePtrDeclarator();
  } else {
    TryParseNoPtrDeclarator();
    if (token.type == TT_ARROW) {
      token = lexer->ReadToken();
      TryParseTypeID();
    }
  }
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

      if (auto exp = TryParseExpression(Precedence::kUnaryPrefix)) {
        return std::make_unique<AST::UnaryPrefixExpression>(std::move(exp), unary_op.type);
      }
      herr->Error(unary_op) << "Expected expression following unary operator";
      break;
    }

    case TT_BEGINPARENTH: {
      token = lexer->ReadToken();
      auto exp = TryParseExpression(Precedence::kAll);
      require_token(TT_ENDPARENTH, "Expected closing parenthesis before", token);

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

      require_token(TT_ENDBRACKET, "Expected closing `]` for array");
      return std::make_unique<AST::Array>(std::move(elements));
    }

    case TT_IDENTIFIER:
    case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
    case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT: {
      Token res = token;
      token = lexer->ReadToken();
      return std::make_unique<AST::Literal>(std::move(res));
    }

    case TT_SIZEOF: {
      auto oper = token;
      token = lexer->ReadToken();

      if (token.type == TT_BEGINPARENTH) {
        // TODO: Implement sizeof type
      } else if (token.type == TT_ELLIPSES) {
        token = lexer->ReadToken();
        require_token(TT_BEGINPARENTH, "Expected opening '(' after 'sizeof ...'");
        auto arg = token;
        if (require_token(TT_IDENTIFIER, "Expected identifier as argument to variadic sizeof")) {
          require_token(TT_ENDPARENTH, "Expected closing ')' after variadic sizeof");
          return std::make_unique<AST::UnaryPrefixExpression>(std::make_unique<AST::Literal>(arg), TT_VAR_SIZEOF);
        } else {
          return nullptr;
        }
      } else {
        auto operand = TryParseExpression(Precedence::kUnaryPrefix);
        return std::make_unique<AST::UnaryPrefixExpression>(std::move(operand), oper.type);
      }
    }

    case TT_ALIGNOF: {
      if (require_token(TT_BEGINPARENTH, "Expected opening '(' after 'alignof'")) {
        // TODO: Implement alignof type
      } else {
        return nullptr;
      }
    }

    case TT_CO_AWAIT: {
      auto oper = token;
      token = lexer->ReadToken();
      auto expr = TryParseExpression(Precedence::kUnaryPrefix);
      return std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), oper.type);
    }

    case TT_NOEXCEPT: {
      auto oper = token;
      token = lexer->ReadToken();
      if (require_token(TT_BEGINPARENTH, "Expected opening '(' after noexcept")) {
        token = lexer->ReadToken();
        auto expr = TryParseExpression(Precedence::kAll);
        require_token(TT_ENDPARENTH, "Expected closing ')' after noexcept expression");
        return std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), oper.type);
      } else {
        return nullptr;
      }
    }

    case TT_DYNAMIC_CAST:
    case TT_STATIC_CAST:
    case TT_REINTERPRET_CAST:
    case TT_CONST_CAST:
      // TODO: Implement casts
      return nullptr;

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
        if (precedence < Precedence::kFuncCall) {
          break;
        }
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

  require_token(TT_COLON, "Expected colon (':') after expression in conditional operator");

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

    require_token(TT_ENDBRACKET, "Expected closing bracket (']') at the end of array subscript");
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

    require_token(TT_ENDPARENTH, "Expected ')' after function call");
    operand = std::make_unique<AST::FunctionCallExpression>(std::move(operand), std::move(arguments));
  }

  return dynamic_unique_pointer_cast<AST::FunctionCallExpression>(std::move(operand));
}

std::unique_ptr<AST::Node> TryParseControlExpression() {
  switch (mode) {
    case SyntaxMode::STRICT: {
      require_token(TT_BEGINPARENTH, "Expected '(' before control expression");
      auto expr = TryParseExpression(Precedence::kAll);
      require_token(TT_ENDPARENTH, "Expected ')' after control expression");
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
      require_token(TT_ENDBRACE, "Expected closing brace ('}') after code block");
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
  require_any_of({TT_S_WHILE, TT_S_UNTIL}, "Expected `while` or `until` after do loop body");

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
