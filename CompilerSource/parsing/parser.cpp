#include "parser.h"

namespace enigma::parsing {
class AstBuilder: public AstBuilderTestAPI {
public:

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

    // case TT_IDENTIFIER:
    //   return is_user_defined_type(tok);

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

static std::pair<std::size_t, std::size_t> jdi_decflag_bitmask(std::string_view tok) {
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
    return bitmask->second;
  } else {
    return {-1, -1};
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

void MaybeConsumeSemicolon(){
  if(token.type == TT_SEMICOLON)
    token = lexer->ReadToken();
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

bool maybe_functional_cast(const Token &tok) {
  switch (tok.type) {
    case TT_SCOPEACCESS:
    case TT_TYPENAME:
    case TT_TYPE_NAME:
    case TT_DECLTYPE:
      return true;

    // case TT_IDENTIFIER:
    //   return is_user_defined_type(tok);

    default:
      return false;
  }
}

bool next_maybe_functional_cast() {
  return maybe_functional_cast(token);
}

std::unique_ptr<AST::DeclarationStatement> parse_declarations(
    AST::DeclarationStatement::StorageClass sc, FullType &ft, AST::DeclaratorType decl_type, bool parse_unbounded,
    std::vector<AST::DeclarationStatement::Declaration> decls, bool already_parsed_first = false) {
  while (true) {
    if (!already_parsed_first) {
      TryParseDeclarator(&ft, decl_type);
      decls.emplace_back(std::move(ft), next_is_start_of_initializer() ? TryParseInitializer() : nullptr);
      declarations[decls.back().declarator->decl.name.content] = decls.back().declarator.get();
    }
    if (token.type == TT_COMMA && parse_unbounded) {
      token = lexer->ReadToken();
    } else {
      break;
    }
  }

  return std::make_unique<AST::DeclarationStatement>(sc, ft.def, std::move(decls));
}

void maybe_infer_int(FullType &type) {
  // This is a pretty hacky way to implicitly infer int, but it is the only way I can think of to prevent `int int x`
  // from being legal
  if (type.def == nullptr && (contains_decflag_bitmask(type.flags, "long")
                              || contains_decflag_bitmask(type.flags, "short")
                              || contains_decflag_bitmask(type.flags, "long long")
                              || contains_decflag_bitmask(type.flags, "signed")
                              || contains_decflag_bitmask(type.flags, "unsigned"))) {
    type.def = jdi::builtin_type__int;
  }
}

AstBuilder(Lexer *lexer, ErrorHandler *herr) {
  initialize(lexer, herr);
}

AstBuilder(){}

const Token &current_token() {
  return token;
}

std::unique_ptr<AST::Node> TryParseConstantExpression() {
  return TryParseExpression(Precedence::kTernary);
}

std::unique_ptr<AST::Node> TryParseArrayBoundsExpression(Declarator *decl, bool outside_nested) {
  require_token(TT_BEGINBRACKET, "Expected '[' before array bounds expression");
  std::unique_ptr<AST::Node> expr = nullptr;
  if (token.type != TT_ENDBRACKET) {
    expr = TryParseConstantExpression();
  }
  require_token(TT_ENDBRACKET, "Expected ']' after array bounds expression");

  // TODO: Check that expression is constant, then evaluate it
  // for handling new expressions we need to support also non-constant expressions, like `new int[x]`
  std::size_t arr_size = 0;
  if (expr) {
    if (expr->type == AST::NodeType::LITERAL) {
      auto *lit = expr->As<AST::Literal>();
      try {
        arr_size = std::stoi(std::get<string>(lit->value.value));
      } catch (const std::exception& e) {
        herr->Error(token) << "Array size must be a numeric literal";
      }
    } else {
      herr->Error(token) << "Array size must be a constant expression";
    }
  }
  decl->add_array_bound(arr_size, outside_nested);
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

void TryParseParametersAndQualifiers(Declarator *decl, bool outside_nested, bool did_consume_paren,
                                     bool maybe_expression) {
  if (!did_consume_paren) {
    require_token(TT_BEGINPARENTH, "Expected '(' before function parameters");
  }

  bool is_expression = false;
  FunctionParameterNode params;
  params.outside_nested = outside_nested;
  params.parameters = FunctionParameterNode::ParameterList{};
  params.kind = FunctionParameterNode::Kind::DECLARATOR;
  if (token.type != TT_ENDPARENTH) {
    while (token.type != TT_ENDPARENTH) {
      if (is_decl_specifier(token) && maybe_expression) {
        auto declaration = TryParseEitherFunctionalCastOrDeclaration(
            AST::DeclaratorType::MAYBE_ABSTRACT, false, false,
            AST::DeclarationStatement::StorageClass::TEMPORARY);

        if (declaration->type == AST::NodeType::DECLARATION) {
          auto *param_decl = dynamic_cast<AST::DeclarationStatement *>(declaration.get());
          if (param_decl->declarations.size() != 1) {
            herr->Error(token) <<
                "Internal error: number of declarations in AstBuilder::TryParseParametersAndQualifiers not 1";
          } else {
            auto param = FunctionParameterNode::Parameter{
                false, param_decl->declarations[0].init.release(),
                std::unique_ptr<FullType>(param_decl->declarations[0].declarator.release())};
            params.as<FunctionParameterNode::ParameterList>().emplace_back(std::move(param));
          }
        } else {
          params.kind = FunctionParameterNode::Kind::EXPRESSION;
          is_expression = true;
          auto parameters =
            std::make_unique<AST::FunctionCallExpression>(nullptr, std::vector<std::unique_ptr<AST::Node>>{});

          for (auto &param : params.as<FunctionParameterNode::ParameterList>()) {
            auto decl_expr = std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(param.type->decl.to_expression()));
            if (param.default_value != nullptr) {
              AST::Operation op(TT_EQUALS,"=");
              decl_expr = std::make_unique<AST::BinaryExpression>(
                std::move(decl_expr), std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(param.default_value)),
                op);
              param.default_value = nullptr;
            }
            parameters->arguments.emplace_back(std::move(decl_expr));
          }
          params.parameters = reinterpret_cast<void *>(parameters.release());
        }
      } else if (is_expression) {
        if (!params.is<void *>()) {
          herr->Error(token) <<
            "Internal error: params.parameters is not FunctionCallExpression in AstBuilder::TryParseParametersAndQualifiers";
        } else {
          reinterpret_cast<AST::FunctionCallExpression *>(
            params.as<void *>())->arguments.emplace_back(TryParseExpression(Precedence::kTernary));
        }
      } else {
        FunctionParameterNode::Parameter param;
        FullType type;
        TryParseDeclSpecifierSeq(&type);
        TryParseDeclarator(&type, AST::DeclaratorType::MAYBE_ABSTRACT);
        param.type = std::make_unique<FullType>(std::move(type));
        if (token.type == TT_EQUALS) {
          token = lexer->ReadToken();
          auto init = TryParseExprOrBracedInitList(true, false);
          param.default_value = reinterpret_cast<void *>(init.release());
        }
        params.as<FunctionParameterNode::ParameterList>().emplace_back(std::move(param));
      }

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

  if (is_expression) {
    require_token(TT_ENDPARENTH, "Expected ')' after function arguments");
  } else {
    decl->add_function_params(std::move(params));
    require_token(TT_ENDPARENTH, "Expected ')' after function parameters");
  }

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

jdi::definition *TryParseIdExpression(Declarator *decl) {
  switch (token.type) {
    case TT_SCOPEACCESS: {
      token = lexer->ReadToken();
      if (next_is_start_of_id_expression() && token.type != TT_SCOPEACCESS) {
        return TryParseIdExpression(decl);
      } else {
        herr->Error(token) << "Expected qualified-id after '::', got: '" << token.content << '\'';
        return nullptr;
      }
    }

    case TT_DECLTYPE: {
      auto decltype_ = TryParseDecltype();
      if (token.type == TT_SCOPEACCESS) {
        return TryParseNestedNameSpecifier(decltype_, decl);
      } else {
        herr->Error(token) << "Expected qualified-id after decltype-expression, got: '" << token.content << '\'';
        return nullptr;
      }
    }

    case TT_IDENTIFIER: {
      if (false/*next_is_user_defined_type()*/) {
        return TryParsePrefixIdentifier(decl);
      } else {
        Token name = token;
        auto def = frontend->look_up(token.content);
        token = lexer->ReadToken();
        const bool is_declarator = decl;
        if (is_declarator && token.type != TT_SCOPEACCESS) {
          decl->name = name;  // If we're not accessing a scope then we're probably declaring a variable
          decl->ndef = def;
        } else if (token.type == TT_SCOPEACCESS) {
          return TryParseNestedNameSpecifier(def, decl);
        } else if (map_contains(declarations, name.content)) {
          return declarations[name.content]->def;
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
        FullType type = TryParseTypeID();
        if (type.def) {
          jdi::full_type t = type.to_jdi_fulltype();
          argk[args_given].ft().swap(t);
        }
      } else if (next_is_start_of_id_expression()) {
        herr->Error(token) << "Unimplemented: id-expressions as template arguments";
//        auto id = TryParseIdExpression(nullptr, false);
        // TODO: this thing
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

jdi::definition *TryParsePrefixIdentifier(Declarator *decl = nullptr, bool is_declarator = false) {
  Token id = token;
  require_token(TT_IDENTIFIER, "Expected identifier");
  auto def = require_defined_type(id);

  if (token.type == TT_LESS && is_template_type(def)) {
    TryParseTemplateArgs(def);
  }

  if (token.type == TT_SCOPEACCESS) {
    return TryParseNestedNameSpecifier(def, decl, is_declarator);
  }

  return def;
}

jdi::definition *TryParseNestedNameSpecifier(jdi::definition *scope, Declarator *decl = nullptr, bool is_declarator = false) {
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
    } else if (token.type != TT_STAR) {
      herr->Error(token) << "Expected either identifier or star ('*') after nested name specifier";
      return nullptr;
    }
  }

  if (is_declarator) {
    if (decl == nullptr) {
      herr->Error(name) << "Internal error: nullptr Declarator passed to TryParseNestedNameSpecifier()";
    } else {
      decl->name = name;
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

void TryParseElaboratedName(FullType *type) {
  Token tok = token;

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

  if (def != nullptr && matches_token_type(def, tok)) {
    type->def = def;
  } else {
    herr->Error(name) << "Given specifier does not refer to a declared enum";
  }
}

static bool contains_decflag_bitmask(std::size_t combined, std::string_view name) {
  auto builtin = jdi_decflag_bitmask(name);
  return (combined & builtin.first) == builtin.second;
}

void maybe_assign_full_type(FullType *type, jdi::definition *def, Token token) {
  if (def != nullptr && type->def == nullptr) {
    type->def = def;
  } else if (type->def != nullptr) {
    herr->Error(token) << "Usage of two types in type specifier";
  }
}

jdi::definition *get_builtin(std::string_view name) {
  auto it = jdi::builtin_primitives.find(std::string(name));
  if (it != jdi::builtin_primitives.end()) {
    return it->second;
  }
  return frontend->look_up(std::string(name));
}

void TryParseTypeSpecifier(FullType *type) {
  switch (token.type) {
    case TT_TYPE_NAME: {
      if (token.content == "long" || token.content == "short") {
        if (contains_decflag_bitmask(type->flags, "long")) {
           if (token.content == "long") {
             type->flags &= ~jdi_decflag_bitmask("long").second;
             type->flags |= jdi_decflag_bitmask("long long").second;
           } else if (token.content == "short") {
             herr->Error(token) << "Conflicting usage of 'long' and 'short' in the same type specifier";
           }
        } else if (contains_decflag_bitmask(type->flags, "short") && token.content == "long") {
          herr->Error(token) << "Conflicting usage of 'short' and 'long' in the same type specifier";
        } else if (contains_decflag_bitmask(type->flags, "long long")) {
          if (token.content == "long") {
            herr->Error(token) << "Too many 'long's in type specifier";
          } else if (token.content == "short") {
            herr->Error(token) << "Conflicting usage of 'short' and 'long long' in the same type specifier";
          }
        } else {
          type->flags |= jdi_decflag_bitmask(token.content).second;
        }
      } else {
        maybe_assign_full_type(type, get_builtin(token.content), token);
      }
      token = lexer->ReadToken();
      break;
    }

    case TT_IDENTIFIER: {
      maybe_assign_full_type(type, TryParsePrefixIdentifier(), token);
      break;
    }

    case TT_SCOPEACCESS: {
      maybe_assign_full_type(type, TryParseNestedNameSpecifier(nullptr), token);
      break;
    }

    case TT_DECLTYPE: {
      Token tok = token;
      auto def = TryParseDecltype();

      if (token.type == TT_SCOPEACCESS) {
        def = TryParseNestedNameSpecifier(def);
      }

      if (def != nullptr) {
        maybe_assign_full_type(type, def, tok);
      } else {
        herr->Error(tok) << "Could not parse decltype specifier";
      }
      break;
    }

    case TT_TYPENAME: {
      maybe_assign_full_type(type, TryParseTypenameSpecifier(), token);
      break;
    }

    default: {
      if (token.type == TT_SIGNED || token.type == TT_UNSIGNED || next_is_cv_qualifier()) {
        //        if (contains_decflag_bitmask(type->flags, "signed") && token.type == TT_UNSIGNED) {
        //          // TODO: There is no way to actually detect this, as signed's value is 0
        //          herr->Error(token) << "Conflicting use of 'signed' and 'unsigned' in the same type specifier";
        //        } else
        if (contains_decflag_bitmask(type->flags, "unsigned") && token.type == TT_SIGNED) {
          herr->Error(token) << "Conflicting use of 'unsigned' and 'signed' in the same type specifier";
        } else if (contains_decflag_bitmask(type->flags, token.content) && token.type != TT_SIGNED) {
          herr->Warning(token) << "Duplicate usage of flags in type specifier";
        } else {
          type->flags |= jdi_decflag_bitmask(token.content).second;
        }
        token = lexer->ReadToken();
      } else if (next_is_class_key() || token.type == TT_ENUM) {
        TryParseElaboratedName(type);
      } else {
        herr->Error(token) << "Given token does not specify a valid type specifier";
      }
      break;
    }
  }
}

bool TryParseTypeSpecifierSeq(FullType *type) {
  bool is_global = false;
  while (next_is_type_specifier() || token.content == "global") {
    if (token.content == "global") {
      is_global = true;
      token = lexer->ReadToken();
    } else {
      TryParseTypeSpecifier(type);
    }
  }
  return is_global;
}

void TryParsePtrOperator(FullType *type) {
  if (token.type == TT_STAR) {
    bool is_const = false;
    bool is_volatile = false;
    token = lexer->ReadToken();
    while (next_is_cv_qualifier()) {
      if (token.type == TT_CONST) {
        if (is_const) {
          herr->Warning(token) << "Duplicate 'const' flag in pointer";
        }
        is_const = true;
      } else if (token.type == TT_VOLATILE) {
        if (is_volatile) {
          herr->Warning(token) << "Duplicate 'volatile' flag in pointer";
        }
        is_volatile = true;
      }
      token = lexer->ReadToken();
    }
    type->decl.add_pointer(nullptr, is_const, is_volatile);
  } else if (next_is_ref_qualifier()) {
    if (token.type == TT_AND) {
      type->decl.add_reference(DeclaratorNode::Kind::RVAL_REFERENCE);
    } else {
      type->decl.add_reference(DeclaratorNode::Kind::REFERENCE);
    }
    token = lexer->ReadToken();
  }
}

void TryParseMaybeNestedPtrOperator(FullType *type) {
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
      bool is_const = false;
      bool is_volatile = false;
      token = lexer->ReadToken();
      while (next_is_cv_qualifier()) {
        if (token.type == TT_CONST) {
          if (is_const) {
            herr->Warning(token) << "Duplicate 'const' flag in pointer";
          }
          is_const = true;
        } else if (token.type == TT_VOLATILE) {
          if (is_volatile) {
            herr->Warning(token) << "Duplicate 'volatile' flag in pointer";
          }
          is_volatile = true;
        }
        token = lexer->ReadToken();
      }
      if (!(def->flags & jdi::DEF_CLASS)) {
        herr->Error(token) << "Member pointer to non-class type: '" << def->name << "'";
      } else {
        type->decl.add_pointer(reinterpret_cast<jdi::definition_class *>(def), is_const, is_volatile);
      }
    }
  }
}

FullType TryParseTypeID() {
  FullType type;
  while (next_is_type_specifier()) {
    TryParseTypeSpecifier(&type);
  }

  maybe_infer_int(type);

  if (next_maybe_ptr_decl_operator() || token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
    TryParseDeclarator(&type, AST::DeclaratorType::ABSTRACT);
  }

  return type;
}

void TryParseDeclSpecifier(FullType *type) {
  switch (token.type) {
    case TT_TYPEDEF: {
      TryParseTypeSpecifierSeq(type);
      TryParseDeclarator(type, AST::DeclaratorType::NON_ABSTRACT);
      break;
    }

    case TT_CONSTEXPR:
    case TT_CONSTINIT:
    case TT_CONSTEVAL:
    case TT_EXTERN:
    case TT_THREAD_LOCAL: {
      herr->Error(token) << "Unimplemented: '" << token.content << '\'';
      token = lexer->ReadToken();
      break;
    }

    case TT_MUTABLE:
    case TT_INLINE:
    case TT_STATIC: {
      type->flags |= jdi_decflag_bitmask(token.content).second;
      token = lexer->ReadToken();
    }

    default:
      if (next_is_type_specifier()) {
        TryParseTypeSpecifier(type);
        break;
      }
  }
}

bool TryParseDeclSpecifierSeq(FullType *type) {
  bool is_global = false;
  while (is_decl_specifier(token) || token.content == "global") {
    if (token.content == "global")
      is_global = true, token = lexer->ReadToken();
    else
      TryParseDeclSpecifier(type);
  }
  return is_global;
}

std::unique_ptr<AST::Node> TryParsePtrDeclarator(FullType *type, AST::DeclaratorType is_abstract, bool maybe_expression = false) {
  while (next_maybe_ptr_decl_operator()) {
    if (next_maybe_nested_name()) {
      TryParseMaybeNestedPtrOperator(type);
    } else {
      TryParsePtrOperator(type);
    }
  }

  return TryParseNoPtrDeclarator(type, is_abstract, maybe_expression);
}

std::unique_ptr<AST::Node> TryParseNoPtrDeclarator(FullType *type, AST::DeclaratorType is_abstract, bool maybe_expression = false) {
  auto maybe_prefix_operator = [this]() {
    return Precedence::kUnaryPrefixOps.find(token.type) != Precedence::kUnaryPrefixOps.end();
  };

  auto maybe_infix_operator = [this]() {
    return map_contains(Precedence::kBinaryPrec, token.type) ||
           map_contains(Precedence::kTernaryPrec, token.type);
  };

  auto maybe_postfix_operator = [this]() {
    return map_contains(Precedence::kUnaryPostfixPrec, token.type);
  };

  // Do not accidentally consume the pointer declarators
  if (maybe_expression && maybe_prefix_operator() &&
      token.type != TT_STAR && token.type != TT_AMPERSAND) {
    return TryParseExpression(Precedence::kAll);
  }

  if (token.type == TT_BEGINPARENTH) {
    std::unique_ptr<AST::Node> expr = nullptr;
    token = lexer->ReadToken();
    FullType inner;
    auto inner_decl_expr = TryParsePtrDeclarator(&inner, is_abstract, maybe_expression);
    // Check if the next token is an operator but don't accidentally eat array bounds specifiers or function parameter
    // declarators
    if (maybe_expression && (maybe_infix_operator() || maybe_postfix_operator()) &&
        token.type != TT_BEGINPARENTH && token.type != TT_BEGINBRACKET) {
      if (inner_decl_expr == nullptr) {
        inner_decl_expr = TryParseExpression(Precedence::kAll,
                                        std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(inner.decl.to_expression())));
      } else {
        inner_decl_expr = TryParseExpression(Precedence::kAll, std::move(inner_decl_expr));
      }
      require_token(TT_ENDPARENTH, "Expected ')' after expression");
    } else if (inner_decl_expr == nullptr) {
      require_token(TT_ENDPARENTH, "Expected ')' after declarator");
      if (!inner.decl.name.content.empty()) {
        type->decl.name = inner.decl.name;
      }
    } else {
      require_token(TT_ENDPARENTH, "Expected ')' after expression");
    }

    while (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
      if (token.type == TT_BEGINPARENTH) {
        TryParseParametersAndQualifiers(&inner.decl, true, false, maybe_expression);
      } else {
        TryParseArrayBoundsExpression(&inner.decl, true);
      }
    }

    if (inner_decl_expr != nullptr) {
      type->decl.add_nested(reinterpret_cast<void *>(inner_decl_expr.release()));
      inner_decl_expr = std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(type->decl.to_expression()));
    } else {
      type->decl.add_nested(std::make_unique<Declarator>(std::move(inner.decl)));
    }

    if (maybe_expression && (maybe_infix_operator() || maybe_postfix_operator()) &&
        token.type != TT_BEGINPARENTH && token.type != TT_BEGINBRACKET &&
        token.type != TT_EQUALS && token.type != TT_BEGINBRACE && token.type != TT_COMMA) {
      if (inner_decl_expr != nullptr) {
        return TryParseExpression(Precedence::kAll, std::move(inner_decl_expr));
      } else {
        return TryParseExpression(
            Precedence::kAll, std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(type->decl.to_expression())));
      }
    } else if (inner_decl_expr != nullptr) {
      return inner_decl_expr;
    }
  } else if (is_abstract == AST::DeclaratorType::NON_ABSTRACT) {
    if (token.type == TT_ELLIPSES) {
      token = lexer->ReadToken();
    }
    TryParseIdExpression(&type->decl);
  } else if (is_abstract == AST::DeclaratorType::MAYBE_ABSTRACT && next_is_start_of_id_expression()) {
    TryParseIdExpression(&type->decl);
  }

  while (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACKET) {
    if (token.type == TT_BEGINPARENTH) {
      TryParseParametersAndQualifiers(&type->decl, false, false, maybe_expression);
    } else {
      TryParseArrayBoundsExpression(&type->decl, false);
    }
  }

  // All the array bounds specifiers and function parameter declarators would have been eaten before this
  if (maybe_expression && (maybe_infix_operator() || maybe_postfix_operator()) &&
      token.type != TT_EQUALS && token.type != TT_BEGINBRACE && token.type != TT_COMMA) {
    return TryParseExpression(Precedence::kAll,
                              std::unique_ptr<AST::Node>(reinterpret_cast<AST::Node *>(type->decl.to_expression())));
  }

  return nullptr;
}

void TryParseDeclarator(FullType *type, AST::DeclaratorType is_abstract = AST::DeclaratorType::NON_ABSTRACT) {
  if (next_maybe_ptr_decl_operator()) {
    TryParsePtrDeclarator(type, is_abstract);
  } else {
    TryParseNoPtrDeclarator(type, is_abstract);
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

AST::BraceOrParenInitNode TryParseInitializerList(TokenType closing) {
  AST::BraceOrParenInitNode init = std::make_unique<AST::BraceOrParenInitializer>();
  while (token.type != closing) {
    init->values.emplace_back("", TryParseExprOrBracedInitList(true, true));
    if (token.type == TT_COMMA) {
      token = lexer->ReadToken();
    } else {
      break;
    }
  }
  return init;
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
    init = TryParseInitializerList(TT_ENDBRACE);
    init->kind = AST::BraceOrParenInitializer::Kind::BRACE_INIT;
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
          if (token.type == TT_COMMA) {
            token = lexer->ReadToken();
          } else {
            break;
          }
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

void maybe_assign_def(FullType *type) {
  if ((contains_decflag_bitmask(type->flags, "long long") || contains_decflag_bitmask(type->flags, "long") ||
       contains_decflag_bitmask(type->flags, "short")) &&
      type->def == nullptr) {
    maybe_assign_full_type(type, get_builtin("int"), token);
  }
}

std::unique_ptr<AST::Node> TryParseDeclarations(bool parse_unbounded) {
  bool is_global = token.content == "global";
  if (is_decl_specifier(token)||token.content == "global") {
    FullType type;
    is_global = TryParseDeclSpecifierSeq(&type);
    maybe_infer_int(type);
    maybe_assign_def(&type);
    if (type.def == nullptr) {
      herr->Error(token) << "Unable to parse type specifier in declaration";
      return nullptr;
    }

    auto sc = is_global ? AST::DeclarationStatement::StorageClass::GLOBAL
                        : AST::DeclarationStatement::StorageClass::TEMPORARY;
    return parse_declarations(sc, type, AST::DeclaratorType::NON_ABSTRACT, parse_unbounded, {});
  } else {
    return nullptr;
  }
}

std::unique_ptr<AST::Node> TryParseNewExpression(bool is_global) {
  require_token(TT_S_NEW, "Expected 'new' in new-expression");

  bool is_array = false;
  AST::InitializerNode placement = nullptr;
  FullType type;
  AST::InitializerNode initializer = nullptr;

  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    if (next_is_type_specifier()) {
      type = TryParseTypeID();
      require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after new-expression type");
      if (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACE) {
        initializer = TryParseInitializer(true);
      }
      is_array = !type.decl.components.empty() &&
                 type.decl.components.begin()->kind == DeclaratorNode::Kind::ARRAY_BOUND;

      MaybeConsumeSemicolon();

      return std::make_unique<AST::NewExpression>(is_global, is_array, std::move(placement), std::move(type), std::move(initializer));
    } else {
      auto init = TryParseInitializerList(TT_ENDPARENTH);
      init->kind = AST::BraceOrParenInitializer::Kind::PAREN_INIT;
      placement = AST::Initializer::from(std::move(init));
      placement->kind = AST::Initializer::Kind::PLACEMENT_NEW;
      require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after placement-new arguments");
    }
  }

  // At this point we have handled:
  // ::? new ( <type-id> ) <new-initializer>?

  // Remaining:
  // ::? new <new-placement> ( <type-id> ) <new-initializer>?
  // ::? new <new-placement> <new-type-id> <new-initializer>?
  // ::? new <new-type-id> <new-initializer>?
  //
  // This code path is taken only when <new-placement> is present, otherwise the paren would've been picked up earlier
  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    type = TryParseTypeID();
    require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after new-expression type");
  } else {
    TryParseTypeSpecifierSeq(&type);
    while (next_maybe_ptr_decl_operator()) {
      if (next_maybe_nested_name()) {
        TryParseMaybeNestedPtrOperator(&type);
      } else {
        TryParsePtrOperator(&type);
      }
    }

    while (token.type == TT_BEGINBRACKET) {
      TryParseArrayBoundsExpression(&type.decl, false);
    }
  }

  if (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACE) {
    initializer = TryParseInitializer(true);
  }

  is_array = !type.decl.components.empty() &&
             type.decl.components.begin()->kind == DeclaratorNode::Kind::ARRAY_BOUND;

  MaybeConsumeSemicolon();

  return std::make_unique<AST::NewExpression>(is_global, is_array, std::move(placement), std::move(type), std::move(initializer));
}

std::unique_ptr<AST::Node> TryParseDeleteExpression(bool is_global) {
  require_token(TT_S_DELETE, "Expected 'delete' in delete-expression");

  bool is_array = false;
  if (token.type == TT_BEGINBRACKET) {
    token = lexer->ReadToken();
    is_array = true;
    require_token(TT_ENDBRACKET, "Expected ']' to close '[' in delete-expression");
  }

  auto node =  std::make_unique<AST::DeleteExpression>(is_global, is_array, TryParseExpression(Precedence::kUnaryPrefix));
  MaybeConsumeSemicolon();

  return node;
}

std::unique_ptr<AST::Node> TryParseIdExpression() {
  Declarator decl;
  auto def = TryParseIdExpression(&decl);
  
  if (decl.name.content.empty()) {
    herr->Error(token) << "Unable to parse id-expression";
    return nullptr;
  } else if (map_contains(declarations, decl.name.content)) {
    return std::make_unique<AST::IdentifierAccess>(declarations[decl.name.content], decl.name);
  } else {
    return std::make_unique<AST::IdentifierAccess>(def, decl.name);
  }
}

/// Parse an operand--this includes variables, literals, arrays, and
/// unary expressions on these.
std::unique_ptr<AST::Node> TryParseOperand() {
  switch (token.type) {
    case TT_BEGINBRACE: case TT_ENDBRACE:
    case TT_ENDPARENTH: case TT_ENDBRACKET:
    case TT_ENDOFCODE: case TT_SEMICOLON:
      return nullptr;
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
    
    case TT_JS_ARROW:
      herr->Error(token) << "Expected parameter list before '=>'";
      token = lexer->ReadToken();
      return nullptr;

    case TT_QMARK:
      herr->Error(token) << "Expected expression before ternary operator ?";
      token = lexer->ReadToken();
      return nullptr;

    case TT_NOT: case TT_BANG: case TT_PLUS: case TT_MINUS:
    case TT_STAR: case TT_AMPERSAND:
    case TT_INCREMENT: case TT_DECREMENT: {
      Token unary_op = token;
      token = lexer->ReadToken();

      if (auto exp = TryParseExpression(Precedence::kUnaryPrefix)) {
        AST::Operation op(unary_op.type, std::string(unary_op.content));
        return std::make_unique<AST::UnaryPrefixExpression>(std::move(exp), op);
      }
      herr->Error(unary_op) << "Expected expression following unary operator, got: '" << token.content << '\'';
      return nullptr;
    }

    case TT_BEGINPARENTH: {
      auto paren = token;
      token = lexer->ReadToken();
      if (next_is_type_specifier()) {
        FullType type = TryParseTypeID();
        require_token(TT_ENDPARENTH, "Expected closing parenthesis before '", token.content, "'");
        auto expr = TryParseExpression(Precedence::kUnaryPrefix);
        return std::make_unique<AST::CastExpression>(paren, std::move(type), std::move(expr), TT_BEGINPARENTH);
      } else {
        auto exp = TryParseExpression(Precedence::kAll);
        require_token(TT_ENDPARENTH, "Expected closing parenthesis before '", token.content, "'");
        return std::make_unique<AST::Parenthetical>(std::move(exp));
      }
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

    case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
    case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT: {
      Token res = token;
      token = lexer->ReadToken();
      return std::make_unique<AST::Literal>(std::move(res));
    }

    case TT_SIZEOF: {
      token = lexer->ReadToken();
      if (token.type == TT_BEGINPARENTH) {
        token = lexer->ReadToken();
        FullType type = TryParseTypeID();
        require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after sizeof-expression");
        return std::make_unique<AST::SizeofExpression>(std::move(type));
      } else if (token.type == TT_ELLIPSES) {
        token = lexer->ReadToken();
        require_token(TT_BEGINPARENTH, "Expected opening '(' after 'sizeof ...'");
        auto arg = token;
        if (require_token(TT_IDENTIFIER, "Expected identifier as argument to variadic sizeof")) {
          require_token(TT_ENDPARENTH, "Expected closing ')' after variadic sizeof");
          return std::make_unique<AST::SizeofExpression>(std::string{arg.content});
        } else {
          return nullptr;
        }
      } else {
        auto operand = TryParseExpression(Precedence::kUnaryPrefix);
        return std::make_unique<AST::SizeofExpression>(std::move(operand));
      }
    }

    case TT_ALIGNOF: {
      token = lexer->ReadToken();
      if (require_token(TT_BEGINPARENTH, "Expected opening parenthesis ('(') after 'alignof'")) {
        FullType type = TryParseTypeID();
        require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after alignof-expression");
        return std::make_unique<AST::AlignofExpression>(std::move(type));
      } else {
        return nullptr;
      }
    }

    case TT_CO_AWAIT: {
      auto oper = token;
      token = lexer->ReadToken();
      auto expr = TryParseExpression(Precedence::kUnaryPrefix);
      AST::Operation op(oper.type, std::string(oper.content));
      return std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), op);
    }

    case TT_NOEXCEPT: {
      auto oper = token;
      token = lexer->ReadToken();
      if (require_token(TT_BEGINPARENTH, "Expected opening '(' after noexcept")) {
        token = lexer->ReadToken();
        auto expr = TryParseExpression(Precedence::kAll);
        require_token(TT_ENDPARENTH, "Expected closing ')' after noexcept expression");
        AST::Operation op(oper.type, std::string(oper.content));
        return std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), op);
      } else {
        return nullptr;
      }
    }

    case TT_DYNAMIC_CAST:
    case TT_STATIC_CAST:
    case TT_REINTERPRET_CAST:
    case TT_CONST_CAST: {
      Token oper = token;
      token = lexer->ReadToken();
      require_token(TT_LESS, "Expected '<' after '", oper.content, "'");
      FullType type = TryParseTypeID();
      require_token(TT_GREATER, "Expected '>' after '", oper.content, "' type");
      require_token(TT_BEGINPARENTH, "Expected '(' before '", oper.content, "' expression");
      auto expr = TryParseExpression(Precedence::kAll);
      require_token(TT_ENDPARENTH, "Expected ')' after '", oper.content, "' expression");
      return std::make_unique<AST::CastExpression>(oper, std::move(type), std::move(expr), TT_ERROR);
    }

    case TT_SCOPEACCESS: {
      token = lexer->ReadToken();
      if (token.type == TT_S_NEW) {
        return TryParseNewExpression(true);
      } else if (token.type == TT_S_DELETE) {
        return TryParseDeleteExpression(true);
      } else {
        return TryParseIdExpression();
      }
    }

    case TT_TILDE: {
      if (!next_is_user_defined_type() && token.type != TT_DECLTYPE) {
        Token unary_op = token;
        token = lexer->ReadToken();

        if (auto exp = TryParseExpression(Precedence::kUnaryPrefix)) {
          AST::Operation op(unary_op.type, std::string(unary_op.content));
          return std::make_unique<AST::UnaryPrefixExpression>(std::move(exp), op);
        }
        herr->Error(unary_op) << "Expected expression following unary operator";
        return nullptr;
      } else {
        [[fallthrough]];
      }
    }

    case TT_DECLTYPE: {
      return TryParseIdExpression();
    }

    case TT_S_NEW: return TryParseNewExpression(false);
    case TT_S_DELETE: return TryParseDeleteExpression(false);

    case TT_IDENTIFIER:
    case TT_TYPE_NAME:
    case TT_GLOBAL:
    case TT_LOCAL: {
      if (next_maybe_functional_cast()) {
        FullType type;
        TryParseTypeSpecifier(&type);
        if (token.type == TT_BEGINPARENTH) {
          auto tok = token;
          token = lexer->ReadToken();
          auto expr = TryParseExpression(Precedence::kAll);
          require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after functional cast");
          return std::make_unique<AST::CastExpression>(AST::CastExpression::Kind::FUNCTIONAL, tok,
                                                       std::move(type), std::move(expr), TT_BEGINPARENTH);
        } else if (token.type == TT_BEGINBRACE) {
          auto tok = token;
          auto init = TryParseInitializer(false);
          require_token(TT_ENDBRACE, "Expected closing brace ('}') after temporary object initializer");
          return std::make_unique<AST::CastExpression>(AST::CastExpression::Kind::FUNCTIONAL, tok,
                                                       std::move(type), std::move(init), TT_BEGINBRACE);
        } else {
          herr->Error(token) << "Expected opening parenthesis ('(') or brace ('{') after functional-cast type";
          return nullptr;
        }
      } else {
        return TryParseIdExpression();
      }
      herr->Error(token) << "INTERNAL ERROR: " __FILE__ ":" << __LINE__ << ": Unreachable";
      return nullptr;
    }

    case TT_RETURN:   case TT_EXIT:   case TT_BREAK:   case TT_CONTINUE:
    case TT_S_SWITCH: case TT_S_CASE: case TT_S_DEFAULT:
    case TT_S_FOR:    case TT_S_DO:   case TT_S_WHILE: case TT_S_UNTIL:
    case TT_S_REPEAT: case TT_S_IF:   case TT_S_THEN:  case TT_S_ELSE:
    case TT_S_WITH:   case TT_S_TRY:  case TT_S_CATCH:

    case TT_CLASS:    case TT_STRUCT:
    case TTM_WHITESPACE: case TTM_CONCAT: case TTM_STRINGIFY:
      return nullptr;

    case TT_ELLIPSES: {
      herr->Error(token) << "Stray ellipses ('...') in program";
      token = lexer->ReadToken();
      return nullptr;
    }

    case TT_TYPENAME: case TT_OPERATOR: case TT_CONSTEXPR:
    case TT_CONSTINIT: case TT_CONSTEVAL: case TT_INLINE:
    case TT_STATIC: case TT_THREAD_LOCAL: case TT_EXTERN:
    case TT_MUTABLE: case TT_UNION: case TT_SIGNED:
    case TT_UNSIGNED: case TT_CONST: case TT_VOLATILE:
    case TT_ENUM: case TT_TYPEDEF: {
      herr->Error(token) << "Unexpected declarator";
      return nullptr;
    }

    case TT_ERROR:
      return nullptr;
  }
  herr->Error(token)
      << "Internal error: unreachable (" __FILE__ ":" << __LINE__ << ")";
  return nullptr;
}

static bool ShouldAcceptPrecedence(const OperatorPrecedence &prec,
                                   int target_prec) {
  return target_prec >= prec.precedence ||
            (target_prec == prec.precedence &&
                prec.associativity == Associativity::RTL);
}

std::unique_ptr<AST::Node> TryParseLambdaExpression(std::unique_ptr<AST::Node> operand) {
  token = lexer->ReadToken();
  auto body = ParseStatementOrBlock();
  return std::make_unique<AST::LambdaExpression>(std::move(operand), std::move(body));
}

std::unique_ptr<AST::Node> TryParseExpression(int precedence, std::unique_ptr<AST::Node> operand = nullptr) {
  if (operand == nullptr) {
    operand = TryParseOperand();
  }
  if (operand != nullptr) {
    if (operand->type == AST::NodeType::DELETE || operand->type == AST::NodeType::NEW) {
      return operand;
    }
    // TODO: Handle binary operators, unary postfix operators
    // (including function calls, array indexing, etc).
    // XXX: Maybe handle TT_IDENTIFIER here when `operand` names a type
    // to parse a declaration as an expression. This is a bold move, but
    // more robust than handling it in TryParseExpression.
    while (token.type != TT_ENDOFCODE) {
      if(token.type == TT_JS_ARROW){
        operand = TryParseLambdaExpression(std::move(operand));
      } else if (auto find_binop = Precedence::kBinaryPrec.find(token.type); find_binop != Precedence::kBinaryPrec.end()) {
        if (!ShouldAcceptPrecedence(find_binop->second, precedence)) {
          break;
        }
        operand = TryParseBinaryExpression(precedence, std::move(operand));
      } else if (map_contains(Precedence::kUnaryPostfixPrec, token.type)) {
        if (operand->type == AST::NodeType::BINARY_EXPRESSION) {
          auto exp = operand->As<AST::BinaryExpression>();
          if (exp->operation.type != TT_DOT && exp->operation.type != TT_ARROW) {
            break;
          }
        }
        bool valid_operand_type =
            operand->type == AST::NodeType::IDENTIFIER || operand->type == AST::NodeType::BINARY_EXPRESSION;
        if (precedence < Precedence::kUnaryPostfix || !valid_operand_type) {
          break;
        }
        operand = TryParseUnaryPostfixExpression(precedence, std::move(operand));
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

    if(token.type == TT_ENDOFCODE || token.type == TT_SEMICOLON){ // there are more cases
      herr->Error(token) << "Uncompleted binary expression";
    }

    auto right = (rule.associativity == Associativity::LTR)
        ? TryParseExpression(rule.precedence - 1)
        : (rule.associativity == Associativity::RTL)
            ? TryParseExpression(rule.precedence)
            : nullptr;

    AST::Operation op(oper.type, std::string(oper.content));
    operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), op);
  }

  return dynamic_unique_pointer_cast<AST::BinaryExpression>(std::move(operand));
}

std::unique_ptr<AST::UnaryPostfixExpression> TryParseUnaryPostfixExpression(
    int precedence, std::unique_ptr<AST::Node> operand) {
  if (Precedence::kUnaryPostfixPrec.find(token.type) != Precedence::kUnaryPostfixPrec.end() &&
      precedence >= Precedence::kUnaryPostfixPrec[token.type].precedence) {
    Token oper = token;
    token = lexer->ReadToken();  // Consume the operator
    AST::Operation op(oper.type, std::string(oper.content));
    operand = std::make_unique<AST::UnaryPostfixExpression>(std::move(operand), op);
  }
  return dynamic_unique_pointer_cast<AST::UnaryPostfixExpression>(std::move(operand));
}

std::unique_ptr<AST::TernaryExpression> TryParseTernaryExpression(int precedence, std::unique_ptr<AST::Node> operand) {
  (void)precedence;
//  Token oper = token;
  token = lexer->ReadToken(); // Consume the operator

  auto middle = TryParseExpression(Precedence::kBoolOr);

  require_token(TT_COLON, "Expected colon (':') after expression in conditional operator");

  auto right = TryParseExpression(Precedence::kTernary);
  operand = std::make_unique<AST::TernaryExpression>(std::move(operand), std::move(middle), std::move(right));

  return dynamic_unique_pointer_cast<AST::TernaryExpression>(std::move(operand));
}

std::unique_ptr<AST::BinaryExpression> TryParseSubscriptExpression(int precedence, std::unique_ptr<AST::Node> operand) {
  (void)precedence;
  while (token.type == TT_BEGINBRACKET) {
    Token oper = token;
    token = lexer->ReadToken(); // Consume the operator

    auto right = TryParseExpression(Precedence::kMin);

    AST::Operation op(oper.type, std::string(oper.content));
    operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), op);

    require_token(TT_ENDBRACKET, "Expected closing bracket (']') at the end of array subscript");
  }

  return dynamic_unique_pointer_cast<AST::BinaryExpression>(std::move(operand));
}

std::unique_ptr<AST::FunctionCallExpression> TryParseFunctionCallExpression(int precedence, std::unique_ptr<AST::Node> operand) {
  (void)precedence;
  while (token.type == TT_BEGINPARENTH) {
    // Token oper = token;
    token = lexer->ReadToken(); // Consume the operator

    std::vector<std::unique_ptr<AST::Node>> arguments{};
    while (token.type != TT_ENDPARENTH && token.type != TT_ENDOFCODE) {
      arguments.emplace_back(TryParseExpression(Precedence::kTernary, nullptr));
      if (token.type != TT_COMMA && token.type != TT_ENDPARENTH) {
        herr->Error(token) << "Expected ',' or ')' after function argument";
        break;
      } else if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
      }
    }

    require_token(TT_ENDPARENTH, "Expected ')' after function call");
    operand = std::make_unique<AST::FunctionCallExpression>(std::move(operand), std::move(arguments));
  }

  return dynamic_unique_pointer_cast<AST::FunctionCallExpression>(std::move(operand));
}

std::unique_ptr<AST::Node> TryParseControlExpression(SyntaxMode mode_) {
  if (mode_ != setting::SyntaxMode::GML && mode != setting::SyntaxMode::QUIRKS && mode != setting::SyntaxMode::STRICT) {
    herr->Error(token) << "Internal error: unreachable (" __FILE__ ":" << __LINE__ << "): SyntaxMode " << (int)mode_
                       << " unknown to system";
  }

  if (mode_ == SyntaxMode::STRICT) {
    require_token(TT_BEGINPARENTH, "Expected '(' before control expression");
  }

  auto expr = TryParseExpression(Precedence::kAll);

  if (mode_ == SyntaxMode::STRICT) {
    require_token(TT_ENDPARENTH, "Expected ')' after control expression");
  }

  return expr;
}

std::unique_ptr<AST::Node> TryParseDeclOrTypeExpression() {
  if (next_is_decl_specifier() || next_maybe_functional_cast()) {
    if (token.type == TT_SCOPEACCESS) {
      token = lexer->ReadToken();
      if (token.type == TT_S_NEW) {
        return TryParseNewExpression(true);
      } else if (token.type == TT_S_DELETE) {
        return TryParseDeleteExpression(true);
      }
    }
    return TryParseEitherFunctionalCastOrDeclaration(AST::DeclaratorType::NON_ABSTRACT, true, true,
                                                     AST::DeclarationStatement::StorageClass::TEMPORARY);
  }
  return nullptr;
}

// since we can place declarations in the same places we can place statements,
// so this function will check first if this is a declaration, to be able to handle both
std::unique_ptr<AST::Node> TryParseStatement() {
  auto decl_node = TryParseDeclOrTypeExpression();
  if (decl_node != nullptr) {
    MaybeConsumeSemicolon();
    return decl_node;
  }
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
    case TT_SCOPEACCESS: case TT_CO_AWAIT:
    case TT_NOEXCEPT: case TT_ALIGNOF: case TT_SIZEOF:
    case TT_STATIC_CAST: case TT_DYNAMIC_CAST:
    case TT_REINTERPRET_CAST: case TT_CONST_CAST:
    case TT_S_NEW: case TT_S_DELETE:{
      auto node = TryParseExpression(Precedence::kAll);
      MaybeConsumeSemicolon();
      return node;
    }

    case TT_ENDBRACE:
      return nullptr;

    case TT_BEGINBRACE: {
      herr->Error(token) << "Internal error: trying to parse <block-stmt> within <stmt>";
      return ParseCodeBlock(); // Parse it anyways
    }

    case TT_DECLTYPE:
    case TT_TYPENAME:
    case TT_IDENTIFIER:
    case TT_TYPE_NAME:
    case TT_TYPEDEF:
    case TT_CONSTEXPR: case TT_CONSTINIT: case TT_CONSTEVAL:
    case TT_SIGNED: case TT_UNSIGNED: case TT_CONST: case TT_VOLATILE:
    case TT_INLINE: case TT_STATIC: case TT_EXTERN:
    case TT_MUTABLE: case TT_THREAD_LOCAL: {
      AST::DeclarationStatement::StorageClass sc;
      if (false) {
        if (false) case TT_LOCAL:
          sc = AST::DeclarationStatement::StorageClass::LOCAL;
        if (false) case TT_GLOBAL:
          sc = AST::DeclarationStatement::StorageClass::GLOBAL;
      } else {
        sc = AST::DeclarationStatement::StorageClass::TEMPORARY;
      }
      bool is_global = token.content == "global";
      Token maybe_global = token;
      if (is_global) {
        token = lexer->ReadToken();
        sc = AST::DeclarationStatement::StorageClass::GLOBAL;
      }
      if (is_decl_specifier(token) || next_maybe_functional_cast() || (is_global && token.type != TT_DOT)) {
        Token start = token;
        auto decl = TryParseEitherFunctionalCastOrDeclaration(AST::DeclaratorType::NON_ABSTRACT, true, false, sc);
        MaybeConsumeSemicolon();
        return decl;
      } else {
        jdi::definition *def = nullptr;
        std::unique_ptr<AST::Node> operand =
            is_global ? std::make_unique<AST::IdentifierAccess>(def, maybe_global) : nullptr;
        auto node = TryParseExpression(Precedence::kAll, std::move(operand));
        MaybeConsumeSemicolon();
        return node;
      }
    }

    case TT_RETURN: return ParseReturnStatement();
    case TT_EXIT: return ParseExitStatement();
    case TT_BREAK: return ParseBreakStatement();
    case TT_CONTINUE: return ParseContinueStatement();
    case TT_S_SWITCH: return ParseSwitchStatement();
    case TT_S_REPEAT: return ParseRepeatStatement();
    case TT_S_CASE:   return ParseCaseOrDefaultStatement(false);
    case TT_S_DEFAULT: return ParseCaseOrDefaultStatement(true);
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

    case TT_JS_ARROW:
      herr->ReportError(token, "`=>` not paired with a lambda expression");
      token = lexer->ReadToken();
      return nullptr;

    case TT_S_TRY: case TT_S_CATCH: {
      herr->Error(token) << "Unimplemented: try/catch statements";
      return nullptr;
    }

    case TT_ELLIPSES: {
      herr->Error(token) << "Stray ellipses ('...') in the program";
      token = lexer->ReadToken();
      return nullptr;
    }

    case TT_ENUM: case TT_CLASS: case TT_STRUCT: case TT_UNION:
    case TT_OPERATOR: {
      herr->Error(token) << "Trying to read declaration within <stmt>";
      return TryParseDeclarations(true); // Parse it anyways
    }

    case TT_ENDOFCODE: return nullptr;
  }
  herr->Error(token)
      << "Internal error: unreachable (" __FILE__ ":" << __LINE__ << ")";
  return nullptr;
}

// Parse control flow statement body
std::unique_ptr<AST::Node> ParseCFStmtBody() { return ParseStatementOrBlock(); }

bool next_is_decl_specifier() {
  return is_decl_specifier(token);
}

std::unique_ptr<AST::Node> ParseStatementOrBlock() {
  if (token.type == TT_BEGINBRACE) {
    return ParseCodeBlock();
  } else {
    return TryParseStatement();
  }
}

std::unique_ptr<AST::CodeBlock> ParseCode() {
  std::vector<std::unique_ptr<AST::Node>> statements{};

  while (token.type != TT_ENDBRACE && token.type != TT_ENDOFCODE) {
    statements.emplace_back(ParseStatementOrBlock());
  }

  return std::make_unique<AST::CodeBlock>(std::move(statements));
}

std::unique_ptr<AST::CodeBlock> ParseCodeBlock() {
  require_token(TT_BEGINBRACE, "Internal error: Expected opening brace ('{') at the start of code block");
  auto res = ParseCode();
  require_token(TT_ENDBRACE, "Expected closing brace ('}') at the end of code block");
  return res;
}

std::unique_ptr<AST::IfStatement> ParseIfStatement() {
  token = lexer->ReadToken();
  auto condition = TryParseControlExpression(mode);
  if (token.type == TT_S_THEN) {
    if (mode == SyntaxMode::STRICT) {
      herr->Warning(token) << "Use of `then` keyword in if statement";
    }
    token = lexer->ReadToken();
  }

  AST::PNode true_branch = nullptr;
  if (token.type != TT_SEMICOLON) {
    true_branch = ParseCFStmtBody();
  } else {
    token = lexer->ReadToken();
  }

  AST::PNode false_branch = nullptr;
  if (token.type == TT_S_ELSE) {
    token = lexer->ReadToken();
    false_branch = ParseCFStmtBody();
  }
  
  return std::make_unique<AST::IfStatement>(std::move(condition), std::move(true_branch), std::move(false_branch));
}

std::unique_ptr<AST::Node> TryParseEitherFunctionalCastOrDeclaration(
    AST::DeclaratorType decl_type, bool parse_unbounded,
    bool maybe_c_style_cast, AST::DeclarationStatement::StorageClass sc) {
  if (next_maybe_functional_cast()) {
    FullType type;
    TryParseTypeSpecifier(&type);
    if (next_is_type_specifier() ||
        // Make sure we don't accidentally consume a c-style cast when its required
        (!(maybe_c_style_cast && token.type == TT_ENDPARENTH) &&
         (token.type != TT_BEGINBRACE && token.type != TT_BEGINPARENTH))) {
      bool is_global = TryParseTypeSpecifierSeq(&type);
      sc = is_global ? AST::DeclarationStatement::StorageClass::GLOBAL : sc;
      maybe_assign_def(&type);
      return parse_declarations(sc, type, decl_type, parse_unbounded, {});
    } else if (token.type == TT_BEGINBRACE) {
      Token tok = token;
      return std::make_unique<AST::CastExpression>(AST::CastExpression::Kind::FUNCTIONAL, tok, FullType{type.def},
                                                   TryParseInitializer(), TT_BEGINBRACE);
    } else if (token.type == TT_BEGINPARENTH) {
      Token tok = token;
      auto declarator = TryParseNoPtrDeclarator(&type, decl_type, true);
      if (declarator != nullptr) {
        return std::make_unique<AST::CastExpression>(AST::CastExpression::Kind::FUNCTIONAL, tok, FullType{type.def},
                                                     std::move(declarator), TT_BEGINPARENTH);
      } else {
        if (type.decl.has_nested_declarator && type.decl.nested_declarator == 0) {
          type.decl = std::move(*type.decl.components[0]
                                 .as<NestedNode>()
                                 .as<std::unique_ptr<Declarator>>()
                                 .release());
        }
        std::vector<AST::DeclarationStatement::Declaration> decls = {};
        decls.emplace_back(std::move(type), next_is_start_of_initializer() ? TryParseInitializer() : nullptr);
        if (token.type == TT_COMMA && parse_unbounded) {
          maybe_assign_def(&type);
          return parse_declarations(sc, type, decl_type, parse_unbounded, std::move(decls), true);
        } else {
          return std::make_unique<AST::DeclarationStatement>(sc, decls[0].declarator->def, std::move(decls));
        }
      }
    } else if (token.type == TT_ENDPARENTH && maybe_c_style_cast) {
      token = lexer->ReadToken();
      return std::make_unique<AST::CastExpression>(AST::CastExpression::Kind::C_STYLE, token, std::move(type),
                                                   TryParseExpression(Precedence::kAll), TT_BEGINPARENTH);
    } else {
      // This should be unreachable...
      return TryParseDeclarations(parse_unbounded);
    }
  } else {
    return TryParseDeclarations(parse_unbounded);
  }
}

std::unique_ptr<AST::ForLoop> ParseForLoop() {
  require_token(TT_S_FOR, "Expected 'for' in for-loop");

  std::unique_ptr<AST::Node> init = nullptr;
  std::unique_ptr<AST::Node> cond = nullptr;
  std::unique_ptr<AST::Node> incr = nullptr;

  bool is_conventional = false; // Conventional means `for ()`
  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    is_conventional = true;
  }
  if (is_decl_specifier(token)) {
    init = TryParseEitherFunctionalCastOrDeclaration(
        AST::DeclaratorType::NON_ABSTRACT, true, is_conventional,
        AST::DeclarationStatement::StorageClass::TEMPORARY);
    if (init->type == AST::NodeType::CAST) {
      auto *cast = dynamic_cast<AST::CastExpression *>(init.get());
      if (cast->kind == AST::CastExpression::Kind::C_STYLE && is_conventional) {
        is_conventional = false;
      }
    }
  } else {
    init = TryParseExpression(Precedence::kAll, nullptr);
  }
  if (token.type == TT_ENDPARENTH) {
    is_conventional = false;
    token = lexer->ReadToken();
  }
  require_token(TT_SEMICOLON, "Expected semicolon (';') after for-loop initializer");
  if (token.type != TT_SEMICOLON) {
    cond = TryParseControlExpression(SyntaxMode::GML);
  }
  require_token(TT_SEMICOLON, "Expected semicolon (';') after for-loop condition");
  if (token.type != TT_SEMICOLON) {
    incr = TryParseExpression(Precedence::kAll);
  }

