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

#include <JDI/src/System/builtins.h>
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
  } else if (node.expression->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.expression->As<FunctionCallExpression>());
  } else if (node.expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.expression->As<TernaryExpression>());
  } else if (node.expression->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.expression->As<SizeofExpression>());
  } else if (node.expression->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.expression->As<AlignofExpression>());
  } else if (node.expression->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.expression->As<CastExpression>());
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
  } else if (node.operand->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.operand->As<UnaryPrefixExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.operand->As<UnaryPostfixExpression>());
  } else if (node.operand->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.operand->As<FunctionCallExpression>());
  } else if (node.operand->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.operand->As<TernaryExpression>());
  }

  print(ToSymbol(node.operation));
}

bool AST::Visitor::VisitUnaryPrefixExpression(UnaryPrefixExpression &node) {
  print(ToSymbol(node.operation));

  if (node.operation == TT_STAR && node.operand->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.operand->As<Parenthetical>());
  } else if (node.operand->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.operand->As<Literal>());
  } else if (node.operand->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.operand->As<BinaryExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.operand->As<UnaryPrefixExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.operand->As<UnaryPostfixExpression>());
  } else if (node.operand->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.operand->As<FunctionCallExpression>());
  } else if (node.operand->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.operand->As<TernaryExpression>());
  }

  if (node.operation == TT_STAR && node.operand->type != AST::NodeType::PARENTHETICAL) {
    print(")");
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
  } else if (node.left->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.left->As<FunctionCallExpression>());
  } else if (node.left->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.left->As<TernaryExpression>());
  } else if (node.left->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.left->As<SizeofExpression>());
  } else if (node.left->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.left->As<AlignofExpression>());
  } else if (node.left->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.left->As<CastExpression>());
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
  } else if (node.right->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.right->As<FunctionCallExpression>());
  } else if (node.right->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.right->As<TernaryExpression>());
  } else if (node.right->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.right->As<SizeofExpression>());
  } else if (node.right->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.right->As<AlignofExpression>());
  } else if (node.right->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.right->As<CastExpression>());
  } else if (node.right->type == AST::NodeType::ARRAY) {
    VisitArray(*node.right->As<Array>());
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
    } else if (arg->type == AST::NodeType::FUNCTION_CALL) {
      VisitFunctionCallExpression(*arg->As<FunctionCallExpression>());
    } else if (arg->type == AST::NodeType::SIZEOF) {
      VisitSizeofExpression(*arg->As<SizeofExpression>());
    } else if (arg->type == AST::NodeType::ALIGNOF) {
      VisitAlignofExpression(*arg->As<AlignofExpression>());
    } else if (arg->type == AST::NodeType::CAST) {
      VisitCastExpression(*arg->As<CastExpression>());
    } else if (arg->type == AST::NodeType::ARRAY) {
      VisitArray(*arg->As<Array>());
    }

    if (&arg != &node.arguments.back()) {
      print(", ");
    }
  }

  print(")");
}

