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

bool AST::Visitor::VisitIdentifierAccess(IdentifierAccess &node) {
  print(std::string(node.name.content));
  return true;
}

bool AST::Visitor::VisitLiteral(Literal &node) {
  std::string value = std::get<std::string>(node.value.value);
  if (value == "'\n'") {
    print("'\\n'");
  } else if (value == "'\t'") {
    print("'\\t'");
  } else if (value == "'\v'") {
    print("'\\v'");
  } else if (value == "'\b'") {
    print("'\\b'");
  } else if (value == "'\r'") {
    print("'\\r'");
  } else if (value == "'\f'") {
    print("'\\f'");
  } else if (value == "'\a'") {
    print("'\\a'");
  } else if (value == "'\\'") {
    print("'\\\'");
  } else if (value == "'\''") {
    print("'\\''");
  } else if (value == "'\"'") {
    print("'\\\"'");
  } else if (value == "'\?'") {
    print("'\\?'");
  } else {
    print(value);
  }
  return true;
}

bool AST::Visitor::VisitParenthetical(Parenthetical &node) {
  bool printed = false;
  print("(");

  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.expression->As<Literal>());
  } else if (node.expression->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.expression->As<BinaryExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.expression->As<UnaryPrefixExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.expression->As<UnaryPostfixExpression>());
  } else if (node.expression->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.expression->As<FunctionCallExpression>());
  } else if (node.expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.expression->As<TernaryExpression>());
  } else if (node.expression->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.expression->As<SizeofExpression>());
  } else if (node.expression->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.expression->As<AlignofExpression>());
  } else if (node.expression->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.expression->As<CastExpression>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.expression->As<Parenthetical>());
  } else if (node.expression->type == AST::NodeType::ARRAY) {
    printed = VisitArray(*node.expression->As<Array>());
  }

  if (!printed) return false;
  print(")");

  return true;
}

bool AST::Visitor::VisitUnaryPostfixExpression(UnaryPostfixExpression &node) {
  bool printed = false;

  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.operand->As<Parenthetical>());
  } else if (node.operand->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.operand->As<Literal>());
  } else if (node.operand->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.operand->As<BinaryExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.operand->As<UnaryPrefixExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.operand->As<UnaryPostfixExpression>());
  } else if (node.operand->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.operand->As<FunctionCallExpression>());
  } else if (node.operand->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.operand->As<TernaryExpression>());
  }

  if (!printed) return false;
  print(ToSymbol(node.operation));

  return true;
}

bool AST::Visitor::VisitUnaryPrefixExpression(UnaryPrefixExpression &node) {
  bool printed = false;
  print(ToSymbol(node.operation));

  if (node.operation == TT_STAR && node.operand->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.operand->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.operand->As<IdentifierAccess>());
  } else if (node.operand->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.operand->As<Parenthetical>());
  } else if (node.operand->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.operand->As<Literal>());
  } else if (node.operand->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.operand->As<BinaryExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.operand->As<UnaryPrefixExpression>());
  } else if (node.operand->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.operand->As<UnaryPostfixExpression>());
  } else if (node.operand->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.operand->As<FunctionCallExpression>());
  } else if (node.operand->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.operand->As<TernaryExpression>());
  }

  if (!printed) return false;
  if (node.operation == TT_STAR && node.operand->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }

  return true;
}

bool AST::Visitor::VisitDeleteExpression(DeleteExpression &node) {
  if (node.is_global) {
    print("::");
  }
  print("delete ");
  if (node.is_array) {
    print("[] ");
  }

  bool printed = false;
  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.expression->As<Parenthetical>());
  }

  return printed;
}

bool AST::Visitor::VisitBreakStatement(BreakStatement &node) {
  print("break");

  bool printed = false;
  if (node.count) {
    print(" ");
    printed = VisitLiteral(*node.count->As<Literal>());
    if (!printed) return false;
  }

  return true;
}

bool AST::Visitor::VisitContinueStatement(ContinueStatement &node) {
  print("continue");

  bool printed = false;
  if (node.count) {
    print(" ");
    printed = VisitLiteral(*node.count->As<Literal>());
    if (!printed) return false;
  }

  return true;
}

