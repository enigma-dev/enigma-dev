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
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.expression->As<Parenthetical>());
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

  print(" " + node.operation.token + " ");

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
  } else if (node.right->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.right->As<NewExpression>());
  }

  if (node.operation.type == TT_BEGINBRACKET) {
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
  } else if (node.true_expression->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.true_expression->As<NewExpression>());
  }

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
  } else if (node.true_expression->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.true_expression->As<NewExpression>());
  }
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
  } else if (node.kind == AST::CastExpression::Kind::C_STYLE) {
    print("(");
    VisitFullType(node.ft);
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
    VisitFullType(node.ft);
    print(">(");
  }

  if (node.expr->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.expr->As<BinaryExpression>());
  } else if (node.expr->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.expr->As<UnaryPrefixExpression>());
  } else if (node.expr->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.expr->As<UnaryPostfixExpression>());
  } else if (node.expr->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.expr->As<IdentifierAccess>());
  } else if (node.expr->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.expr->As<Parenthetical>());
  } else if (node.expr->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.expr->As<FunctionCallExpression>());
  } else if (node.expr->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.expr->As<TernaryExpression>());
  } else if (node.expr->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.expr->As<SizeofExpression>());
  } else if (node.expr->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.expr->As<AlignofExpression>());
  } else if (node.expr->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.expr->As<CastExpression>());
  }

  if (node.kind == AST::CastExpression::Kind::FUNCTIONAL) {
    print(")");
  } else if (node.kind != AST::CastExpression::Kind::C_STYLE) {
    print(")");
  }
}

bool AST::Visitor::VisitArray(Array &node) {
  print("[");

  if (node.elements.size()) {
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
      print(val.first + "=");
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
    } else if (expr->type == AST::NodeType::NEW) {
      VisitNewExpression(*expr->As<NewExpression>());
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

bool AST::Visitor::VisitDeclarationStatement(DeclarationStatement &node) {
  for (std::size_t i = 0; i < node.declarations.size(); i++) {
    VisitFullType(*node.declarations[i].declarator, !i);

    if (node.declarations[i].init) {
      print(" = ");  // TODO: corner case: int x {}, maybe we need extra flag in the AST?
      VisitInitializer(*node.declarations[i].init);
    }

    if (i != node.declarations.size() - 1) {
      print(", ");
    }
  }
}

bool AST::Visitor::VisitCode(CodeBlock &node) {
  for (auto &stmt : node.statements) {
    if (stmt->type == AST::NodeType::DECLARATION) {
      VisitDeclarationStatement(*stmt->As<DeclarationStatement>());
    } else if (stmt->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*stmt->As<AST::IdentifierAccess>());
    } else if (stmt->type == AST::NodeType::PARENTHETICAL) {
      VisitParenthetical(*stmt->As<Parenthetical>());
    } else if (stmt->type == AST::NodeType::RETURN) {
      VisitReturnStatement(*stmt->As<ReturnStatement>());
    } else if (stmt->type == AST::NodeType::BREAK) {
      VisitBreakStatement(*stmt->As<BreakStatement>());
    } else if (stmt->type == AST::NodeType::CONTINUE) {
      VisitContinueStatement(*stmt->As<ContinueStatement>());
    } else if (stmt->type == AST::NodeType::NEW) {
      VisitNewExpression(*stmt->As<NewExpression>());
    } else if (stmt->type == AST::NodeType::DELETE) {
      VisitDeleteExpression(*stmt->As<DeleteExpression>());
    } else if (stmt->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*stmt->As<BinaryExpression>());
    } else if (stmt->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*stmt->As<UnaryPrefixExpression>());
    } else if (stmt->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*stmt->As<UnaryPostfixExpression>());
    } else if (stmt->type == AST::NodeType::FUNCTION_CALL) {
      VisitFunctionCallExpression(*stmt->As<FunctionCallExpression>());
    } else if (stmt->type == AST::NodeType::TERNARY_EXPRESSION) {
      VisitTernaryExpression(*stmt->As<TernaryExpression>());
    } else if (stmt->type == AST::NodeType::SIZEOF) {
      VisitSizeofExpression(*stmt->As<SizeofExpression>());
    } else if (stmt->type == AST::NodeType::ALIGNOF) {
      VisitAlignofExpression(*stmt->As<AlignofExpression>());
    } else if (stmt->type == AST::NodeType::CAST) {
      VisitCastExpression(*stmt->As<CastExpression>());
    } else if (stmt->type == AST::NodeType::BLOCK) {
      VisitCodeBlock(*stmt->As<CodeBlock>());
    } else if (stmt->type == AST::NodeType::IF) {
      VisitIfStatement(*stmt->As<IfStatement>());
    } else if (stmt->type == AST::NodeType::FOR) {
      VisitForLoop(*stmt->As<ForLoop>());
    } else if (stmt->type == AST::NodeType::CASE) {
      VisitCaseStatement(*stmt->As<CaseStatement>());
    } else if (stmt->type == AST::NodeType::DEFAULT) {
      VisitDefaultStatement(*stmt->As<DefaultStatement>());
    } else if (stmt->type == AST::NodeType::SWITCH) {
      VisitSwitchStatement(*stmt->As<SwitchStatement>());
    } else if (stmt->type == AST::NodeType::WHILE) {
      VisitWhileLoop(*stmt->As<WhileLoop>());
    } else if (stmt->type == AST::NodeType::DO) {
      VisitDoLoop(*stmt->As<DoLoop>());
    }

    if (stmt->type != AST::NodeType::BLOCK && stmt->type != AST::NodeType::IF && stmt->type != AST::NodeType::FOR &&
        stmt->type != AST::NodeType::CASE && stmt->type != AST::NodeType::DEFAULT &&
        stmt->type != AST::NodeType::SWITCH && stmt->type != AST::NodeType::WHILE && stmt->type != AST::NodeType::DO) {
      print("; ");
    }
  }
}

