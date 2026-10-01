#include "parser.h"

#include <JDI/src/Parser/context_parser.h>

#include <charconv>

namespace enigma::parsing {

// RAII guard: set a bool for the duration of a scope, restore the prior value
// on exit. Used to push/pop parser-production state (e.g. allow_abstract_operand_)
// around the productions that establish it, without threading a parameter
// through the whole expression-parsing call graph.
struct ScopedFlag {
  bool &flag;
  bool saved;
  ScopedFlag(bool &flag, bool value) : flag(flag), saved(flag) { flag = value; }
  ~ScopedFlag() { flag = saved; }
  ScopedFlag(const ScopedFlag &) = delete;
  ScopedFlag &operator=(const ScopedFlag &) = delete;
};

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

bool is_class_key(TokenType tok) {
  switch (tok) {
    case TT_CLASS:
    case TT_STRUCT:
    case TT_UNION:
      return true;
    default:
      return false;
  }
}

bool next_is_class_key() {
  return is_class_key(token.type);
}

bool is_cv_qualifier(const Token &tok) {
  return tok.type == TT_DECLSPEC && (tok.content == "const" || tok.content == "volatile");
}

bool next_is_cv_qualifier() {
  return is_cv_qualifier(token);
}

bool is_ref_qualifier(TokenType tok) {
  return tok == TT_AMPERSAND || tok == TT_AND;
}

bool next_is_ref_qualifier() {
  return is_ref_qualifier(token.type);
}

std::string read_required_operatorkw() {
  if (next_is_any_operator()) {
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

/// Returns true iff the given token type can appear after the `operator`
/// keyword to name an operator function (used by the above routine).
bool is_any_operator(TokenType tok) {
  switch (tok) {
    case TT_COMMA:
    case TT_ASSIGN:
    case TT_ASSOP:
    case TT_EQUALS:
    case TT_ARROW:
    case TT_ARROW_STAR:
    case TT_PLUS:
    case TT_MINUS:
    case TT_STAR:
    case TT_SLASH:
    case TT_PERCENT:
    case TT_AMPERSAND:
    case TT_PIPE:
    case TT_CARET:
    case TT_AND:
    case TT_OR:
    case TT_XOR:
    case TT_DIV:
    case TT_EQUALTO:
    case TT_NOTEQUAL:
    case TT_BANG:
    case TT_TILDE:
    case TT_INCREMENT:
    case TT_DECREMENT:
    case TT_LESS:
    case TT_GREATER:
    case TT_LESSEQUAL:
    case TT_GREATEREQUAL:
    case TT_THREEWAY:
    case TT_LSH:
    case TT_RSH:
    case TT_S_NEW:
    case TT_S_DELETE:
    case TT_BEGINPARENTH:
    case TT_BEGINBRACKET:
      return true;

    default:
      return false;
  }
}

bool next_is_any_operator() {
  return is_any_operator(token.type);
}

bool is_user_defined_type(const Token &tok) {
  switch (tok.type) {
    case TT_IDENTIFIER:
      if (auto def = frontend->look_up(tok.content); def != nullptr) {
        // A function template is a function, not a type: DEF_TEMPLATE is in
        // this mask for class templates (which carry no DEF_TYPENAME), and
        // JDI1 never surfaced function templates here -- clang population
        // does, flagged DEF_FUNCTION | DEF_TEMPLATE.
        if (def->flags & jdi::DEF_FUNCTION) return false;
        // DEF_TYPED is set on every variable, so it can't be in this mask.
        return def->flags & (jdi::DEF_TYPENAME | jdi::DEF_CLASS | jdi::DEF_ENUM | jdi::DEF_TEMPLATE);
      }

      [[fallthrough]];
    default:
      return false;
  }
}

bool next_is_user_defined_type() {
  return is_user_defined_type(token);
}

bool is_type_specifier(TokenType tok) {
  switch (tok) {
    case TT_TYPE_NAME:
    case TT_TYPENAME:
    case TT_DECLTYPE:
    case TT_ENUM:
    case TT_DECLSPEC:
      return true;

    default:
      return is_class_key(tok);
  }
}

bool next_is_type_specifier() {
  return is_type_specifier(token.type);
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

bool is_decl_specifier(TokenType tok) {
  switch (tok) {
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

// Resolve a decl-specifier spelling to its JDI typeflag. This is the only
// name-keyed step: it runs once per specifier *token*; all repeated flag
// *checks* test the typeflag globals directly -- JDI's flags are bitmasks
// designed for mask compares, not string-map probes. The map stores the
// *address* of each jdi global flag pointer, not its dereferenced value: the
// globals are null until JDI builtins are initialized, and this static map is
// built on first call, so dereferencing at build time crashes if the first
// call precedes init (e.g. a test harness that never ran the full JDI setup).
static const jdi::typeflag *lookup_decflag(std::string_view tok) {
  static const std::unordered_map<std::string_view, jdi::typeflag *const *> flags{
    { "volatile",  &jdi::builtin_flag__volatile  },
    { "static",    &jdi::builtin_flag__static    },
    { "const",     &jdi::builtin_flag__const     },
    { "mutable",   &jdi::builtin_flag__mutable   },
    { "register",  &jdi::builtin_flag__register  },
    { "inline",    &jdi::builtin_flag__inline    },
    { "_Complex",  &jdi::builtin_flag__Complex   },
    { "restrict",  &jdi::builtin_flag__restrict  },
    { "unsigned",  &jdi::builtin_flag__unsigned  },
    { "long",      &jdi::builtin_flag__long      },
    { "signed",    &jdi::builtin_flag__signed    },
    { "short",     &jdi::builtin_flag__short     },
    { "virtual",   &jdi::builtin_flag__virtual   },
    { "explicit",  &jdi::builtin_flag__explicit  },
  };

  if (auto it = flags.find(tok); it != flags.end()) return *it->second;
  return nullptr;
}

// Null-tolerant typeflag tests/reads, for use both with lookup_decflag results
// and with the jdi::builtin_flag__* globals directly (null before builtins
// init). NB: `signed` cannot be tested this way -- JDI encodes it as
// mask=<the unsigned bit>, value=0, i.e. the *absence* of unsigned, so its
// Matches() accepts every non-unsigned flag set. Detect a written `signed`
// from the spec list's tokens instead (see specs_imply_int).
static bool flag_matches(std::size_t flags, const jdi::typeflag *flag) {
  return flag != nullptr && flag->Matches(flags);
}
static std::size_t flag_value(const jdi::typeflag *flag) {
  return flag != nullptr ? flag->value : 0;
}
static std::size_t flag_mask(const jdi::typeflag *flag) {
  return flag != nullptr ? flag->mask : 0;
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

bool is_start_of_initializer(TokenType tok) {
  switch (tok) {
    case TT_EQUALS:
    case TT_BEGINBRACE:
    case TT_BEGINPARENTH:
      return true;

    default:
      return false;
  }
}

bool next_is_start_of_initializer() {
  return is_start_of_initializer(token.type);
}

bool maybe_functional_cast(TokenType tok) {
  switch (tok) {
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
  // A resolvable user-defined type (class/enum/template) lexes as a plain
  // TT_IDENTIFIER when its definition lacks DEF_TYPENAME (class templates
  // carry only DEF_TEMPLATE), so the token-type test alone misses it.
  return maybe_functional_cast(token.type) || next_is_user_defined_type();
}

std::unique_ptr<AST::DeclarationStatement> parse_declarations(
    AST::DeclarationStatement::StorageClass sc,
    std::unique_ptr<AST::DeclSpecList> declspecs, AST::PNode id_expression,
    AST::DeclaratorType decl_type, bool parse_unbounded,
    std::vector<std::unique_ptr<AST::InitDeclarator>> decls, bool already_parsed_first = false) {
  while (true) {
    if (!already_parsed_first) {
      // Parse the declarator through the unified expression parser (types-as-
      // trees): prefix `* &` and the tighter-binding postfix `[] ()` build the
      // declarator-expression-tree directly, no legacy Declarator/components/
      // to_expression bridge. The name is recovered structurally from the tree
      // spine. allow_abstract_operand_ is set only when this production permits
      // an unnamed declarator (parameter / type-id contexts).
      AST::PNode declarator_expr;
      {
        ScopedFlag allow_abstract(allow_abstract_operand_,
                                  decl_type != AST::DeclaratorType::NON_ABSTRACT);
        declarator_expr = ParseExpression(Precedence::kUnaryPrefix);
      }
      const Token *namep = find_declarator_name(declarator_expr.get());
      Token name = namep ? *namep : Token{};
      decls.emplace_back(std::make_unique<AST::InitDeclarator>(
          name, std::move(declarator_expr),
          next_is_start_of_initializer() ? TryParseInitializer() : nullptr));
    }
    if (token.type == TT_COMMA && parse_unbounded) {
      token = lexer->ReadToken();
    } else {
      break;
    }
  }

  auto type_node = MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs));
  auto clause = std::make_unique<AST::DeclaratorClause>(std::move(type_node), std::move(decls));
  register_declarations(clause.get());
  return std::make_unique<AST::DeclarationStatement>(sc, std::move(clause));
}

// Record each declared name's owning clause so later id-expressions can
// resolve it (the base type lives on clause->specifiers; the per-name
// declarator modifiers on its InitDeclarator). Borrowed pointer, valid for
// the parse -- the clause is heap-stable across its move into the statement.
// Abstract declarators have no name and register nothing.
void register_declarations(AST::DeclaratorClause *clause) {
  for (auto &id : clause->declarators)
    if (!id->name.content.empty())
      declarations[id->name.content] = clause;
}

// True when this spec run carries a length/sign specifier, which names C's
// implied `int`. The sign and length families own disjoint bit ranges of the
// flag word, and `long`'s mask spans the whole length field (long, long long,
// and short all write within it), so one combined-mask presence test covers
// the lot -- the same `flags & flag->mask` idiom JDI uses (e.g. AST.cpp's
// unsigned checks). `signed` is the exception: its JDI encoding
// (mask=<the unsigned bit>, value=0) writes no bits, so a written `signed` is
// only detectable in the spec list's retained source tokens.
static bool specs_imply_int(const AST::DeclSpecList &declspecs) {
  const std::size_t sign_or_length =
      flag_mask(jdi::builtin_flag__unsigned) | flag_mask(jdi::builtin_flag__long);
  if (declspecs.flags & sign_or_length) return true;
  for (const Token &spec : declspecs.specs)
    if (spec.content == "signed") return true;
  return false;
}

// Build a TypeSpecifierSeq from a fully-consumed type-specifier run -- the one
// place a parsed spec run becomes a node, so every construction site funnels
// through here. When the user wrote a (non-empty) specifier run but no
// type-name, the base type is inferred and materialized as a token-free
// ImplicitType leaf (mirroring JDI's read_type, which fixes the base once the
// spec-seq closes), so the seq's Definition() stays a pure tree read: a
// length/sign specifier (`unsigned x;`) names C's `int`; any other untyped run
// (`const x;`) is EDL's universal `var`, same as an undeclared variable. An
// EMPTY run stays null -- type-id positions parse vacuously when the operand
// is really a value expression (`sizeof(local_var)`), and inventing a base
// type there would mis-tag them.
std::unique_ptr<AST::TypeSpecifierSeq> MakeTypeSpecifierSeq(
    AST::PNode id_expression, std::unique_ptr<AST::DeclSpecList> declspecs) {
  if (id_expression == nullptr && declspecs && !declspecs->specs.empty()) {
    id_expression = specs_imply_int(*declspecs)
        ? std::make_unique<AST::ImplicitType>(AST::ImplicitType::Kind::INT,
                                              jdi::builtin_type__int)
        : std::make_unique<AST::ImplicitType>(AST::ImplicitType::Kind::UNTYPED,
                                              frontend->look_up("var"));
  }
  return std::make_unique<AST::TypeSpecifierSeq>(std::move(id_expression), std::move(declspecs));
}

AstBuilder(Lexer *lexer, ErrorHandler *herr) {
  initialize(lexer, herr);
}

AstBuilder(){}

const Token &current_token() {
  return token;
}

std::unique_ptr<AST::Node> TryParseConstantExpression() {
  return ParseExpression(Precedence::kTernary);
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

// Null-safe read of the definition an id-expression's root node denotes; `def`
// lives on the node itself (Node::Definition). A null tree denotes nothing.
static jdi::definition *id_expression_def(const AST::PNode &node) {
  return node ? node->Definition() : nullptr;
}

AST::PNode TryParseTypeName() {
  Token name = token;
  require_token(TT_IDENTIFIER, "Expected identifier in type name");
  auto def = frontend->look_up(name.content);
  if (is_template_type(name) && token.type == TT_LESS) {
    std::vector<AST::PNode> args;
    jdi::definition *inst = TryParseTemplateArgs(def, &args);
    // The leaf names the template; the TemplateId's def is the instantiation
    // (null when deferred to the semantic phase).
    return std::make_unique<AST::TemplateId>(
        std::make_unique<AST::IdentifierAccess>(def, name), std::move(args), inst);
  }
  return std::make_unique<AST::IdentifierAccess>(def, name);
}

bool can_begin_id_expression(TokenType tok) {
  switch (tok) {
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

bool next_can_begin_id_expression() {
  return can_begin_id_expression(token.type);
}

// Parses an `id-expression` in expression context -- a `qualified-id` or
// `unqualified-id` -- as a tree: a bare name is an IdentifierAccess leaf, a
// qualified name a ScopeAccess chain (template segments wrapped in a
// TemplateId), the same shapes the type-context resolvers emit. Names resolve
// where possible (the leading segment by ordinary lookup, later segments in
// their parent's scope); anything unresolved stays on the tree with a null
// def for the semantic phase to bind -- not a parse error: EDL permits
// implicitly declared variables, and a not-yet-known qualified name is the
// semantic phase's to classify.
std::unique_ptr<AST::Node> TryParseIdExpression() {
  switch (token.type) {
    case TT_IDENTIFIER: {
      Token name = token;
      token = lexer->ReadToken();
      if (token.type == TT_SCOPEACCESS) {
        return TryParseNestedNameSpecifier(std::make_unique<AST::IdentifierAccess>(
            frontend->look_up(name.content), name));
      }
      // A locally declared name reads through its declaration, so it stays an
      // unresolved leaf even when a C++ global shares the name.
      if (map_contains(declarations, name.content)) {
        return std::make_unique<AST::IdentifierAccess>(name);
      }
      if (jdi::definition *def = frontend->look_up(name.content)) {
        return std::make_unique<AST::IdentifierAccess>(def, name);
      }
      return std::make_unique<AST::IdentifierAccess>(name);
    }

    case TT_SCOPEACCESS:
      // Leading `::` (global qualification). The operand parser usually
      // dispatches this earlier; kept for callers that reach an
      // id-expression directly.
      return TryParseNestedNameSpecifier(nullptr);

    case TT_DECLTYPE: {
      auto base = TryParseDecltype();
      if (token.type == TT_SCOPEACCESS) {
        return TryParseNestedNameSpecifier(std::move(base));
      }
      return base;
    }

    case TT_OPERATOR: {
      // operator-function-ids are not modeled yet; consume the form so the
      // parse can continue, and report.
      Token op = token;
      token = lexer->ReadToken();
      read_required_operatorkw();
      herr->Error(op) << "Unimplemented: operator-function id-expressions";
      return nullptr;
    }

    case TT_TILDE: {
      // Destructor-ids are not modeled yet; consume `~type-name` and report.
      Token tilde = token;
      token = lexer->ReadToken();
      if (token.type == TT_IDENTIFIER) {
        TryParseTypeName();
      } else if (token.type == TT_DECLTYPE) {
        TryParseDecltype();
      }
      herr->Error(tilde) << "Unimplemented: destructor id-expressions";
      return nullptr;
    }

    default: {
      herr->Error(token) << "Given token cannot be used to specify a qualified or unqualified expression: '"
                         << token.content << '\'';
      return nullptr;
    }
  }
}

AST::PNode TryParseDecltype() {
  require_token(TT_DECLTYPE, "Expected 'decltype' keyword");
  require_token(TT_BEGINPARENTH, "Expected '(' after 'decltype'");
  auto expr = ParseExpression(Precedence::kAll);
  require_token(TT_ENDPARENTH, "Expected ')' after decltype expression");

  return std::make_unique<AST::Decltype>(std::move(expr));
}

// `out_args`, when non-null, collects the parsed argument trees in source order
// Forwards jdi diagnostics raised while filling defaults / instantiating a
// template onto the EDL error handler, reported against the template-name
// token. Ordinarily silent: it fires for user-side argument problems (count
// mismatches, bad defaults) or an engine-side definition breaking under a
// user instantiation.
struct JdiDiagnosticForwarder : jdi::ErrorHandler {
  enigma::parsing::ErrorHandler *herr;
  Token at;
  JdiDiagnosticForwarder(enigma::parsing::ErrorHandler *herr, Token at):
      herr(herr), at(std::move(at)) {}
  void error(std::string_view msg, jdi::SourceLocation) final {
    herr->Error(at) << std::string(msg);
  }
  void warning(std::string_view msg, jdi::SourceLocation) final {
    herr->Warning(at) << std::string(msg);
  }
  void info(std::string_view, int, jdi::SourceLocation) final {}
};

// Evaluate a parsed non-type template argument to a JDI value, when the tree
// is one of the two forms the parser can fold without a real evaluator: an
// integer literal, or an id-expression whose root resolved to a constant
// (jdi definition_valued). Returns VT_NONE for anything else -- arbitrary
// constant expressions await the semantic phase.
static jdi::value nttp_value(const AST::Node *arg) {
  if (arg == nullptr) return {};
  if (arg->type == AST::NodeType::LITERAL) {
    auto &cv = const_cast<AST::Node*>(arg)->As<AST::Literal>()->value;
    if (cv.type == TT_DECLITERAL) {
      if (auto *s = std::get_if<std::string>(&cv.value)) {
        long v = 0;
        auto [end, ec] = std::from_chars(s->data(), s->data() + s->size(), v);
        if (ec == std::errc{} && end == s->data() + s->size()) return jdi::value{v};
      }
    }
    return {};
  }
  jdi::definition *def = arg->Definition();
  if (def != nullptr && (def->flags & jdi::DEF_VALUED))
    return static_cast<jdi::definition_valued*>(def)->value_of;
  return {};
}

// Parse `< template-argument-list >` for a template `def`, build the JDI
// arg_key, and instantiate. Returns the instantiated definition -- cached by
// JDI, so `vec<int>` is one definition and `vec<float>` another, and repeats
// are the same pointer -- or null when any argument is unresolved or beyond
// the parse-time evaluator (the TemplateId then carries a null def for the
// semantic phase). Count validation and default-argument fill-in are JDI's
// check_read_template_parameters, reported through the forwarder. `out_args`,
// when non-null, collects the parsed argument trees in source order (type-id
// clauses for type params, expressions for NTTPs) for the TemplateId to hold.
//
// NTTP expressions parse at kShift: a top-level `>` must close the argument
// list, not compare (C++ requires parenthesizing a comparison here), and the
// comma is the list separator. (`vec<vec<int>>` still lexes `>>` as one
// TT_RSH token -- the C++11 re-lex hack is not implemented.)
jdi::definition *TryParseTemplateArgs(jdi::definition *def, std::vector<AST::PNode> *out_args = nullptr) {
  if (!(def->flags & jdi::DEF_TEMPLATE)) return def;
  Token name_token = token;
  require_token(TT_LESS, "Expected '<' at start of template arguments");
  auto template_def = static_cast<jdi::definition_template *>(def);
  // Sized construction is load-bearing: the default arg_key ctor leaves its
  // slot array null, and mirror_types placement-news into it.
  jdi::arg_key argk(template_def->params.size());
  argk.mirror_types(template_def);
  std::size_t args_given = 0;
  bool resolved = true;
  while (token.type != TT_GREATER && token.type != TT_ENDOFCODE) {
    const bool in_range = args_given < template_def->params.size();
    const bool is_type_param =
        in_range && (template_def->params[args_given]->flags & jdi::DEF_TYPENAME);
    // Beyond the parameter list there is no kind to parse against; read the
    // extra argument as a type-id clause when it looks like one (so the token
    // stream stays sane) and let check_read_template_parameters report the
    // count. Never indexes params out of range.
    if (is_type_param || (!in_range && next_is_type_specifier())) {
      auto clause = ParseTypeIdClause();
      // In template-argument position the untyped fallback is `variant`, not
      // `var`: it's lighter weight, and var makes a poor element/key type.
      // The kind stays UNTYPED; only the inferred def is retagged.
      if (clause->specifiers && clause->specifiers->id_expression &&
          clause->specifiers->id_expression->type == AST::NodeType::IMPLICIT_TYPE) {
        auto *implicit = clause->specifiers->id_expression->As<AST::ImplicitType>();
        if (implicit->kind == AST::ImplicitType::Kind::UNTYPED)
          implicit->def = frontend->look_up("variant");
      }
      // The clause's full_type carries the declarator too, so `vec<int*>`
      // keys differently from `vec<int>`.
      if (jdi::full_type t = clause->to_jdi_fulltype(0); t.def != nullptr) {
        if (in_range) argk[args_given].ft().swap(t);
      } else {
        resolved = false;
      }
      if (out_args) out_args->push_back(std::move(clause));
    } else {
      auto expr = ParseExpression(Precedence::kShift);
      if (jdi::value v = nttp_value(expr.get()); v.type != jdi::VT_NONE) {
        if (in_range) argk.put_value(args_given, v);
      } else {
        herr->Error(token) << "Unimplemented: non-constant template arguments";
        resolved = false;
      }
      if (out_args) out_args->push_back(std::move(expr));
    }
    ++args_given;

    if (token.type == TT_ELLIPSES) {
      herr->Error(token) << "Unimplemented: variadic template arguments";
      token = lexer->ReadToken();
    }
    if (token.type == TT_COMMA) {
      token = lexer->ReadToken();
      continue;
    }
    break;
  }

  if (!require_token(TT_GREATER, "Expected '>' after template arguments"))
    return nullptr;
  // An unresolved argument defers the whole instantiation to the semantic
  // phase -- silently, per types-as-trees: the parser cannot judge whether a
  // not-yet-known name is an error.
  if (!resolved) return nullptr;

  JdiDiagnosticForwarder fwd(herr, name_token);
  jdi::ErrorContext errc(&fwd, jdi::SourceLocation{
      "user code", name_token.position, name_token.line});
  if (jdi::check_read_template_parameters(argk, args_given, template_def, errc))
    return nullptr;
  return template_def->instantiate(argk, errc);
}

AST::PNode TryParseTypenameSpecifier() {
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

// Parses a type name, potentially followed by template arguments and/or a nested
// name specifier. Corresponds roughly to the start of a `qualified-id` or just a
// `type-name`. Returns the id-expression tree; its root carries the resolved def.
AST::PNode TryParsePrefixIdentifier() {
  Token id = token;
  require_token(TT_IDENTIFIER, "Expected identifier");
  auto def = require_defined_type(id);

  AST::PNode leaf;
  if (token.type == TT_LESS && is_template_type(def)) {
    std::vector<AST::PNode> args;
    jdi::definition *inst = TryParseTemplateArgs(def, &args);
    leaf = std::make_unique<AST::TemplateId>(
        std::make_unique<AST::IdentifierAccess>(def, id), std::move(args), inst);
  } else {
    leaf = std::make_unique<AST::IdentifierAccess>(def, id);
  }

  if (token.type == TT_SCOPEACCESS) {
    return TryParseNestedNameSpecifier(std::move(leaf));
  }

  return leaf;
}

// Parses a `nested-name-specifier` (starting with `::`) and the following
// `unqualified-id`. Despite the name, it parses the rest of a qualified-id, not
// just the specifier. `scope_tree` is the already-parsed scope id-expression (the
// chain's `lhs`), null for a global-scope `::name`. Returns the assembled
// id-expression tree -- a ScopeAccess chain, with any template segment wrapped
// in a TemplateId. Each segment resolves in its parent's scope when that scope
// is known; an unresolved or non-scope parent defers the rest of the chain
// (null segment defs, no diagnostic) for the semantic phase to bind -- whether
// a dangling name is an error depends on what the chain turns out to be, which
// the parser cannot judge.
AST::PNode TryParseNestedNameSpecifier(AST::PNode scope_tree) {
  if (token.type != TT_SCOPEACCESS) {
    herr->Error(token) << "Expected scope access '::' in nested name specifier, got: '" << token.content << '\'';
    return scope_tree;
  }

  AST::PNode current = std::move(scope_tree);
  while (token.type == TT_SCOPEACCESS) {
    token = lexer->ReadToken();
    if (token.type == TT_IDENTIFIER) {
      Token id = token;
      token = lexer->ReadToken();
      // Resolve the segment name in its parent's scope first (global lookup
      // for a leading `::name`), THEN test for a template segment -- the
      // template must be found where the chain says it lives, not by an
      // unqualified global probe.
      jdi::definition *seg_def = nullptr;
      if (current == nullptr) {
        seg_def = frontend->look_up(id.content);
      } else if (jdi::definition *parent = current->Definition();
                 parent != nullptr && (parent->flags & (jdi::DEF_SCOPE | jdi::DEF_CLASS))) {
        if (auto *parent_scope = dynamic_cast<jdi::definition_scope *>(parent)) {
          seg_def = parent_scope->look_up(std::string{id.content});
        }
      }
      bool is_template_seg = token.type == TT_LESS && seg_def != nullptr &&
                             (seg_def->flags & jdi::DEF_TEMPLATE);
      AST::PNode seg = std::make_unique<AST::ScopeAccess>(std::move(current), id, seg_def);
      if (is_template_seg) {
        std::vector<AST::PNode> args;
        jdi::definition *inst = TryParseTemplateArgs(seg_def, &args);
        seg = std::make_unique<AST::TemplateId>(std::move(seg), std::move(args), inst);
      }
      current = std::move(seg);
    } else if (token.type == TT_STAR) {
      // `A::*` -- the star is a pointer-to-member declarator operator; the
      // nested-name ends here and the `*` is the caller's.
      break;
    } else {
      herr->Error(token) << "Expected either identifier or star ('*') after nested name specifier";
      return current;
    }
  }

  return current;
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

AST::PNode TryParseElaboratedName() {
  Token tok = token;

  token = lexer->ReadToken();
  Token name = token;
  AST::PNode scope_tree;

  if (token.type == TT_IDENTIFIER) {
    scope_tree = std::make_unique<AST::IdentifierAccess>(frontend->look_up(token.content), token);
    token = lexer->ReadToken();
  } else if (token.type == TT_DECLTYPE) {
    token = lexer->ReadToken();
    scope_tree = TryParseDecltype();
    if (token.type != TT_SCOPEACCESS) {
      herr->Error(token) << "Expected scope access after decltype";
    }
  }

  if (token.type == TT_SCOPEACCESS) {
    scope_tree = TryParseNestedNameSpecifier(std::move(scope_tree));
  }

  // The elaborated form asserts its referent's kind; check it only when the
  // name actually resolved. An unresolved name stays on the tree for the
  // semantic phase -- whether it exists is not the parser's judgment.
  jdi::definition *def = id_expression_def(scope_tree);
  if (def != nullptr && !matches_token_type(def, tok)) {
    herr->Error(name) << "'" << name.content << "' does not name "
                      << (tok.type == TT_ENUM ? "an " : "a ") << tok.content;
  }
  return scope_tree;
}

// Install `base` as the run's base type. A specifier run names at most one;
// if the slot is already taken, the earlier tree is retained on the spec
// list's extra_base_types as evidence for the post-parse checker, which owns
// the "two types in one specifier" diagnostic. The later name wins the slot,
// matching the id-expression threading since step 2A.
void stash_base_type(AST::DeclSpecList *specs, AST::PNode &slot, AST::PNode base) {
  if (base == nullptr) return;
  if (slot != nullptr) specs->extra_base_types.push_back(std::move(slot));
  slot = std::move(base);
}

jdi::definition *get_builtin(std::string_view name) {
  auto it = jdi::builtin_primitives.find(std::string(name));
  if (it != jdi::builtin_primitives.end()) {
    return it->second;
  }
  return frontend->look_up(std::string(name));
}

AST::PNode TryParseTypeSpecifier(AST::DeclSpecList *specs) {
  AST::PNode id_expression;
  Token start = token;
  switch (token.type) {
    case TT_TYPE_NAME: {
      if (token.content == "long" || token.content == "short") {
        if (flag_matches(specs->flags, jdi::builtin_flag__long)) {
           if (token.content == "long") {
             // long + long = long long: swap the length field's value.
             const std::size_t field = flag_mask(jdi::builtin_flag__long);
             specs->flags = (specs->flags & ~field) | flag_value(jdi::builtin_flag__long_long);
             specs->specs.push_back(token);
           } else if (token.content == "short") {
             herr->Error(token) << "Conflicting usage of 'long' and 'short' in the same type specifier";
           }
        } else if (flag_matches(specs->flags, jdi::builtin_flag__short) && token.content == "long") {
          herr->Error(token) << "Conflicting usage of 'short' and 'long' in the same type specifier";
        } else if (flag_matches(specs->flags, jdi::builtin_flag__long_long)) {
          if (token.content == "long") {
            herr->Error(token) << "Too many 'long's in type specifier";
          } else if (token.content == "short") {
            herr->Error(token) << "Conflicting usage of 'short' and 'long long' in the same type specifier";
          }
        } else {
          specs->flags |= flag_value(lookup_decflag(token.content));
          specs->specs.push_back(token);
        }
      } else {
        id_expression = std::make_unique<AST::IdentifierAccess>(get_builtin(token.content), token);
      }
      token = lexer->ReadToken();
      break;
    }

    case TT_IDENTIFIER: {
      id_expression = TryParsePrefixIdentifier();
      break;
    }

    case TT_SCOPEACCESS: {
      id_expression = TryParseNestedNameSpecifier(nullptr);
      break;
    }

    case TT_DECLTYPE: {
      id_expression = TryParseDecltype();

      if (token.type == TT_SCOPEACCESS) {
        id_expression = TryParseNestedNameSpecifier(std::move(id_expression));
      }

      // decltype RESOLUTION is unimplemented (Decltype::Definition() is
      // always null), so a decltype base type still reports here.
      if (id_expression_def(id_expression) == nullptr) {
        herr->Error(start) << "Could not parse decltype specifier";
      }
      break;
    }

    case TT_TYPENAME: {
      id_expression = TryParseTypenameSpecifier();
      break;
    }

    default: {
      if (token.type == TT_DECLSPEC) {
        const jdi::typeflag *flag = lookup_decflag(token.content);
        // unsigned-then-signed is flag-detectable (unsigned wrote a bit); the
        // reverse order is not (`signed` writes none) and is caught post-parse
        // by the SyntaxChecker's token-based check. The duplicate warning
        // likewise exempts `signed`, whose all-zero flag "matches" any run.
        if (flag_matches(specs->flags, jdi::builtin_flag__unsigned) && token.content == "signed") {
          herr->Error(token) << "Conflicting use of 'unsigned' and 'signed' in the same type specifier";
        } else if (flag_matches(specs->flags, flag) && token.content != "signed") {
          herr->Warning(token) << "Duplicate usage of flags in type specifier";
        } else {
          specs->flags |= flag_value(flag);
          specs->specs.push_back(token);
        }
        token = lexer->ReadToken();
      } else if (next_is_class_key() || token.type == TT_ENUM) {
        id_expression = TryParseElaboratedName();
      } else {
        herr->Error(token) << "Given token does not specify a valid type specifier";
      }
      break;
    }
  }
  return id_expression;
}

std::pair<bool, bool> TryParseTypeSpecifierSeq(AST::DeclSpecList *specs,
                                               AST::PNode &out_id_expression) {
  std::pair<bool, bool> global_local = {false, false};
  while (next_is_type_specifier() || next_is_user_defined_type() ||
         token.content == "global" || token.content == "local") {
    if (token.content == "global") {
      global_local.first = true;
      token = lexer->ReadToken();
    } else if (token.content == "local") {
      global_local.second = true;
      token = lexer->ReadToken();
    } else {
      stash_base_type(specs, out_id_expression, TryParseTypeSpecifier(specs));
    }
  }
  return global_local;
}

// Build the TypeSpecifierSeq for a `<type-id>` grammar production. The parser
// records the decl-spec chain (declspecs, which carries both the source-order
// specifier tokens and the flag bitmask) and the base type's id-expression tree;
// the seq's Definition() reads the resolved type back from that tree root.
std::unique_ptr<AST::TypeSpecifierSeq> TryParseTypeID() {
  auto declspecs = std::make_unique<AST::DeclSpecList>();
  AST::PNode id_expression;
  while (next_is_type_specifier() || next_is_user_defined_type()) {
    stash_base_type(declspecs.get(), id_expression, TryParseTypeSpecifier(declspecs.get()));
  }

  return MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs));
}

// Parse a full type-id (type-specifier-seq + abstract-declarator) into a
// DeclaratorClause. The declarator is parsed through the unified expression
// path with allow_abstract_operand_ set, so a missing operand -- bare `int`,
// `int *`, `int **(*)[10]`, ... -- becomes an abstract-declarator leaf instead
// of an error. This is the first setter of the abstract-operand mechanism.
// Used at type-id-demanding sites bounded by `)` / `>` (sizeof/alignof, and
// C-style + named cast targets). The lone InitDeclarator carries the declarator
// tree in declarator_expr; its name/init stay empty for a type-id. NB:
// postfix-only abstract declarators with no grouping (`int[10]`, `int(args)`)
// aren't covered yet -- the abstract leaf only fires at `) ] , ; > EOF`, not
// before a leading `[`/`(`.
//
// The declarator is parsed at kUnaryPrefix, not kAll: a single type-id's
// declarator is built solely from prefix `* &` and the postfix `[] ()` that
// bind tighter, never a top-level binary or comma operator. Parsing at kAll
// would wrongly let the binary loop consume a trailing `>` (as greater-than)
// when this is a named-cast target -- kUnaryPrefix stops before any binary
// operator, so the `>` is left for the caller's require_token to close.
//
// Parse*, never returns null: TryParseTypeID always builds a TypeSpecifierSeq
// (despite the Try* name -- it's misnamed at the call site), and ParseExpression
// only yields null on genuine malformed input, in which case the inner call has
// already reported the error on herr.
std::unique_ptr<AST::DeclaratorClause> ParseTypeIdClause() {
  auto specifiers = TryParseTypeID();
  AST::PNode declarator_expr;
  {
    ScopedFlag allow_abstract(allow_abstract_operand_, true);
    declarator_expr = ParseExpression(Precedence::kUnaryPrefix);
  }
  std::vector<std::unique_ptr<AST::InitDeclarator>> declarators;
  auto decl = std::make_unique<AST::InitDeclarator>();
  decl->declarator_expr = std::move(declarator_expr);
  declarators.push_back(std::move(decl));
  return std::make_unique<AST::DeclaratorClause>(std::move(specifiers), std::move(declarators));
}

// A parenthesized group is a C-style-cast target iff its single element is an
// abstract type-id clause (a declarator with no name in its spine). A named
// declarator (`int x`) or a comma-list (a tuple) is not a cast; those flow on
// as a Parenthetical for the semantic phase / the outer `=>`,`=` dispatch.
bool clause_is_abstract_type_id(AST::Node *node) {
  if (!node || node->type != AST::NodeType::DECLARATOR_CLAUSE) return false;
  auto *clause = node->As<AST::DeclaratorClause>();
  return clause->declarators.size() == 1 &&
         find_declarator_name(clause->declarators[0]->declarator_expr.get()) == nullptr;
}

// All operand-position `(...)` groups funnel through here: grouping, C-style
// cast, tuple/tie, and lambda-parameter lists. The contents parse as one
// comma-list under maybe_declarator_group_, so a type-name element becomes a
// (possibly abstract) declarator clause and ParseExpression itself ties the
// commas -- the uniform tuple. Role is deferred to the semantic phase, with one
// parse-time carve-out: a lone abstract type-id is the only shape that must
// consume a trailing operand here (the C-style cast), since nothing downstream
// would. (The postfix `(` -- call args / function-declarator params -- will
// delegate here once Family A is retired; see #33/#34.)
std::unique_ptr<AST::Node> ParseParenthetical() {
  auto paren = token;
  token = lexer->ReadToken();
  std::unique_ptr<AST::Node> body;
  // `()` is a legitimate empty group (an empty lambda parameter list); the
  // Parenthetical carries a null expression rather than a sentinel.
  if (token.type != TT_ENDPARENTH) {
    ScopedFlag group(maybe_declarator_group_, true);
    body = ParseExpression(Precedence::kAll);
  }
  require_token(TT_ENDPARENTH, "Expected closing parenthesis before '", token.content, "'");
  if (clause_is_abstract_type_id(body.get())) {
    auto expr = ParseExpression(Precedence::kUnaryPrefix);
    return std::make_unique<AST::CastExpression>(
        AST::CastExpression::Kind::C_STYLE, paren, std::move(body), std::move(expr));
  }
  return std::make_unique<AST::Parenthetical>(std::move(body));
}

void TryParseDeclSpecifier(AST::DeclSpecList *specs, AST::PNode &out_id_expression) {
  switch (token.type) {
    case TT_TYPEDEF: {
      // `typedef` is a decl-specifier; the following type-specifier-seq and
      // declarator are parsed by the unified path, exactly as for any other
      // declaration. Registering the declared name as a type alias is a
      // semantic-phase concern.
      // TODO: there is no jdi typeflag for typedef (no DEF_TYPEDEF / decflag),
      // so the typedef-ness is currently only preserved as a token in
      // specs->specs, not as a semantic flag. Add a real marker so the semantic
      // phase can register the declared name as a type alias.
      specs->specs.push_back(token);
      token = lexer->ReadToken();
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
      const jdi::typeflag *flag =
          token.type == TT_MUTABLE ? jdi::builtin_flag__mutable
          : token.type == TT_INLINE ? jdi::builtin_flag__inline
                                    : jdi::builtin_flag__static;
      specs->flags |= flag_value(flag);
      specs->specs.push_back(token);
      token = lexer->ReadToken();
      break;
    }

    default:
      if (next_is_type_specifier() || next_is_user_defined_type()) {
        stash_base_type(specs, out_id_expression, TryParseTypeSpecifier(specs));
        break;
      }
  }
}

std::pair<bool, bool> TryParseDeclSpecifierSeq(AST::DeclSpecList *specs,
                                               AST::PNode &out_id_expression) {
  std::pair<bool, bool> global_local = {false, false};
  while (next_is_decl_specifier() || next_is_user_defined_type() ||
         token.content == "global" || token.content == "local") {
    if (token.content == "global") {
      global_local.first = true;
      token = lexer->ReadToken();
    } else if (token.content == "local") {
      global_local.second = true;
      token = lexer->ReadToken();
    } else
      TryParseDeclSpecifier(specs, out_id_expression);
  }
  return global_local;
}

AST::PNode TryParseExprOrBracedInitList(bool is_init_clause, bool in_init_list) {
  // This function handles:
  // <brace-or-equal-initializer>    ::= = <initializer-clause>
  //                                   | <braced-init-list>
  // <initializer-clause>            ::= <assignment-expression>
  //                                   | <braced-init-list>
  if (token.type == TT_EQUALS && !is_init_clause) {
    // `= <initializer-clause>`: delegate to the clause production and wrap as
    // ASSIGN (so `= {...}` is an ASSIGN over a brace-init-list, not a bare
    // BRACE), rather than peeking for `{` here.
    token = lexer->ReadToken();
    std::vector<AST::PNode> vals;
    vals.push_back(TryParseExprOrBracedInitList(/*is_init_clause=*/true, in_init_list));
    return std::make_unique<AST::Initializer>(AST::Initializer::Kind::ASSIGN, nullptr, std::move(vals));
  } else if (token.type == TT_BEGINBRACE) {
    return TryParseBraceInitializer();
  } else if (is_init_clause) {
    return ParseExpression(Precedence::kAssign);
  } else {
    herr->Error(token) << "Expected equals ('=') or opening brace ('{') at start of initializer, got: '"
                       << token.content << '\'';
    return nullptr;
  }
}

AST::InitializerNode TryParseInitializerList(TokenType closing) {
  std::vector<AST::PNode> values;
  while (token.type != closing) {
    values.push_back(TryParseExprOrBracedInitList(true, true));
    if (token.type == TT_COMMA) {
      token = lexer->ReadToken();
    } else {
      break;
    }
  }
  require_token(closing, "Expected closing brace ('}') at the end of brace initializer");
  return std::make_unique<AST::Initializer>(AST::Initializer::Kind::BRACE, nullptr, std::move(values));
}

AST::InitializerNode TryParseBraceInitializer() {
  require_token(TT_BEGINBRACE, "Expected opening brace ('{') at the start of brace initializer");
  std::vector<AST::PNode> values;
  if (token.type == TT_DOT) {
    while (token.type != TT_ENDBRACE) {
      token = lexer->ReadToken();
      Token name = token;
      require_token(TT_IDENTIFIER, "Expected identifier after dot in designated initializer");
      require_token(TT_EQUALS, "Expected '=' in designated initializer");
      
      // The designator's `=` is already consumed above, so the value is a plain
      // initializer-clause (assignment-expression or braced-init-list), not a
      // brace-or-equal-initializer: pass is_init_clause=true.
      std::vector<AST::PNode> assign_vals;
      assign_vals.push_back(TryParseExprOrBracedInitList(true, false));
      
      auto designator = std::make_unique<AST::IdentifierAccess>(name);
      values.push_back(std::make_unique<AST::Initializer>(AST::Initializer::Kind::ASSIGN, std::move(designator), std::move(assign_vals)));

      if (token.type == TT_COMMA) {
        token = lexer->ReadToken();
      } else {
        break;
      }
    }
    require_token(TT_ENDBRACE, "Expected closing brace ('}') at the end of brace initializer");
    return std::make_unique<AST::Initializer>(AST::Initializer::Kind::BRACE, nullptr, std::move(values));
  } else {
    return TryParseInitializerList(TT_ENDBRACE);
  }
}

AST::InitializerNode TryParseInitializer(bool allow_paren_init = true) {
  switch (token.type) {
    case TT_EQUALS: {
      // `= <initializer-clause>`: consume the `=`, then delegate to the
      // initializer-clause production (which itself handles either an
      // assignment-expression or a braced-init-list) and wrap as ASSIGN. We do
      // not peek for `{` here -- a copy-list-init `= {...}` is an ASSIGN whose
      // value is a brace-init-list, which preserves the `=` for round-tripping.
      token = lexer->ReadToken();
      std::vector<AST::PNode> vals;
      vals.push_back(TryParseExprOrBracedInitList(/*is_init_clause=*/true, /*in_init_list=*/false));
      return std::make_unique<AST::Initializer>(AST::Initializer::Kind::ASSIGN, nullptr, std::move(vals));
    }

    case TT_BEGINBRACE: return TryParseBraceInitializer();

    case TT_BEGINPARENTH: {
      if (!allow_paren_init) {
        std::vector<AST::PNode> vals;
        vals.push_back(ParseExpression(Precedence::kAll));
        return std::make_unique<AST::Initializer>(AST::Initializer::Kind::ASSIGN, nullptr, std::move(vals));
      } else {
        std::vector<AST::PNode> values;
        token = lexer->ReadToken();
        while (token.type != TT_ENDPARENTH) {
          values.push_back(TryParseExprOrBracedInitList(true, true));
          if (token.type == TT_COMMA) {
            token = lexer->ReadToken();
          } else {
            break;
          }
        }
        require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after initializer");
        return std::make_unique<AST::Initializer>(AST::Initializer::Kind::PAREN, nullptr, std::move(values));
      }
    }

    default: {
      herr->Error(token) << "Junk in initializer, expected one of =, {, (; got: '" << token.content << '\'';
      return nullptr;
    }
  }
}

std::unique_ptr<AST::Node> TryParseDeclarations(
    bool parse_unbounded,
    AST::DeclaratorType decl_type = AST::DeclaratorType::NON_ABSTRACT) {
  bool is_global = token.content == "global";
  bool is_local = token.content == "local";
  if (next_is_decl_specifier() || is_global || is_local) {
    auto declspecs = std::make_unique<AST::DeclSpecList>();
    AST::PNode id_expression;
    std::pair<bool, bool> global_local = TryParseDeclSpecifierSeq(declspecs.get(), id_expression);
    if (global_local.first && global_local.second) {
      herr->Error(token) << "Cannot have both 'global' and 'local' in the same declaration";
      return nullptr;
    }
    // No guard against a missing type-name here: a non-empty spec run with no
    // type-name still declares -- a sign/length specifier implies `int`, and
    // any other untyped run (`const x;`) is EDL's universal `var` --
    // materialized by parse_declarations via MakeTypeSpecifierSeq.
    auto sc = is_global || global_local.first   ? AST::DeclarationStatement::StorageClass::GLOBAL
              : is_local || global_local.second ? AST::DeclarationStatement::StorageClass::LOCAL
                                                : AST::DeclarationStatement::StorageClass::TEMPORARY;
    return parse_declarations(sc, std::move(declspecs), std::move(id_expression), decl_type, parse_unbounded, {});
  }
  return nullptr;
}

// "Is this `new` an array-new?" -- i.e. is the outermost derivation of the
// type-id an array? Reads the unified declarator-expression-tree: an abstract
// array declarator parses to a subscript `BinaryExpression` (op TT_BEGINBRACKET)
// at the root, e.g. `int[]` -> Subscript(<abstract leaf>, <bound>) and
// `int[][15]` -> Subscript(Subscript(...), 15). A pointer/grouped outermost
// (`int *(**)[10]`) roots in a prefix `*`, so it's not array-new.
static bool DeclaratorClauseIsArray(const std::unique_ptr<AST::DeclaratorClause> &c) {
  if (!c || c->declarators.empty()) return false;
  const auto &decl = c->declarators.front();
  if (!decl->declarator_expr ||
      decl->declarator_expr->type != AST::NodeType::BINARY_EXPRESSION)
    return false;
  return decl->declarator_expr->As<AST::BinaryExpression>()->operation.type == TT_BEGINBRACKET;
}

// In a new-type-id, grouping is never a declarator: the standard forbids parens
// in a new-declarator precisely to disambiguate `new T(x)`'s `(x)` from a
// function-declarator. We don't disambiguate at parse time -- we build the
// operand tree like any other type-id, then reinterpret here. A top-level
// grouped/call form on the declarator-expression is therefore the
// new-initializer:
//   `new int(5)`    -> declarator-expr Parenthetical(5)       -> init (5)
//   `new int[2](5)` -> declarator-expr FunctionCall([2], 5)   -> init (5),
//                      declarator drops to the bracketed array part.
// Returns the peeled initializer (PAREN, no target) or null, mutating the
// clause's declarator to drop the consumed group.
AST::InitializerNode PeelNewInitializer(AST::DeclaratorClause *clause) {
  if (!clause || clause->declarators.empty()) return nullptr;
  auto &dexpr = clause->declarators.front()->declarator_expr;
  if (!dexpr) return nullptr;
  if (dexpr->type == AST::NodeType::PARENTHETICAL) {
    std::vector<AST::PNode> vals;
    vals.push_back(std::move(dexpr->As<AST::Parenthetical>()->expression));
    dexpr = make_abstract_operand();
    return std::make_unique<AST::Initializer>(AST::Initializer::Kind::PAREN, nullptr, std::move(vals));
  }
  if (dexpr->type == AST::NodeType::FUNCTION_CALL) {
    auto *call = dexpr->As<AST::FunctionCallExpression>();
    std::vector<AST::PNode> vals = std::move(call->arguments);
    dexpr = std::move(call->function);
    return std::make_unique<AST::Initializer>(AST::Initializer::Kind::PAREN, nullptr, std::move(vals));
  }
  return nullptr;
}

std::unique_ptr<AST::Node> TryParseNewExpression(bool is_global) {
  require_token(TT_S_NEW, "Expected 'new' in new-expression");

  std::vector<AST::PNode> placement_args;
  std::unique_ptr<AST::DeclaratorClause> type_node;
  // True once the type-id was taken from a parenthesized form `new (T)`. Such a
  // type is *wholly* parenthesized -- nothing inside it is an initializer (a
  // grouped declarator like `int(*)()` must survive as the type), so we don't
  // peel its tree; only a trailing token-group after the `)` is its initializer.
  bool parenthesized_type = false;

  // Optional leading `(...)`: either placement-new arguments (`new (ptr) T`) or
  // a parenthesized type-id (`new (T)`). Only placement reads a *second*
  // expression (the type) afterward; the parenthesized type-id is the type.
  if (token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    if (next_is_type_specifier()) {
      type_node = ParseTypeIdClause();
      require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after new-expression type");
      parenthesized_type = true;
    } else {
      while (token.type != TT_ENDPARENTH) {
        placement_args.push_back(ParseExpression(Precedence::kAssign));
        if (token.type == TT_COMMA) token = lexer->ReadToken();
        else if (token.type != TT_ENDPARENTH) {
          herr->Error(token) << "Expected end of placement-new arguments";
          return nullptr;
        }
      }
      require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after placement-new arguments");
    }
  }

  // The type itself, when not already taken as a parenthesized form above. After
  // placement it may still be parenthesized (`new (ptr) (T)`); otherwise it's a
  // bare new-type-id: type-specifier-seq + abstract declarator, parsed through
  // the unified expression path (array bounds and any trailing initializer are
  // captured into the one declarator-expression tree, to be peeled below).
  if (!type_node && token.type == TT_BEGINPARENTH) {
    token = lexer->ReadToken();
    type_node = ParseTypeIdClause();
    require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after new-expression type");
    parenthesized_type = true;
  }
  if (!type_node) {
    type_node = ParseTypeIdClause();
  }

  // Peel the built tree, in order: a top-level grouped/call form on the bare
  // type-id is the new-initializer; failing that, a trailing `(`/`{` token-group
  // (the only place a parenthesized type-id's initializer can appear) is the
  // initializer; a subscript root marks array-new; the remainder is the type.
  AST::InitializerNode initializer;
  if (!parenthesized_type) {
    initializer = PeelNewInitializer(type_node.get());
  }
  if (!initializer && (token.type == TT_BEGINPARENTH || token.type == TT_BEGINBRACE)) {
    initializer = TryParseInitializer(true);
  }
  bool is_array = DeclaratorClauseIsArray(type_node);

  MaybeConsumeSemicolon();

  return std::make_unique<AST::NewExpression>(is_global, is_array, std::move(placement_args), std::move(type_node), std::move(initializer));
}

std::unique_ptr<AST::Node> TryParseDeleteExpression(bool is_global) {
  require_token(TT_S_DELETE, "Expected 'delete' in delete-expression");

  bool is_array = false;
  if (token.type == TT_BEGINBRACKET) {
    token = lexer->ReadToken();
    is_array = true;
    require_token(TT_ENDBRACKET, "Expected ']' to close '[' in delete-expression");
  }

  auto node =  std::make_unique<AST::DeleteExpression>(is_global, is_array, ParseExpression(Precedence::kUnaryPrefix));
  MaybeConsumeSemicolon();

  return node;
}



/// Parse an operand--this includes variables, literals, arrays, and
/// unary expressions on these.
// An abstract (nameless) declarator -- the operand position of a type-id with
// no declarator-id. Encoded, per the established convention, as an
// IdentifierAccess leaf with empty name content. Distinct from SyntaxError,
// which marks a genuine parse failure.
std::unique_ptr<AST::Node> make_abstract_operand() {
  return std::make_unique<AST::IdentifierAccess>(Token{});
}

// Walk the declarator spine of an expression-tree to its terminal leaf and,
// if that leaf is a non-empty identifier, return its name Token; otherwise
// return nullptr. The spine is the set of edges a declarator may legally
// recurse through: prefix `*`/`&`/`&&` -> operand, `[bound]` -> left (the
// array's element), a call group -> function (the parenthesised declarator),
// and `( ... )` -> expression. Any other node (e.g. a `+`, a literal, an
// abstract empty-name leaf) means the operand is not a named declarator.
// This is a purely structural, parse-time test -- no type lookup -- and is
// how the most-vexing-parse branch decides `T(x) = ...` is a declaration
// (named x) versus `T(expr) = ...` a temporary-assignment expression.
const Token *find_declarator_name(AST::Node *expr) {
  for (;;) {
    if (!expr) return nullptr;
    switch (expr->type) {
      case AST::NodeType::IDENTIFIER: {
        auto *id = expr->As<AST::IdentifierAccess>();
        return id->name.content.empty() ? nullptr : &id->name;
      }
      case AST::NodeType::UNARY_PREFIX_EXPRESSION: {
        auto *u = expr->As<AST::UnaryPrefixExpression>();
        if (u->operation.type != TT_STAR && u->operation.type != TT_AMPERSAND &&
            u->operation.type != TT_AND)
          return nullptr;
        expr = u->operand.get();
        break;
      }
      case AST::NodeType::BINARY_EXPRESSION: {
        auto *b = expr->As<AST::BinaryExpression>();
        if (b->operation.type != TT_BEGINBRACKET) return nullptr;
        expr = b->left.get();
        break;
      }
      case AST::NodeType::FUNCTION_CALL:
        expr = expr->As<AST::FunctionCallExpression>()->function.get();
        break;
      case AST::NodeType::PARENTHETICAL:
        expr = expr->As<AST::Parenthetical>()->expression.get();
        break;
      default:
        return nullptr;
    }
  }
}

// The deliberate set of tokens at which an operand may be *legitimately* absent
// -- i.e. where an abstract declarator ends (a closing or separating token).
// This is NOT the same as "tokens that return null in TryParseOperand": that
// larger set also includes malformed-operand cases (`+`, `%`, `:`, ...) which
// must stay errors even in a type-id context. This is the discriminator
// between "abstract declarator" and "broken expression", so it gets its own
// explicit list rather than piggybacking on a switch case. Only consulted when
// allow_abstract_operand_ is set.
// TODO(setters): revisit `=` (default-argument after an abstract param, e.g.
// `f(int = 5)`) when the default-value flip-to-false is wired in.
bool at_abstract_declarator_end() {
  switch (token.type) {
    case TT_ENDPARENTH: case TT_ENDBRACKET: case TT_COMMA:
    case TT_SEMICOLON:  case TT_GREATER:    case TT_ENDOFCODE:
    // A block close ends a semicolonless declaration (`{int cc = new int}`),
    // so it ends an abstract declarator the same way `;` does.
    case TT_ENDBRACE:
      return true;
    // A leading `[` is an abstract *array* declarator (`int[10]`, `new int[]`).
    // Unlike `(`, it's unambiguous -- no grouped-declarator form begins with
    // `[` -- so the operand is legitimately absent and we insert the abstract
    // leaf here; the postfix-subscript loop in ParseExpression then attaches
    // the `[bound]` to it. (`(` is deliberately excluded: in a `new` type-id a
    // trailing `(` is the initializer, and elsewhere a grouped `(*)` `(` must
    // stay a primary so the grouped declarator parses.)
    case TT_BEGINBRACKET:
      return true;
    default:
      return false;
  }
}

std::unique_ptr<AST::Node> TryParseOperand() {
  // In a type-id / declarator production (allow_abstract_operand_), a missing
  // operand is an abstract declarator, not an error. Yielding the placeholder
  // here also covers `*`/`&`-then-terminator (e.g. `int *)`), since the prefix
  // operator parses its operand by recursing back through this function.
  if (allow_abstract_operand_ && at_abstract_declarator_end()) {
    return make_abstract_operand();
  }
  switch (token.type) {
    case TT_BEGINBRACE: case TT_ENDBRACE:
    case TT_ENDPARENTH: case TT_ENDBRACKET:
    case TT_ENDOFCODE: case TT_SEMICOLON:
      // Not consumed: closers and terminators belong to the caller. Callers
      // with OPTIONAL expression slots (return values, for-loop clauses,
      // empty groups and array literals) peek their current token before
      // parsing, so reaching here is a genuinely missing expression -- raise,
      // so the SyntaxError substituted upstream always has a diagnostic.
      if (token.type == TT_ENDOFCODE) {
        herr->Error(token) << "Expected an expression before end of code";
      } else {
        herr->Error(token) << "Expected an expression before '" << token.content << '\'';
      }
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

      if (auto exp = ParseExpression(Precedence::kUnaryPrefix)) {
        AST::Operation op(unary_op.type, std::string(unary_op.content));
        return std::make_unique<AST::UnaryPrefixExpression>(std::move(exp), op);
      }
      herr->Error(unary_op) << "Expected expression following unary operator, got: '" << token.content << '\'';
      return nullptr;
    }

    case TT_BEGINPARENTH:
      return ParseParenthetical();

    case TT_BEGINBRACKET: {
      token = lexer->ReadToken();
      std::vector<std::unique_ptr<AST::Node>> elements;

      // `[]` is a legitimate empty array literal; peek the closer rather
      // than reading an empty expression.
      while (token.type != TT_ENDBRACKET && token.type != TT_ENDOFCODE) {
        elements.push_back(ParseExpression(Precedence::kComma));
        if (token.type != TT_COMMA) break;
        token = lexer->ReadToken();
      }

      require_token(TT_ENDBRACKET, "Expected closing `]` for array");
      return std::make_unique<AST::Array>(std::move(elements));
    }

    case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
    case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT:
    case TT_BOOLLITERAL: {
      Token res = token;
      token = lexer->ReadToken();
      return std::make_unique<AST::Literal>(std::move(res));
    }

    case TT_SIZEOF: {
      token = lexer->ReadToken();
      if (token.type == TT_BEGINPARENTH) {
        // `sizeof(...)`: the parenthesized operand is a type-id OR a value
        // expression. Parse it through the uniform group (as ParseParenthetical
        // does) so a type-name promotes to a DeclaratorClause and anything else
        // parses as a full expression -- the old unconditional type-id parse
        // choked on any top-level binary operator (`sizeof(x + 1)`). Classify
        // by the resulting shape; a value keeps its source parens as a
        // Parenthetical so the EXPR printing round-trips them.
        token = lexer->ReadToken();
        std::unique_ptr<AST::Node> body;
        {
          ScopedFlag group(maybe_declarator_group_, true);
          body = ParseExpression(Precedence::kAll);
        }
        require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after sizeof-expression");
        if (body && body->type == AST::NodeType::DECLARATOR_CLAUSE) {
          return std::make_unique<AST::SizeofExpression>(AST::SizeofExpression::Kind::TYPE, std::move(body));
        }
        return std::make_unique<AST::SizeofExpression>(
            AST::SizeofExpression::Kind::EXPR,
            std::make_unique<AST::Parenthetical>(std::move(body)));
      } else if (token.type == TT_ELLIPSES) {
        token = lexer->ReadToken();
        require_token(TT_BEGINPARENTH, "Expected opening '(' after 'sizeof ...'");
        auto arg = token;
        if (require_token(TT_IDENTIFIER, "Expected identifier as argument to variadic sizeof")) {
          require_token(TT_ENDPARENTH, "Expected closing ')' after variadic sizeof");
          // TODO: model pack-expansion explicitly; for now drop into an IdentifierAccess.
          return std::make_unique<AST::SizeofExpression>(
              AST::SizeofExpression::Kind::VARIADIC, std::make_unique<AST::IdentifierAccess>(arg));
        } else {
          return nullptr;
        }
      } else {
        auto operand = ParseExpression(Precedence::kUnaryPrefix);
        return std::make_unique<AST::SizeofExpression>(std::move(operand));
      }
    }

    case TT_ALIGNOF: {
      token = lexer->ReadToken();
      if (require_token(TT_BEGINPARENTH, "Expected opening parenthesis ('(') after 'alignof'")) {
        // Same uniform group parse as sizeof; no Parenthetical wrap here
        // because the alignof printing always supplies the parens.
        std::unique_ptr<AST::Node> body;
        {
          ScopedFlag group(maybe_declarator_group_, true);
          body = ParseExpression(Precedence::kAll);
        }
        require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after alignof-expression");
        return std::make_unique<AST::AlignofExpression>(std::move(body));
      } else {
        return nullptr;
      }
    }

    case TT_CO_AWAIT: {
      auto oper = token;
      token = lexer->ReadToken();
      auto expr = ParseExpression(Precedence::kUnaryPrefix);
      AST::Operation op(oper.type, std::string(oper.content));
      return std::make_unique<AST::UnaryPrefixExpression>(std::move(expr), op);
    }

    case TT_NOEXCEPT: {
      auto oper = token;
      token = lexer->ReadToken();
      if (require_token(TT_BEGINPARENTH, "Expected opening '(' after noexcept")) {
        token = lexer->ReadToken();
        auto expr = ParseExpression(Precedence::kAll);
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
      auto type_node = ParseTypeIdClause();
      require_token(TT_GREATER, "Expected '>' after '", oper.content, "' type");
      require_token(TT_BEGINPARENTH, "Expected '(' before '", oper.content, "' expression");
      auto expr = ParseExpression(Precedence::kAll);
      require_token(TT_ENDPARENTH, "Expected ')' after '", oper.content, "' expression");
      return std::make_unique<AST::CastExpression>(oper, std::move(type_node), std::move(expr));
    }

    case TT_SCOPEACCESS: {
      // A global `::` can qualify only new, delete, or an unqualified-id (a
      // globally qualified user TYPE arrives as a plain identifier too,
      // indistinguishable from a value without resolution -- classifying it
      // is the semantic phase's job).
      token = lexer->ReadToken();
      if (token.type == TT_S_NEW) {
        return TryParseNewExpression(true);
      } else if (token.type == TT_S_DELETE) {
        return TryParseDeleteExpression(true);
      }
      // A global-qualified id denotes the GLOBAL name only: do NOT delegate
      // to TryParseIdExpression, whose unqualified path consults the local
      // `declarations` map and EDL's implicit-local fallback -- neither may
      // apply under explicit qualification. The qualification is carried as
      // a ScopeAccess with a null lhs (= global scope); an unresolved name
      // stays unresolved on the node for the semantic phase to bind in
      // global scope.
      if (token.type == TT_IDENTIFIER) {
        Token name = token;
        token = lexer->ReadToken();
        auto access = std::make_unique<AST::ScopeAccess>(
            nullptr, name, frontend->look_up(name.content));
        if (token.type == TT_SCOPEACCESS) {
          return TryParseNestedNameSpecifier(std::move(access));
        }
        return access;
      }
      herr->Error(token) << "Expected qualified-id after '::', got: '" << token.content << '\'';
      return nullptr;
    }

    case TT_TILDE: {
      if (!next_is_user_defined_type() && token.type != TT_DECLTYPE) {
        Token unary_op = token;
        token = lexer->ReadToken();

        if (auto exp = ParseExpression(Precedence::kUnaryPrefix)) {
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
      // Inside a maybe-declarator group (parenthesized list), a type-name is
      // promoted to a full type-id clause (specifiers + declarator) so the
      // surrounding ParseExpression keeps going and ties the comma-list -- the
      // uniform tuple. This fires before the functional-cast heuristic below,
      // which would otherwise demand a `(`/`{` and reject a bare `(int)` / a
      // declarator. A bare identifier (not a type) falls through to normal
      // handling so `(int x, y, float z)`'s `y` stays an expression leaf.
      if (maybe_declarator_group_ && (token.type == TT_TYPE_NAME || next_is_user_defined_type())) {
        return ParseTypeIdClause();
      }
      if (next_maybe_functional_cast()) {
        auto declspecs = std::make_unique<AST::DeclSpecList>();
        AST::PNode id_expression = TryParseTypeSpecifier(declspecs.get());
        if (token.type == TT_BEGINPARENTH) {
          token = lexer->ReadToken();
          std::vector<AST::PNode> args;
          {
            // `Type(...)` is the same most-vexing ambiguity as the statement
            // form: the parenthesized content may be an abstract declarator
            // (`int (*)` as a type-id), so a missing operand is the abstract
            // leaf, not an error -- the semantic phase classifies the shape.
            ScopedFlag allow_abstract(allow_abstract_operand_, true);
            args.push_back(ParseExpression(Precedence::kAll));
          }
          require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after functional cast");
          return std::make_unique<AST::Initializer>(AST::Initializer::Kind::PAREN,
                                                    MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs)),
                                                    std::move(args));
        } else if (token.type == TT_BEGINBRACE) {
          auto init = TryParseInitializer(false);
          require_token(TT_ENDBRACE, "Expected closing brace ('}') after temporary object initializer");
          init->target = MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs));
          return init;
        } else if (declspecs->specs.empty() && id_expression != nullptr) {
          // A lone id-expression with no specifier run and no cast tokens is
          // just a (possibly qualified) name read -- `test_vec<int>::size` as
          // a value. Return the tree; whether the chain denotes a type or a
          // value is the semantic phase's call.
          return id_expression;
        } else {
          herr->Error(token) << "Expected opening parenthesis ('(') or brace ('{') after functional-cast type";
          return nullptr;
        }
      }
      // If the operand names a type (built-in via TT_TYPE_NAME, or a
      // user-defined typedef/class/enum resolved through the frontend),
      // produce a TypeSpecifierSeq and let the surrounding ParseExpression bail to
      // the caller -- the caller decides whether this is a declaration, a
      // cast target, a sizeof argument, etc.
      if (token.type == TT_TYPE_NAME || next_is_user_defined_type()) {
        return TryParseTypeID();
      }
      return TryParseIdExpression();
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
    case TT_MUTABLE: case TT_UNION: case TT_DECLSPEC:
    case TT_ENUM: case TT_TYPEDEF: {
      // Inside a maybe-declarator group, a decl-spec-led run (`const int`,
      // bare `unsigned`) is a type-id clause, same as the type-name promotion
      // in the identifier case above.
      if (maybe_declarator_group_ && next_is_type_specifier()) {
        return ParseTypeIdClause();
      }
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

const OperatorPrecedence *BinaryPrecedence(TokenType type) {
  if (lexer->GetContext().compatibility_opts.use_gml_precedence)
    if (auto it = Precedence::kGmlBinaryPrec.find(type); it != Precedence::kGmlBinaryPrec.end())
      return &it->second;
  auto it = Precedence::kBinaryPrec.find(type);
  return it == Precedence::kBinaryPrec.end() ? nullptr : &it->second;
}

static bool ShouldAcceptPrecedence(const OperatorPrecedence &prec,
                                   int target_prec) {
  return target_prec >= prec.precedence ||
            (target_prec == prec.precedence &&
                prec.associativity == Associativity::RTL);
}

// Operand shapes postfix ++/-- binds to: a name (qualified or not) or an
// access chain (member access, subscript). Call results and parenthesized
// groups are deliberately excluded: EDL statements need no semicolon, so
// after `foo(12)` or a `while (cond)` header, a following `--x`/`++i` starts
// the NEXT statement (or the loop body) -- the statement boundary outranks
// postfix binding. Names and access chains carry no such boundary reading
// (a bare `a[1]` statement does nothing), so `grid[x]++` binds.
static bool operand_takes_postfix(AST::Node *operand) {
  switch (operand->type) {
    case AST::NodeType::IDENTIFIER:
    case AST::NodeType::SCOPE_ACCESS:
      return true;
    case AST::NodeType::BINARY_EXPRESSION: {
      TokenType op = operand->As<AST::BinaryExpression>()->operation.type;
      return op == TT_DOT || op == TT_ARROW || op == TT_BEGINBRACKET;
    }
    default:
      return false;
  }
}

std::unique_ptr<AST::Node> TryParseLambdaExpression(std::unique_ptr<AST::Node> operand) {
  token = lexer->ReadToken();
  auto body = ParseStatementOrBlock();
  return std::make_unique<AST::LambdaExpression>(std::move(operand), std::move(body));
}

std::unique_ptr<AST::Node> ParseExpression(int precedence, std::unique_ptr<AST::Node> operand = nullptr) {
  Token origin = token;
  if (operand == nullptr) {
    operand = TryParseOperand();
  }
  if (operand != nullptr) {
    if (operand->type == AST::NodeType::DELETE || operand->type == AST::NodeType::NEW) {
      return operand;
    }
    // A bare type-id at LHS isn't an expression -- it's a declaration,
    // c-style cast target, sizeof arg, etc. Return immediately so the
    // caller can decide. (Per the longstanding XXX in this slot: this is
    // the "bold move" that lets us drop the maybe_expression / to_expression
    // panic-rollback machinery; the semantic phase is now load-bearing for
    // disambiguating these uses.)
    if (operand->type == AST::NodeType::TYPE_SPECIFIER_SEQ) {
      return operand;
    }
    while (token.type != TT_ENDOFCODE) {
      if(token.type == TT_JS_ARROW){
        operand = TryParseLambdaExpression(std::move(operand));
      } else if (const OperatorPrecedence *binop = BinaryPrecedence(token.type)) {
        if (!ShouldAcceptPrecedence(*binop, precedence)) {
          break;
        }
        operand = TryParseBinaryExpression(precedence, std::move(operand));
      } else if (map_contains(Precedence::kUnaryPostfixPrec, token.type)) {
        if (precedence < Precedence::kUnaryPostfix ||
            !operand_takes_postfix(operand.get())) {
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
  // No operand parsed -- expression site reached without anything to parse.
  // Naming convention: Parse* never returns null. TryParseOperand raises a
  // diagnostic on every null return (optional-expression callers peek before
  // calling), so a SyntaxError node never appears in a legitimate tree --
  // substitute one so callers can rely on the result being non-null.
  return std::make_unique<AST::SyntaxError>(origin);
}

std::unique_ptr<AST::Node> TryParseBinaryExpression(int precedence, std::unique_ptr<AST::Node> operand) {
  while (BinaryPrecedence(token.type) &&
         precedence >= BinaryPrecedence(token.type)->precedence && token.type != TT_ENDOFCODE) {
    Token oper = token;
    OperatorPrecedence rule = *BinaryPrecedence(token.type);
    token = lexer->ReadToken(); // Consume the operator

    if(token.type == TT_ENDOFCODE || token.type == TT_SEMICOLON){ // there are more cases
      herr->Error(token) << "Uncompleted binary expression";
    }

    // EDL's universal access operator builds the uniform access node, the
    // same shape `::` produces in id-expressions: chains left-fold as
    // ScopeAccess(ScopeAccess(a, b), c). The semantic annotator classifies
    // each link; the parser only records the shape.
    if (oper.type == TT_DOT) {
      Token name = token;
      require_token(TT_IDENTIFIER, "Expected identifier after member access");
      operand = std::make_unique<AST::ScopeAccess>(std::move(operand), name, oper);
      continue;
    }

    auto right = (rule.associativity == Associativity::LTR)
        ? ParseExpression(rule.precedence - 1)
        : (rule.associativity == Associativity::RTL)
            ? ParseExpression(rule.precedence)
            : nullptr;

    AST::Operation op(oper.type, std::string(oper.content));
    operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), op);
  }

  return operand;
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

  auto middle = ParseExpression(Precedence::kBoolOr);

  require_token(TT_COLON, "Expected colon (':') after expression in conditional operator");

  auto right = ParseExpression(Precedence::kTernary);
  operand = std::make_unique<AST::TernaryExpression>(std::move(operand), std::move(middle), std::move(right));

  return dynamic_unique_pointer_cast<AST::TernaryExpression>(std::move(operand));
}

std::unique_ptr<AST::BinaryExpression> TryParseSubscriptExpression(int precedence, std::unique_ptr<AST::Node> operand) {
  (void)precedence;
  while (token.type == TT_BEGINBRACKET) {
    Token oper = token;
    token = lexer->ReadToken(); // Consume the operator

    auto right = ParseExpression(Precedence::kAll);

    AST::Operation op(oper.type, std::string(oper.content));
    operand = std::make_unique<AST::BinaryExpression>(std::move(operand), std::move(right), op);

    require_token(TT_ENDBRACKET, "Expected closing bracket (']') at the end of array subscript");
  }

  return dynamic_unique_pointer_cast<AST::BinaryExpression>(std::move(operand));
}

std::unique_ptr<AST::FunctionCallExpression> TryParseFunctionCallExpression(int precedence, std::unique_ptr<AST::Node> operand) {
  (void)precedence;
  while (token.type == TT_BEGINPARENTH) {
    // A named value callee (`foo(...)`, `a::b(...)`) is a CALL: its
    // parenthesized list can never be a function-declarator's parameter
    // list, so a type-specifier-led argument there is a functional cast or
    // temporary, not a parameter-declaration. Only type-flavored or grouped
    // callees keep the maybe-declarator argument dispatch below.
    const bool callee_is_named_value =
        operand->type == AST::NodeType::IDENTIFIER ||
        operand->type == AST::NodeType::SCOPE_ACCESS;
    token = lexer->ReadToken(); // Consume the operator

    std::vector<std::unique_ptr<AST::Node>> arguments{};
    while (token.type != TT_ENDPARENTH && token.type != TT_ENDOFCODE) {
      // A type-specifier-led argument is a function-declarator parameter
      // (`int x`, `int (*x)(int x)`, abstract `int (*)[10]`), not a value
      // expression. Parse it as a single (maybe-abstract) parameter-declaration
      // -- TPEFCOD yields a DeclarationStatement (or, for an unnamed functional
      // cast, an expression) and recurses through nested parameter lists. The
      // comma stays ours: parse_unbounded=false so commas separate parameters,
      // not declarators. The call stays uniformly shaped; the semantic phase
      // decides call-vs-declarator and reinterprets typed args accordingly.
      if (!callee_is_named_value && next_is_decl_specifier()) {
        arguments.emplace_back(TryParseEitherFunctionalCastOrDeclaration(
            AST::DeclaratorType::MAYBE_ABSTRACT, /*parse_unbounded=*/false,
            /*maybe_c_style_cast=*/false,
            AST::DeclarationStatement::StorageClass::TEMPORARY));
      } else {
        arguments.emplace_back(ParseExpression(Precedence::kTernary, nullptr));
      }
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

  auto expr = ParseExpression(Precedence::kAll);

  if (mode_ == SyntaxMode::STRICT) {
    require_token(TT_ENDPARENTH, "Expected ')' after control expression");
  }

  return expr;
}

std::unique_ptr<AST::Node> TryParseDeclOrTypeExpression() {
  if (next_is_decl_specifier() || next_maybe_functional_cast()) {
    if (token.type == TT_SCOPEACCESS) {
      // The operand parser owns the global-`::` dispatch (new / delete /
      // qualified-id); parse the whole expression statement from here. This
      // used to consume the `::` itself and drop it on the floor when neither
      // new nor delete followed, so `::x = 5` parsed unqualified.
      return ParseExpression(Precedence::kAll);
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
      // Allow ParseExpression to handle errors.
      // (Fall through.)
    case TT_PLUS: case TT_MINUS: case TT_STAR: case TT_AMPERSAND:
    case TT_NOT: case TT_BANG: case TT_TILDE:
    case TT_INCREMENT: case TT_DECREMENT:
    case TT_BEGINPARENTH: case TT_BEGINBRACKET:
    case TT_DECLITERAL: case TT_BINLITERAL: case TT_OCTLITERAL:
    case TT_HEXLITERAL: case TT_STRINGLIT: case TT_CHARLIT:
    case TT_BOOLLITERAL:
    case TT_SCOPEACCESS: case TT_CO_AWAIT:
    case TT_NOEXCEPT: case TT_ALIGNOF: case TT_SIZEOF:
    case TT_STATIC_CAST: case TT_DYNAMIC_CAST:
    case TT_REINTERPRET_CAST: case TT_CONST_CAST:
    case TT_S_NEW: case TT_S_DELETE:{
      auto node = ParseExpression(Precedence::kAll);
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
    case TT_DECLSPEC:
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
      bool is_global_local = token.content == "global" || token.content == "local";
      Token maybe_global_local = token;
      if (is_global_local) {
        sc = token.content == "global" ? AST::DeclarationStatement::StorageClass::GLOBAL
                                       : AST::DeclarationStatement::StorageClass::LOCAL;
        token = lexer->ReadToken();
      }
      if (next_is_decl_specifier() || next_maybe_functional_cast() || (is_global_local && token.type != TT_DOT)) {
        Token start = token;
        auto decl = TryParseEitherFunctionalCastOrDeclaration(AST::DeclaratorType::NON_ABSTRACT, true, false, sc);
        MaybeConsumeSemicolon();
        return decl;
      }
      jdi::definition *def = nullptr;
      std::unique_ptr<AST::Node> operand =
          is_global_local ? std::make_unique<AST::IdentifierAccess>(def, maybe_global_local) : nullptr;
      auto node = ParseExpression(Precedence::kAll, std::move(operand));
      MaybeConsumeSemicolon();
      return node;
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
  return is_decl_specifier(token.type);
}

std::unique_ptr<AST::Node> ParseStatementOrBlock() {
  if (token.type == TT_BEGINBRACE) return ParseCodeBlock();
  Token origin = token;
  if (auto stmt = TryParseStatement()) return stmt;
  // Naming convention: Parse* never returns null. TryParseStatement should
  // have raised a diagnostic on herr; if it also failed to advance the
  // token, that's a parser bug -- report it and force-advance so loop
  // callers don't spin.
  if (token.position == origin.position) {
    herr->Error(token) << "Internal parser error: TryParseStatement returned null without advancing past `" << token.content << "`";
    token = lexer->ReadToken();
  }
  return std::make_unique<AST::SyntaxError>(origin);
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
  bool not_condition = false;

  if (token.type == TT_NOT) {
    if (mode == SyntaxMode::STRICT) {
      herr->Warning(token) << "Use of `not` keyword in if statement";
    }
    not_condition = true;
    token = lexer->ReadToken();
  }

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
  
  return std::make_unique<AST::IfStatement>(std::move(condition), std::move(true_branch), std::move(false_branch),
                                            not_condition);
}

std::unique_ptr<AST::Node> TryParseEitherFunctionalCastOrDeclaration(
    AST::DeclaratorType decl_type, bool parse_unbounded,
    bool maybe_c_style_cast, AST::DeclarationStatement::StorageClass sc) {
  if (next_maybe_functional_cast()) {
    auto declspecs = std::make_unique<AST::DeclSpecList>();
    AST::PNode id_expression = TryParseTypeSpecifier(declspecs.get());
    if (next_is_type_specifier() ||
        // Make sure we don't accidentally consume a c-style cast when its required
        (!(maybe_c_style_cast && token.type == TT_ENDPARENTH) &&
         (token.type != TT_BEGINBRACE && token.type != TT_BEGINPARENTH))) {
      std::pair<bool, bool> global_local = TryParseTypeSpecifierSeq(declspecs.get(), id_expression);
      if (global_local.first && global_local.second) {
        herr->Error(token) << "Cannot have both `global` and `local` storage class specifiers";
      }
      sc = global_local.first    ? AST::DeclarationStatement::StorageClass::GLOBAL
           : global_local.second ? AST::DeclarationStatement::StorageClass::LOCAL
                                 : sc;
      return parse_declarations(sc, std::move(declspecs), std::move(id_expression), decl_type, parse_unbounded, {});
    } else if (token.type == TT_BEGINBRACE) {
      auto init = TryParseBraceInitializer();
      init->target = MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs));
      return init;
    } else if (token.type == TT_BEGINPARENTH) {
      // `Foo( ... )`: parse as a call-shaped expression with a TypeSpecifierSeq callee.
      // This is the most-vexing-parse: it could be a functional cast, a
      // temporary-object expression, or a declaration whose declarator is
      // parenthesized (`Foo (*p)`). The parser stays context-free and emits
      // the call shape uniformly; the semantic phase disambiguates. (Replaces
      // the old speculative declarator-parse-then-rollback via to_expression.)
      //
      // NB comma precedence: kAll includes the comma operator, so
      // `Foo(*p), q` parses as a comma-expression here, NOT as two declarators
      // sharing the `Foo` spec (the way the declaration parser treats a comma).
      // When the semantic phase resolves this construct to a declaration it
      // must split the top-level comma into per-declarator nodes; when it
      // resolves to an expression the comma-expression stands.
      auto callee = MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs));
      // The call shape is a maybe-declarator context: when this production
      // permits an abstract declarator, its argument positions may hold
      // abstract shapes too (`int (*)[10]` as a nested parameter), so the
      // abstract-operand flag follows decl_type through the call parse.
      std::unique_ptr<AST::FunctionCallExpression> call;
      {
        ScopedFlag allow_abstract(allow_abstract_operand_,
                                  decl_type != AST::DeclaratorType::NON_ABSTRACT);
        call = TryParseFunctionCallExpression(Precedence::kAll, std::move(callee));
      }

      // [stmt.ambig]: "if it can be a declaration, it is."
      // Convert a cast like `int(identifier)` to a declaration.
      // Notably, that cast must have exactly one argument:
      // `int(a,b)` is not declaring both `a` and `b`.
      if (call->arguments.size() == 1) {
        if (const Token *namep = find_declarator_name(call->arguments[0].get())) {
          Token name = *namep;
          auto specifiers = dynamic_unique_pointer_cast<AST::TypeSpecifierSeq>(std::move(call->function));
          // The first parenthesized group was consumed as the call's argument
          // list, so its content is the *inner* of a parenthesized declarator.
          // Whether those parens are meaningful grouping depends on what follows
          // the group: a trailing declarator postfix (`int(*p)[10]`,
          // `int(*p)(int)`) binds to the whole group, so re-wrap it as a grouping
          // Parenthetical and continue the postfix loop (kFuncCall continues only
          // `[]`/`()` and stops at any binary op or `=`). With nothing trailing
          // (`int(*(*a)[10])`) the parens are redundant grouping around the entire
          // declarator -- strip them and take the bare argument as the declarator.
          AST::PNode declarator_expr;
          if (token.type == TT_BEGINBRACKET || token.type == TT_BEGINPARENTH) {
            auto paren = std::make_unique<AST::Parenthetical>(std::move(call->arguments[0]));
            declarator_expr = ParseExpression(Precedence::kFuncCall, std::move(paren));
          } else {
            declarator_expr = std::move(call->arguments[0]);
          }
          std::vector<std::unique_ptr<AST::InitDeclarator>> declarators;
          declarators.push_back(std::make_unique<AST::InitDeclarator>(
              name, std::move(declarator_expr),
              next_is_start_of_initializer() ? TryParseInitializer() : nullptr));
          auto clause = std::make_unique<AST::DeclaratorClause>(std::move(specifiers), std::move(declarators));
          register_declarations(clause.get());
          return std::make_unique<AST::DeclarationStatement>(sc, std::move(clause));
        }
      }
      return ParseExpression(Precedence::kAll, std::move(call));
    } else if (token.type == TT_ENDPARENTH && maybe_c_style_cast) {
      token = lexer->ReadToken();
      // The spec-seq was consumed up-front and the `)` proves there's no
      // declarator, so wrap it in a DeclaratorClause with a single abstract
      // (declarator-less) init-declarator -- keeping cast->type uniformly a
      // DeclaratorClause, same as the operand-level C-style and named casts.
      auto specifiers = MakeTypeSpecifierSeq(std::move(id_expression), std::move(declspecs));
      std::vector<std::unique_ptr<AST::InitDeclarator>> declarators;
      declarators.push_back(std::make_unique<AST::InitDeclarator>());
      auto type_node = std::make_unique<AST::DeclaratorClause>(std::move(specifiers), std::move(declarators));
      return std::make_unique<AST::CastExpression>(AST::CastExpression::Kind::C_STYLE, token, std::move(type_node),
                                                   ParseExpression(Precedence::kAll));
    } else {
      // This should be unreachable...
      return TryParseDeclarations(parse_unbounded, decl_type);
    }
  } else {
    return TryParseDeclarations(parse_unbounded, decl_type);
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
  if (next_is_decl_specifier()) {
    init = TryParseEitherFunctionalCastOrDeclaration(
        AST::DeclaratorType::NON_ABSTRACT, true, is_conventional,
        AST::DeclarationStatement::StorageClass::TEMPORARY);
    if (init->type == AST::NodeType::CAST) {
      auto *cast = dynamic_cast<AST::CastExpression *>(init.get());
      if (cast->kind == AST::CastExpression::Kind::C_STYLE && is_conventional) {
        is_conventional = false;
      }
    }
  } else if (token.type != TT_SEMICOLON) {
    // All three clauses are optional; an omitted one is null, not a sentinel.
    init = ParseExpression(Precedence::kAll, nullptr);
  }
  if (token.type == TT_ENDPARENTH) {
    is_conventional = false;
    token = lexer->ReadToken();
  }
  require_token(TT_SEMICOLON, "Expected semicolon (';') after for-loop initializer");
  if (token.type != TT_SEMICOLON && token.type != TT_ENDPARENTH) {
    cond = TryParseControlExpression(SyntaxMode::GML);
  }
  require_token(TT_SEMICOLON, "Expected semicolon (';') after for-loop condition");
  if (token.type != TT_SEMICOLON && token.type != TT_ENDPARENTH) {
    incr = ParseExpression(Precedence::kAll);
  }
  // GM8 accepts a trailing ; after the increment: for (i = 0; i < n; i += 1;)
  if (is_conventional && token.type == TT_SEMICOLON) token = lexer->ReadToken();

  if (is_conventional) {
    require_token(TT_ENDPARENTH, "Expected closing parenthesis (')') after for-loop header");
  }

  AST::PNode body = nullptr;
  if (token.type == TT_SEMICOLON) {
    token = lexer->ReadToken();
  } else {
    body = ParseCFStmtBody();
  }

  return std::make_unique<AST::ForLoop>(std::move(init), std::move(cond), std::move(incr), std::move(body));
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
  // The value is optional: a bare `return` carries a null expression, not a
  // sentinel. Peek the statement's legitimate enders before parsing.
  AST::PNode value;
  if (token.type != TT_SEMICOLON && token.type != TT_ENDBRACE &&
      token.type != TT_ENDOFCODE) {
    value = ParseExpression(Precedence::kAll);
  }

  MaybeConsumeSemicolon();

  return std::make_unique<AST::ReturnStatement>(std::move(value), false);
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
  MaybeConsumeSemicolon();
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
    expr = ParseExpression(Precedence::kAll);
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

// The source token of an id-expression tree's final segment, for reporting
// against; null for token-free roots (ImplicitType, Decltype).
static const Token *id_expression_token(AST::Node *node) {
  if (node == nullptr) return nullptr;
  switch (node->type) {
    case AST::NodeType::IDENTIFIER:
      return &node->As<AST::IdentifierAccess>()->name;
    case AST::NodeType::SCOPE_ACCESS:
      return &node->As<AST::ScopeAccess>()->name;
    case AST::NodeType::TEMPLATE_ID:
      return id_expression_token(node->As<AST::TemplateId>()->name.get());
    default:
      return nullptr;
  }
}

class SyntaxChecker : public AST::Visitor {
  ErrorHandler *herr;
  const LanguageFrontend * frontend;

 public:
  SyntaxChecker(ErrorHandler *herr, const LanguageFrontend *fe) : herr(herr), frontend(fe) {}

  bool VisitTypeSpecifierSeq(AST::TypeSpecifierSeq &node) {
    // A specifier run names at most one base type. The parser retains any
    // surplus trees on the spec list (last one wins the id-expression slot)
    // rather than judging mid-parse; the diagnostic lives here, reported
    // against the winning (i.e. colliding) name.
    if (node.declspecs && !node.declspecs->extra_base_types.empty()) {
      if (const Token *at = id_expression_token(node.id_expression.get())) {
        herr->Error(*at) << "Usage of two types in type specifier";
      } else if (!node.declspecs->specs.empty()) {
        herr->Error(node.declspecs->specs.front()) << "Usage of two types in type specifier";
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  // Call argument checking is deliberately absent here: a callee's identity
  // (and thus its arity) isn't knowable until the semantic layer resolves
  // dot accesses and instance lookups, so validate_call lives on the
  // SemanticAnnotator.

  bool VisitDeclarationStatement(AST::DeclarationStatement &node) {
    // TODO: belongs in a dedicated semantic-validation pass, not interleaved
    // with syntactic checks. Tracked separately.
    //
    // Flag conflicts are properties of the shared decl-spec sequence, read
    // from the clause's specifiers->declspecs. Pairs that the parser already
    // rejects while folding (`long short`) are not re-checked here.
    auto *type_id = node.clause ? node.clause->specifiers.get() : nullptr;
    if (type_id && type_id->declspecs) {
      const AST::DeclSpecList &declspecs = *type_id->declspecs;
      const std::size_t flags = declspecs.flags;
      // signed/unsigned: `signed` writes no flag bits (JDI encodes it as the
      // absence of unsigned), so the pair is only detectable from the retained
      // spec tokens. The parser catches unsigned-then-signed at fold time;
      // this catches both orders, reported on the real `signed` token.
      if (flags & AstBuilder::flag_mask(jdi::builtin_flag__unsigned)) {
        for (const Token &spec : declspecs.specs) {
          if (spec.content == "signed") {
            herr->Error(spec) << "Conflicting use of 'signed' and 'unsigned' in the same type specifier";
            break;
          }
        }
      }
      static const struct { jdi::typeflag *const *a, *const *b; } conflicts[] = {
        {&jdi::builtin_flag__const,   &jdi::builtin_flag__mutable },
        {&jdi::builtin_flag__static,  &jdi::builtin_flag__register},
        {&jdi::builtin_flag__inline,  &jdi::builtin_flag__register},
        {&jdi::builtin_flag__mutable, &jdi::builtin_flag__static  },
      };
      for (const auto &c : conflicts) {
        const jdi::typeflag *a = *c.a, *b = *c.b;
        if (AstBuilder::flag_matches(flags, a) && AstBuilder::flag_matches(flags, b)) {
          const Token *at = nullptr;
          for (const Token &spec : declspecs.specs)
            if (spec.content == a->name) { at = &spec; break; }
          if (at != nullptr) {
            herr->Error(*at) << "Conflicting use of '" << a->name << "' and '"
                             << b->name << "' in the same type specifier";
          }
        }
      }
    }
    node.RecursiveSubVisit(*this);
    return false;
  }

  bool VisitIfStatement(AST::IfStatement &node) {
    if (node.condition->type == AST::NodeType::BINARY_EXPRESSION) {
      if (node.condition->As<AST::BinaryExpression>()->operation.type == TT_EQUALS) {
        node.condition->As<AST::BinaryExpression>()->operation.type = TT_EQUALTO;
        node.condition->As<AST::BinaryExpression>()->operation.token = "==";
      }
    } else if (node.condition->type == AST::NodeType::PARENTHETICAL) {
      auto paren = node.condition->As<AST::Parenthetical>();
      if (paren->expression->type == AST::NodeType::BINARY_EXPRESSION) {
        if (paren->expression->As<AST::BinaryExpression>()->operation.type == TT_EQUALS) {
          paren->expression->As<AST::BinaryExpression>()->operation.type = TT_EQUALTO;
          paren->expression->As<AST::BinaryExpression>()->operation.token = "==";
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