bool AST::Visitor::VisitTernaryExpression(TernaryExpression &node) {
  if (node.condition->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.condition->As<IdentifierAccess>());
  } else if (node.condition->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.condition->As<Literal>());
  } else if (node.condition->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.condition->As<Parenthetical>());
  } else if (node.condition->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.condition->As<BinaryExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.condition->As<UnaryPrefixExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.condition->As<UnaryPostfixExpression>());
  } else if (node.condition->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.condition->As<FunctionCallExpression>());
  } else if (node.condition->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.condition->As<SizeofExpression>());
  } else if (node.condition->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.condition->As<AlignofExpression>());
  } else if (node.condition->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.condition->As<CastExpression>());
  }

  print(" ? ");

  if (node.true_expression->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.true_expression->As<IdentifierAccess>());
  } else if (node.true_expression->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.true_expression->As<Literal>());
  } else if (node.true_expression->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.true_expression->As<Parenthetical>());
  } else if (node.true_expression->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.true_expression->As<BinaryExpression>());
  } else if (node.true_expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.true_expression->As<UnaryPrefixExpression>());
  } else if (node.true_expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.true_expression->As<UnaryPostfixExpression>());
  } else if (node.true_expression->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.true_expression->As<FunctionCallExpression>());
  } else if (node.true_expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.true_expression->As<TernaryExpression>());
  } else if (node.true_expression->type == AST::NodeType::DELETE) {
    VisitDeleteExpression(*node.true_expression->As<DeleteExpression>());
  } else if (node.true_expression->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.true_expression->As<SizeofExpression>());
  } else if (node.true_expression->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.true_expression->As<AlignofExpression>());
  } else if (node.true_expression->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.true_expression->As<CastExpression>());
  } else if (node.true_expression->type == AST::NodeType::ARRAY) {
    VisitArray(*node.true_expression->As<Array>());
  }
  // else new expression

  print(" : ");

  if (node.false_expression->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.false_expression->As<IdentifierAccess>());
  } else if (node.false_expression->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.false_expression->As<Literal>());
  } else if (node.false_expression->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.false_expression->As<Parenthetical>());
  } else if (node.false_expression->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.false_expression->As<BinaryExpression>());
  } else if (node.false_expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.false_expression->As<UnaryPrefixExpression>());
  } else if (node.false_expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.false_expression->As<UnaryPostfixExpression>());
  } else if (node.false_expression->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.false_expression->As<FunctionCallExpression>());
  } else if (node.false_expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.false_expression->As<TernaryExpression>());
  } else if (node.false_expression->type == AST::NodeType::DELETE) {
    VisitDeleteExpression(*node.false_expression->As<DeleteExpression>());
  } else if (node.false_expression->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.false_expression->As<SizeofExpression>());
  } else if (node.false_expression->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.false_expression->As<AlignofExpression>());
  } else if (node.false_expression->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.false_expression->As<CastExpression>());
  } else if (node.false_expression->type == AST::NodeType::ARRAY) {
    VisitArray(*node.false_expression->As<Array>());
  }
  // else new expression
}

bool AST::Visitor::VisitReturnStatement(ReturnStatement &node) {
  print("return ");

  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.expression->As<Literal>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.expression->As<Parenthetical>());
  } else if (node.expression->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.expression->As<BinaryExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.expression->As<UnaryPrefixExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.expression->As<UnaryPostfixExpression>());
  } else if (node.expression->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.expression->As<FunctionCallExpression>());
  } else if (node.expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.expression->As<TernaryExpression>());
  } else if (node.expression->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.expression->As<SizeofExpression>());
  } else if (node.expression->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.expression->As<AlignofExpression>());
  } else if (node.expression->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.expression->As<CastExpression>());
  } else if (node.expression->type == AST::NodeType::ARRAY) {
    VisitArray(*node.expression->As<Array>());
  }
}

bool AST::Visitor::VisitFullType(FullType &ft) {
  std::vector<std::size_t> flags_values = {
      jdi::builtin_flag__const->value,    jdi::builtin_flag__static->value,       jdi::builtin_flag__volatile->value,
      jdi::builtin_flag__mutable->value,  jdi::builtin_flag__register->value,     jdi::builtin_flag__inline->value,
      jdi::builtin_flag__Complex->value,  jdi::builtin_flag__unsigned->value,     jdi::builtin_flag__signed->value,
      jdi::builtin_flag__short->value,    jdi::builtin_flag__long->value,         jdi::builtin_flag__long_long->value,
      jdi::builtin_flag__restrict->value, jdi::builtin_typeflag__override->value, jdi::builtin_typeflag__final->value};

  std::vector<std::size_t> flags_masks = {
      jdi::builtin_flag__const->mask,    jdi::builtin_flag__static->mask,       jdi::builtin_flag__volatile->mask,
      jdi::builtin_flag__mutable->mask,  jdi::builtin_flag__register->mask,     jdi::builtin_flag__inline->mask,
      jdi::builtin_flag__Complex->mask,  jdi::builtin_flag__unsigned->mask,     jdi::builtin_flag__signed->mask,
      jdi::builtin_flag__short->mask,    jdi::builtin_flag__long->mask,         jdi::builtin_flag__long_long->mask,
      jdi::builtin_flag__restrict->mask, jdi::builtin_typeflag__override->mask, jdi::builtin_typeflag__final->mask};

  std::vector<std::string> flags_names = {"const",  "static",    "volatile", "mutable",  "register",
                                          "inline", "complex",   "unsigned", "signed",   "short",
                                          "long",   "long long", "restrict", "override", "final"};

  for (std::size_t i = 0; i < flags_values.size(); i++) {
    if ((ft.flags & flags_masks[i]) == flags_values[i]) {
      print(flags_names[i]);
      print(" ");
    }
  }

  print(ft.def->name);
  print(" ");

  jdi::ref_stack stack;
  ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();

  std::string ref;
  bool flag = false;

  for (auto it = first; it != stack.end(); it++) {
    if (it->type == jdi::ref_stack::RT_POINTERTO) {
      flag = true;
      ref = '*' + ref;
    } else if (it->type == jdi::ref_stack::RT_REFERENCE) {
      flag = true;
      ref = '&' + ref;
    } else {
      if (it->type == jdi::ref_stack::RT_ARRAYBOUND) {
        if (flag) {
          ref = '(' + ref + ')';
        }

        std::size_t arr_size = it->arraysize();
        if (arr_size != 0) {
          ref += '[' + std::to_string(arr_size) + ']';
        } else {
          ref += "[]";
        }
      }

      // TODO: RT_MEMBER_POINTER

      flag = false;
    }

    if (it == first) {
      std::string name = std::string(ft.decl.name.content);
      if (name != "") {
        ref += name;
      }
    }
  }

  print(ref);
}