bool AST::Visitor::VisitBinaryExpression(BinaryExpression &node) {
  bool printed = false;

  if (node.left->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.left->As<IdentifierAccess>());
  } else if (node.left->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.left->As<Parenthetical>());
  } else if (node.left->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.left->As<Literal>());
  } else if (node.left->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.left->As<BinaryExpression>());
  } else if (node.left->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.left->As<UnaryPrefixExpression>());
  } else if (node.left->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.left->As<UnaryPostfixExpression>());
  } else if (node.left->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.left->As<FunctionCallExpression>());
  } else if (node.left->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.left->As<TernaryExpression>());
  } else if (node.left->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.left->As<SizeofExpression>());
  } else if (node.left->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.left->As<AlignofExpression>());
  } else if (node.left->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.left->As<CastExpression>());
  }

  if (!printed) return false;
  print(" " + node.operation.token + " ");

  if (node.right->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.right->As<IdentifierAccess>());
  } else if (node.right->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.right->As<Parenthetical>());
  } else if (node.right->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.right->As<Literal>());
  } else if (node.right->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.right->As<BinaryExpression>());
  } else if (node.right->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.right->As<UnaryPrefixExpression>());
  } else if (node.right->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.right->As<UnaryPostfixExpression>());
  } else if (node.right->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.right->As<FunctionCallExpression>());
  } else if (node.right->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.right->As<TernaryExpression>());
  } else if (node.right->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.right->As<SizeofExpression>());
  } else if (node.right->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.right->As<AlignofExpression>());
  } else if (node.right->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.right->As<CastExpression>());
  } else if (node.right->type == AST::NodeType::ARRAY) {
    printed = VisitArray(*node.right->As<Array>());
  } else if (node.right->type == AST::NodeType::NEW) {
    printed = VisitNewExpression(*node.right->As<NewExpression>());
  }

  if (!printed) return false;
  if (node.operation.type == TT_BEGINBRACKET) {
    print("]");
  }

  return true;
}

bool AST::Visitor::VisitFunctionCallExpression(FunctionCallExpression &node) {
  bool printed = false;
  if (node.function->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.function->As<IdentifierAccess>());
  } else if (node.function->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.function->As<BinaryExpression>());
  }

  if (!printed) return false;
  print("(");

  for (auto &arg : node.arguments) {
    if (arg->type == AST::NodeType::IDENTIFIER) {
      printed = VisitIdentifierAccess(*arg->As<IdentifierAccess>());
    } else if (arg->type == AST::NodeType::LITERAL) {
      printed = VisitLiteral(*arg->As<Literal>());
    } else if (arg->type == AST::NodeType::PARENTHETICAL) {
      printed = VisitParenthetical(*arg->As<Parenthetical>());
    } else if (arg->type == AST::NodeType::BINARY_EXPRESSION) {
      printed = VisitBinaryExpression(*arg->As<BinaryExpression>());
    } else if (arg->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      printed = VisitUnaryPrefixExpression(*arg->As<UnaryPrefixExpression>());
    } else if (arg->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      printed = VisitUnaryPostfixExpression(*arg->As<UnaryPostfixExpression>());
    } else if (arg->type == AST::NodeType::FUNCTION_CALL) {
      printed = VisitFunctionCallExpression(*arg->As<FunctionCallExpression>());
    } else if (arg->type == AST::NodeType::SIZEOF) {
      printed = VisitSizeofExpression(*arg->As<SizeofExpression>());
    } else if (arg->type == AST::NodeType::ALIGNOF) {
      printed = VisitAlignofExpression(*arg->As<AlignofExpression>());
    } else if (arg->type == AST::NodeType::CAST) {
      printed = VisitCastExpression(*arg->As<CastExpression>());
    } else if (arg->type == AST::NodeType::ARRAY) {
      printed = VisitArray(*arg->As<Array>());
    }

    if (!printed) return false;
    if (&arg != &node.arguments.back()) {
      print(", ");
    }
  }

  print(")");
  return true;
}

bool AST::Visitor::VisitTernaryExpression(TernaryExpression &node) {
  bool printed = false;

  printed = VisitCondition(node.condition);

  if (!printed) return false;
  print(" ? ");

  if (node.true_expression->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.true_expression->As<IdentifierAccess>());
  } else if (node.true_expression->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.true_expression->As<Literal>());
  } else if (node.true_expression->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.true_expression->As<Parenthetical>());
  } else if (node.true_expression->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.true_expression->As<BinaryExpression>());
  } else if (node.true_expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.true_expression->As<UnaryPrefixExpression>());
  } else if (node.true_expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.true_expression->As<UnaryPostfixExpression>());
  } else if (node.true_expression->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.true_expression->As<FunctionCallExpression>());
  } else if (node.true_expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.true_expression->As<TernaryExpression>());
  } else if (node.true_expression->type == AST::NodeType::DELETE) {
    printed = VisitDeleteExpression(*node.true_expression->As<DeleteExpression>());
  } else if (node.true_expression->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.true_expression->As<SizeofExpression>());
  } else if (node.true_expression->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.true_expression->As<AlignofExpression>());
  } else if (node.true_expression->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.true_expression->As<CastExpression>());
  } else if (node.true_expression->type == AST::NodeType::ARRAY) {
    printed = VisitArray(*node.true_expression->As<Array>());
  } else if (node.true_expression->type == AST::NodeType::NEW) {
    printed = VisitNewExpression(*node.true_expression->As<NewExpression>());
  }

  if (!printed) return false;
  print(" : ");

  if (node.false_expression->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.false_expression->As<IdentifierAccess>());
  } else if (node.false_expression->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.false_expression->As<Literal>());
  } else if (node.false_expression->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.false_expression->As<Parenthetical>());
  } else if (node.false_expression->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.false_expression->As<BinaryExpression>());
  } else if (node.false_expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.false_expression->As<UnaryPrefixExpression>());
  } else if (node.false_expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.false_expression->As<UnaryPostfixExpression>());
  } else if (node.false_expression->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.false_expression->As<FunctionCallExpression>());
  } else if (node.false_expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.false_expression->As<TernaryExpression>());
  } else if (node.false_expression->type == AST::NodeType::DELETE) {
    printed = VisitDeleteExpression(*node.false_expression->As<DeleteExpression>());
  } else if (node.false_expression->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.false_expression->As<SizeofExpression>());
  } else if (node.false_expression->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.false_expression->As<AlignofExpression>());
  } else if (node.false_expression->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.false_expression->As<CastExpression>());
  } else if (node.false_expression->type == AST::NodeType::ARRAY) {
    printed = VisitArray(*node.false_expression->As<Array>());
  } else if (node.true_expression->type == AST::NodeType::NEW) {
    printed = VisitNewExpression(*node.true_expression->As<NewExpression>());
  }

  return printed;
}

