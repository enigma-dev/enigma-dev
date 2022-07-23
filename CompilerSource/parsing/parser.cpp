#include "parser.h"

#include "ast.h"
#include "lexer.h"
#include "tokens.h"
#include "precedence.h"

#include <JDI/src/System/builtins.h>
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
      (herr->Error(token) << ... << messages) << ", got: '" << token.content << '\'';
    } else {
      herr->Error(token) << "Unexpected token: '" << token.ToString() << '\'';
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

std::string read_required_operatorkw() {
  if (next_is_operatorkw()) {
    if (token.type == TT_S_NEW || token.type == TT_S_DELETE) {
      TokenType type = token.type;
      token = lexer->ReadToken();
      if (token.type == TT_BEGINBRACKET) {
        token = lexer->ReadToken();
        require_token(TT_ENDBRACKET, "Expected ']' after '[' in '", type == TT_S_NEW ? "new" : "delete", "[]'");
        return type == TT_S_NEW ? " new[]" : " delete[]";
      } else {
        return type == TT_S_NEW ? " new" : " delete";
      }
    } else if (token.type == TT_BEGINPARENTH) {
      token = lexer->ReadToken();
      require_token(TT_ENDPARENTH, "Expected ')' after '('");
      return "()";
    } else if (token.type == TT_BEGINBRACKET) {
      token = lexer->ReadToken();
      require_token(TT_ENDBRACKET, "Expected ']' after '['");
      return "[]";
    } else {
      std::string oper{token.content};
      token = lexer->ReadToken();
      return oper;
    }
  } else {
    herr->Error(token) << "Expected an overloadable operator, got: '" << token.content << '\'';
    return "";
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

bool is_user_defined_type(const Token &tok) {
  switch (tok.type) {
    case TT_IDENTIFIER:
      if (auto def = frontend->look_up(tok.content); def != nullptr) {
        return def->flags & (jdi::DEF_TYPENAME | jdi::DEF_CLASS | jdi::DEF_ENUM | jdi::DEF_TYPED | jdi::DEF_TEMPLATE);
      }

      [[fallthrough]];
    default:
      return false;
  }
}

bool next_is_user_defined_type() {
  return is_user_defined_type(token);
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

    case TT_IDENTIFIER:
      return is_user_defined_type(tok);

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
    case TT_DECLTYPE:
      return true;

    case TT_IDENTIFIER: {
      auto *def = frontend->look_up(tok.content);
      return def != nullptr && (def->flags & (jdi::DEF_CLASS | jdi::DEF_SCOPE));
    }

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

bool is_template_type(jdi::definition *def) {
  return def->flags & jdi::DEF_TEMPLATE;
}

bool next_is_template_type() {
  return is_template_type(token);
}

bool maybe_ptr_decl_operator(const Token &tok) {
  return tok.type == TT_STAR || is_ref_qualifier(tok) || maybe_nested_name(tok);
}

bool next_maybe_ptr_decl_operator() {
  return maybe_ptr_decl_operator(token);
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

std::size_t sizeof_builtin_type(std::string_view type) {
  static const std::unordered_map<std::string_view, std::size_t> sizes{
    { "char",     sizeof(char) },
    { "char8_t",  1 },
    { "char16_t", sizeof(char16_t) },
    { "char32_t", sizeof(char32_t) },
    { "wchar_t",  sizeof(wchar_t) },
    { "bool",     sizeof(bool) },
    { "short",    sizeof(short) },
    { "int",      sizeof(int) },
    { "long",     sizeof(long) },
    { "float",    sizeof(float) },
    { "double",   sizeof(double) },
    { "void",     1 },
  };

  if (auto size = sizes.find(type); size != sizes.end()) {
    return size->second;
  } else {
    return -1;
  }
}

std::size_t jdi_decflag_bitmask(std::string_view tok) {
  static const std::unordered_map<std::string_view, std::pair<std::size_t, std::size_t>> bitmasks{
    { "volatile",  {jdi::builtin_flag__volatile->mask,  jdi::builtin_flag__volatile->value}  },
    { "static",    {jdi::builtin_flag__static->mask,    jdi::builtin_flag__static->value}    },
    { "const",     {jdi::builtin_flag__const->mask,     jdi::builtin_flag__const->value}     },
    { "mutable",   {jdi::builtin_flag__mutable->mask,   jdi::builtin_flag__mutable->value}   },
    { "register",  {jdi::builtin_flag__register->mask,  jdi::builtin_flag__register->value}  },
    { "inline",    {jdi::builtin_flag__inline->mask,    jdi::builtin_flag__inline->value}    },
    { "_Complex",  {jdi::builtin_flag__Complex->mask,   jdi::builtin_flag__Complex->value}   },
    { "restrict",  {jdi::builtin_flag__restrict->mask,  jdi::builtin_flag__restrict->value}  },
    { "unsigned",  {jdi::builtin_flag__unsigned->mask,  jdi::builtin_flag__unsigned->value}  },
    { "long",      {jdi::builtin_flag__long->mask,      jdi::builtin_flag__long->value}      },
    { "signed",    {jdi::builtin_flag__signed->mask,    jdi::builtin_flag__signed->value}    },
    { "short",     {jdi::builtin_flag__short->mask,     jdi::builtin_flag__short->value}     },
    { "long long", {jdi::builtin_flag__long_long->mask, jdi::builtin_flag__long_long->value} },
    { "virtual",   {jdi::builtin_flag__virtual->mask,   jdi::builtin_flag__virtual->value}   },
    { "explicit",  {jdi::builtin_flag__explicit->mask,  jdi::builtin_flag__explicit->value}  },
    { "throw",     {128,  128}    },
    { "override",  {512,  512}    },
    { "final",     {1024, 1024}   },
  };

  if (auto bitmask = bitmasks.find(tok); bitmask != bitmasks.end()) {
    return bitmask->second.second;
  } else {
    return -1;
  }
}

jdi::definition *require_defined_type(const Token &tok) {
  if (auto def = frontend->look_up(tok.content); def != nullptr) {
    return def;
  } else {
    herr->Error(tok) << "Invalid type name: '" << tok.content << '\'';
    return nullptr;
  }
}

jdi::definition *require_scope_type(const Token &tok) {
  if (auto def = frontend->look_up(tok.content);
      def != nullptr && (def->flags & jdi::DEF_SCOPE || def->flags & jdi::DEF_CLASS)) {
    return def;
  } else {
    herr->Error(tok) << "The given identifier is not a scope name: '" << tok.content << '\'';
    return nullptr;
  }
}

jdi::definition_scope *require_scope_type(jdi::definition *def, const Token &tok) {
  if (def != nullptr && (def->flags & jdi::DEF_SCOPE || def->flags & jdi::DEF_CLASS)) {
    return dynamic_cast<jdi::definition_scope *>(def);
  } else {
    herr->Error(tok) << "Given specifier does not name or refer to a scope";
    return nullptr;
  }
}

bool is_start_of_initializer(const Token &tok) {
  switch (tok.type) {
    case TT_EQUALS:
    case TT_BEGINBRACE:
    case TT_BEGINPARENTH:
      return true;

    default:
      return false;
  }
}

bool next_is_start_of_initializer() {
  return is_start_of_initializer(token);
}

bool is_start_of_id_expression(const Token &tok) {
  switch (tok.type) {
    case TT_TILDE:
    case TT_IDENTIFIER:
    case TT_OPERATOR:
    case TT_SCOPEACCESS:
    case TT_DECLTYPE:
      return true;

    default:
      return false;
  }
}

bool next_is_start_of_id_expression() {
  return is_start_of_id_expression(token);
}

public:

AstBuilder(Lexer *lexer, ErrorHandler *herr, SyntaxMode mode, LanguageFrontend *frontend): lexer{lexer}, herr{herr},
  mode{mode}, frontend{frontend} {
  frontend->definitionsModified(nullptr, "");
  token = lexer->ReadToken();
}

const Token &current_token() {
  return token;
}

std::unique_ptr<AST::Node> TryParseConstantExpression() {
  return TryParseExpression(Precedence::kTernary);
}

std::unique_ptr<AST::Node> TryParseArrayBoundsExpression(jdi::ref_stack *ft) {
  require_token(TT_BEGINBRACKET, "Expected '[' before array bounds expression");
  std::unique_ptr<AST::Node> expr = nullptr;
  if (token.type != TT_ENDBRACKET) {
    expr = TryParseConstantExpression();
  }
  require_token(TT_ENDBRACKET, "Expected ']' after array bounds expression");

  // TODO: Check that expression is constant, then evaluate it
  ft->push_array(jdi::ref_stack::node_array::nbound);

  return nullptr;
}

jdi::definition *TryParseNoexceptSpecifier() {
  herr->Error(token) << "Unimplemented: noexcept";

  require_token(TT_NOEXCEPT, "Expected 'noexcept' in noexcept specifier");
  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    TryParseConstantExpression();
    require_token(TT_ENDPARENTH, "Expected ')' after noexcept expression");
  }

  return nullptr;
}

void TryParseParametersAndQualifiers(jdi::ref_stack *rt, bool did_consume_paren = false) {
  if (!did_consume_paren) {
    require_token(TT_BEGINPARENTH, "Expected '(' before function parameters");
  }

  jdi::ref_stack::parameter_ct params;
  if (token.type != TT_ENDPARENTH) {
    while (token.type != TT_ENDPARENTH) {
      jdi::ref_stack::parameter param;
      jdi::full_type type;
      TryParseDeclSpecifierSeq(&type);
      TryParseDeclarator(&type, AST::DeclaratorType::MAYBE_ABSTRACT);

      param.swap_in(type);
      if (token.type == TT_EQUALS) {
        herr->Error(token) << "Unimplemented: default values in function arguments";
        token = lexer->ReadToken();
        auto init = TryParseExprOrBracedInitList(true, false);
        // param.default_value = init.release();
        // TODO: Somehow fix JDI or inherit from it to handle this
      }
      params.throw_on(param);

      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
        if (token.type == TT_ELLIPSES) {
          token = lexer->ReadToken();
          if (token.type != TT_ENDPARENTH) {
            herr->Error(token) << "Extra junk after ellipses in function parameter";
          }
          break;
        }
      } else {
        break;
      }
    }

    if (token.type == TT_ELLIPSES) {
      herr->Error(token) << "Unimplemented: varargs";
      token = lexer->ReadToken();
    }
  }

  rt->push_func(params);
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
}

jdi::definition *TryParseTypeName() {
  Token name = token;
  require_token(TT_IDENTIFIER, "Expected identifier in type name");
  if (is_template_type(name) && token.type == TT_LESS) {
    TryParseTemplateArgs(frontend->look_up(name.content));
  }

  return frontend->look_up(name.content);
}

jdi::definition *TryParseIdExpression(jdi::ref_stack *rt = nullptr, bool is_declarator = false) {
  switch (token.type) {
    case TT_IDENTIFIER: {
      if (next_is_user_defined_type()) {
        return TryParsePrefixIdentifier(rt, is_declarator);
      } else {
        std::string name{token.content};
        auto def = frontend->look_up(token.content);
        token = lexer->ReadToken();
        if (is_declarator && token.type != TT_SCOPEACCESS) {
          rt->name = name; // If we're not accessing a scope then we're probably declaring a variable
          rt->ndef = def;
        } else if (token.type == TT_SCOPEACCESS) {
          return TryParseNestedNameSpecifier(def, rt, is_declarator);
        } else if (def == nullptr) {
          herr->Error(token) << "No such name exists in global scope";
        }

        return def;
      }
    }

    case TT_OPERATOR: {
      Token op = token;
      token = lexer->ReadToken();
      std::string type = read_required_operatorkw();

      if (type != "") {
        std::string oper = "operator" + type;
        if (token.type == TT_LESS && is_template_type(op)) {
          TryParseTemplateArgs(frontend->look_up(oper));
        } else {
          return frontend->look_up(oper);
        }
      }

      return nullptr;
    }

    case TT_TILDE: {
      token = lexer->ReadToken();
      if (token.type == TT_IDENTIFIER) {
        return TryParseTypeName();
      } else if (token.type == TT_DECLTYPE) {
        return TryParseDecltype();
      } else {
        herr->Error(token) << "Given token is not valid for specifying a destructor to call";
        return nullptr;
      }
    }

    default: {
      herr->Error(token) << "Given token cannot be used to specify a qualified or unqualified expression: '"
                         << token.content << '\'';
      return nullptr;
    }
  }
}

jdi::definition *TryParseDecltype() {
  Token tok = token;
  require_token(TT_DECLTYPE, "Expected 'decltype' keyword");
  require_token(TT_BEGINPARENTH, "Expected '(' after 'decltype'");
  auto expr = TryParseExpression(Precedence::kAll);
  require_token(TT_ENDPARENTH, "Expected ')' after decltype expression");

  return nullptr;
}

void TryParseTemplateArgs(jdi::definition *def) {
  if (def->flags & jdi::DEF_TEMPLATE) {
    require_token(TT_LESS, "Expected '<' at start of template arguments");
    auto template_def = reinterpret_cast<jdi::definition_template *>(def);
    jdi::arg_key argk;
    argk.mirror_types(template_def);
    std::size_t args_given = 0;
    for (; token.type != TT_GREATER && token.type != TT_ENDOFCODE;) {
      if (template_def->params[args_given]->flags & jdi::DEF_TYPENAME) {
        jdi::full_type ft = TryParseTypeID();
        if (ft.def) {
          argk[args_given].ft().swap(ft);
        }
      } else if (next_is_start_of_id_expression()) {
        herr->Error(token) << "Unimplemented: id-expressions as template arguments";
        auto id = TryParseIdExpression(nullptr, false);
      } else {
        herr->Error(token) << "Unimplemented: NTTP template arguments";
        auto expr = TryParseConstantExpression();
      }

      if (token.type == TT_ELLIPSES) {
        herr->Error(token) << "Unimplemented: variadic template arguments";
        token = lexer->ReadToken();
      }

      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
        args_given++;
        if (args_given > template_def->params.size()) {
          herr->Error(token) << "Too many types in template instantiation";
          break;
        }
      } else {
        break;
      }
    }

    if (require_token(TT_GREATER, "Expected '>' after template arguments")) {
      jdi::remap_set remap;
      for (std::size_t i = 0; i < args_given; i++) {
        if (argk[i].type == jdi::arg_key::AKT_FULLTYPE) {
          remap[template_def->params[i].get()] =
              std::make_unique<jdi::definition_typed>(template_def->params[i]->name, template_def, argk[i].ft(),
                                                      jdi::DEF_TYPENAME | jdi::DEF_TYPED).release();
        } else if (argk[i].type == jdi::arg_key::AKT_VALUE) {
          remap[template_def->params[i].get()] =
              std::make_unique<jdi::definition_valued>(template_def->params[i]->name, template_def,
                                                       template_def->params[i]->integer_type.def,
                                                       template_def->params[i]->integer_type.flags,
                                                       jdi::DEF_VALUED, argk[i].val()).release();
        } else {
          herr->Error(token) << "Internal error: type of template parameter unknown";
        }
      }

      // TODO: Fix whatever this garbage is
      auto errc = jdi::ErrorContext{new jdi::DefaultErrorHandler{}, jdi::SourceLocation{"lol", token.position, token.line}};
      for (std::size_t i = args_given; i < template_def->params.size(); i++) {
        if (template_def->params[i]->default_assignment) {
          jdi::AST ast(*template_def->params[i]->default_assignment, true);
          ast.remap(remap, errc);
          if (template_def->params[i]->flags & jdi::DEF_TYPENAME)
            argk.put_type(i, ast.coerce(errc));
          else
            argk.put_value(i, ast.eval(errc));
        } else {
          herr->Error(token) << "Expected template argument, parameter " << i << " has no default value";
        }
      }

      for (auto &value: remap) {
        delete value.second;
      }
    }
  }
}

jdi::definition *TryParseTypenameSpecifier() {
  require_token(TT_TYPENAME, "Expected 'typename' in typename specifier");

  switch (token.type) {
    case TT_IDENTIFIER: return TryParsePrefixIdentifier();
    case TT_DECLTYPE: return TryParseNestedNameSpecifier(TryParseDecltype());
    case TT_SCOPEACCESS: return TryParseNestedNameSpecifier(nullptr);

    default: {
      herr->Error(token) << "Expected nested name specifier after 'typename'";
      return nullptr;
    }
  }
}

jdi::definition *TryParsePrefixIdentifier(jdi::ref_stack *rt = nullptr, bool is_declarator = false) {
  Token id = token;
  require_token(TT_IDENTIFIER, "Expected identifier");
  auto def = require_defined_type(id);

  if (token.type == TT_LESS && is_template_type(def)) {
    TryParseTemplateArgs(def);
  }

  if (token.type == TT_SCOPEACCESS) {
    return TryParseNestedNameSpecifier(def, rt, is_declarator);
  }

  return def;
}

jdi::definition *TryParseNestedNameSpecifier(jdi::definition *scope, jdi::ref_stack *rt = nullptr, bool is_declarator = false) {
  if (token.type != TT_SCOPEACCESS) {
    herr->Error(token) << "Expected scope access '::' in nested name specifier, got: '" << token.content << '\'';
    return nullptr;
  }

  jdi::definition *def = scope;
  if (def != nullptr && !(def->flags & jdi::DEF_SCOPE)) {
    herr->Error(token) << "Given specifier does not refer to any existing scopes";
  }

  bool is_global_scope = def == nullptr;

  Token prev{};
  Token name{};
  while (token.type == TT_SCOPEACCESS) {
    prev = token;
    token = lexer->ReadToken();
    if (token.type == TT_IDENTIFIER) {
      Token id = token;
      name = token;
      token = lexer->ReadToken();
      if (token.type == TT_LESS && is_template_type(id)) {
        TryParseTemplateArgs(frontend->look_up(id.content));
      } else if (is_global_scope) {
        is_global_scope = false;
        def = frontend->look_up(id.content);
        if (def == nullptr) {
          herr->Error(id) << "Given name does not exist in the scope: '" << id.content << '\'';
          break;
        }
      } else if (auto *def_scope = require_scope_type(def, prev); def_scope != nullptr) {
        def = def_scope->look_up(std::string{id.content});
        if (def == nullptr) {
          herr->Error(id) << "Given name does not exist in the scope: '" << id.content << '\'';
          break;
        }
      }
    }
  }

  if (is_declarator) {
    if (rt == nullptr) {
      herr->Error(name) << "Internal error: nullptr jdi::ref_stack passed to TryParseNestedNameSpecifier()";
    } else {
      rt->name = name.content;
    }
  }

  return def;
}

bool matches_token_type(jdi::definition *def, const Token &tok) {
  switch (tok.type) {
    case TT_ENUM: return def->flags & jdi::DEF_ENUM;
    case TT_STRUCT:
    case TT_CLASS: return def->flags & jdi::DEF_CLASS;
    case TT_UNION: return def->flags & jdi::DEF_UNION;

    default:
      herr->Error(tok) << "Internal error: incorrect token type passed to `matches_token_type`";
      return false;
  }
}

void TryParseElaboratedName(jdi::full_type *ft) {
  Token type = token;

  token = lexer->ReadToken();
  Token name = token;
  jdi::definition *def = nullptr;

  if (token.type == TT_IDENTIFIER) {
    def = frontend->look_up(token.content);
    token = lexer->ReadToken();
  } else if (token.type == TT_DECLTYPE) {
    token = lexer->ReadToken();
    def = TryParseDecltype();
    if (token.type != TT_SCOPEACCESS) {
      herr->Error(token) << "Expected scope access after decltype";
    }
  }

  if (token.type == TT_SCOPEACCESS) {
    def = TryParseNestedNameSpecifier(def);
  }

  if (def != nullptr && matches_token_type(def, type)) {
    ft->def = def;
  } else {
    herr->Error(name) << "Given specifier does not refer to a declared enum";
  }
}

void maybe_assign_full_type(jdi::full_type *ft, jdi::definition *def, Token token, bool is_allocated = false) {
  if (def != nullptr && ft->def == nullptr) {
    ft->def = def;
  } else if (ft->def != nullptr) {
    if (is_allocated) {
      delete def;
    }
    herr->Error(token) << "Usage of two different types in type specifier";
  }
}

void TryParseTypeSpecifier(jdi::full_type *ft) {
  switch (token.type) {
    case TT_TYPE_NAME: {
      maybe_assign_full_type(ft,
        std::make_unique<jdi::definition_atomic>(std::string{token.content}, nullptr, jdi::DEF_TYPENAME,
                                                 sizeof_builtin_type(token.content)).release(),
        token, true);
      token = lexer->ReadToken();
      break;
    }

    case TT_IDENTIFIER: {
      maybe_assign_full_type(ft, TryParsePrefixIdentifier(), token);
      break;
    }

    case TT_SCOPEACCESS: {
      maybe_assign_full_type(ft, TryParseNestedNameSpecifier(nullptr), token);
      break;
    }

    case TT_DECLTYPE: {
      Token tok = token;
      auto def = TryParseDecltype();

      if (token.type == TT_SCOPEACCESS) {
        def = TryParseNestedNameSpecifier(def);
      }

      if (def != nullptr) {
        maybe_assign_full_type(ft, def, tok);
      } else {
        herr->Error(tok) << "Could not parse decltype specifier";
      }
      break;
    }

    case TT_TYPENAME: {
      maybe_assign_full_type(ft, TryParseTypenameSpecifier(), token);
      break;
    }

    default: {
      if (token.type == TT_SIGNED || token.type == TT_UNSIGNED || next_is_cv_qualifier()) {
        if ((ft->flags & jdi_decflag_bitmask(token.content)) == jdi_decflag_bitmask(token.content)) {
          herr->Warning(token) << "Duplicate usage of flags in type specifier";
        } else {
          ft->flags |= jdi_decflag_bitmask(token.content);
        }
        token = lexer->ReadToken();
      } else if (next_is_class_key() || token.type == TT_ENUM) {
        TryParseElaboratedName(ft);
      } else {
        herr->Error(token) << "Given token does not specify a valid type specifier";
      }
      break;
    }
  }
}

void TryParseTypeSpecifierSeq(jdi::full_type *ft) {
  while (next_is_type_specifier()) {
    TryParseTypeSpecifier(ft);
  }
}

void TryParsePtrOperator(jdi::full_type *ft) {
  if (token.type == TT_STAR) {
    ft->refs.push(jdi::ref_stack::RT_POINTERTO);
    token = lexer->ReadToken();
    if (next_is_cv_qualifier()) {
      herr->Warning(token) << "Unimplemented: const/volatile pointer";
      token = lexer->ReadToken();
    }
  } else if (next_is_ref_qualifier()) {
    if (token.type == TT_AND) {
      herr->Warning(token) << "Unimplemented: universal references";
    } else {
      ft->refs.push(jdi::ref_stack::RT_REFERENCE);
    }
    token = lexer->ReadToken();
  }
}

void TryParseMaybeNestedPtrOperator(jdi::full_type *ft) {
  if (next_maybe_nested_name()) {
    jdi::definition *def = nullptr;
    if (token.type == TT_IDENTIFIER) {
      def = frontend->look_up(token.content);
      token = lexer->ReadToken();
    } else if (token.type == TT_DECLTYPE) {
      def = TryParseDecltype();
    }

    def = TryParseNestedNameSpecifier(def);
    if (token.type == TT_STAR) {
      if (!(def->flags & jdi::DEF_CLASS)) {
        herr->Error(token) << "Member pointer to non-class type: '" << def->name << "'";
      } else {
        ft->refs.push_memptr(reinterpret_cast<jdi::definition_class *>(def));
      }
      token = lexer->ReadToken();
      if (next_is_cv_qualifier()) {
        herr->Warning(token) << "Unimplemented: const/volatile pointer";
        token = lexer->ReadToken();
      }
    }
  }
}

jdi::full_type TryParseTypeID() {
  jdi::full_type ft;
  while (next_is_type_specifier()) {
    TryParseTypeSpecifier(&ft);
  }
  if (next_maybe_ptr_decl_operator() || token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
    TryParseDeclarator(&ft, AST::DeclaratorType::ABSTRACT);
  }

  return ft;
}

void TryParseDeclSpecifier(jdi::full_type *ft) {
  switch (token.type) {
    case TT_TYPEDEF:
    case TT_CONSTEXPR:
    case TT_CONSTINIT:
    case TT_CONSTEVAL:
    case TT_MUTABLE:
    case TT_INLINE:
    case TT_STATIC:
    case TT_THREAD_LOCAL:
    case TT_EXTERN:
      token = lexer->ReadToken();
      break;

    default:
      if (next_is_type_specifier()) {
        TryParseTypeSpecifier(ft);
        break;
      }
  }
}

void TryParseDeclSpecifierSeq(jdi::full_type *ft) {
  while (next_is_decl_specifier()) {
    TryParseDeclSpecifier(ft);
  }
}

void TryParsePtrDeclarator(jdi::full_type *ft, AST::DeclaratorType is_abstract) {
  while (next_maybe_ptr_decl_operator()) {
    if (next_maybe_nested_name()) {
      TryParseMaybeNestedPtrOperator(ft);
    } else {
      TryParsePtrOperator(ft);
    }
  }

  TryParseNoPtrDeclarator(ft, is_abstract);
}

void TryParseNoPtrDeclarator(jdi::full_type *ft, AST::DeclaratorType is_abstract) {
  jdi::full_type inner;
  bool has_inner = false;
  if (token.type == TT_BEGINPARENTH) {
    has_inner = true;
    token = lexer->ReadToken();
    TryParsePtrDeclarator(&inner, is_abstract);
    require_token(TT_ENDPARENTH, "Expected ')' after declarator");
  } else if (is_abstract == AST::DeclaratorType::NON_ABSTRACT) {
    if (token.type == TT_ELLIPSES) {
      token = lexer->ReadToken();
    }
    TryParseIdExpression(&ft->refs, true);
  } else if (is_abstract == AST::DeclaratorType::MAYBE_ABSTRACT && next_is_start_of_id_expression()) {
    TryParseIdExpression(&ft->refs, true);
  }

  jdi::ref_stack post_declarators;
  while (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
    if (token.type == TT_BEGINPARENTH) {
      TryParseParametersAndQualifiers(&post_declarators);
    } else {
      TryParseArrayBoundsExpression(&post_declarators);
    }
  }

  ft->refs.append_c(post_declarators);
  if (has_inner) {
    ft->refs.append_nest_c(inner.refs);
  }
}

void TryParseDeclarator(jdi::full_type *ft, AST::DeclaratorType is_abstract = AST::DeclaratorType::NON_ABSTRACT) {
  if (next_maybe_ptr_decl_operator()) {
    TryParsePtrDeclarator(ft, is_abstract);
  } else {
    TryParseNoPtrDeclarator(ft, is_abstract);
    if (token.type == TT_ARROW) {
      token = lexer->ReadToken();
      TryParseTypeID();
    }
  }
}

AST::InitializerNode TryParseExprOrBracedInitList(bool is_init_clause, bool in_init_list) {
  // This function handles:
  // <brace-or-equal-initializer>    ::= = <initializer-clause>
  //                                   | <braced-init-list>
  // <initializer-clause>            ::= <assignment-expression>
  //                                   | <braced-init-list>
  // and the `...` in
  // <initializer-list>              ::= <initializer-clause> ...?
  if (token.type == TT_EQUALS && !is_init_clause) {
    token = lexer->ReadToken();
    if (token.type == TT_BEGINBRACE) {
      return AST::Initializer::from(AST::AssignmentInitializer::from(TryParseBraceInitializer()));
    } else {
      return AST::Initializer::from(AST::AssignmentInitializer::from(TryParseExpression(Precedence::kAssign)));
    }
  } else if (token.type == TT_BEGINBRACE) {
    auto val = AST::Initializer::from(TryParseBraceInitializer());
    if (token.type == TT_ELLIPSES) {
      if (in_init_list) {
        token = lexer->ReadToken();
        val->is_variadic = true;
      } else {
        herr->Error(token) << "Cannot use ellipses in brace initializer";
        token = lexer->ReadToken();
      }
    }
    return val;
  } else if (is_init_clause) {
    auto val = AST::Initializer::from(TryParseExpression(Precedence::kAssign));
    if (token.type == TT_ELLIPSES) {
      if (in_init_list) {
        token = lexer->ReadToken();
        val->is_variadic = true;
      } else {
        herr->Error(token) << "Cannot use ellipses in initializer";
        token = lexer->ReadToken();
      }
    }
    return val;
  } else {
    herr->Error(token) << "Expected equals ('=') or opening brace ('{') at start of initializer, got: '"
                       << token.content << '\'';
    return nullptr;
  }
}

AST::BraceOrParenInitNode TryParseBraceInitializer() {
  require_token(TT_BEGINBRACE, "Expected opening brace ('{') at the start of brace initializer");
  AST::BraceOrParenInitNode init = std::make_unique<AST::BraceOrParenInitializer>();
  if (token.type == TT_DOT) {
    init->kind = AST::BraceOrParenInitializer::Kind::DESIGNATED_INIT;
    while (token.type != TT_ENDBRACE) {
      token = lexer->ReadToken();
      std::string name{token.content};
      require_token(TT_IDENTIFIER, "Expected identifier after dot in designated initializer");
      init->values.emplace_back(name, TryParseExprOrBracedInitList(false, false));
      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
      } else {
        break;
      }
    }
  } else {
    init->kind = AST::BraceOrParenInitializer::Kind::BRACE_INIT;
    while (token.type != TT_ENDBRACE) {
      init->values.emplace_back("", TryParseExprOrBracedInitList(true, true));
      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
      } else {
        break;
      }
    }
  }
  require_token(TT_ENDBRACE, "Expected closing brace ('}') at the end of brace initializer");

  return init;
}

