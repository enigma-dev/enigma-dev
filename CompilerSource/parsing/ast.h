/** Copyright (C) 2020 Josh Ventura
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef ENIGMA_COMPILER_PARSING_AST_h
#define ENIGMA_COMPILER_PARSING_AST_h

#include "win32_macro_guard.h"
#include "error_reporting.h"
#include "lexer.h"
#include "tokens.h"
#include "darray.h"

#include <JDI/src/Storage/definition.h>

#include <memory>
#include <optional>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct ParsedScope;  // object_storage.h
struct CompileState;

namespace enigma::parsing {

class AST {
 public:
  enum class NodeType {
    ERROR = 0,
    BLOCK = 1,
    BINARY_EXPRESSION,
    UNARY_PREFIX_EXPRESSION,
    UNARY_POSTFIX_EXPRESSION,
    TERNARY_EXPRESSION,
    LAMBDA_EXPRESSION,
    TYPE_SPECIFIER_SEQ,
    DECL_SPEC_LIST,
    SIZEOF, ALIGNOF, CAST,
    NEW, DELETE,
    PARENTHETICAL, ARRAY,
    IDENTIFIER, SCOPE_ACCESS, LITERAL, FUNCTION_CALL,
    IF, FOR, WHILE, DO, WITH, REPEAT, SWITCH, CASE, DEFAULT,
    BREAK, CONTINUE, RETURN, DECLARATION, INIT_DECLARATOR, INITIALIZER,
    DECLARATOR_CLAUSE, TEMPLATE_ID, DECLTYPE, IMPLICIT_TYPE
  };

  struct Node;
  class Visitor;
  typedef std::unique_ptr<Node> PNode;

  struct Node {
    NodeType type;

    /// Visit this node with the specified visitor. If the visit routine
    /// for this node returns true, visit children of this node as well.
    virtual void RecurusiveVisit(Visitor &visitor) = 0;
    /// Visit children of this node (via RecurusiveVisit) with the given
    /// visitor. Do not invoke the visitor on this node itself.
    virtual void RecursiveSubVisit(Visitor &visitor) = 0;
    /// Cast the node to a given type
    template <typename T>
    T* As() {
        return dynamic_cast<T*>(this);
    }
    // Helper function that calls the appropriate Visitor function for this node type
    virtual bool accept(Visitor& visitor) = 0;

    // JDI bridge: when this node sits in a function-declarator's parameter list,
    // populate `out` (a slot in the ref_stack's parameter_ct) from this node.
    // `parameter` is a full_type + default-arg/variadic; today we fill only the
    // full_type portion (parity with legacy to_jdi_refstack). Default false =
    // "not a parameter-declaration"; overridden by the node shapes the parser
    // leaves parameters in (today: DeclarationStatement).
    virtual bool to_jdi_refstack_parameter(jdi::ref_stack::parameter &out) { (void)out; return false; }

    // The JDI definition this node denotes. The id-expression node kinds
    // (IdentifierAccess / ScopeAccess / TemplateId) carry a `def` member and
    // override this to expose it; every other node names nothing and returns null.
    virtual jdi::definition *Definition() const { return nullptr; }

    Node(NodeType t = NodeType::ERROR): type(t) {}
    virtual ~Node() = default;

   protected:
    template<typename... SubNodes>
    void RV(Visitor &visitor, const SubNodes &...nodes);

   private:
    // unique_ptr<T> is invariant for lvalue conversions even when T : Node,
    // so RV()'s forwarding to RVF needs to accept any unique_ptr<T : Node>
    // (and any vector thereof) directly rather than via the base PNode type.
    template<typename T>
    void RVF(Visitor &visitor, const std::unique_ptr<T> &single_node) {
      if (single_node) single_node->RecurusiveVisit(visitor);
    }
    template<typename T>
    void RVF(Visitor &visitor, const std::vector<std::unique_ptr<T>> &node_list) {
      for (const auto &node : node_list) node->RecurusiveVisit(visitor);
    }
  };

  template<NodeType kType> struct TypedNode : Node {
    TypedNode(): Node(kType) {}
  };

  struct ConstValue {
    /// Hardware representation of supported values.
    typedef std::variant<long double, long long, std::string> HardwareValue;
    HardwareValue value;
    TokenType type;

    /// When processed from a C++-compatible token, this is the original
    /// spelling. Useful for preserving floats like 0.123.
    /// When the original spelling is not available, this serves as a
    /// cache of the latest computed spelling.
    std::optional<std::string> literal_representation;

    // TODO: Make this parse the data correctly
    ConstValue(const Token &t): value{std::string{t.content}}, type{t.type} {}
    std::string ToCppLiteral() const { return ""; }
    std::string ToCppLiteral() { return "";}
  };

#define BASIC_NODE_ROUTINES(name)                                            \
  bool accept(Visitor &visitor) final { return visitor.Visit##name(*this); } \
  void RecurusiveVisit(Visitor &visitor) final {                             \
    if (visitor.Visit##name(*this)) RecursiveSubVisit(visitor);              \
  }                                                                          \
  void RecursiveSubVisit(Visitor &visitor) final

  // Sentinel for a sub-tree that failed to parse. Carries the token where
  // parsing went off the rails; the diagnostic itself is already on `herr`.
  // Visitors no-op by default (return true so traversal continues).
  struct SyntaxError : TypedNode<NodeType::ERROR> {
    Token origin;

    BASIC_NODE_ROUTINES(SyntaxError);

    SyntaxError() noexcept = default;
    explicit SyntaxError(Token origin): origin{std::move(origin)} {}
  };

  // Simple block of code, containing zero or more statements.
  // The root node of any piece of code will be a block node.
  struct CodeBlock : TypedNode<NodeType::BLOCK> {
    // Individual statements or 
    std::vector<PNode> statements;

    BASIC_NODE_ROUTINES(CodeBlock);

    CodeBlock() noexcept = default;
    CodeBlock(std::vector<PNode> statements): statements{std::move(statements)} {}
  };

  struct Operation{
    TokenType type;
    std::string token;

    Operation(TokenType type_, std::string token_):type(type_), token(token_){}
  };

  // Binary expressions; generally top-level will be "foo = expression"
  struct BinaryExpression : TypedNode<NodeType::BINARY_EXPRESSION> {
    PNode left, right;
    Operation operation;

    /// EDL's / is real division (div is the integer kind); the semantic
    /// annotator marks divisions for lowering and the printer coerces.
    /// Unannotated trees print verbatim.
    bool lower_real_division = false;

    /// GML's = compares in value position (`if (a = 1)`); only a
    /// statement-position = assigns. The annotator marks value-position
    /// TT_EQUALS nodes and the printer emits ==.
    bool lower_gml_equals = false;

    /// GML evaluates operands left to right; C++ leaves it unspecified.
    /// Marked by the annotator where the order is observable.
    bool evaluate_in_order = false;

    BASIC_NODE_ROUTINES(BinaryExpression);

    BinaryExpression(PNode left_, PNode right_, Operation operation_):
        left(std::move(left_)), right(std::move(right_)),
        operation(operation_) {}
  };

  // Function call expression, foo(bar)
  struct FunctionCallExpression : TypedNode<NodeType::FUNCTION_CALL> {
    PNode function;
    std::vector<PNode> arguments;
    /// As BinaryExpression::evaluate_in_order, for the arguments.
    bool evaluate_in_order = false;

    BASIC_NODE_ROUTINES(FunctionCallExpression);

    FunctionCallExpression(PNode function_, std::vector<PNode> &&arguments_): function{std::move(function_)}, arguments{std::move(arguments_)} {}
  };

  // Unary prefix expressions; generally top-level will be "++varname"
  struct UnaryPrefixExpression : TypedNode<NodeType::UNARY_PREFIX_EXPRESSION> {
    PNode operand;
    Operation operation;

    // Returns true iff this operation can be used in a declaration.
    // (e.g. `*x`, `&x`, `&&x`)
    bool CanBeTypeSpecifier() const;

    BASIC_NODE_ROUTINES(UnaryPrefixExpression);

    UnaryPrefixExpression(PNode operand_, Operation operation_):
        operand(std::move(operand_)), operation(operation_) {}
  };

  // Unary postfix expression
  struct UnaryPostfixExpression : TypedNode<NodeType::UNARY_POSTFIX_EXPRESSION> {
    PNode operand;
    Operation operation;

    BASIC_NODE_ROUTINES(UnaryPostfixExpression);

    UnaryPostfixExpression(PNode operand_, Operation operation_):
        operand(std::move(operand_)), operation(operation_) {}
  };

  // Ternary expression; the only one is ?:
  struct TernaryExpression : TypedNode<NodeType::TERNARY_EXPRESSION> {
    PNode condition;
    PNode true_expression;
    PNode false_expression;

    BASIC_NODE_ROUTINES(TernaryExpression);

    TernaryExpression(PNode condition_, PNode true_expression_, PNode false_expression_):
      condition{std::move(condition_)}, true_expression{std::move(true_expression_)}, false_expression{std::move(false_expression_)} {}
  };

  // A run of decl-specifier keywords (`unsigned long const ...`). Order is
  // preserved for pretty-printer fidelity; `flags` is the JDI-bitmask sum
  // for semantic-phase consumers.
  struct DeclSpecList : TypedNode<NodeType::DECL_SPEC_LIST> {
    std::vector<Token> specs;
    std::size_t flags = 0;
    // A specifier run names at most one base type; when the source names more
    // (`int float x;`), the later name wins the owning seq's id-expression
    // slot and the earlier trees are retained here as evidence. The parser
    // does not judge this -- the "two types in one specifier" diagnostic is
    // the post-parse checker's (see SyntaxChecker::VisitTypeSpecifierSeq).
    // Not printed; visited for diagnostics.
    std::vector<PNode> extra_base_types;

    BASIC_NODE_ROUTINES(DeclSpecList);

    DeclSpecList() = default;
    DeclSpecList(std::vector<Token> specs_, std::size_t flags_):
        specs(std::move(specs_)), flags(flags_) {}
  };

  // Models the C++ `type-specifier-seq` production: a base type name plus its
  // run of cv/sign/length specifiers (`unsigned long const …`). It is NOT a
  // full `type-id` — a type-id is `type-specifier-seq` + an (abstract-)declarator
  // (the `*` `&` `[]` `()` part). That declarator is modeled separately: today
  // on `InitDeclarator` (declarator_expr), and
  // in the unified design by a `DeclaratorClause` node pairing this seq with a
  // declarator tree. So a cast/sizeof/new target is a `DeclaratorClause` whose
  // specifier part is one of these; this node alone is just the spec run.
  //
  // The base type is held as the `id_expression` tree; its root carries the JDI
  // definition the seq denotes, recovered on demand via Definition() (a pure tree
  // read). The "seq" is `declspecs` — a `DeclSpecList`, i.e. an ordered
  // `std::vector<Token>`.
  //
  // `flags` is the decl-spec bitmask (cv/sign/length) in JDI's encoding. The base
  // type's definition is not stored: it is the id-expression tree's definition
  // (Definition(), a pure tree read). An inferred base type -- implied `int`
  // from a length/sign specifier, or EDL's `var` for an otherwise-untyped run
  // -- is materialized by the parser as an ImplicitType leaf in
  // `id_expression`, so no flag-driven fallback is needed here. `flags` is
  // kept in sync with `declspecs->flags` by the parser (every flag write hits
  // both); `declspecs` additionally retains the source-order specifier tokens
  // for round-trip fidelity.
  struct TypeSpecifierSeq : TypedNode<NodeType::TYPE_SPECIFIER_SEQ> {
    // Declared before `declspecs` so the primary ctor's mem-init reads the
    // `specs` param's flags before it is moved into `declspecs`.
    std::size_t flags = 0;
    PNode id_expression;
    std::unique_ptr<DeclSpecList> declspecs;

    BASIC_NODE_ROUTINES(TypeSpecifierSeq);

    // The base type this spec-seq denotes: the id-expression tree's definition
    // (an inferred base type is an ImplicitType leaf in that tree, so this
    // stays a pure read).
    jdi::definition *Definition() const override;

    // id_expression is optional (nullable); declspecs is optional (nullable).
    // `flags` is taken from the spec list when present.
    TypeSpecifierSeq(PNode id_exp, std::unique_ptr<DeclSpecList> specs):
        flags(specs ? specs->flags : 0), id_expression(std::move(id_exp)), declspecs(std::move(specs)) {}

    // JDI bridge. Feeds this spec-seq into the legacy JDI machinery (template-arg
    // keys, function-parameter ref-stacks) as a declarator-less full_type. The
    // declarator half is built separately from the AST declarator-expression-tree
    // by the owning DeclaratorClause via walk_declarator_expr.
    jdi::full_type to_jdi_fulltype();
  };

  // Lambda expression: x => x + 10;
  struct LambdaExpression : TypedNode<NodeType::LAMBDA_EXPRESSION> {
    PNode parameters;
    PNode body;

    BASIC_NODE_ROUTINES(LambdaExpression);

    LambdaExpression(PNode parameters_, PNode body_):
      parameters{std::move(parameters_)}, body{std::move(body_)} {}
  };

  // Sizeof expression
  struct SizeofExpression : TypedNode<NodeType::SIZEOF> {
    enum class Kind { EXPR, VARIADIC, TYPE } kind;
    PNode argument;

    BASIC_NODE_ROUTINES(SizeofExpression);

    explicit SizeofExpression(PNode arg): kind{Kind::EXPR}, argument{std::move(arg)} {}
    SizeofExpression(Kind k, PNode arg): kind{k}, argument{std::move(arg)} {}
  };

  // Alignof expression
  struct AlignofExpression : TypedNode<NodeType::ALIGNOF> {
    PNode type;

    BASIC_NODE_ROUTINES(AlignofExpression);

    explicit AlignofExpression(PNode type_): type{std::move(type_)} {}
  };

  // Cast expressions
  struct CastExpression : TypedNode<NodeType::CAST> {
    // Note that FUNCTIONAL casts are now modeled as initializing a TypeSpecifierSeq.
    enum class Kind { C_STYLE, STATIC, DYNAMIC, REINTERPRET, CONST } kind;
    PNode type;
    PNode expr;

    static const std::vector<std::string> KindNames;

    BASIC_NODE_ROUTINES(CastExpression);
    static std::string KindToString(Kind k);

    CastExpression(const Token &token, PNode type_, PNode expr_):
       type{std::move(type_)}, expr{std::move(expr_)} {
      switch (token.type) {
        case TT_ENDPARENTH:     kind = Kind::C_STYLE; break;
        case TT_STATIC_CAST:      kind = Kind::STATIC; break;
        case TT_DYNAMIC_CAST:     kind = Kind::DYNAMIC; break;
        case TT_REINTERPRET_CAST: kind = Kind::REINTERPRET; break;
        case TT_CONST_CAST:       kind = Kind::CONST; break;
        default:                  break;
      }
    }

    CastExpression(Kind kind_, const Token &token, PNode type_, PNode expr_):
      CastExpression(token, std::move(type_), std::move(expr_)) {
      kind = kind_;
    }
  };

  // No-op tree node that allows true-to-original pretty printing and
  // establishes a formal place for empty (null) nodes in a complete tree.
  struct Parenthetical : TypedNode<NodeType::PARENTHETICAL> {
    PNode expression;

    BASIC_NODE_ROUTINES(Parenthetical);

    Parenthetical(PNode expression_): expression(std::move(expression_)) {}
  };

  struct Array : TypedNode<NodeType::ARRAY> {
    std::vector<PNode> elements;

    BASIC_NODE_ROUTINES(Array);

    Array(std::vector<PNode> &&elements_): elements(std::move(elements_)) {}
  };

  struct IdentifierAccess : TypedNode<NodeType::IDENTIFIER> {
    // A bare name. If it resolved to a C++ symbol during the parse, `def` is the
    // jdi::definition; otherwise (an EDL-declared local, or an as-yet-unresolved
    // name) `def` is null and the semantic phase binds it. The pretty-printer
    // reads non-null `def` as "emit the name verbatim" rather than wrapping it
    // in an EDL variable accessor.
    jdi::definition *def = nullptr;
    // When this IdentifierAccess is the leaf of a declarator chain, `name.content`
    // may be empty — that encodes an *abstract* declarator (no name, e.g. the
    // type in `(int*)x` or an unnamed function parameter). Consumers that read
    // the name should tolerate empty content; the existing convention in
    // parser.cpp:678 etc. already does.
    Token name;

    BASIC_NODE_ROUTINES(IdentifierAccess);

    IdentifierAccess(jdi::definition *def, Token name): def{def}, name{name} {}
    IdentifierAccess(Token name): name{name} {}

    jdi::definition *Definition() const override { return def; }
  };

  // A qualified-id: `lhs :: name`. EDL also accepts `.` and `->` here as sugar for
  // `::`; one node covers all three and the semantic phase picks the operator from
  // what `lhs` resolves to. `lhs` is the scope (another id-expression), null for a
  // global-scope `::name`. A template specialization on the final segment
  // (`a::b<int>`) is modeled by wrapping this node in a TemplateId.
  //
  // `def` is the definition the whole qualified-id denotes (`name` resolved in
  // `lhs`'s scope), null when unresolved.
  struct ScopeAccess : TypedNode<NodeType::SCOPE_ACCESS> {
    PNode lhs;   // scope id-expression; null = global scope (`::name`)
    Token name;  // trailing unqualified-id
    jdi::definition *def = nullptr;
    // The access operator as written (:: or EDL's universal .). Formatting
    // keeps the user's spelling; emission decides from access_kind.
    Token op;

    /// Semantic classification of a dot access, written by the semantic
    /// annotator after linking. Printers read it; UNRESOLVED trees (unit
    /// harnesses, pre-annotation prints) fall back to spelling heuristics.
    enum class AccessKind : char { UNRESOLVED, MEMBER, VARACCESS, GLOBAL, LOCAL, SHARED };
    AccessKind access_kind = AccessKind::UNRESOLVED;

    BASIC_NODE_ROUTINES(ScopeAccess);

    ScopeAccess(PNode lhs_, Token name_, jdi::definition *def_ = nullptr):
        lhs(std::move(lhs_)), name(std::move(name_)), def(def_) {}
    ScopeAccess(PNode lhs_, Token name_, Token op_):
        lhs(std::move(lhs_)), name(std::move(name_)), op(std::move(op_)) {}

    jdi::definition *Definition() const override { return def; }
  };

  // A template-id: `name < args... >`. `name` is the template-name id-expression.
  // Each arg is a type-id tree (a DeclaratorClause) or a constant-expression; the
  // parser leaves them unclassified for the semantic phase. `def` is the resolved
  // specialization, null if not yet instantiated.
  struct TemplateId : TypedNode<NodeType::TEMPLATE_ID> {
    PNode name;
    std::vector<PNode> args;
    jdi::definition *def = nullptr;

    BASIC_NODE_ROUTINES(TemplateId);

    TemplateId(PNode name_, std::vector<PNode> args_, jdi::definition *def_ = nullptr):
        name(std::move(name_)), args(std::move(args_)), def(def_) {}

    jdi::definition *Definition() const override { return def; }
  };

  // A `decltype(operand)` specifier. `operand` is the parsed expression whose type
  // this names. The type it denotes is computed by the semantic phase, so there is
  // no `def` here yet (Definition() stays null). May appear standalone as a type or
  // as the `lhs` of a ScopeAccess (`decltype(x)::y`).
  struct Decltype : TypedNode<NodeType::DECLTYPE> {
    PNode operand;

    BASIC_NODE_ROUTINES(Decltype);

    explicit Decltype(PNode operand_): operand(std::move(operand_)) {}
  };

  // An implicit base type: the user wrote a (non-empty) specifier run but no
  // type-name, so the base type is inferred by rule. A length/sign specifier
  // (`unsigned`, `long`, `short`, `signed`) names C's `int`; any other untyped
  // run (`const x;`) falls back to EDL's universal `var` -- undeclared
  // variables are var, so untyped declared ones are too (`variant` in
  // template-argument position, where var makes a poor element type). There is
  // no source token for the inferred name, so this token-free leaf carries the
  // definition directly. The parser materializes it once the spec-seq is fully
  // consumed (mirroring JDI's read_type), so a TypeSpecifierSeq's Definition()
  // stays a pure read of its id-expression tree rather than a flag-driven
  // fallback. `kind` records which rule fired -- the def alone can't (var may
  // be unresolvable in a header-less harness, leaving it null).
  struct ImplicitType : TypedNode<NodeType::IMPLICIT_TYPE> {
    enum class Kind { INT, UNTYPED } kind;
    jdi::definition *def;

    BASIC_NODE_ROUTINES(ImplicitType);

    ImplicitType(Kind kind, jdi::definition *def): kind(kind), def(def) {}

    jdi::definition *Definition() const override { return def; }
  };

  struct Literal : TypedNode<NodeType::LITERAL> {
    ConstValue value;

    BASIC_NODE_ROUTINES(Literal);

    Literal(const Token &token): value{token} {}
  };
  
  struct IfStatement : TypedNode<NodeType::IF> {
    PNode condition;
    PNode true_branch, false_branch;
    bool not_condition;

    BASIC_NODE_ROUTINES(IfStatement);

    IfStatement(PNode condition_, PNode true_branch_, PNode false_branch_, bool not_condition_): condition{std::move(condition_)},
        true_branch{std::move(true_branch_)}, false_branch{std::move(false_branch_)}, not_condition(not_condition_){}
  };

  struct ForLoop : TypedNode<NodeType::FOR> {
    PNode assignment, condition, increment;
    PNode body;

    BASIC_NODE_ROUTINES(ForLoop);


    ForLoop(PNode assignment_, PNode condition_, PNode increment_, PNode body_): assignment{std::move(assignment_)},
        condition{std::move(condition_)}, increment{std::move(increment_)}, body{std::move(body_)} {}
  };

  struct WhileLoop : TypedNode<NodeType::WHILE> {
    PNode condition;
    PNode body;

    enum class Kind { WHILE, UNTIL, REPEAT } kind;

    BASIC_NODE_ROUTINES(WhileLoop);

    WhileLoop(PNode condition_, PNode body_, Kind kind_): condition{std::move(condition_)},
        body{std::move(body_)}, kind{kind_} {}
  };

  struct DoLoop : TypedNode<NodeType::DO> {
    PNode body;
    PNode condition;
    bool is_until;

    BASIC_NODE_ROUTINES(DoLoop);

    DoLoop(PNode body_, PNode condition_, bool until): body{std::move(body_)}, condition{std::move(condition_)},
        is_until(until) {}
  };

  struct CaseStatement : TypedNode<NodeType::CASE> {
    PNode value;
    std::unique_ptr<AST::CodeBlock> statements;

    BASIC_NODE_ROUTINES(CaseStatement);

    CaseStatement(PNode value, std::unique_ptr<AST::CodeBlock> statements): value{std::move(value)},
                                                                             statements{std::move(statements)} {}
  };

  struct DefaultStatement : TypedNode<NodeType::DEFAULT> {
    std::unique_ptr<AST::CodeBlock> statements;

    BASIC_NODE_ROUTINES(DefaultStatement);

    DefaultStatement(std::unique_ptr<AST::CodeBlock> statements): statements{std::move(statements)} {}
  };

  struct SwitchStatement : TypedNode<NodeType::SWITCH> {
    PNode expression;
    std::unique_ptr<AST::CodeBlock> body;
    // Need to track these because case labels must be unique
    // and there can only be one default label.
    //
    // Use @c std::size_t as vector addresses are not stable thus pointers can cause bugs
    std::unordered_map<ConstValue::HardwareValue, std::size_t> cases;
    std::optional<std::size_t> default_branch = std::nullopt;
    // GML: cases compare with == in order and may be any expression.
    bool lower_gml_switch = false;

    BASIC_NODE_ROUTINES(SwitchStatement);
  };

  struct ReturnStatement : TypedNode<NodeType::RETURN> {
    // Optional: the return value. Default: T()
    PNode expression;
    bool is_exit;

    BASIC_NODE_ROUTINES(ReturnStatement);

    ReturnStatement(PNode expression_, bool is_exit_): expression{std::move(expression_)}, is_exit{is_exit_} {}
  };

  struct BreakStatement : TypedNode<NodeType::BREAK> {
    // Optional: the number of nested loops to break out of (default = 1)
    PNode count;

    BASIC_NODE_ROUTINES(BreakStatement);

    explicit BreakStatement(PNode count_): count{std::move(count_)} {}
  };

  struct ContinueStatement : TypedNode<NodeType::CONTINUE> {
    // Optional: the number of nested loops to continue past (default = 1)
    PNode count;

    BASIC_NODE_ROUTINES(ContinueStatement);

    explicit ContinueStatement(PNode count_): count{std::move(count_)} {}
  };

  struct WithStatement : TypedNode<NodeType::WITH> {
    PNode object;
    PNode body;

    BASIC_NODE_ROUTINES(WithStatement);

    WithStatement(PNode object_, PNode body_): object{std::move(object_)}, body{std::move(body_)} {}
  };

  enum class DeclaratorType {
    ABSTRACT, NON_ABSTRACT, MAYBE_ABSTRACT
  };

  struct Initializer;
  struct DeclaratorClause;

  using InitializerNode = std::unique_ptr<Initializer>;

  struct Initializer : TypedNode<NodeType::INITIALIZER> {
    enum class Kind {
      ASSIGN,          // = expr
      EXPR,            // expr (also used for pack expansions like args...)
      BRACE,           // { ... }
      PAREN            // ( ... )
    } kind;

    PNode target;
    std::vector<PNode> values;
    BASIC_NODE_ROUTINES(Initializer);

    Initializer(Kind k, PNode target, std::vector<PNode> vals):
      kind(k), target(std::move(target)), values(std::move(vals)) {}
  };

  // New expression
  struct NewExpression : TypedNode<NodeType::NEW> {
    bool is_global;
    bool is_array;
    std::vector<PNode> placement_args;
    std::unique_ptr<DeclaratorClause> type;
    std::unique_ptr<Initializer> initializer;

    BASIC_NODE_ROUTINES(NewExpression);

    NewExpression(bool is_global, bool is_array, std::vector<PNode> placement_args, std::unique_ptr<DeclaratorClause> type,
                  std::unique_ptr<Initializer> initializer):
      is_global{is_global}, is_array{is_array}, placement_args{std::move(placement_args)}, type{std::move(type)},
      initializer{std::move(initializer)} {}
  };

  // Delete expression
  struct DeleteExpression : TypedNode<NodeType::DELETE> {
    bool is_global;
    bool is_array;
    PNode expression;

    BASIC_NODE_ROUTINES(DeleteExpression);

    DeleteExpression(bool is_global, bool is_array, PNode expression): is_global{is_global}, is_array{is_array},
                                                                       expression{std::move(expression)} {}
  };

  // One init-declarator (the `<declarator> [= <init>]` half of a declaration).
  // `name` is the declared identifier (source of truth — written directly by
  // the parser). `declarator_expr` is the AST-layer declarator-as-expression-
  // tree describing the type-modifier chain (pointers, refs, array bounds,
  // function-params). The base type lives on the owning DeclaratorClause's
  // `specifiers`; the JDI-bridge full_type is recomposed on demand via
  // DeclaratorClause::to_jdi_fulltype.
  struct InitDeclarator : TypedNode<NodeType::INIT_DECLARATOR> {
    Token name;
    PNode declarator_expr;
    InitializerNode init;

    BASIC_NODE_ROUTINES(InitDeclarator);

    InitDeclarator() noexcept = default;
    InitDeclarator(Token name, PNode declarator_expr_, InitializerNode init):
      name{std::move(name)},
      declarator_expr{std::move(declarator_expr_)}, init{std::move(init)} {}
  };

  // A type-specifier-seq paired with its declarator(s): the unified shape that
  // can sit in expression position (cast/sizeof/alignof/new targets, and later
  // function-params/arrow-fns/tuples) as well as back a DeclarationStatement.
  // `declarators` holds a single (possibly abstract) InitDeclarator for a lone
  // type-id, or several for an init-declarator-list. An abstract declarator is
  // an InitDeclarator whose declarator_expr bottoms out in an empty-name leaf
  // (see make_abstract_operand in the parser). Context-illegal combinations
  // (e.g. a name or initializer inside a sizeof type-id) are left for the
  // semantic phase to reject, not screened out here -- the parser stays
  // context-free, per the types-as-trees rule.
  struct DeclaratorClause : TypedNode<NodeType::DECLARATOR_CLAUSE> {
    std::unique_ptr<TypeSpecifierSeq> specifiers;
    std::vector<std::unique_ptr<InitDeclarator>> declarators;

    BASIC_NODE_ROUTINES(DeclaratorClause);

    // Combine the shared spec-seq with the i-th declarator's expression-tree
    // into the JDI-bridge full_type: { base def, walk_declarator_expr(...), base
    // flags }. This is the single home for the spec+declarator recomposition the
    // legacy Declarator::to_jdi_refstack used to own.
    jdi::full_type to_jdi_fulltype(std::size_t i = 0);

    DeclaratorClause(std::unique_ptr<TypeSpecifierSeq> specifiers_,
                     std::vector<std::unique_ptr<InitDeclarator>> declarators_):
        specifiers(std::move(specifiers_)), declarators(std::move(declarators_)) {}
  };

  struct DeclarationStatement: TypedNode<NodeType::DECLARATION> {
    enum class StorageClass {
      TEMPORARY,
      LOCAL,
      GLOBAL,
    };

    // A declaration is its DeclaratorClause (shared type-specifier-seq + the
    // init-declarator-list) plus how the surrounding statement scopes it.
    std::unique_ptr<DeclaratorClause> clause;
    StorageClass storage_class;
    static const std::vector<std::string> StorageNames; // what is the use of this?

    BASIC_NODE_ROUTINES(DeclarationStatement);
    static std::string StorageToString(StorageClass st);
    bool to_jdi_refstack_parameter(jdi::ref_stack::parameter &out) override;

    DeclarationStatement(StorageClass sc, std::unique_ptr<DeclaratorClause> clause_):
        clause{std::move(clause_)}, storage_class{sc} {}
  };

  class Visitor {
   public:
    virtual bool DefaultVisit(Node &node) {
      (void)node;
      return true;
    }
    virtual bool VisitSyntaxError(SyntaxError &node){ return DefaultVisit(node); }
    virtual bool VisitCodeBlock(CodeBlock &node){ return DefaultVisit(node); }
    virtual bool VisitBinaryExpression(BinaryExpression &node){ return DefaultVisit(node); }
    virtual bool VisitFunctionCallExpression(FunctionCallExpression &node){ return DefaultVisit(node); }
    virtual bool VisitUnaryPrefixExpression(UnaryPrefixExpression &node){ return DefaultVisit(node); }
    virtual bool VisitUnaryPostfixExpression(UnaryPostfixExpression &node){ return DefaultVisit(node); }
    virtual bool VisitTernaryExpression(TernaryExpression &node){ return DefaultVisit(node); }
    virtual bool VisitTypeSpecifierSeq(TypeSpecifierSeq &node){ return DefaultVisit(node); }
    virtual bool VisitDeclSpecList(DeclSpecList &node){ return DefaultVisit(node); }
    virtual bool VisitLambdaExpression(LambdaExpression &node){ return DefaultVisit(node); }
    virtual bool VisitSizeofExpression(SizeofExpression &node){ return DefaultVisit(node); }
    virtual bool VisitAlignofExpression(AlignofExpression &node){ return DefaultVisit(node); }
    virtual bool VisitCastExpression(CastExpression &node){ return DefaultVisit(node); }
    virtual bool VisitParenthetical(Parenthetical &node){ return DefaultVisit(node); }
    virtual bool VisitArray(Array &node){ return DefaultVisit(node); }
    virtual bool VisitIdentifierAccess(IdentifierAccess &node){ return DefaultVisit(node); }
    virtual bool VisitScopeAccess(ScopeAccess &node){ return DefaultVisit(node); }
    virtual bool VisitTemplateId(TemplateId &node){ return DefaultVisit(node); }
    virtual bool VisitDecltype(Decltype &node){ return DefaultVisit(node); }
    virtual bool VisitImplicitType(ImplicitType &node){ return DefaultVisit(node); }
    virtual bool VisitLiteral(Literal &node){ return DefaultVisit(node); }
    virtual bool VisitIfStatement(IfStatement &node){ return DefaultVisit(node); }
    virtual bool VisitForLoop(ForLoop &node){ return DefaultVisit(node); }
    virtual bool VisitWhileLoop(WhileLoop &node){ return DefaultVisit(node); }
    virtual bool VisitDoLoop(DoLoop &node){ return DefaultVisit(node); }
    virtual bool VisitCaseStatement(CaseStatement &node){ return DefaultVisit(node); }
    virtual bool VisitDefaultStatement(DefaultStatement &node){ return DefaultVisit(node); }
    virtual bool VisitSwitchStatement(SwitchStatement &node){ return DefaultVisit(node); }
    virtual bool VisitReturnStatement(ReturnStatement &node){ return DefaultVisit(node); }
    virtual bool VisitBreakStatement(BreakStatement &node){ return DefaultVisit(node); }
    virtual bool VisitContinueStatement(ContinueStatement &node){ return DefaultVisit(node); }
    virtual bool VisitWithStatement(WithStatement &node){ return DefaultVisit(node); }
    virtual bool VisitInitializer(Initializer &node){ return DefaultVisit(node); }
    virtual bool VisitNewExpression(NewExpression &node){ return DefaultVisit(node); }
    virtual bool VisitDeleteExpression(DeleteExpression &node){ return DefaultVisit(node); }
    virtual bool VisitDeclarationStatement(DeclarationStatement &node){ return DefaultVisit(node); }
    virtual bool VisitInitDeclarator(InitDeclarator &node){ return DefaultVisit(node); }
    virtual bool VisitDeclaratorClause(DeclaratorClause &node){ return DefaultVisit(node); }
    virtual bool Visit(PNode &node) {
      return node->accept(*this);
    }
  };

  class CppPrettyPrinter : public AST::Visitor {
    std::ofstream *of;
    bool print_type;
    bool is_script;
    const LanguageFrontend *language_fe = nullptr;
    // Nesting depth of `repeat` lowerings; numbers each level's counter.
    int repeat_depth_ = 0;
    // Names declared within the code being printed. They are C++ locals in
    // the emitted function, so the script lowering must leave them alone
    // (`var` names shadow instance variables, and GML hoists them).
    std::set<std::string, std::less<>> declared_names_;
    std::set<std::string, std::less<>> hoisted_vars_;
    // Set while printing a declarator: its identifiers are declared names,
    // never value reads, regardless of what the lowering knows about them.
    bool in_declarator_ = false;

   public:
    CppPrettyPrinter();
    CppPrettyPrinter(const LanguageFrontend *lfe);
    CppPrettyPrinter(std::ofstream &ofs, const LanguageFrontend *lfe, bool is_script);
    // Pre-pass: collect declared names from the tree about to be printed.
    void CollectDeclaredNames(Node &root);
    // GML `var` is scoped to the whole event or script: declare every one up front.
    void HoistVarDeclarations(Node &root);
    void print(std::string code);
    void PrintSemiColon(PNode &node);
    std::string GetPrintedCode();
    bool VisitCode(CodeBlock &node);
    bool VisitCodeBlock(CodeBlock &node);
    bool VisitBinaryExpression(BinaryExpression &node);
    bool VisitFunctionCallExpression(FunctionCallExpression &node);
    bool VisitUnaryPrefixExpression(UnaryPrefixExpression &node);
    bool VisitUnaryPostfixExpression(UnaryPostfixExpression &node);
    bool VisitTernaryExpression(TernaryExpression &node);
    bool VisitTypeSpecifierSeq(TypeSpecifierSeq &node);
    bool VisitDeclSpecList(DeclSpecList &node);
    bool VisitLambdaExpression(LambdaExpression &node);
    bool VisitSizeofExpression(SizeofExpression &node);
    bool VisitAlignofExpression(AlignofExpression &node);
    bool VisitCastExpression(CastExpression &node);
    bool VisitParenthetical(Parenthetical &node);
    bool VisitArray(Array &node);
    bool VisitIdentifierAccess(IdentifierAccess &node);
    bool VisitScopeAccess(ScopeAccess &node);
    bool VisitTemplateId(TemplateId &node);
    bool VisitDecltype(Decltype &node);
    bool VisitImplicitType(ImplicitType &node);
    bool VisitLiteral(Literal &node);
    bool VisitIfStatement(IfStatement &node);
    bool VisitForLoop(ForLoop &node);
    bool VisitWhileLoop(WhileLoop &node);
    bool VisitDoLoop(DoLoop &node);
    bool VisitCaseStatement(CaseStatement &node);
    bool VisitDefaultStatement(DefaultStatement &node);
    bool VisitSwitchStatement(SwitchStatement &node);
    bool VisitReturnStatement(ReturnStatement &node);
    bool VisitBreakStatement(BreakStatement &node);
    bool VisitContinueStatement(ContinueStatement &node);
    bool VisitWithStatement(WithStatement &node);
    bool VisitInitializer(Initializer &node);
    bool VisitNewExpression(NewExpression &node);
    bool VisitDeleteExpression(DeleteExpression &node);
    bool VisitDeclarationStatement(DeclarationStatement &node);
    bool VisitInitDeclarator(InitDeclarator &node);
    bool VisitDeclaratorClause(DeclaratorClause &node);
  };

  // Structural dump of an AST subtree: one indented line per node, type name
  // plus a few salient attributes (operator token, literal text, identifier).
  // For debugging/diagnostics only -- not part of code generation. Generic
  // traversal lives in DefaultVisit; richer node kinds override for detail.
  class DebugPrinter : public AST::Visitor {
    std::ostream &out;
    int depth = 0;
    bool emit(Node &node, const std::string &detail);

   public:
    explicit DebugPrinter(std::ostream &out_): out{out_} {}
    // Convenience: dump `node` to a string.
    static std::string Dump(Node &node);

    bool DefaultVisit(Node &node) override;
    bool VisitUnaryPrefixExpression(UnaryPrefixExpression &node) override;
    bool VisitUnaryPostfixExpression(UnaryPostfixExpression &node) override;
    bool VisitBinaryExpression(BinaryExpression &node) override;
    bool VisitFunctionCallExpression(FunctionCallExpression &node) override;
    bool VisitLiteral(Literal &node) override;
    bool VisitIdentifierAccess(IdentifierAccess &node) override;
    bool VisitScopeAccess(ScopeAccess &node) override;
    bool VisitTemplateId(TemplateId &node) override;
    bool VisitDecltype(Decltype &node) override;
    bool VisitImplicitType(ImplicitType &node) override;
  };

  // Used to adapt to current single-error syntax checking interface.
  ErrorCollector herr;
  // A lexed (tokenized) view of the code.
  const std::shared_ptr<Lexer> lexer;
  // The raw input code, owned by the lexer.
  const std::string &code;
  // Lambda function to get nodes types as strings 
  static const std::vector<std::string> NodesNames;

  bool HasError() { return !herr.errors.empty(); }
  std::string ErrorString() {
    if (herr.errors.empty()) return "No error";
    return herr.errors.front().ToString();
  }

  // Returns true if there's no actual executable code in this AST.
  bool empty() const;

  // Utility routine: Apply this AST to a specified instance.
  void ApplyTo(int instance_id);

  // Extract declarations from this AST into the specified scope.
  void ExtractDeclarations(ParsedScope *destination_scope, CompileState *cs);

  // Pretty-prints this code to a stream with the given base indentation.
  // void PrettyPrint(std::ofstream &of, int base_indent = 2) const;

  // Writes this code in C++ format to the given stream with the given
  // base indentation. The compiler will attempt to preserve token line
  // numbers and will emit a #file directive for the original source.
  //
  // The caller is responsible for having already printed applicable
  // function declarations and opening braces, statements, etc, and for
  // printing the closing statements and braces afterward.
  void WriteCppToStream(std::ofstream &of, int base_indent = 2, bool is_script = false) const;

  // Parses the given code, returning an AST*. The resulting AST* is never null.
  // If syntax errors were encountered, they are stored within the AST.
  static AST Parse(std::string_view code, const ParseContext* ctex);

  // Full recursive walk: each node's Visit routine runs, and children are
  // visited automatically when it returns true. Visitors that recurse
  // manually (RecursiveSubVisit + return false) behave identically.
  void VisitNodes(Visitor &visitor) {
    if (root_) root_->RecurusiveVisit(visitor);
  }

  // Get the node type as a string
  static std::string NodeToString(NodeType nt);

  // Disallow copy. Our tokens point into our code.
  AST(const AST &) = delete;
  AST(AST &&other) = default;

 private:
  std::unique_ptr<Node> root_;
  // When specified, emits code to apply to a specific instance.
  std::optional<int> apply_to_;
  

  // Constructs an AST from the code it will parse. Does not initiate parse.
  AST(std::string_view code_, const ParseContext *ctex):
      lexer(std::make_unique<Lexer>(code_, ctex, &herr)),
      code(lexer->GetCode()) {}
};

// AST→JDI bridge: walk a declarator-expression-tree into a jdi::ref_stack.
// Returns false on a malformed sub-tree (unsupported node type, missing array
// size, etc.). Combine with a TypeSpecifierSeq's Definition()/flags to produce a full jdi::full_type.
bool walk_declarator_expr(AST::Node *expr, jdi::ref_stack &result);

}  // namespace enigma::parsing

#endif  // ENIGMA_COMPILER_PARSING_AST_h