bool AST::Visitor::VisitReturnStatement(ReturnStatement &node) {
  bool printed = false;
  print("return ");

  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.expression->As<Literal>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.expression->As<Parenthetical>());
  } else if (node.expression->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.expression->As<BinaryExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.expression->As<UnaryPrefixExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.expression->As<UnaryPostfixExpression>());
  } else if (node.expression->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.expression->As<FunctionCallExpression>());
  } else if (node.expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.expression->As<TernaryExpression>());
  } else if (node.expression->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.expression->As<SizeofExpression>());
  } else if (node.expression->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.expression->As<AlignofExpression>());
  } else if (node.expression->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.expression->As<CastExpression>());
  } else if (node.expression->type == AST::NodeType::ARRAY) {
    printed = VisitArray(*node.expression->As<Array>());
  } else if (node.expression->type == AST::NodeType::BLOCK) {
    printed = true;
  }

  return printed;
}

bool AST::Visitor::VisitFullType(FullType &ft, bool print_type) {
  if (print_type) {
    std::vector<std::size_t> flags_values = {jdi::builtin_flag__const->value,    jdi::builtin_flag__static->value,
                                             jdi::builtin_flag__volatile->value, jdi::builtin_flag__mutable->value,
                                             jdi::builtin_flag__register->value, jdi::builtin_flag__inline->value,
                                             jdi::builtin_flag__Complex->value,  jdi::builtin_flag__unsigned->value,
                                             jdi::builtin_flag__signed->value,   jdi::builtin_flag__short->value,
                                             jdi::builtin_flag__long->value,     jdi::builtin_flag__long_long->value,
                                             jdi::builtin_flag__restrict->value, jdi::builtin_typeflag__override->value,
                                             jdi::builtin_typeflag__final->value};

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
        if (i != 8 || (i == 8 && ft.def->name == "char")) {
          print(flags_names[i] + " ");
        }
      }
    }

    print(ft.def->name + " ");
  }

  std::string name = std::string(ft.decl.name.content);
  if (name != "" && !ft.decl.components.size()) {
    print(name + " ");
  }

  jdi::ref_stack stack;
  ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();

  std::string ref;
  bool flag = false;
  bool print_name = true;

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
      } else {
        print("RT_MEMBER_POINTER");
      }

      // TODO: RT_MEMBER_POINTER

      flag = false;
    }

    if (print_name) {
      std::string name = std::string(ft.decl.name.content);
      if (name != "") {
        if (it->type == jdi::ref_stack::RT_ARRAYBOUND) {
          ref = name + ref;
        } else {
          ref += name;
        }
      }
      print_name = false;
    }
  }

  print(ref);
  return true;
}

bool AST::Visitor::VisitSizeofExpression(SizeofExpression &node) {
  bool printed = false;
  print("sizeof");

  if (node.kind == AST::SizeofExpression::Kind::EXPR) {
    print(" ");

    auto &arg = std::get<AST::PNode>(node.argument);
    if (arg->type == AST::NodeType::LITERAL) {
      printed = VisitLiteral(*arg->As<AST::Literal>());
    } else if (arg->type == AST::NodeType::IDENTIFIER) {
      printed = VisitIdentifierAccess(*arg->As<AST::IdentifierAccess>());
    }
  } else if (node.kind == AST::SizeofExpression::Kind::VARIADIC) {
    print("...(");

    std::string arg = std::get<std::string>(node.argument);
    print(arg + ")");

    printed = true;
  } else {
    print("(");

    FullType &ft = std::get<FullType>(node.argument);
    printed = VisitFullType(ft);

    print(")");
  }

  return printed;
}

