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

  printed = Visit(node.expression);

  if (!printed) return false;
  print(")");

  return true;
}

bool AST::Visitor::VisitUnaryPostfixExpression(UnaryPostfixExpression &node) {
  bool printed = false;

  printed = Visit(node.operand);

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

  printed = Visit(node.operand);

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

  printed = Visit(node.left);

  if (!printed) return false;
  print(" " + node.operation.token + " ");

  printed = Visit(node.right);

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
    printed = Visit(arg);

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

  printed = Visit(node.condition);

  if (!printed) return false;
  print(" ? ");

  printed = Visit(node.true_expression);

  if (!printed) return false;
  print(" : ");

  printed = Visit(node.false_expression);

  return printed;
}

bool AST::Visitor::VisitReturnStatement(ReturnStatement &node) {
  bool printed = true;
  print("return ");

  if (node.expression) {
    printed = Visit(node.expression);
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

  printed = Visit(node.expr);

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
    printed = Visit(node.elements[0]);
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

    printed = Visit(expr);
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
    printed = Visit(stmt);

    if (!printed) return false;
    PrintSemiColon(stmt);
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

bool AST::Visitor::VisitIfStatement(IfStatement &node) {
  bool printed = false;

  print("if");
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print("(");
  }

  printed = Visit(node.condition);

  if (!printed) return false;
  if (node.condition->type != AST::NodeType::PARENTHETICAL) {
    print(")");
  }
  print(" ");

  printed = Visit(node.true_branch);

  if (!printed) return false;
  PrintSemiColon(node.true_branch);
  print(" ");

  if (node.false_branch) {
    print("else ");

    printed = Visit(node.false_branch);

    if (!printed) return false;

    PrintSemiColon(node.false_branch);
    print(" ");
  }
  return true;
}

bool AST::Visitor::VisitForLoop(ForLoop &node) {
  bool printed = false;
  print("for(");

  printed = Visit(node.assignment);

  if (!printed) return false;
  print("; ");

  printed = Visit(node.condition);

  if (!printed) return false;
  print("; ");

  printed = Visit(node.increment);

  if (!printed) return false;
  print(") ");

  printed = Visit(node.body);

  if (!printed) return false;
  PrintSemiColon(node.body);
  print(" ");

  return true;
}

bool AST::Visitor::VisitCaseStatement(CaseStatement &node) {
  bool printed = false;
  print("case ");

  printed = Visit(node.value);

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

  printed = Visit(node.expression);

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

  printed = Visit(node.condition);

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

  printed = Visit(node.body);

  if (!printed) return false;
  PrintSemiColon(node.body);
  return true;
}

bool AST::Visitor::VisitDoLoop(DoLoop &node) {
  bool printed = false;
  print("do");

  if (node.body->type != AST::NodeType::BLOCK) {
    print("{");
  }

  printed = Visit(node.body);

  if (!printed) return false;
  PrintSemiColon(node.body);

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

  printed = Visit(node.condition);

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
