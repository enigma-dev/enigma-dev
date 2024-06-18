/** Copyright (C) 2024 Fares Atef
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
using namespace enigma::parsing;

bool AST::Visitor::VisitIdentifierAccess(IdentifierAccess &node) { print(std::string(node.name.content)); }

bool AST::Visitor::VisitParenthetical(Parenthetical &node) {
  print("(");
  // assuming the idenetifier is the expression
  VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());  // after sometime AS<> can be removed
  print(")");
}

bool AST::Visitor::VisitUnaryPostfixExpression(UnaryPostfixExpression &node) {
  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.operand->As<Parenthetical>());
  }
  print(ToSymbol(node.operation));
}

bool AST::Visitor::VisitUnaryPrefixExpression(UnaryPrefixExpression &node) {
  print(ToSymbol(node.operation));
  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.operand->As<Parenthetical>());
  }
}

bool AST::Visitor::VisitDeleteExpression(DeleteExpression &node) {
  if (node.is_global) {
    print("::");
  }
  print("delete ");
  if (node.is_array) {
    print("[] ");
  }
  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.expression->As<Parenthetical>());
  }
}

bool AST::Visitor::VisitBreakStatement(BreakStatement &node) {
  print("break");
  if(node.count){
    print(" ");
    print(std::get<std::string>(node.count->As<Literal>()->value.value));
  }
}

bool AST::Visitor::VisitContinueStatement(ContinueStatement &node){
  print("continue");
  if(node.count){
    print(" ");
    print(std::get<std::string>(node.count->As<Literal>()->value.value));
  }
}