bool AST::Visitor::VisitAlignofExpression(AlignofExpression &node) {
  print("alignof(");

  bool printed = VisitFullType(node.ft);
  print(")");

  return printed;
}

bool AST::Visitor::VisitCastExpression(CastExpression &node) {
  bool printed = false;

  if (node.kind == AST::CastExpression::Kind::FUNCTIONAL) {
    printed = VisitFullType(node.ft);
    print("(");
  } else if (node.kind == AST::CastExpression::Kind::C_STYLE) {
    print("(");
    printed = VisitFullType(node.ft);
    print(")");
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
    printed = VisitFullType(node.ft);
    print(">(");
  }

  if (!printed) return false;

  if (node.expr->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.expr->As<BinaryExpression>());
  } else if (node.expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.expr->As<UnaryPrefixExpression>());
  } else if (node.expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.expr->As<UnaryPostfixExpression>());
  } else if (node.expr->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.expr->As<IdentifierAccess>());
  } else if (node.expr->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.expr->As<Parenthetical>());
  } else if (node.expr->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.expr->As<FunctionCallExpression>());
  } else if (node.expr->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.expr->As<TernaryExpression>());
  } else if (node.expr->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.expr->As<SizeofExpression>());
  } else if (node.expr->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.expr->As<AlignofExpression>());
  } else if (node.expr->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.expr->As<CastExpression>());
  }

  if (!printed) return false;

  if (node.kind == AST::CastExpression::Kind::FUNCTIONAL) {
    print(")");
  } else if (node.kind != AST::CastExpression::Kind::C_STYLE) {
    print(")");
  }

  return true;
}

bool AST::Visitor::VisitArray(Array &node) {
  bool printed = false;
  print("[");

  if (node.elements.size()) {
    if (node.elements[0]->type == AST::NodeType::IDENTIFIER) {
      printed = VisitIdentifierAccess(*node.elements[0]->As<IdentifierAccess>());
    } else if (node.elements[0]->type == AST::NodeType::LITERAL) {
      printed = VisitLiteral(*node.elements[0]->As<Literal>());
    } else if (node.elements[0]->type == AST::NodeType::PARENTHETICAL) {
      printed = VisitParenthetical(*node.elements[0]->As<Parenthetical>());
    } else if (node.elements[0]->type == AST::NodeType::BINARY_EXPRESSION) {
      printed = VisitBinaryExpression(*node.elements[0]->As<BinaryExpression>());
    } else if (node.elements[0]->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      printed = VisitUnaryPrefixExpression(*node.elements[0]->As<UnaryPrefixExpression>());
    } else if (node.elements[0]->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      printed = VisitUnaryPostfixExpression(*node.elements[0]->As<UnaryPostfixExpression>());
    } else if (node.elements[0]->type == AST::NodeType::FUNCTION_CALL) {
      printed = VisitFunctionCallExpression(*node.elements[0]->As<FunctionCallExpression>());
    } else if (node.elements[0]->type == AST::NodeType::SIZEOF) {
      printed = VisitSizeofExpression(*node.elements[0]->As<SizeofExpression>());
    } else if (node.elements[0]->type == AST::NodeType::ALIGNOF) {
      printed = VisitAlignofExpression(*node.elements[0]->As<AlignofExpression>());
    } else if (node.elements[0]->type == AST::NodeType::CAST) {
      printed = VisitCastExpression(*node.elements[0]->As<CastExpression>());
    } else if (node.elements[0]->type == AST::NodeType::ARRAY) {
      printed = VisitArray(*node.elements[0]->As<Array>());
    }
  } else {
    printed = true;
  }

  if (!printed) return false;

  print("]");
  return true;
}

bool AST::Visitor::VisitBraceOrParenInitializer(BraceOrParenInitializer &node) {
  bool printed = false;

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
      print(val.first + "=");
    }

    printed = VisitInitializer(*val.second);

    if (&val != &node.values.back()) {
      print(", ");
    }

    if (!printed) return false;
  }

  if (node.kind == BraceOrParenInitializer::Kind::PAREN_INIT) {
    print(")");
  } else {
    print("}");
  }

  return true;
}