bool AST::Visitor::VisitCodeBlock(CodeBlock &node) {
  print("{");

  VisitCode(node);

  print("}");
}

bool AST::Visitor::VisitIfStatement(IfStatement &node) {
  print("if");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.condition->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.condition->As<AST::IdentifierAccess>());
  } else if (node.condition->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.condition->As<AST::Literal>());
  } else if (node.condition->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.condition->As<Parenthetical>());
  } else if (node.condition->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.condition->As<NewExpression>());
  } else if (node.condition->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.condition->As<BinaryExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.condition->As<UnaryPrefixExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.condition->As<UnaryPostfixExpression>());
  } else if (node.condition->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.condition->As<FunctionCallExpression>());
  } else if (node.condition->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.condition->As<TernaryExpression>());
  } else if (node.condition->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.condition->As<SizeofExpression>());
  } else if (node.condition->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.condition->As<AlignofExpression>());
  } else if (node.condition->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.condition->As<CastExpression>());
  } else if (node.condition->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.condition->As<DeclarationStatement>());
  }

  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(" ");

  if (node.true_branch->type == AST::NodeType::BLOCK) {
    VisitCodeBlock(*node.true_branch->As<CodeBlock>());
  } else if (node.true_branch->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.true_branch->As<AST::IdentifierAccess>());
  } else if (node.true_branch->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.true_branch->As<Parenthetical>());
  } else if (node.true_branch->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.true_branch->As<NewExpression>());
  } else if (node.true_branch->type == AST::NodeType::DELETE) {
    VisitDeleteExpression(*node.true_branch->As<DeleteExpression>());
  } else if (node.true_branch->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.true_branch->As<BinaryExpression>());
  } else if (node.true_branch->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.true_branch->As<UnaryPrefixExpression>());
  } else if (node.true_branch->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.true_branch->As<UnaryPostfixExpression>());
  } else if (node.true_branch->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.true_branch->As<FunctionCallExpression>());
  } else if (node.true_branch->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.true_branch->As<TernaryExpression>());
  } else if (node.true_branch->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.true_branch->As<SizeofExpression>());
  } else if (node.true_branch->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.true_branch->As<AlignofExpression>());
  } else if (node.true_branch->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.true_branch->As<CastExpression>());
  } else if (node.true_branch->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.true_branch->As<DeclarationStatement>());
  } else if (node.true_branch->type == AST::NodeType::BREAK) {
    VisitBreakStatement(*node.true_branch->As<BreakStatement>());
  } else if (node.true_branch->type == AST::NodeType::CONTINUE) {
    VisitContinueStatement(*node.true_branch->As<ContinueStatement>());
  } else if (node.true_branch->type == AST::NodeType::RETURN) {
    VisitReturnStatement(*node.true_branch->As<ReturnStatement>());
  } else if (node.true_branch->type == AST::NodeType::IF) {
    VisitIfStatement(*node.true_branch->As<IfStatement>());
  } else if (node.true_branch->type == AST::NodeType::FOR) {
    VisitForLoop(*node.true_branch->As<ForLoop>());
  } else if (node.true_branch->type == AST::NodeType::SWITCH) {
    VisitSwitchStatement(*node.true_branch->As<SwitchStatement>());
  } else if (node.true_branch->type == AST::NodeType::WHILE) {
    VisitWhileLoop(*node.true_branch->As<WhileLoop>());
  } else if (node.true_branch->type == AST::NodeType::DO) {
    VisitDoLoop(*node.true_branch->As<DoLoop>());
  }

  if (node.true_branch->type != AST::NodeType::BLOCK && node.true_branch->type != AST::NodeType::IF &&
      node.true_branch->type != AST::NodeType::FOR && node.true_branch->type != AST::NodeType::SWITCH &&
      node.true_branch->type != AST::NodeType::WHILE && node.true_branch->type != AST::NodeType::DO) {
    print(";");
  }
  print(" ");

  if (node.false_branch) {
    print("else ");

    if (node.false_branch->type == AST::NodeType::BLOCK) {
      VisitCodeBlock(*node.false_branch->As<CodeBlock>());
    } else if (node.false_branch->type == AST::NodeType::IDENTIFIER) {
      VisitIdentifierAccess(*node.false_branch->As<AST::IdentifierAccess>());
    } else if (node.false_branch->type == AST::NodeType::PARENTHETICAL) {
      VisitParenthetical(*node.false_branch->As<Parenthetical>());
    } else if (node.false_branch->type == AST::NodeType::NEW) {
      VisitNewExpression(*node.false_branch->As<NewExpression>());
    } else if (node.false_branch->type == AST::NodeType::DELETE) {
      VisitDeleteExpression(*node.false_branch->As<DeleteExpression>());
    } else if (node.false_branch->type == AST::NodeType::BINARY_EXPRESSION) {
      VisitBinaryExpression(*node.false_branch->As<BinaryExpression>());
    } else if (node.false_branch->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
      VisitUnaryPrefixExpression(*node.false_branch->As<UnaryPrefixExpression>());
    } else if (node.false_branch->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
      VisitUnaryPostfixExpression(*node.false_branch->As<UnaryPostfixExpression>());
    } else if (node.false_branch->type == AST::NodeType::FUNCTION_CALL) {
      VisitFunctionCallExpression(*node.false_branch->As<FunctionCallExpression>());
    } else if (node.false_branch->type == AST::NodeType::TERNARY_EXPRESSION) {
      VisitTernaryExpression(*node.false_branch->As<TernaryExpression>());
    } else if (node.false_branch->type == AST::NodeType::SIZEOF) {
      VisitSizeofExpression(*node.false_branch->As<SizeofExpression>());
    } else if (node.false_branch->type == AST::NodeType::ALIGNOF) {
      VisitAlignofExpression(*node.false_branch->As<AlignofExpression>());
    } else if (node.false_branch->type == AST::NodeType::CAST) {
      VisitCastExpression(*node.false_branch->As<CastExpression>());
    } else if (node.false_branch->type == AST::NodeType::DECLARATION) {
      VisitDeclarationStatement(*node.false_branch->As<DeclarationStatement>());
    } else if (node.false_branch->type == AST::NodeType::BREAK) {
      VisitBreakStatement(*node.false_branch->As<BreakStatement>());
    } else if (node.false_branch->type == AST::NodeType::CONTINUE) {
      VisitContinueStatement(*node.false_branch->As<ContinueStatement>());
    } else if (node.false_branch->type == AST::NodeType::RETURN) {
      VisitReturnStatement(*node.false_branch->As<ReturnStatement>());
    } else if (node.false_branch->type == AST::NodeType::IF) {
      VisitIfStatement(*node.false_branch->As<IfStatement>());
    } else if (node.false_branch->type == AST::NodeType::FOR) {
      VisitForLoop(*node.false_branch->As<ForLoop>());
    } else if (node.false_branch->type == AST::NodeType::SWITCH) {
      VisitSwitchStatement(*node.false_branch->As<SwitchStatement>());
    } else if (node.false_branch->type == AST::NodeType::WHILE) {
      VisitWhileLoop(*node.false_branch->As<WhileLoop>());
    } else if (node.false_branch->type == AST::NodeType::DO) {
      VisitDoLoop(*node.false_branch->As<DoLoop>());
    }

    if (node.false_branch->type != AST::NodeType::BLOCK && node.false_branch->type != AST::NodeType::IF &&
        node.false_branch->type != AST::NodeType::FOR && node.false_branch->type != AST::NodeType::SWITCH &&
        node.false_branch->type != AST::NodeType::WHILE && node.false_branch->type != AST::NodeType::DO) {
      print(";");
    }
    print(" ");
  }
}