  if (is_conventional) {
    require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after for-loop header");
  }

  auto body = ParseCFStmtBody();
  return std::make_unique<AST::ForLoop>(std::move(init), std::move(cond),
                                        std::move(incr), std::move(body));
}

std::unique_ptr<AST::WhileLoop> ParseWhileLoop() {
  token = lexer->ReadToken();
  auto condition = TryParseControlExpression(mode);
  auto body = ParseCFStmtBody();

  return std::make_unique<AST::WhileLoop>(std::move(condition), std::move(body), AST::WhileLoop::Kind::WHILE);
}

std::unique_ptr<AST::WhileLoop> ParseUntilLoop() {
  token = lexer->ReadToken();
  auto condition = TryParseControlExpression(mode);
  auto body = ParseCFStmtBody();

  return std::make_unique<AST::WhileLoop>(std::move(condition), std::move(body), AST::WhileLoop::Kind::UNTIL);
}

std::unique_ptr<AST::DoLoop> ParseDoLoop() {
  token = lexer->ReadToken();
  auto body = ParseCFStmtBody();

  Token kind = token;
  require_any_of({TT_S_WHILE, TT_S_UNTIL}, "Expected `while` or `until` after do loop body");

  auto condition = TryParseControlExpression(mode);

  MaybeConsumeSemicolon();

  return std::make_unique<AST::DoLoop>(std::move(body), std::move(condition), kind.type == TT_S_UNTIL);
}