bool AST::Visitor::VisitAssignmentInitializer(AssignmentInitializer &node) {
  bool printed = false;

  if (node.kind == AssignmentInitializer::Kind::BRACE_INIT) {
    printed = VisitBraceOrParenInitializer(*std::get<BraceOrParenInitNode>(node.initializer));
  } else {
    auto &expr = std::get<AST::PNode>(node.initializer);

    if (expr->type == AST::NodeType::IDENTIFIER) {
      printed = VisitIdentifierAccess(*expr->As<AST::IdentifierAccess>());
    } else if (expr->type == AST::NodeType::LITERAL) {
      printed = VisitLiteral(*expr->As<AST::Literal>());
    } else if (expr->type == AST::NodeType::PARENTHETICAL) {
      printed = VisitParenthetical(*expr->As<AST::Parenthetical>());
    } else if (expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      printed = VisitUnaryPostfixExpression(*expr->As<AST::UnaryPostfixExpression>());
    } else if (expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      printed = VisitUnaryPrefixExpression(*expr->As<AST::UnaryPrefixExpression>());
    } else if (expr->type == AST::NodeType::BINARY_EXPRESSION) {
      printed = VisitBinaryExpression(*expr->As<AST::BinaryExpression>());
    } else if (expr->type == AST::NodeType::FUNCTION_CALL) {
      printed = VisitFunctionCallExpression(*expr->As<FunctionCallExpression>());
    } else if (expr->type == AST::NodeType::SIZEOF) {
      printed = VisitSizeofExpression(*expr->As<SizeofExpression>());
    } else if (expr->type == AST::NodeType::ALIGNOF) {
      printed = VisitAlignofExpression(*expr->As<AlignofExpression>());
    } else if (expr->type == AST::NodeType::CAST) {
      printed = VisitCastExpression(*expr->As<CastExpression>());
    } else if (expr->type == AST::NodeType::ARRAY) {
      printed = VisitArray(*expr->As<Array>());
    } else if (expr->type == AST::NodeType::NEW) {
      printed = VisitNewExpression(*expr->As<NewExpression>());
    }
  }

  return printed;
}

bool AST::Visitor::VisitInitializer(Initializer &node) {
  bool printed = false;

  if (node.kind == Initializer::Kind::BRACE_INIT || node.kind == Initializer::Kind::PLACEMENT_NEW) {
    auto &init = std::get<BraceOrParenInitNode>(node.initializer);
    printed = VisitBraceOrParenInitializer(*init);
  } else if (node.kind == Initializer::Kind::ASSIGN_EXPR) {
    auto &init = std::get<AssignmentInitNode>(node.initializer);
    printed = VisitAssignmentInitializer(*init);
  }

  if (!printed) return false;
  if (node.is_variadic) {
    print("...");
  }

  return true;
}

bool AST::Visitor::VisitNewExpression(NewExpression &node) {
  bool printed = false;

  if (node.is_global) {
    print("::");
  }

  print("new ");

  if (node.placement) {
    printed = VisitInitializer(*node.placement);
    print(" ");
    if (!printed) return false;
  }

  print("(");
  printed = VisitFullType(node.ft);
  print(")");

  if (!printed) return false;
  if (node.initializer) {
    printed = VisitInitializer(*node.initializer);
  }

  return printed;
}

bool AST::Visitor::VisitDeclarationStatement(DeclarationStatement &node) {
  bool printed = false;

  for (std::size_t i = 0; i < node.declarations.size(); i++) {
    printed = VisitFullType(*node.declarations[i].declarator, !i);
    if (!printed) return false;
    if (node.declarations[i].init) {
      print(" = ");  // TODO: corner case: int x {}, maybe we need extra flag in the AST?
      printed = VisitInitializer(*node.declarations[i].init);
    }
    if (!printed) return false;
    if (i != node.declarations.size() - 1) {
      print(", ");
    }
  }
  return true;
}