bool AST::Visitor::VisitForLoop(ForLoop &node) {
  print("for(");

  if (node.assignment->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.assignment->As<DeclarationStatement>());
  } else if (node.assignment->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.assignment->As<AST::IdentifierAccess>());
  } else if (node.assignment->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.assignment->As<Parenthetical>());
  } else if (node.assignment->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.assignment->As<BinaryExpression>());
  } else if (node.assignment->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.assignment->As<UnaryPrefixExpression>());
  } else if (node.assignment->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.assignment->As<UnaryPostfixExpression>());
  } else if (node.assignment->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.assignment->As<FunctionCallExpression>());
  } else if (node.assignment->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.assignment->As<TernaryExpression>());
  } else if (node.assignment->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.assignment->As<SizeofExpression>());
  } else if (node.assignment->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.assignment->As<AlignofExpression>());
  } else if (node.assignment->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.assignment->As<CastExpression>());
  }

  print("; ");

  if (node.condition->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.condition->As<AST::IdentifierAccess>());
  } else if (node.condition->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.condition->As<AST::Literal>());
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
  } else if (node.condition->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.condition->As<TernaryExpression>());
  } else if (node.condition->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.condition->As<SizeofExpression>());
  } else if (node.condition->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.condition->As<AlignofExpression>());
  } else if (node.condition->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.condition->As<CastExpression>());
  } else if (node.condition->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.condition->As<DeclarationStatement>());
  } else if (node.condition->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.condition->As<NewExpression>());
  }

  print("; ");

  if (node.increment->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.increment->As<AST::IdentifierAccess>());
  } else if (node.increment->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.increment->As<AST::Literal>());
  } else if (node.increment->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.increment->As<Parenthetical>());
  } else if (node.increment->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.increment->As<BinaryExpression>());
  } else if (node.increment->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.increment->As<UnaryPrefixExpression>());
  } else if (node.increment->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.increment->As<UnaryPostfixExpression>());
  } else if (node.increment->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.increment->As<FunctionCallExpression>());
  } else if (node.increment->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.increment->As<TernaryExpression>());
  } else if (node.increment->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.increment->As<SizeofExpression>());
  } else if (node.increment->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.increment->As<AlignofExpression>());
  } else if (node.increment->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.increment->As<CastExpression>());
  } else if (node.increment->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.increment->As<NewExpression>());
  }

  print(") ");

  if (node.body->type == AST::NodeType::BLOCK) {
    VisitCodeBlock(*node.body->As<CodeBlock>());
  } else if (node.body->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.body->As<AST::IdentifierAccess>());
  } else if (node.body->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.body->As<Parenthetical>());
  } else if (node.body->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.body->As<NewExpression>());
  } else if (node.body->type == AST::NodeType::DELETE) {
    VisitDeleteExpression(*node.body->As<DeleteExpression>());
  } else if (node.body->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.body->As<BinaryExpression>());
  } else if (node.body->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.body->As<UnaryPrefixExpression>());
  } else if (node.body->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.body->As<UnaryPostfixExpression>());
  } else if (node.body->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.body->As<FunctionCallExpression>());
  } else if (node.body->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.body->As<TernaryExpression>());
  } else if (node.body->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.body->As<SizeofExpression>());
  } else if (node.body->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.body->As<AlignofExpression>());
  } else if (node.body->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.body->As<CastExpression>());
  } else if (node.body->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.body->As<DeclarationStatement>());
  } else if (node.body->type == AST::NodeType::BREAK) {
    VisitBreakStatement(*node.body->As<BreakStatement>());
  } else if (node.body->type == AST::NodeType::CONTINUE) {
    VisitContinueStatement(*node.body->As<ContinueStatement>());
  } else if (node.body->type == AST::NodeType::RETURN) {
    VisitReturnStatement(*node.body->As<ReturnStatement>());
  } else if (node.body->type == AST::NodeType::IF) {
    VisitIfStatement(*node.body->As<IfStatement>());
  } else if (node.body->type == AST::NodeType::FOR) {
    VisitForLoop(*node.body->As<ForLoop>());
  } else if (node.body->type == AST::NodeType::SWITCH) {
    VisitSwitchStatement(*node.body->As<SwitchStatement>());
  } else if (node.body->type == AST::NodeType::WHILE) {
    VisitWhileLoop(*node.body->As<WhileLoop>());
  } else if (node.body->type == AST::NodeType::DO) {
    VisitDoLoop(*node.body->As<DoLoop>());
  }

  if (node.body->type != AST::NodeType::BLOCK && node.body->type != AST::NodeType::IF &&
      node.body->type != AST::NodeType::FOR && node.body->type != AST::NodeType::SWITCH &&
      node.body->type != AST::NodeType::WHILE && node.body->type != AST::NodeType::DO) {
    print(";");
  }
  print(" ");
}

