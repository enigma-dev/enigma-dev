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

bool AST::Visitor::VisitLiteral(Literal &node) { print(std::get<std::string>(node.value.value)); }

bool AST::Visitor::VisitParenthetical(Parenthetical &node) {
  print("(");

  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.expression->As<Literal>());
  } else if (node.expression->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.expression->As<BinaryExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.expression->As<UnaryPrefixExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.expression->As<UnaryPostfixExpression>());
  }

  print(")");
}

bool AST::Visitor::VisitUnaryPostfixExpression(UnaryPostfixExpression &node) {
  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.operand->As<Parenthetical>());
  } else if (node.operand->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.operand->As<Literal>());
  } else if (node.operand->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.operand->As<BinaryExpression>());
  }

  print(ToSymbol(node.operation));
}

bool AST::Visitor::VisitUnaryPrefixExpression(UnaryPrefixExpression &node) {
  print(ToSymbol(node.operation));

  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.operand->As<Parenthetical>());
  } else if (node.operand->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.operand->As<Literal>());
  } else if (node.operand->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.operand->As<BinaryExpression>());
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

  if (node.count) {
    print(" ");
    VisitLiteral(*node.count->As<Literal>());
  }
}

bool AST::Visitor::VisitContinueStatement(ContinueStatement &node) {
  print("continue");

  if (node.count) {
    print(" ");
    VisitLiteral(*node.count->As<Literal>());
  }
}

bool AST::Visitor::VisitBinaryExpression(BinaryExpression &node) {
  if (node.left->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.left->As<IdentifierAccess>());
  } else if (node.left->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.left->As<Parenthetical>());
  } else if (node.left->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.left->As<Literal>());
  } else if (node.left->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.left->As<BinaryExpression>());
  } else if (node.left->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.left->As<UnaryPrefixExpression>());
  } else if (node.left->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.left->As<UnaryPostfixExpression>());
  }

  print(" ");
  print(ToSymbol(node.operation));
  print(" ");

  if (node.right->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.right->As<IdentifierAccess>());
  } else if (node.right->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.right->As<Parenthetical>());
  } else if (node.right->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.right->As<Literal>());
  } else if (node.right->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.right->As<BinaryExpression>());
  } else if (node.right->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.right->As<UnaryPrefixExpression>());
  } else if (node.right->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.right->As<UnaryPostfixExpression>());
  }

  if (node.operation == TT_BEGINBRACKET) {
    print("]");
  }
}

bool AST::Visitor::VisitFunctionCallExpression(FunctionCallExpression &node) {
  if (node.function->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.function->As<IdentifierAccess>());
  } else if (node.function->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.function->As<BinaryExpression>());
  }

  print("(");

  for (auto &arg : node.arguments) {
    if (arg->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*arg->As<IdentifierAccess>());
    } else if (arg->type == AST::NodeType::LITERAL) {
      VisitLiteral(*arg->As<Literal>());
    } else if (arg->type == AST::NodeType::PARENTHETICAL) {
      VisitParenthetical(*arg->As<Parenthetical>());
    } else if (arg->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*arg->As<BinaryExpression>());
    } else if (arg->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*arg->As<UnaryPrefixExpression>());
    } else if (arg->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*arg->As<UnaryPostfixExpression>());
    }

    if (&arg != &node.arguments.back()) {
      print(", ");
    }
  }

  print(")");
}
