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

#ifndef MATCHERS_H
#define MATCHERS_H

#include <gmock/gmock.h>
using namespace ::enigma::parsing;
using namespace ::testing;

extern std::string ExpectedMsg;

MATCHER_P2(IsDeclaration, decls, decl_type, "") {
  if (arg->type != AST::NodeType::DECLARATION) {
    ExpectedMsg = "From IsDeclaration Matcher: NodeType = DECLARATION\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *decl = arg->template As<AST::DeclarationStatement>();
  if (!decl) {
    ExpectedMsg += "From IsDeclaration Matcher: decl isn't nullptr\n";
    *result_listener << "got decl = nullptr\n";
    return false;
  }

  bool b1 = decl->storage_class == AST::DeclarationStatement::StorageClass::TEMPORARY,
       b2 = decl->declarations.size() == decls.size();

  if (!b1 || !b2) {
    ExpectedMsg = "From IsDeclaration Matcher: ";

    if (!b1) {
      ExpectedMsg += "StorageClass = TEMPORARY\n";
      *result_listener << "got StorageClass = " << AST::DeclarationStatement::StorageToString(decl->storage_class)
                       << "\n";
    }
    if (!b2) {
      ExpectedMsg += "DeclarationsSize = " + to_string(decls.size()) + "\n";
      *result_listener << "got DeclarationsSize = " << to_string(decl->declarations.size()) << "\n";
    }
  }

  bool b3 = true;
  for (size_t i = 0; i < decls.size(); i++) {
    auto &decli = decl->declarations[i].declarator->decl;
    b3 = b3 && decl->declarations[i].init != nullptr;
    b3 = b3 && decl->declarations[i].declarator->def == decl_type;
    b3 = b3 && decl->declarations[i].declarator->flags == 0;
    b3 = b3 && decli.name.content == decls[i];
    b3 = b3 && decli.components.size() == 0;
    if (!b3) {
      if (ExpectedMsg == "") ExpectedMsg = "From IsDeclaration Matcher: ";
      ExpectedMsg +=
          "Declaration [" + to_string(i) +
          "] has init != nullptr, def = jdi::builtin_type__int, flags = 0, name.content = " +  // modify type here
          decls[i] + ", components.size() = 0\n";
      *result_listener << " got Declaration [" << to_string(i) << "] has init "
                       << ((decl->declarations[i].init) ? "!=" : "=")
                       << " nullptr, def = jdi::builtin_type__int, flags = "  // and here
                       << to_string(decl->declarations[i].declarator->flags)
                       << ", name.content = " << decli.name.content
                       << ", components.size() = " << to_string(decli.components.size()) << "\n";
    }
  }

  return b1 && b2 && b3;
}

MATCHER_P3(IsCast, cast_kind, expr_type, type, "") {
  if (arg->type != AST::NodeType::CAST) {
    ExpectedMsg = "From IsCast Matcher: NodeType = CAST\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *cast = arg->template As<AST::CastExpression>();
  if (!cast) {
    ExpectedMsg += "From IsCast Matcher: cast isn't nullptr\n";
    *result_listener << "got cast = nullptr\n";
    return false;
  }

  auto *expr = cast->expr->template As<AST::Node>();
  if (!expr) {
    ExpectedMsg += "From IsCast Matcher: expr isn't nullptr\n";
    *result_listener << "got expr = nullptr\n";
    return false;
  }

  bool b1 = cast->ft.def == type, b2 = cast->ft.flags == 0, b3 = cast->ft.decl.components.size() == 0,
       b4 = cast->ft.decl.name.content == "", b5 = cast->ft.decl.has_nested_declarator == false,
       b6 = cast->kind == cast_kind, b7 = expr->type == expr_type;

  bool res = b1 && b2 && b3 && b4 && b5 && b6 && b7;

  if (!res) {
    ExpectedMsg = "From IsCast Matcher: ";

    if (!b2) {
      ExpectedMsg += "ft.flags = 0\n";
      *result_listener << "got ft.flags = " << to_string(cast->ft.flags) << "\n";
    }
    if (!b3) {
      ExpectedMsg += "ft.decl.components.size() = 0\n";
      *result_listener << "got ft.decl.components.size() = " << to_string(cast->ft.decl.components.size()) << "\n";
    }
    if (!b4) {
      ExpectedMsg += "ft.decl.name.content = \"\"\n";
      *result_listener << "got ft.decl.name.content = " << cast->ft.decl.name.content << "\n";
    }
    if (!b5) {
      ExpectedMsg += "ft.decl.has_nested_declarator = false\n";
      *result_listener << "got ft.decl.has_nested_declarator = " << to_string(cast->ft.decl.has_nested_declarator)
                       << "\n";
    }
    if (!b6) {
      ExpectedMsg += "cast->kind = " + AST::CastExpression::KindToString(cast_kind) + "\n";
      *result_listener << "got cast->kind = " << AST::CastExpression::KindToString(cast->kind) << "\n";
    }
    if (!b7) {
      ExpectedMsg += "expr->type = " + AST::NodeToString(expr_type) + "\n";
      *result_listener << "got expr->type = " << AST::NodeToString(expr->type) << "\n";
    }
  }

  return res;
}

MATCHER_P(IsIdentifier, iden, "") {
  if (arg->type != AST::NodeType::IDENTIFIER) {
    ExpectedMsg += "From IsIdentifier Matcher: NodeType = IDENTIFIER\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *iden_access = arg->template As<AST::IdentifierAccess>();
  if (!iden_access) {
    ExpectedMsg += "From IsIdentifier Matcher: iden_access isn't nullptr\n";
    *result_listener << "got iden_access = nullptr\n";
    return false;
  }

  bool b1 = iden_access->name.content == iden;
  if (!b1) {
    ExpectedMsg += "From IsIdentifier Matcher: ";
    ExpectedMsg += "Identifier = " + PrintToString(iden) + "\n";
    *result_listener << "got Identifier = " << arg->template As<AST::IdentifierAccess>()->name.content << "\n";
    return false;
  }

  return true;
}

MATCHER_P(IsLiteral, lit, "") {
  if (arg->type != AST::NodeType::LITERAL) {
    ExpectedMsg += "From IsLiteral Matcher: NodeType = LITERAL\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *literal = arg->template As<AST::Literal>();
  if (!literal) {
    ExpectedMsg += "From IsLiteral Matcher: literal isn't nullptr\n";
    *result_listener << "got literal = nullptr\n";
    return false;
  }

  bool b1 = std::get<std::string>(literal->value.value) == lit;
  if (!b1) {
    ExpectedMsg += "From IsLiteral Matcher: ";
    ExpectedMsg += "Literal = " + PrintToString(lit) + "\n";
    *result_listener << "got Literal = " << std::get<std::string>(arg->template As<AST::Literal>()->value.value)
                     << "\n";
    return false;
  }

  return true;
}

MATCHER_P3(IsBinaryOperation, op, M1, M2, "") {
  if (arg->type != AST::NodeType::BINARY_EXPRESSION) {
    ExpectedMsg += "From IsBinaryOperation Matcher: NodeType = BINARY_EXPRESSION\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *binary = arg->template As<AST::BinaryExpression>();
  if (!binary) {
    ExpectedMsg += "From IsBinaryOperation Matcher: binary isn't nullptr\n";
    *result_listener << "got binary = nullptr\n";
    return false;
  }

  bool b1 = binary->operation.type == op, b2 = ExplainMatchResult(M1, binary->left, result_listener),
       b3 = ExplainMatchResult(M2, binary->right, result_listener);

  bool res = b1 && b2 && b3;
  if (!b1) {
    ExpectedMsg += "From IsBinaryOperation Matcher: ";
    ExpectedMsg += "Operation = " + ToString(op) + "\n";
    *result_listener << "got Operation = " << ToString(binary->operation.type) << "\n";
  }

  return res;
}

MATCHER_P2(IsUnaryPostfixOperator, op, M1, "") {
  if (arg->type != AST::NodeType::UNARY_POSTFIX_EXPRESSION) {
    ExpectedMsg = "From IsUnaryPostfixOperator Matcher: NodeType = UNARY_POSTFIX_EXPRESSION\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *unary = arg->template As<AST::UnaryPostfixExpression>();
  if (!unary) {
    ExpectedMsg += "From IsUnaryPostfixOperator Matcher: unary isn't nullptr\n";
    *result_listener << "got unary = nullptr\n";
    return false;
  }

  bool b1 = unary->operation == op, b2 = ExplainMatchResult(M1, unary->operand, result_listener);

  bool res = b1 && b2;
  if (!b1) {
    ExpectedMsg = "From IsUnaryPostfixOperator Matcher: ";
    ExpectedMsg += "Operation = " + ToString(op) + "\n";
    *result_listener << "got Operation = " << ToString(unary->operation) << "\n";
  }

  return res;
}

MATCHER_P2(IsUnaryPrefixOperator, op, M1, "") {
  if (arg->type != AST::NodeType::UNARY_PREFIX_EXPRESSION) {
    ExpectedMsg = "From IsUnaryPrefixOperator Matcher: NodeType = UNARY_PREFIX_EXPRESSION\n";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *unary = arg->template As<AST::UnaryPrefixExpression>();
  if (!unary) {
    ExpectedMsg += "From IsUnaryPrefixOperator Matcher: unary isn't nullptr\n";
    *result_listener << "got unary = nullptr\n";
    return false;
  }

  bool b1 = unary->operation == op, b2 = ExplainMatchResult(M1, unary->operand, result_listener);

  bool res = b1 && b2;
  if (!b1) {
    ExpectedMsg = "From IsUnaryPrefixOperator Matcher: ";
    ExpectedMsg += "Operation = " + ToString(op) + "\n";
    *result_listener << "got Operation = " << ToString(unary->operation) << "\n";
  }

  return res;
}

MATCHER_P(IsStatementBlock, stateSize, "") {
  if (arg->type != AST::NodeType::BLOCK) {
    ExpectedMsg = "From IsStatementBlock Matcher: NodeType = BLOCK";
    *result_listener << "got NodeType = " << AST::NodeToString(arg->type) << "\n";
    return false;
  }

  auto *block = arg->template As<AST::CodeBlock>();
  if (!block) {
    ExpectedMsg += "From IsStatementBlock Matcher: block isn't nullptr\n";
    *result_listener << "got block = nullptr\n";
    return false;
  }

  bool b1 = block->statements.size() == size_t(stateSize);

  if (!b1) {
    ExpectedMsg = "From IsStatementBlock Matcher: ";
    ExpectedMsg = "IsStatementBlock Matcher: Statements Size = " + to_string(stateSize);
    *result_listener << "Statements Size = " << to_string(block->statements.size()) << "\n";
    return false;
  }

  return true;
}

MATCHER_P4(IsForLoopWithChildren, M1, M2, M3, M4, ExpectedMsg) {
  auto *assign = arg->assignment.get();
  auto *binary = arg->condition.get();
  auto *unary = arg->increment.get();
  auto *block = arg->body.get();

  return ExplainMatchResult(M1, assign, result_listener) && ExplainMatchResult(M2, binary, result_listener) &&
         ExplainMatchResult(M3, unary, result_listener) && ExplainMatchResult(M4, block, result_listener);
}

#endif
