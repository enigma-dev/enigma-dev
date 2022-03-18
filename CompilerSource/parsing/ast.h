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

#include "error_reporting.h"
#include "lexer.h"
#include "tokens.h"
#include "darray.h"

#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

struct ParsedScope;  // object_storage.h

namespace enigma::parsing {

class AST {
 public:
  enum class NodeType {
    ERROR = 0,
    BLOCK = 1,
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    TERNARY_EXPRESSION,
    PARENTHETICAL, ARRAY,
    IDENTIFIER, LITERAL, FUNCTION_CALL,
    IF, FOR, WHILE, DO, WITH, REPEAT,
    BREAK, CONTINUE, RETURN,
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

  // Simple block of code, containing zero or more statements.
  // The root node of any piece of code will be a block node.
  struct CodeBlock : TypedNode<NodeType::BLOCK> {
    // Individual statements or 
    std::vector<PNode> statements;
  };
  // Binary expressions; generally top-level will be "foo = expression"
  struct BinaryExpression : TypedNode<NodeType::BINARY_EXPRESSION> {
    PNode left, right;
    TokenType operation;
    BinaryExpression(PNode left_, PNode right_, TokenType operation_):
        left(std::move(left_)), right(std::move(right_)),
        operation(operation_) {}
  };
  // Unary expressions; generally top-level will be "++varname"
  struct UnaryExpression : TypedNode<NodeType::UNARY_EXPRESSION> {
    PNode operand;
    TokenType operation;
    UnaryExpression(PNode operand_, TokenType operation_):
        operand(std::move(operand_)), operation(operation_) {}
  };
  // Ternary expression; the only one is ?:
  struct TernaryExpression : TypedNode<NodeType::TERNARY_EXPRESSION> {
    PNode condition;
    PNode true_expression;
    PNode false_expression;
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
  
  struct Variable : TypedNode<NodeType::IDENTIFIER> {
    std::string name;
  };
  
  struct IfStatement : TypedNode<NodeType::IF> {
    PNode condition;
    PNode true_branch, false_branch;
  };
  struct ForLoop : TypedNode<NodeType::FOR> {
    PNode assignment, condition, increment;
    PNode body;
  };
  struct WhileLoop : TypedNode<NodeType::WHILE> {
    PNode condition;
    PNode body;
    bool is_until;

    WhileLoop(bool until): is_until(until) {}
  };
  struct DoLoop : TypedNode<NodeType::DO> {
    PNode body;
    PNode condition;
    bool is_until;

    DoLoop(bool until): is_until(until) {}
  };
  struct ReturnStatement : TypedNode<NodeType::RETURN> {
    // Optional: the return value. Default: T()
    PNode expression;
  };
  struct BreakStatement : TypedNode<NodeType::BREAK> {
    // Optional: the number of nested loops to break out of (default = 1)
    PNode count;
  };
  struct ContinueStatement : TypedNode<NodeType::CONTINUE> {
    // Optional: the number of nested loops to continue past (default = 1)
    PNode count;
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
