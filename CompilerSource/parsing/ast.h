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

#include "full_type.h"
#include "error_reporting.h"
#include "lexer.h"
#include "tokens.h"
#include "darray.h"

#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct ParsedScope;  // object_storage.h

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
    SIZEOF, ALIGNOF, CAST,
    NEW, DELETE,
    PARENTHETICAL, ARRAY,
    IDENTIFIER, SCOPE_ACCESS, LITERAL, FUNCTION_CALL,
    IF, FOR, WHILE, DO, WITH, REPEAT, SWITCH, CASE, DEFAULT,
    BREAK, CONTINUE, RETURN, DECLARATION, INITIALIZER
  };
  struct Node {
    NodeType type;
    
    Node(NodeType t = NodeType::ERROR): type(t) {}
    virtual ~Node() = default;
  };
  typedef std::unique_ptr<Node> PNode;

  template<NodeType kType> struct TypedNode : Node {
    TypedNode(): Node(kType) {}
  };

  struct ConstValue {
    /// Hardware representation of supported values.
    typedef std::variant<long double, long long, std::string> HardwareValue;
    HardwareValue value;

    /// When processed from a C++-compatible token, this is the original
    /// spelling. Useful for preserving floats like 0.123.
    /// When the original spelling is not available, this serves as a
    /// cache of the latest computed spelling.
    std::optional<std::string> literal_representation;

    // TODO: Make this parse the data correctly
    ConstValue(const Token &t): value{std::string{t.content}} {}
    std::string ToCppLiteral() const { return ""; }
    std::string ToCppLiteral() { return "";}
  };

  // Simple block of code, containing zero or more statements.
  // The root node of any piece of code will be a block node.
  struct CodeBlock : TypedNode<NodeType::BLOCK> {
    // Individual statements or 
    std::vector<PNode> statements;

    CodeBlock() noexcept = default;
    CodeBlock(std::vector<PNode> statements): statements{std::move(statements)} {}
  };
  // Binary expressions; generally top-level will be "foo = expression"
  struct BinaryExpression : TypedNode<NodeType::BINARY_EXPRESSION> {
    PNode left, right;
    TokenType operation;
    BinaryExpression(PNode left_, PNode right_, TokenType operation_):
        left(std::move(left_)), right(std::move(right_)),
        operation(operation_) {}
  };
  // Function call expression, foo(bar)
  struct FunctionCallExpression: TypedNode<NodeType::FUNCTION_CALL> {
    PNode function;
    std::vector<PNode> arguments;
    FunctionCallExpression(PNode function_, std::vector<PNode> &&arguments_): function{std::move(function_)}, arguments{std::move(arguments_)} {}
  };
  // Unary prefix expressions; generally top-level will be "++varname"
  struct UnaryPrefixExpression : TypedNode<NodeType::UNARY_PREFIX_EXPRESSION> {
    PNode operand;
    TokenType operation;
    UnaryPrefixExpression(PNode operand_, TokenType operation_):
        operand(std::move(operand_)), operation(operation_) {}
  };
  // Unary postfix expression
  struct UnaryPostfixExpression: TypedNode<NodeType::UNARY_POSTFIX_EXPRESSION> {
    PNode operand;
    TokenType operation;
    UnaryPostfixExpression(PNode operand_, TokenType operation_):
        operand(std::move(operand_)), operation(operation_) {}
  };
  // Ternary expression; the only one is ?:
  struct TernaryExpression : TypedNode<NodeType::TERNARY_EXPRESSION> {
    PNode condition;
    PNode true_expression;
    PNode false_expression;
    TernaryExpression(PNode condition_, PNode true_expression_, PNode false_expression_):
      condition{std::move(condition_)}, true_expression{std::move(true_expression_)}, false_expression{std::move(false_expression_)} {}
  };
  // Sizeof expression
  struct SizeofExpression : TypedNode<NodeType::SIZEOF> {
    enum class Kind { EXPR, VARIADIC, TYPE } kind;
    std::variant<PNode, std::string, FullType> argument;

    explicit SizeofExpression(PNode arg): kind{Kind::EXPR}, argument{std::move(arg)} {}
    explicit SizeofExpression(std::string ident): kind{Kind::VARIADIC}, argument{std::move(ident)} {}
    explicit SizeofExpression(FullType ft): kind{Kind::TYPE}, argument{std::move(ft)} {}
  };
  // Alignof expression
  struct AlignofExpression : TypedNode<NodeType::ALIGNOF> {
    FullType ft;

    explicit AlignofExpression(FullType type): ft{std::move(type)} {}
  };
  // Cast expressions
  struct CastExpression : TypedNode<NodeType::CAST> {
    enum class Kind { C_STYLE, STATIC, DYNAMIC, REINTERPRET, CONST, FUNCTIONAL } kind;
    FullType ft;
    PNode expr;
    TokenType functional_cast_type;

    CastExpression(const Token &token, FullType type, PNode expr, TokenType cast_type):
       ft{std::move(type)}, expr{std::move(expr)}, functional_cast_type{cast_type} {
      switch (token.type) {
        case TT_BEGINPARENTH:     kind = Kind::C_STYLE; break;
        case TT_STATIC_CAST:      kind = Kind::STATIC; break;
        case TT_DYNAMIC_CAST:     kind = Kind::DYNAMIC; break;
        case TT_REINTERPRET_CAST: kind = Kind::REINTERPRET; break;
        case TT_CONST_CAST:       kind = Kind::CONST; break;
        case TT_BEGINBRACE:       kind = Kind::FUNCTIONAL; break;
        default:                  break;
      }
    }

    CastExpression(Kind kind_, const Token &token, FullType type, PNode expr, TokenType cast_type):
      CastExpression(token, std::move(type), std::move(expr), cast_type) {
      kind = kind_;
    }
  };

  // No-op tree node that allows true-to-original pretty printing and
  // establishes a formal place for empty (null) nodes in a complete tree.
  struct Parenthetical : TypedNode<NodeType::PARENTHETICAL> {
    PNode expression;
    Parenthetical(PNode expression_): expression(std::move(expression_)) {}
  };
  struct Array : TypedNode<NodeType::ARRAY> {
    std::vector<PNode> elements;
    Array(std::vector<PNode> &&elements_): elements(std::move(elements_)) {}
  };

  struct IdentifierAccess: TypedNode<NodeType::IDENTIFIER> {
    // We can access identifiers declared either in C++ or EDL
    enum class Kind { EDL, CPP } kind;
    std::variant<FullType *, jdi::definition *> type;
    Token name;

    IdentifierAccess(FullType *type, Token name): kind{Kind::EDL}, type{type}, name{name} {}
    IdentifierAccess(jdi::definition *type, Token name): kind{Kind::CPP}, type{type}, name{name} {}
  };

  struct Literal : TypedNode<NodeType::LITERAL> {
    ConstValue value;
    Literal(const Token &token): value{token} {}
  };
  
  struct IfStatement : TypedNode<NodeType::IF> {
    PNode condition;
    PNode true_branch, false_branch;

    IfStatement(PNode condition_, PNode true_branch_, PNode false_branch_): condition{std::move(condition_)},
        true_branch{std::move(true_branch_)}, false_branch{std::move(false_branch_)} {}
  };
  struct ForLoop : TypedNode<NodeType::FOR> {
    PNode assignment, condition, increment;
    PNode body;

    ForLoop(PNode assignment_, PNode condition_, PNode increment_, PNode body_): assignment{std::move(assignment_)},
        condition{std::move(condition_)}, increment{std::move(increment_)}, body{std::move(body_)} {}
  };
  struct WhileLoop : TypedNode<NodeType::WHILE> {
    PNode condition;
    PNode body;

    enum class Kind { WHILE, UNTIL, REPEAT } kind;

    WhileLoop(PNode condition_, PNode body_, Kind kind_): condition{std::move(condition_)},
        body{std::move(body_)}, kind{kind_} {}
  };
  struct DoLoop : TypedNode<NodeType::DO> {
    PNode body;
    PNode condition;
    bool is_until;

    DoLoop(PNode body_, PNode condition_, bool until): body{std::move(body_)}, condition{std::move(condition_)},
        is_until(until) {}
  };

  struct CaseStatement : TypedNode<NodeType::CASE> {
    PNode value;
    std::unique_ptr<AST::CodeBlock> statements;

    CaseStatement(PNode value, std::unique_ptr<AST::CodeBlock> statements): value{std::move(value)},
                                                                             statements{std::move(statements)} {}
  };

  struct DefaultStatement : TypedNode<NodeType::DEFAULT> {
    std::unique_ptr<AST::CodeBlock> statements;

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
  };

  struct ReturnStatement : TypedNode<NodeType::RETURN> {
    // Optional: the return value. Default: T()
    PNode expression;
    bool is_exit;

    ReturnStatement(PNode expression_, bool is_exit_): expression{std::move(expression_)}, is_exit{is_exit_} {}
  };
  struct BreakStatement : TypedNode<NodeType::BREAK> {
    // Optional: the number of nested loops to break out of (default = 1)
    PNode count;

    explicit BreakStatement(PNode count_): count{std::move(count_)} {}
  };
  struct ContinueStatement : TypedNode<NodeType::CONTINUE> {
    // Optional: the number of nested loops to continue past (default = 1)
    PNode count;

    explicit ContinueStatement(PNode count_): count{std::move(count_)} {}
  };
  struct WithStatement : TypedNode<NodeType::WITH> {
    PNode object;
    PNode body;

    WithStatement(PNode object_, PNode body_): object{std::move(object_)}, body{std::move(body_)} {}
  };

  enum class DeclaratorType {
    ABSTRACT, NON_ABSTRACT, MAYBE_ABSTRACT
  };

  struct Initializer;
  struct BraceOrParenInitializer;
  struct AssignmentInitializer;

  using InitializerNode = std::unique_ptr<Initializer>;
  using BraceOrParenInitNode = std::unique_ptr<BraceOrParenInitializer>;
  using AssignmentInitNode = std::unique_ptr<AssignmentInitializer>;

  struct BraceOrParenInitializer {
    enum class Kind { BRACE_INIT, DESIGNATED_INIT, PAREN_INIT } kind;
    std::vector<std::pair<std::string, InitializerNode>> values{};

    template <typename T>
    static BraceOrParenInitNode from(T&& value, Kind kind = Kind::BRACE_INIT) {
      return std::make_unique<BraceOrParenInitializer>(kind, std::forward<T>(value));
    }
  };

  struct AssignmentInitializer {
    enum class Kind { BRACE_INIT, EXPR } kind;
    std::variant<BraceOrParenInitNode, PNode> initializer{};

    explicit AssignmentInitializer(BraceOrParenInitNode init): kind{Kind::BRACE_INIT}, initializer{std::move(init)} {}
    explicit AssignmentInitializer(PNode expr): kind{Kind::EXPR}, initializer{std::move(expr)} {}

    template <typename T>
    static AssignmentInitNode from(T&& value) {
      return std::make_unique<AssignmentInitializer>(std::forward<T>(value));
    }
  };

  struct Initializer : TypedNode<NodeType::INITIALIZER> {
    enum class Kind { BRACE_INIT, ASSIGN_EXPR, PLACEMENT_NEW } kind;
    std::variant<BraceOrParenInitNode, AssignmentInitNode> initializer;
    bool is_variadic{};

    explicit Initializer(BraceOrParenInitNode init, bool is_variadic = false):
      kind{Kind::BRACE_INIT}, initializer{std::move(init)}, is_variadic{is_variadic} {}
    explicit Initializer(BraceOrParenInitializer init, bool is_variadic = false):
      Initializer(std::make_unique<BraceOrParenInitializer>(std::move(init)), is_variadic) {}
    explicit Initializer(AssignmentInitNode node, bool is_variadic = false):
      kind{Kind::ASSIGN_EXPR}, initializer{std::move(node)}, is_variadic{is_variadic} {}
    explicit Initializer(AssignmentInitializer node, bool is_variadic = false):
      Initializer(std::make_unique<AssignmentInitializer>(std::move(node)), is_variadic) {}
    explicit Initializer(PNode node, bool is_variadic = false):
      Initializer(std::make_unique<AssignmentInitializer>(std::move(node)), is_variadic) {}

    template <typename T>
    static InitializerNode from(T&& value, bool is_variadic = false) {
      // I don't have the energy to constrain T
      return std::make_unique<Initializer>(std::forward<T>(value), is_variadic);
    }
  };

  // New expression
  struct NewExpression : TypedNode<NodeType::NEW> {
    bool is_global;
    bool is_array;
    std::unique_ptr<Initializer> placement;
    FullType ft;
    std::unique_ptr<Initializer> initializer;

    NewExpression(bool is_global, bool is_array, std::unique_ptr<Initializer> placement, FullType type,
                  std::unique_ptr<Initializer> initializer):
      is_global{is_global}, is_array{is_array}, placement{std::move(placement)}, ft{std::move(type)},
      initializer{std::move(initializer)} {}
  };
  // Delete expression
  struct DeleteExpression : TypedNode<NodeType::DELETE> {
    bool is_global;
    bool is_array;
    PNode expression;

    DeleteExpression(bool is_global, bool is_array, PNode expression): is_global{is_global}, is_array{is_array},
                                                                       expression{std::move(expression)} {}
  };

  struct DeclarationStatement: TypedNode<NodeType::DECLARATION> {
    struct Declaration {
      std::unique_ptr<FullType> declarator;
      InitializerNode init;

      Declaration() noexcept = default;
      Declaration(FullType declarator, InitializerNode init):
        declarator{std::make_unique<FullType>(std::move(declarator))}, init{std::move(init)} {}
    };

    jdi::definition *def;
    std::vector<Declaration> declarations;

    DeclarationStatement(jdi::definition *type, std::vector<Declaration> declarations): def{type},
      declarations{std::move(declarations)} {}
  };

  // Used to adapt to current single-error syntax checking interface.
  ErrorCollector herr;
  // A lexed (tokenized) view of the code.
  const std::shared_ptr<Lexer> lexer;
  // The raw input code, owned by the lexer.
  const std::string &code;

  // Original ENIGMA code/synt bufs
  struct HackyTackyThingyThing {
    string code;
    string synt;
    unsigned int strc;
    varray<string> strs;
  } junkshit;

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
  void ExtractDeclarations(ParsedScope *destination_scope);

  // Pretty-prints this code to a stream with the given base indentation.
  // The caller is responsible for having already printed applicable function
  // declarations and opening braces, statements, etc, and for printing the
  // closing statements and braces afterward.
  void PrettyPrint(std::ofstream &of, int base_indent = 2) const;

  // Parses the given code, returning an AST*. The resulting AST* is never null.
  // If syntax errors were encountered, they are stored within the AST.
  static AST Parse(std::string code, const ParseContext *ctex);

  // Disallow copy. Our tokens point into our code.
  AST(const AST &) = delete;
  AST(AST &&other) = default;

 private:
  // When specified, emits code to apply to a specific instance.
  std::optional<int> apply_to_;
  // Constructs an AST from the code it will parse. Does not initiate parse.
  AST(std::string &&code_, const ParseContext *ctex):
      lexer(std::make_unique<Lexer>(std::move(code_), ctex, &herr)),
      code(lexer->GetCode()) {}
};

}  // namespace enigma::parsing

#endif  // ENIGMA_COMPILER_PARSING_AST_h