bool AST::Visitor::VisitSizeofExpression(SizeofExpression &node) {
  print("sizeof");

  if (node.kind == AST::SizeofExpression::Kind::EXPR) {
    print(" ");

    auto &arg = std::get<AST::PNode>(node.argument);
    if (arg->type == AST::NodeType::LITERAL) {
      VisitLiteral(*arg->As<AST::Literal>());
    } else if (arg->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*arg->As<AST::IdentifierAccess>());
    }
  } else if (node.kind == AST::SizeofExpression::Kind::VARIADIC) {
    print("...(");

    std::string arg = std::get<std::string>(node.argument);
    print(arg + ")");
  } else {
    print("(");

    FullType &ft = std::get<FullType>(node.argument);
    VisitFullType(ft);

    print(")");
  }
}

bool AST::Visitor::VisitAlignofExpression(AlignofExpression &node) {
  print("alignof(");

  VisitFullType(node.ft);

  print(")");
}

bool AST::Visitor::VisitCastExpression(CastExpression &node) {
  if (node.kind == AST::CastExpression::Kind::FUNCTIONAL) {
    VisitFullType(node.ft);
    print("(");

    if (node.expr->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*node.expr->As<BinaryExpression>());
    } else if (node.expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*node.expr->As<UnaryPrefixExpression>());
    } else if (node.expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*node.expr->As<UnaryPostfixExpression>());
    }

    print(")");
  } else if (node.kind == AST::CastExpression::Kind::C_STYLE) {
    print("(");
    VisitFullType(node.ft);
    print(")");

    if (node.expr->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*node.expr->As<BinaryExpression>());
    } else if (node.expr->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*node.expr->As<IdentifierAccess>());
    } else if (node.expr->type == AST::NodeType::PARENTHETICAL) {
      VisitParenthetical(*node.expr->As<Parenthetical>());
    } else if (node.expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*node.expr->As<UnaryPrefixExpression>());
    } else if (node.expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*node.expr->As<UnaryPostfixExpression>());
    }
  } else {
    if (node.kind == AST::CastExpression::Kind::STATIC) {
      print("static_cast<");
    } else if (node.kind == AST::CastExpression::Kind::DYNAMIC) {
      print("dynamic_cast<");
    } else if (node.kind == AST::CastExpression::Kind::CONST) {
      print("const_cast<");
    } else if (node.kind == AST::CastExpression::Kind::REINTERPRET) {
      print("reinterpret_cast<");
    }

    VisitFullType(node.ft);
    print(">(");

    if (node.expr->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*node.expr->As<BinaryExpression>());
    } else if (node.expr->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*node.expr->As<IdentifierAccess>());
    } else if (node.expr->type == AST::NodeType::PARENTHETICAL) {
      VisitParenthetical(*node.expr->As<Parenthetical>());
    } else if (node.expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*node.expr->As<UnaryPrefixExpression>());
    } else if (node.expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*node.expr->As<UnaryPostfixExpression>());
    }

    print(")");
  }
}

