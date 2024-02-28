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

#include "ast.h"
#include "parser.h"
#include "parser/collect_variables.h"  // collect_variables

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace enigma::parsing {
  
#define REGISTER(enum, name) [[fallthrough]]; case enum::name: res[(int) enum::name] = #name

bool AST::empty() const { return !root_; }

// Utility routine: Apply this AST to a specified instance.
void AST::ApplyTo(int instance_id) {
  apply_to_ = instance_id;
}

void AST::WriteCppToStream(std::ofstream &of, int base_indent) const {
  // TODO: Implement
  if (apply_to_) {
    of << std::string(base_indent, ' ') << "with (" << *apply_to_ << ") {\n";
    // print_to_file(lexer->GetContext(), junkshit.code, junkshit.synt,
    //               junkshit.strc, junkshit.strs, base_indent, of);
    of << std::string(base_indent, ' ') << "}\n";
  } else {
    // print_to_file(lexer->GetContext(), junkshit.code, junkshit.synt,
    //               junkshit.strc, junkshit.strs, base_indent, of);
  }
}

AST AST::Parse(std::string code, const ParseContext *ctex) {
  AST res(std::move(code), ctex);
  res.root_ = enigma::parsing::Parse(res.lexer.get(), &res.herr);
  return res;
}

void AST::ExtractDeclarations(ParsedScope *destination_scope) {
  std::cout << "collecting variables..." << std::flush;
  const ParseContext &ctex = lexer->GetContext();
  collect_variables(ctex.language_fe, this, destination_scope, ctex.script_names);
  std::cout << " done." << std::endl;
}

template<typename... SubNodes>
void AST::Node::RV(AST::Visitor &visitor, const SubNodes &...nodes) {
  (RVF(visitor, nodes), ...);
}
void AST::Node::RVF(AST::Visitor &visitor, const PNode &single_node) {
  if (single_node) single_node->RecurusiveVisit(visitor);
}
void AST::Node::RVF(AST::Visitor &visitor,
                    const std::vector<PNode> &node_list) {
  for (const PNode &node : node_list) node->RecurusiveVisit(visitor);
}

void AST::CodeBlock::RecursiveSubVisit(AST::Visitor &visitor) {
  RV(visitor, statements);
}
void AST::BinaryExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, left, right);
}
void AST::FunctionCallExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, function, arguments);
}
void AST::UnaryPrefixExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, operand);
}
void AST::UnaryPostfixExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, operand);
}
void AST::TernaryExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, condition, true_expression, false_expression);
}
void AST::SizeofExpression::RecursiveSubVisit(Visitor &visitor) {
  if (std::holds_alternative<PNode>(argument))
    RV(visitor, std::get<PNode>(argument));
}
void AST::AlignofExpression::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Nothing to visit.
}
void AST::CastExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expr);
}
void AST::Parenthetical::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::Array::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, elements);
}
void AST::IdentifierAccess::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Varname token that appeared in code; it's a leaf.
}
void AST::Literal::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Literal in the middle of code; also a leaf.
}
void AST::IfStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, condition, true_branch, false_branch);
}
void AST::ForLoop::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, assignment, condition, increment, body);
}
void AST::WhileLoop::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, condition, body);
}
void AST::DoLoop::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, body, condition);
}
void AST::CaseStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, value);
}
void AST::DefaultStatement::RecursiveSubVisit(Visitor &visitor) {
  (void) visitor;  // Label must preceed a statement, but does not own it
}
void AST::SwitchStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::ReturnStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::BreakStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, count);
}
void AST::ContinueStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, count);
}
void AST::WithStatement::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, object, body);
}
void AST::Initializer::RecursiveSubVisit(Visitor &visitor) {
  
}
void AST::NewExpression::RecursiveSubVisit(Visitor &visitor) {
  
}
void AST::DeleteExpression::RecursiveSubVisit(Visitor &visitor) {
  RV(visitor, expression);
}
void AST::DeclarationStatement::RecursiveSubVisit(Visitor &visitor) {
  
}

const std::vector<std::string> AST::NodesNames = [](){
  std::vector<std::string> res;
  int nsize = static_cast<int>(NodeType::INITIALIZER)+1;
  res.resize(nsize);

  switch (NodeType::ERROR) {
    case NodeType::ERROR: res[(int)NodeType::ERROR] = "ERROR";
    REGISTER(NodeType, BLOCK);
    REGISTER(NodeType, BINARY_EXPRESSION);
    REGISTER(NodeType, UNARY_PREFIX_EXPRESSION);
    REGISTER(NodeType, UNARY_POSTFIX_EXPRESSION);
    REGISTER(NodeType, TERNARY_EXPRESSION);
    REGISTER(NodeType, SIZEOF);
    REGISTER(NodeType, ALIGNOF);
    REGISTER(NodeType, CAST);
    REGISTER(NodeType, NEW);
    REGISTER(NodeType, DELETE);
    REGISTER(NodeType, PARENTHETICAL);
    REGISTER(NodeType, ARRAY);
    REGISTER(NodeType, IDENTIFIER);
    REGISTER(NodeType, SCOPE_ACCESS);
    REGISTER(NodeType, LITERAL);
    REGISTER(NodeType, FUNCTION_CALL);
    REGISTER(NodeType, IF);
    REGISTER(NodeType, FOR);
    REGISTER(NodeType, WHILE);
    REGISTER(NodeType, DO);
    REGISTER(NodeType, WITH);
    REGISTER(NodeType, REPEAT);
    REGISTER(NodeType, SWITCH);
    REGISTER(NodeType, CASE);
    REGISTER(NodeType, DEFAULT);
    REGISTER(NodeType, BREAK);
    REGISTER(NodeType, CONTINUE);
    REGISTER(NodeType, RETURN);
    REGISTER(NodeType, DECLARATION);
    REGISTER(NodeType, INITIALIZER);
  }
  return res;
}();

const std::vector<std::string> AST::DeclarationStatement::StorageNames = [](){
  std::vector<std::string> res;
  int stsize = static_cast<int>(StorageClass::GLOBAL)+1;
  res.resize(stsize);

  switch (StorageClass::TEMPORARY) {
    case StorageClass::TEMPORARY: res[(int)StorageClass::TEMPORARY] = "TEMPORARY";
    REGISTER(StorageClass, LOCAL);
    REGISTER(StorageClass, GLOBAL);
  }
  return res;
}();

const std::vector<std::string> AST::CastExpression::KindNames = [](){
  std::vector<std::string> res;
  int ksize = static_cast<int>(Kind::FUNCTIONAL)+1;
  res.resize(ksize);

  switch (Kind::C_STYLE) {
    case Kind::C_STYLE: res[(int)Kind::C_STYLE] = "C_STYLE";
    REGISTER(Kind, STATIC);
    REGISTER(Kind, DYNAMIC);
    REGISTER(Kind, REINTERPRET);
    REGISTER(Kind, CONST);
    REGISTER(Kind, FUNCTIONAL);
  }
  return res;
}();

std::string AST::NodeToString(NodeType nt){
  return NodesNames[(int)nt];
}

std::string AST::DeclarationStatement::StorageToString(StorageClass st){
  return StorageNames[(int)st];
}

std::string AST::CastExpression::KindToString(Kind k){
  return KindNames[(int)k];
}

}  // namespace enigma::parsing