bool AST::Visitor::VisitCaseStatement(CaseStatement &node) {
  print("case ");

  if (node.value->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.value->As<IdentifierAccess>());
  } else if (node.value->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.value->As<Literal>());
  } else if (node.value->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitLiteral(*node.value->As<Literal>());
  } else if (node.value->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.value->As<Parenthetical>());
  } else if (node.value->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.value->As<FunctionCallExpression>());
  } else if (node.value->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.value->As<TernaryExpression>());
  } else if (node.value->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.value->As<SizeofExpression>());
  } else if (node.value->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.value->As<AlignofExpression>());
  } else if (node.value->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.value->As<CastExpression>());
  }

  print(": ");

  VisitCodeBlock(*node.statements->As<AST::CodeBlock>());
  print(" ");
}

bool AST::Visitor::VisitDefaultStatement(DefaultStatement &node) {
  print("default: ");
  VisitCodeBlock(*node.statements->As<CodeBlock>());
  print(" ");
}

bool AST::Visitor::VisitSwitchStatement(SwitchStatement &node) {
  print("switch");
  if (node.expression->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  if (node.expression->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.expression->As<IdentifierAccess>());
  } else if (node.expression->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.expression->As<Literal>());
  } else if (node.expression->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.expression->As<Parenthetical>());
  } else if (node.expression->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.expression->As<UnaryPostfixExpression>());
  } else if (node.expression->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.expression->As<UnaryPrefixExpression>());
  } else if (node.expression->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.expression->As<BinaryExpression>());
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
  } else if (node.expression->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.expression->As<DeclarationStatement>());
  }

  if (node.expression->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(" ");

  VisitCodeBlock(*node.body->As<CodeBlock>());
  print(" ");
}