std::unique_ptr<AST::WhileLoop> ParseRepeatStatement() {
  token = lexer->ReadToken();
  auto condition = TryParseControlExpression(mode);
  auto body = ParseCFStmtBody();

  return std::make_unique<AST::WhileLoop>(std::move(condition), std::move(body), AST::WhileLoop::Kind::REPEAT);
}

std::unique_ptr<AST::ReturnStatement> ParseReturnStatement() {
  token = lexer->ReadToken();
  auto value = TryParseExpression(Precedence::kAll);

  MaybeConsumeSemicolon();

  if (value) {
    return std::make_unique<AST::ReturnStatement>(std::move(value), false);
  } else {
    return std::make_unique<AST::ReturnStatement>(nullptr, false);
  }
}

std::unique_ptr<AST::BreakStatement> ParseBreakStatement() {
  token = lexer->ReadToken(); // Consume the break
  std::unique_ptr<AST::BreakStatement> node ;

  if (token.type != TT_DECLITERAL && token.type != TT_BINLITERAL &&
      token.type != TT_OCTLITERAL && token.type != TT_HEXLITERAL) {
    node = std::make_unique<AST::BreakStatement>(nullptr);
  } else {
    node = std::make_unique<AST::BreakStatement>(TryParseOperand());
  }

  MaybeConsumeSemicolon();

  return node;
}