AST::InitializerNode TryParseInitializer(bool allow_paren_init = true) {
  switch (token.type) {
    case TT_EQUALS: {
      token = lexer->ReadToken();
      if (token.type == TT_BEGINBRACE) {
        return AST::Initializer::from(AST::AssignmentInitializer::from(TryParseBraceInitializer()));
      } else {
        return AST::Initializer::from(AST::AssignmentInitializer::from(TryParseExpression(Precedence::kAssign)));
      }
    }

    case TT_BEGINBRACE: return AST::Initializer::from(TryParseBraceInitializer());

    case TT_BEGINPARENTH: {
      if (!allow_paren_init) {
        return AST::Initializer::from(TryParseExpression(Precedence::kAll));
      } else {
        AST::BraceOrParenInitNode init = std::make_unique<AST::BraceOrParenInitializer>();
        init->kind = AST::BraceOrParenInitializer::Kind::PAREN_INIT;
        token = lexer->ReadToken();
        while (token.type != TT_ENDPARENTH) {
          init->values.emplace_back("", TryParseExprOrBracedInitList(true, true));
        }
        require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after initializer");
        return AST::Initializer::from(std::move(init));
      }
    }

    default: {
      herr->Error(token) << "Junk in initializer, expected one of =, {, (; got: '" << token.content << '\'';
      return nullptr;
    }
  }
}

std::unique_ptr<AST::Node> TryParseDeclarations() {
  if (next_is_decl_specifier()) {
    using Declaration = AST::DeclarationStatement::Declaration;

    jdi::full_type ft;
    TryParseDeclSpecifierSeq(&ft);

    if (ft.def == nullptr) {
      herr->Error(token) << "Unable to parse type specifier in declaration";
      return nullptr;
    }

    std::vector<Declaration> decls{};

    auto parse_decl = [this](jdi::full_type *ft) -> Declaration {
      Declaration decl{};
      decl.declarator.def = ft->def;
      TryParseDeclarator(&decl.declarator);
      if (next_is_start_of_initializer()) {
        decl.init = TryParseInitializer();
      }
      return decl;
    };

    decls.emplace_back(parse_decl(&ft));
    while (token.type == TT_COMMA) {
      token = lexer->ReadToken();
      decls.emplace_back(parse_decl(&ft));
    }

    return std::make_unique<AST::DeclarationStatement>(ft.def, std::move(decls));
  } else {
    return nullptr;
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