bool AST::Visitor::VisitWhileLoop(WhileLoop &node) {
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

  if (node.condition->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.condition->As<AST::IdentifierAccess>());
  } else if (node.condition->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.condition->As<AST::Literal>());
  } else if (node.condition->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.condition->As<Parenthetical>());
  } else if (node.condition->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.condition->As<NewExpression>());
  } else if (node.condition->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.condition->As<BinaryExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.condition->As<UnaryPrefixExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.condition->As<UnaryPostfixExpression>());
  } else if (node.condition->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.condition->As<FunctionCallExpression>());
  } else if (node.condition->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.condition->As<TernaryExpression>());
  } else if (node.condition->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.condition->As<SizeofExpression>());
  } else if (node.condition->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.condition->As<AlignofExpression>());
  } else if (node.condition->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.condition->As<CastExpression>());
  } else if (node.condition->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.condition->As<DeclarationStatement>());
  }

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

  if (node.body->type == AST::NodeType::BLOCK) {
    VisitCodeBlock(*node.body->As<CodeBlock>());
  } else if (node.body->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.body->As<AST::IdentifierAccess>());
  } else if (node.body->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.body->As<Parenthetical>());
  } else if (node.body->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.body->As<NewExpression>());
  } else if (node.body->type == AST::NodeType::DELETE) {
    VisitDeleteExpression(*node.body->As<DeleteExpression>());
  } else if (node.body->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.body->As<BinaryExpression>());
  } else if (node.body->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.body->As<UnaryPrefixExpression>());
  } else if (node.body->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.body->As<UnaryPostfixExpression>());
  } else if (node.body->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.body->As<FunctionCallExpression>());
  } else if (node.body->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.body->As<TernaryExpression>());
  } else if (node.body->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.body->As<SizeofExpression>());
  } else if (node.body->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.body->As<AlignofExpression>());
  } else if (node.body->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.body->As<CastExpression>());
  } else if (node.body->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.body->As<DeclarationStatement>());
  } else if (node.body->type == AST::NodeType::BREAK) {
    VisitBreakStatement(*node.body->As<BreakStatement>());
  } else if (node.body->type == AST::NodeType::CONTINUE) {
    VisitContinueStatement(*node.body->As<ContinueStatement>());
  } else if (node.body->type == AST::NodeType::RETURN) {
    VisitReturnStatement(*node.body->As<ReturnStatement>());
  } else if (node.body->type == AST::NodeType::IF) {
    VisitIfStatement(*node.body->As<IfStatement>());
  } else if (node.body->type == AST::NodeType::FOR) {
    VisitForLoop(*node.body->As<ForLoop>());
  } else if (node.body->type == AST::NodeType::SWITCH) {
    VisitSwitchStatement(*node.body->As<SwitchStatement>());
  } else if (node.body->type == AST::NodeType::WHILE) {
    VisitWhileLoop(*node.body->As<WhileLoop>());
  } else if (node.body->type == AST::NodeType::DO) {
    VisitDoLoop(*node.body->As<DoLoop>());
  }

  if (node.body->type != AST::NodeType::BLOCK && node.body->type != AST::NodeType::IF &&
      node.body->type != AST::NodeType::FOR && node.body->type != AST::NodeType::SWITCH &&
      node.body->type != AST::NodeType::WHILE && node.body->type != AST::NodeType::DO) {
    print(";");
  }
}