std::unique_ptr<AST::ContinueStatement> ParseContinueStatement() {
  token = lexer->ReadToken(); // Consume the continue
  std::unique_ptr<AST::ContinueStatement> node;

  if (token.type != TT_DECLITERAL && token.type != TT_BINLITERAL &&
      token.type != TT_OCTLITERAL && token.type != TT_HEXLITERAL) {
    node = std::make_unique<AST::ContinueStatement>(nullptr);
  } else {
    node = std::make_unique<AST::ContinueStatement>(TryParseOperand());
  }

  MaybeConsumeSemicolon();

  return node;
}

std::unique_ptr<AST::ReturnStatement> ParseExitStatement() {
  token = lexer->ReadToken();
  return std::make_unique<AST::ReturnStatement>(nullptr, true);
}

std::unique_ptr<AST::SwitchStatement> ParseSwitchStatement() {
  require_token(TT_S_SWITCH, "Expected 'switch' in switch-statement");

  auto switch_ = std::make_unique<AST::SwitchStatement>();
  switch_->expression = TryParseControlExpression(mode);
  switch_->body = std::make_unique<AST::CodeBlock>();
  require_token(TT_BEGINBRACE, "Expected '{' after switch-statement condition");
  while (token.type != TT_ENDBRACE) {
    if (token.type == TT_S_CASE) {
      // TODO: Handle case mappings
      switch_->body->statements.emplace_back(ParseCaseOrDefaultStatement(false));
    } else if (token.type == TT_S_DEFAULT) {
      if (switch_->default_branch.has_value()) {
        herr->Error(token) << "Redefinition of default case of switch";
        ParseCaseOrDefaultStatement(true); // ignore the default case
      } else {
        switch_->body->statements.emplace_back(ParseCaseOrDefaultStatement(true));
        switch_->default_branch = std::make_optional<std::size_t>(switch_->body->statements.size() - 1);
      }
    } else {
      herr->Error(token) << "Expected 'case' or 'default' in switch body";
    }
  }
  require_token(TT_ENDBRACE, "Expected closing brace ('}') after switch-statement");

  return switch_;
}