bool AST::Visitor::VisitArray(Array &node) {
  print("[");

  if (node.elements[0]->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.elements[0]->As<IdentifierAccess>());
  } else if (node.elements[0]->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.elements[0]->As<Literal>());
  } else if (node.elements[0]->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.elements[0]->As<Parenthetical>());
  } else if (node.elements[0]->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.elements[0]->As<BinaryExpression>());
  } else if (node.elements[0]->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.elements[0]->As<UnaryPrefixExpression>());
  } else if (node.elements[0]->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.elements[0]->As<UnaryPostfixExpression>());
  } else if (node.elements[0]->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.elements[0]->As<FunctionCallExpression>());
  } else if (node.elements[0]->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.elements[0]->As<SizeofExpression>());
  } else if (node.elements[0]->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.elements[0]->As<AlignofExpression>());
  } else if (node.elements[0]->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.elements[0]->As<CastExpression>());
  } else if (node.elements[0]->type == AST::NodeType::ARRAY) {
    VisitArray(*node.elements[0]->As<Array>());
  }

  print("]");
}

bool AST::Visitor::VisitBraceOrParenInitializer(BraceOrParenInitializer &node) {
  if (node.kind == BraceOrParenInitializer::Kind::PAREN_INIT) {
    print("(");
  } else {
    print("{");
  }

  for (auto &val : node.values) {
    if (node.kind == BraceOrParenInitializer::Kind::DESIGNATED_INIT) {
      print(".");
    }

    if (val.first != "") {
      print(val.first);
      print("=");
    }

    VisitInitializer(*val.second);

    if (&val != &node.values.back()) {
      print(", ");
    }
  }

  if (node.kind == BraceOrParenInitializer::Kind::PAREN_INIT) {
    print(")");
  } else {
    print("}");
  }
}

bool AST::Visitor::VisitAssignmentInitializer(AssignmentInitializer &node) {
  if (node.kind == AssignmentInitializer::Kind::BRACE_INIT) {
    VisitBraceOrParenInitializer(*std::get<BraceOrParenInitNode>(node.initializer));
  } else {
    auto &expr = std::get<AST::PNode>(node.initializer);

    if (expr->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*expr->As<AST::IdentifierAccess>());
    } else if (expr->type == AST::NodeType::LITERAL) {
      VisitLiteral(*expr->As<AST::Literal>());
    } else if (expr->type == AST::NodeType::PARENTHETICAL) {
      VisitParenthetical(*expr->As<AST::Parenthetical>());
    } else if (expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*expr->As<AST::UnaryPostfixExpression>());
    } else if (expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*expr->As<AST::UnaryPrefixExpression>());
    } else if (expr->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*expr->As<AST::BinaryExpression>());
    } else if (expr->type == AST::NodeType::FUNCTION_CALL) {
      VisitFunctionCallExpression(*expr->As<FunctionCallExpression>());
    } else if (expr->type == AST::NodeType::SIZEOF) {
      VisitSizeofExpression(*expr->As<SizeofExpression>());
    } else if (expr->type == AST::NodeType::ALIGNOF) {
      VisitAlignofExpression(*expr->As<AlignofExpression>());
    } else if (expr->type == AST::NodeType::CAST) {
      VisitCastExpression(*expr->As<CastExpression>());
    } else if (expr->type == AST::NodeType::ARRAY) {
      VisitArray(*expr->As<Array>());
    }
  }
}

bool AST::Visitor::VisitInitializer(Initializer &node) {
  if (node.kind == Initializer::Kind::BRACE_INIT || node.kind == Initializer::Kind::PLACEMENT_NEW) {
    auto &init = std::get<BraceOrParenInitNode>(node.initializer);
    VisitBraceOrParenInitializer(*init);
  } else if (node.kind == Initializer::Kind::ASSIGN_EXPR) {
    auto &init = std::get<AssignmentInitNode>(node.initializer);
    VisitAssignmentInitializer(*init);
  }

  if (node.is_variadic) {
    print("...");
  }
}

bool AST::Visitor::VisitNewExpression(NewExpression &node) {
  if (node.is_global) {
    print("::");
  }

  print("new ");

  if (node.placement) {
    VisitInitializer(*node.placement);
    print(" ");
  }

  print("(");
  VisitFullType(node.ft);
  print(")");

  if (node.initializer) {
    VisitInitializer(*node.initializer);
  }
}