bool AST::Visitor::VisitDoLoop(DoLoop &node) {
  print("do");

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }

  if (node.body->type == AST::NodeType::BLOCK) {
    VisitCodeBlock(*node.body->As<CodeBlock>());
  } else if (node.body->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.body->As<AST::IdentifierAccess>());
  } else if (node.body->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.body->As<Parenthetical>());
  } else if (node.body->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.body->As<NewExpression>());
  } else if (node.body->type == AST::NodeType::DELETE) {
    VisitDeleteExpression(*node.body->As<DeleteExpression>());
  } else if (node.body->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.body->As<BinaryExpression>());
  } else if (node.body->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.body->As<UnaryPrefixExpression>());
  } else if (node.body->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.body->As<UnaryPostfixExpression>());
  } else if (node.body->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.body->As<FunctionCallExpression>());
  } else if (node.body->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.body->As<TernaryExpression>());
  } else if (node.body->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.body->As<SizeofExpression>());
  } else if (node.body->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.body->As<AlignofExpression>());
  } else if (node.body->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.body->As<CastExpression>());
  } else if (node.body->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.body->As<DeclarationStatement>());
  } else if (node.body->type == AST::NodeType::BREAK) {
    VisitBreakStatement(*node.body->As<BreakStatement>());
  } else if (node.body->type == AST::NodeType::CONTINUE) {
    VisitContinueStatement(*node.body->As<ContinueStatement>());
  } else if (node.body->type == AST::NodeType::RETURN) {
    VisitReturnStatement(*node.body->As<ReturnStatement>());
  } else if (node.body->type == AST::NodeType::IF) {
    VisitIfStatement(*node.body->As<IfStatement>());
  } else if (node.body->type == AST::NodeType::FOR) {
    VisitForLoop(*node.body->As<ForLoop>());
  } else if (node.body->type == AST::NodeType::SWITCH) {
    VisitSwitchStatement(*node.body->As<SwitchStatement>());
  } else if (node.body->type == AST::NodeType::WHILE) {
    VisitWhileLoop(*node.body->As<WhileLoop>());
  } else if (node.body->type == AST::NodeType::DO) {
    VisitDoLoop(*node.body->As<DoLoop>());
  }

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

  if (node.condition->type == AST::NodeType::IDENTIFIER) {
    VisitIdentifierAccess(*node.condition->As<AST::IdentifierAccess>());
  } else if (node.condition->type == AST::NodeType::LITERAL) {
    VisitLiteral(*node.condition->As<AST::Literal>());
  } else if (node.condition->type == AST::NodeType::PARENTHETICAL) {
    VisitParenthetical(*node.condition->As<Parenthetical>());
  } else if (node.condition->type == AST::NodeType::NEW) {
    VisitNewExpression(*node.condition->As<NewExpression>());
  } else if (node.condition->type == AST::NodeType::BINARY_EXPRESSION) {
    VisitBinaryExpression(*node.condition->As<BinaryExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    VisitUnaryPrefixExpression(*node.condition->As<UnaryPrefixExpression>());
  } else if (node.condition->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    VisitUnaryPostfixExpression(*node.condition->As<UnaryPostfixExpression>());
  } else if (node.condition->type == AST::NodeType::FUNCTION_CALL) {
    VisitFunctionCallExpression(*node.condition->As<FunctionCallExpression>());
  } else if (node.condition->type == AST::NodeType::TERNARY_EXPRESSION) {
    VisitTernaryExpression(*node.condition->As<TernaryExpression>());
  } else if (node.condition->type == AST::NodeType::SIZEOF) {
    VisitSizeofExpression(*node.condition->As<SizeofExpression>());
  } else if (node.condition->type == AST::NodeType::ALIGNOF) {
    VisitAlignofExpression(*node.condition->As<AlignofExpression>());
  } else if (node.condition->type == AST::NodeType::CAST) {
    VisitCastExpression(*node.condition->As<CastExpression>());
  } else if (node.condition->type == AST::NodeType::DECLARATION) {
    VisitDeclarationStatement(*node.condition->As<DeclarationStatement>());
  }

  if (node.is_until) {
    print(")");
  }

  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }

  print(";");
}