std::unique_ptr<AST::Node> ParseCaseOrDefaultStatement(bool is_default) {
  std::unique_ptr<AST::Node> expr = nullptr;
  if (is_default) {
    require_token(TT_S_DEFAULT, "Expected 'default' at the start of default statement");
    require_token(TT_COLON, "Expected colon (':') after 'default'");
  } else {
    require_token(TT_S_CASE, "Expected 'case' at the start of case statement");
    expr = TryParseExpression(Precedence::kAll);
    require_token(TT_COLON, "Expected colon (':') after case expression");
  }

  auto body = std::make_unique<AST::CodeBlock>();
  while (token.type != TT_S_CASE && token.type != TT_S_DEFAULT && token.type != TT_ENDBRACE) {
    if (token.type == TT_BEGINBRACE) {
      body->statements.emplace_back(ParseCodeBlock());
    } else {
      if (auto stmt = TryParseStatement())
        body->statements.emplace_back(std::move(stmt));
    }
  }

  if (is_default) {
    return std::make_unique<AST::DefaultStatement>(std::move(body));
  } else {
    return std::make_unique<AST::CaseStatement>(std::move(expr), std::move(body));
  }
}

std::unique_ptr<AST::WithStatement> ParseWithStatement() {
  token = lexer->ReadToken();
  auto object = TryParseControlExpression(mode);
  auto body = ParseCFStmtBody();

  return std::make_unique<AST::WithStatement>(std::move(object), std::move(body));
}

};  // class AstBuilder