bool AST::Visitor::VisitCode(CodeBlock &node) {
  bool printed = false;
  for (auto &stmt : node.statements) {
    if (stmt->type == AST::NodeType::DECLARATION) {
      printed = VisitDeclarationStatement(*stmt->As<DeclarationStatement>());
    } else if (stmt->type == AST::NodeType::IDENTIFIER) {
      printed = VisitIdentifierAccess(*stmt->As<AST::IdentifierAccess>());
    } else if (stmt->type == AST::NodeType::PARENTHETICAL) {
      printed = VisitParenthetical(*stmt->As<Parenthetical>());
    } else if (stmt->type == AST::NodeType::RETURN) {
      printed = VisitReturnStatement(*stmt->As<ReturnStatement>());
    } else if (stmt->type == AST::NodeType::BREAK) {
      printed = VisitBreakStatement(*stmt->As<BreakStatement>());
    } else if (stmt->type == AST::NodeType::CONTINUE) {
      printed = VisitContinueStatement(*stmt->As<ContinueStatement>());
    } else if (stmt->type == AST::NodeType::NEW) {
      printed = VisitNewExpression(*stmt->As<NewExpression>());
    } else if (stmt->type == AST::NodeType::DELETE) {
      printed = VisitDeleteExpression(*stmt->As<DeleteExpression>());
    } else if (stmt->type == AST::NodeType::BINARY_EXPRESSION) {
      printed = VisitBinaryExpression(*stmt->As<BinaryExpression>());
    } else if (stmt->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      printed = VisitUnaryPrefixExpression(*stmt->As<UnaryPrefixExpression>());
    } else if (stmt->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      printed = VisitUnaryPostfixExpression(*stmt->As<UnaryPostfixExpression>());
    } else if (stmt->type == AST::NodeType::FUNCTION_CALL) {
      printed = VisitFunctionCallExpression(*stmt->As<FunctionCallExpression>());
    } else if (stmt->type == AST::NodeType::TERNARY_EXPRESSION) {
      printed = VisitTernaryExpression(*stmt->As<TernaryExpression>());
    } else if (stmt->type == AST::NodeType::SIZEOF) {
      printed = VisitSizeofExpression(*stmt->As<SizeofExpression>());
    } else if (stmt->type == AST::NodeType::ALIGNOF) {
      printed = VisitAlignofExpression(*stmt->As<AlignofExpression>());
    } else if (stmt->type == AST::NodeType::CAST) {
      printed = VisitCastExpression(*stmt->As<CastExpression>());
    } else if (stmt->type == AST::NodeType::BLOCK) {
      printed = VisitCodeBlock(*stmt->As<CodeBlock>());
    } else if (stmt->type == AST::NodeType::IF) {
      printed = VisitIfStatement(*stmt->As<IfStatement>());
    } else if (stmt->type == AST::NodeType::FOR) {
      printed = VisitForLoop(*stmt->As<ForLoop>());
    } else if (stmt->type == AST::NodeType::CASE) {
      printed = VisitCaseStatement(*stmt->As<CaseStatement>());
    } else if (stmt->type == AST::NodeType::DEFAULT) {
      printed = VisitDefaultStatement(*stmt->As<DefaultStatement>());
    } else if (stmt->type == AST::NodeType::SWITCH) {
      printed = VisitSwitchStatement(*stmt->As<SwitchStatement>());
    } else if (stmt->type == AST::NodeType::WHILE) {
      printed = VisitWhileLoop(*stmt->As<WhileLoop>());
    } else if (stmt->type == AST::NodeType::DO) {
      printed = VisitDoLoop(*stmt->As<DoLoop>());
    }

    if (!printed) return false;
    if (stmt->type != AST::NodeType::BLOCK && stmt->type != AST::NodeType::IF && stmt->type != AST::NodeType::FOR &&
        stmt->type != AST::NodeType::CASE && stmt->type != AST::NodeType::DEFAULT &&
        stmt->type != AST::NodeType::SWITCH && stmt->type != AST::NodeType::WHILE && stmt->type != AST::NodeType::DO) {
      print("; ");
    }
  }

  return true;
}

bool AST::Visitor::VisitCodeBlock(CodeBlock &node) {
  print("{");

  bool printed = VisitCode(node);
  if (!printed) return false;
  print("}");

  return true;
}

bool AST::Visitor::VisitCondition(PNode &node) {
  bool printed = false;

  if (node->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node->As<AST::IdentifierAccess>());
  } else if (node->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node->As<AST::Literal>());
  } else if (node->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node->As<Parenthetical>());
  } else if (node->type == AST::NodeType::NEW) {
    printed = VisitNewExpression(*node->As<NewExpression>());
  } else if (node->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node->As<BinaryExpression>());
  } else if (node->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node->As<UnaryPrefixExpression>());
  } else if (node->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node->As<UnaryPostfixExpression>());
  } else if (node->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node->As<FunctionCallExpression>());
  } else if (node->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node->As<TernaryExpression>());
  } else if (node->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node->As<SizeofExpression>());
  } else if (node->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node->As<AlignofExpression>());
  } else if (node->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node->As<CastExpression>());
  } else if (node->type == AST::NodeType::DECLARATION) {
    printed = VisitDeclarationStatement(*node->As<DeclarationStatement>());
  }

  return printed;
}

bool AST::Visitor::VisitBody(PNode &node) {
  bool printed = false;

  if (node->type == AST::NodeType::BLOCK) {
    printed = VisitCodeBlock(*node->As<CodeBlock>());
  } else if (node->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node->As<AST::IdentifierAccess>());
  } else if (node->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node->As<Parenthetical>());
  } else if (node->type == AST::NodeType::NEW) {
    printed = VisitNewExpression(*node->As<NewExpression>());
  } else if (node->type == AST::NodeType::DELETE) {
    printed = VisitDeleteExpression(*node->As<DeleteExpression>());
  } else if (node->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node->As<BinaryExpression>());
  } else if (node->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node->As<UnaryPrefixExpression>());
  } else if (node->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node->As<UnaryPostfixExpression>());
  } else if (node->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node->As<FunctionCallExpression>());
  } else if (node->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node->As<TernaryExpression>());
  } else if (node->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node->As<SizeofExpression>());
  } else if (node->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node->As<AlignofExpression>());
  } else if (node->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node->As<CastExpression>());
  } else if (node->type == AST::NodeType::DECLARATION) {
    printed = VisitDeclarationStatement(*node->As<DeclarationStatement>());
  } else if (node->type == AST::NodeType::BREAK) {
    printed = VisitBreakStatement(*node->As<BreakStatement>());
  } else if (node->type == AST::NodeType::CONTINUE) {
    printed = VisitContinueStatement(*node->As<ContinueStatement>());
  } else if (node->type == AST::NodeType::RETURN) {
    printed = VisitReturnStatement(*node->As<ReturnStatement>());
  } else if (node->type == AST::NodeType::IF) {
    printed = VisitIfStatement(*node->As<IfStatement>());
  } else if (node->type == AST::NodeType::FOR) {
    printed = VisitForLoop(*node->As<ForLoop>());
  } else if (node->type == AST::NodeType::SWITCH) {
    printed = VisitSwitchStatement(*node->As<SwitchStatement>());
  } else if (node->type == AST::NodeType::WHILE) {
    printed = VisitWhileLoop(*node->As<WhileLoop>());
  } else if (node->type == AST::NodeType::DO) {
    printed = VisitDoLoop(*node->As<DoLoop>());
  }

  return printed;
}