class SyntaxChecker : public AST::Visitor {
  ErrorHandler *herr;
  const LanguageFrontend * frontend;

 public:
  SyntaxChecker(ErrorHandler *herr, const LanguageFrontend *fe) : herr(herr), frontend(fe) {}
  bool VisitFunctionCallExpression(AST::FunctionCallExpression &node) {
    if (node.function->type == AST::NodeType::IDENTIFIER) {
      auto func = node.function->As<AST::IdentifierAccess>();
      jdi::definition *def = nullptr;
      if (std::holds_alternative<jdi::definition *>(func->type)) {
        def = std::get<jdi::definition *>(func->type);
      } else {
        node.RecursiveSubVisit(*this);
        return false;
      }
      unsigned int min = 0;
      unsigned int max = 0;
      frontend->definition_parameter_bounds(def, min, max);
      Token tok;
      tok.content = func->name.content;
      tok.type = TT_IDENTIFIER;
      if (max != unsigned(-1)) {
        if (node.arguments.size() < min) {
          herr->Error(tok) << "Too few arguments to function call";
        } else if (node.arguments.size() > max) {
          herr->Error(tok) << "Too many arguments to function call";
        }
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDeclarationStatement(AST::DeclarationStatement &node) {
    if (node.declarations.size()) {
      auto &type = node.declarations[0].declarator;
      if (type) {
        if (AstBuilder::contains_decflag_bitmask(type->flags, "unsigned") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "signed")) {
          Token tok;
          tok.content = "unsigned";
          tok.type = TT_UNSIGNED;
          herr->Error(tok) << "Conflicting use of 'unsigned' and 'signed' in the same type specifier";
        }
        if (AstBuilder::contains_decflag_bitmask(type->flags, "long") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "short")) {
          Token tok;
          tok.content = "long";
          tok.type = TT_TYPE_NAME;
          herr->Error(tok) << "Conflicting use of 'long' and 'short' in the same type specifier";
        }
        if (AstBuilder::contains_decflag_bitmask(type->flags, "const") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "mutable")) {
          Token tok;
          tok.content = "const";
          tok.type = TT_CONST;
          herr->Error(tok) << "Conflicting use of 'const' and 'mutable' in the same type specifier";
        }
        if (AstBuilder::contains_decflag_bitmask(type->flags, "static") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "register")) {
          Token tok;
          tok.content = "static";
          tok.type = TT_STATIC;
          herr->Error(tok) << "Conflicting use of 'static' and 'register' in the same type specifier";
        }
        if (AstBuilder::contains_decflag_bitmask(type->flags, "inline") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "register")) {
          Token tok;
          tok.content = "inline";
          tok.type = TT_INLINE;
          herr->Error(tok) << "Conflicting use of 'inline' and 'register' in the same type specifier";
        }
        if (AstBuilder::contains_decflag_bitmask(type->flags, "extern") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "register")) {
          Token tok;
          tok.content = "extern";
          tok.type = TT_EXTERN;
          herr->Error(tok) << "Conflicting use of 'extern' and 'register' in the same type specifier";
        }
        if (AstBuilder::contains_decflag_bitmask(type->flags, "mutable") &&
            AstBuilder::contains_decflag_bitmask(type->flags, "static")) {
          Token tok;
          tok.content = "mutable";
          tok.type = TT_MUTABLE;
          herr->Error(tok) << "Conflicting use of 'mutable' and 'static' in the same type specifier";
        }
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitCodeBlock(AST::CodeBlock &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitBinaryExpression(AST::BinaryExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitUnaryPrefixExpression(AST::UnaryPrefixExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitUnaryPostfixExpression(AST::UnaryPostfixExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitTernaryExpression(AST::TernaryExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitLambdaExpression(AST::LambdaExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitSizeofExpression(AST::SizeofExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitAlignofExpression(AST::AlignofExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitCastExpression(AST::CastExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitParenthetical(AST::Parenthetical &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitArray(AST::Array &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitIdentifierAccess(AST::IdentifierAccess &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitLiteral(AST::Literal &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitIfStatement(AST::IfStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitForLoop(AST::ForLoop &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitWhileLoop(AST::WhileLoop &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDoLoop(AST::DoLoop &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitCaseStatement(AST::CaseStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDefaultStatement(AST::DefaultStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitSwitchStatement(AST::SwitchStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitReturnStatement(AST::ReturnStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitBreakStatement(AST::BreakStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitContinueStatement(AST::ContinueStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitWithStatement(AST::WithStatement &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitInitializer(AST::Initializer &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitNewExpression(AST::NewExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitDeleteExpression(AST::DeleteExpression &node) {
    node.RecursiveSubVisit(*this);
    return false;
  }
};

std::unique_ptr<AST::Node> Parse(Lexer *lexer, ErrorHandler *herr) {
  AstBuilder ab(lexer, herr);
  auto root = ab.ParseCode();
  SyntaxChecker sc(herr, lexer->GetContext().language_fe);
  root->accept(sc);
  return root;
}

AstBuilderTestAPI *CreateBuilder() {
  AstBuilder *builder = new AstBuilder();
  return builder;
}

}  // namespace enigma::parsing