bool AST::Visitor::VisitIfStatement(IfStatement &node) {
  bool printed = false;

  print("if");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  printed = VisitCondition(node.condition);

  if (!printed) return false;
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(" ");

  printed = VisitBody(node.true_branch);

  if (!printed) return false;
  if (node.true_branch->type != AST::NodeType::BLOCK && node.true_branch->type != AST::NodeType::IF &&
      node.true_branch->type != AST::NodeType::FOR && node.true_branch->type != AST::NodeType::SWITCH &&
      node.true_branch->type != AST::NodeType::WHILE && node.true_branch->type != AST::NodeType::DO) {
    print(";");
  }
  print(" ");

  if (node.false_branch) {
    print("else ");

    printed = VisitBody(node.false_branch);

    if (!printed) return false;
    if (node.false_branch->type != AST::NodeType::BLOCK && node.false_branch->type != AST::NodeType::IF &&
        node.false_branch->type != AST::NodeType::FOR && node.false_branch->type != AST::NodeType::SWITCH &&
        node.false_branch->type != AST::NodeType::WHILE && node.false_branch->type != AST::NodeType::DO) {
      print(";");
    }
    print(" ");
  }
  return true;
}

bool AST::Visitor::VisitForLoop(ForLoop &node) {
  bool printed = false;
  print("for(");

  if (node.assignment->type == AST::NodeType::DECLARATION) {
    printed = VisitDeclarationStatement(*node.assignment->As<DeclarationStatement>());
  } else if (node.assignment->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.assignment->As<AST::IdentifierAccess>());
  } else if (node.assignment->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.assignment->As<Parenthetical>());
  } else if (node.assignment->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.assignment->As<BinaryExpression>());
  } else if (node.assignment->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.assignment->As<UnaryPrefixExpression>());
  } else if (node.assignment->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.assignment->As<UnaryPostfixExpression>());
  } else if (node.assignment->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.assignment->As<FunctionCallExpression>());
  } else if (node.assignment->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.assignment->As<TernaryExpression>());
  } else if (node.assignment->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.assignment->As<SizeofExpression>());
  } else if (node.assignment->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.assignment->As<AlignofExpression>());
  } else if (node.assignment->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.assignment->As<CastExpression>());
  }

  if (!printed) return false;
  print("; ");

  printed = VisitCondition(node.condition);

  if (!printed) return false;
  print("; ");

  if (node.increment->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.increment->As<AST::IdentifierAccess>());
  } else if (node.increment->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.increment->As<AST::Literal>());
  } else if (node.increment->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.increment->As<Parenthetical>());
  } else if (node.increment->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.increment->As<BinaryExpression>());
  } else if (node.increment->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.increment->As<UnaryPrefixExpression>());
  } else if (node.increment->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.increment->As<UnaryPostfixExpression>());
  } else if (node.increment->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.increment->As<FunctionCallExpression>());
  } else if (node.increment->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.increment->As<TernaryExpression>());
  } else if (node.increment->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.increment->As<SizeofExpression>());
  } else if (node.increment->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.increment->As<AlignofExpression>());
  } else if (node.increment->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.increment->As<CastExpression>());
  } else if (node.increment->type == AST::NodeType::NEW) {
    printed = VisitNewExpression(*node.increment->As<NewExpression>());
  }

  if (!printed) return false;
  print(") ");

  printed = VisitBody(node.body);

  if (!printed) return false;
  if (node.body->type != AST::NodeType::BLOCK && node.body->type != AST::NodeType::IF &&
      node.body->type != AST::NodeType::FOR && node.body->type != AST::NodeType::SWITCH &&
      node.body->type != AST::NodeType::WHILE && node.body->type != AST::NodeType::DO) {
    print(";");
  }
  print(" ");

  return true;
}

bool AST::Visitor::VisitCaseStatement(CaseStatement &node) {
  bool printed = false;
  print("case ");

  if (node.value->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.value->As<IdentifierAccess>());
  } else if (node.value->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.value->As<Literal>());
  } else if (node.value->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitLiteral(*node.value->As<Literal>());
  } else if (node.value->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.value->As<Parenthetical>());
  } else if (node.value->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.value->As<FunctionCallExpression>());
  } else if (node.value->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.value->As<TernaryExpression>());
  } else if (node.value->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.value->As<SizeofExpression>());
  } else if (node.value->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.value->As<AlignofExpression>());
  } else if (node.value->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.value->As<CastExpression>());
  }

  if (!printed) return false;
  print(": ");

  printed = VisitCodeBlock(*node.statements->As<AST::CodeBlock>());
  print(" ");

  return printed;
}

bool AST::Visitor::VisitDefaultStatement(DefaultStatement &node) {
  print("default: ");
  bool printed = VisitCodeBlock(*node.statements->As<CodeBlock>());
  print(" ");
  return printed;
}

bool AST::Visitor::VisitSwitchStatement(SwitchStatement &node) {
  bool printed = false;
  print("switch");
  if (node.expression->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    printed = VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::LITERAL) {
    printed = VisitLiteral(*node.expression->As<Literal>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    printed = VisitParenthetical(*node.expression->As<Parenthetical>());
  } else if (node.expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    printed = VisitUnaryPostfixExpression(*node.expression->As<UnaryPostfixExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    printed = VisitUnaryPrefixExpression(*node.expression->As<UnaryPrefixExpression>());
  } else if (node.expression->type == AST::NodeType::BINARY_EXPRESSION) {
    printed = VisitBinaryExpression(*node.expression->As<BinaryExpression>());
  } else if (node.expression->type == AST::NodeType::FUNCTION_CALL) {
    printed = VisitFunctionCallExpression(*node.expression->As<FunctionCallExpression>());
  } else if (node.expression->type == AST::NodeType::TERNARY_EXPRESSION) {
    printed = VisitTernaryExpression(*node.expression->As<TernaryExpression>());
  } else if (node.expression->type == AST::NodeType::SIZEOF) {
    printed = VisitSizeofExpression(*node.expression->As<SizeofExpression>());
  } else if (node.expression->type == AST::NodeType::ALIGNOF) {
    printed = VisitAlignofExpression(*node.expression->As<AlignofExpression>());
  } else if (node.expression->type == AST::NodeType::CAST) {
    printed = VisitCastExpression(*node.expression->As<CastExpression>());
  } else if (node.expression->type == AST::NodeType::DECLARATION) {
    printed = VisitDeclarationStatement(*node.expression->As<DeclarationStatement>());
  }

  if (!printed) return false;
  if (node.expression->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(" ");

  printed = VisitCodeBlock(*node.body->As<CodeBlock>());
  print(" ");

  return printed;
}

bool AST::Visitor::VisitWhileLoop(WhileLoop &node) {
  bool printed = false;
  // temp sol
  if (node.kind == AST::WhileLoop::Kind::REPEAT) {
    print("int strange_name = ");
  }

  else {
    print("while");
    if (node.condition->type != AST::NodeType::PARENTHETICAL) {
      print("(");
    }

    if (node.kind == AST::WhileLoop::Kind::UNTIL) {
      if (node.condition->type == AST::NodeType::PARENTHETICAL) {
        print("(!");
      } else {
        print("!(");
      }
    }
  }

  printed = VisitCondition(node.condition);

  if (!printed) return false;
  if (node.kind != AST::WhileLoop::Kind::REPEAT) {
    if (node.kind == AST::WhileLoop::Kind::UNTIL) {
      print(")");
    }

    if (node.condition->type != AST::NodeType::PARENTHETICAL) {
      print(")");
    }
  } else {
    print("; while(strange_name--)");
  }

  print(" ");

  printed = VisitBody(node.body);

  if (!printed) return false;
  if (node.body->type != AST::NodeType::BLOCK && node.body->type != AST::NodeType::IF &&
      node.body->type != AST::NodeType::FOR && node.body->type != AST::NodeType::SWITCH &&
      node.body->type != AST::NodeType::WHILE && node.body->type != AST::NodeType::DO) {
    print(";");
  }
  return true;
}

bool AST::Visitor::VisitDoLoop(DoLoop &node) {
  bool printed = false;
  print("do");

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }

  printed = VisitBody(node.body);

  if (!printed) return false;
  if (node.body->type != AST::NodeType::BLOCK && node.body->type != AST::NodeType::IF &&
      node.body->type != AST::NodeType::FOR && node.body->type != AST::NodeType::SWITCH &&
      node.body->type != AST::NodeType::WHILE && node.body->type != AST::NodeType::DO) {
    print(";");
  }

  if (node.body->type != AST::NodeType::BLOCK) {
    print("}");
  }

  print("while");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.is_until) {
    if (node.condition->type == AST::NodeType::PARENTHETICAL) {
      print("(!");
    } else {
      print("!(");
    }
  }

  printed = VisitCondition(node.condition);

  if (!printed) return false;
  if (node.is_until) {
    print(")");
  }

  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }

  print(";");
  return true;
}
