#include <languages/lang_CPP.h>
#include <parsing/ast.h>
#include <parsing/parser.cpp>
#include "matchers.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::enigma::parsing;
using namespace ::testing;

class TestFailureErrorHandler : public ErrorHandler {
 public:
  void ReportError(CodeSnippet snippet, std::string_view error) final {
    ADD_FAILURE() << "Test reported an error at line " << snippet.line << ", position " << snippet.position << ": "
                  << error;
  }
  void ReportWarning(CodeSnippet snippet, std::string_view warning) final {
    ADD_FAILURE() << "Test reported a warning at line " << snippet.line << ", position " << snippet.position << ": "
                  << warning;
  }
};

struct ParserTester {
  TestFailureErrorHandler herr;
  const ParseContext *context;
  Lexer lexer;
  lang_CPP cpp{};
  AstBuilder builder;

  AstBuilder *operator->() { return &builder; }

  explicit ParserTester(std::string code, bool use_cpp = false)
      : context(&ParseContext::ForTesting(use_cpp)), lexer(std::move(code), context, &herr), builder{&lexer, &herr} {}
};

void assert_identifier_is(AST::Node *node, std::string_view name) {
  ASSERT_EQ(node->type, AST::NodeType::IDENTIFIER);
  ASSERT_EQ(node->As<AST::IdentifierAccess>()->name.content, name);
}

TEST(ParserTest, Basics) {
  ParserTester test{"(x ? y : z ? a : (z[5](6)));"};

  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::PARENTHETICAL);

  auto *expr = node->As<AST::Parenthetical>()->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::TERNARY_EXPRESSION);

  auto *ternary = expr->As<AST::TernaryExpression>();
  auto *cond = ternary->condition.get();
  auto *true_ = ternary->true_expression.get();
  auto *false_ = ternary->false_expression.get();
  assert_identifier_is(cond, "x");
  assert_identifier_is(true_, "y");

  ASSERT_EQ(false_->type, AST::NodeType::TERNARY_EXPRESSION);

  ternary = false_->As<AST::TernaryExpression>();

  cond = ternary->condition.get();
  true_ = ternary->true_expression.get();
  false_ = ternary->false_expression.get();

  assert_identifier_is(cond, "z");
  assert_identifier_is(true_, "a");

  ASSERT_EQ(false_->type, AST::NodeType::PARENTHETICAL);
  expr = false_->As<AST::Parenthetical>()->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::FUNCTION_CALL);
  auto *function = expr->As<AST::FunctionCallExpression>();
  auto *called = function->function.get();
  auto *args = &function->arguments;

  ASSERT_EQ(called->type, AST::NodeType::BINARY_EXPRESSION);

  auto *bin = called->As<AST::BinaryExpression>();
  ASSERT_EQ(bin->operation, TokenType::TT_BEGINBRACKET);
  assert_identifier_is(bin->left.get(), "z");

  ASSERT_EQ(bin->right->type, AST::NodeType::LITERAL);
  auto *right = bin->right->As<AST::Literal>();
  ASSERT_EQ(std::get<std::string>(right->value.value), "5");

  ASSERT_EQ(args->size(), 1);
  auto *arg = (*args)[0].get();
  ASSERT_EQ(arg->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(arg->As<AST::Literal>()->value.value), "6");
}

TEST(ParserTest, Basics_NoSemicolon) {
  ParserTester test{"(x ? y : z ? a : (z[5](6)))"};

  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::PARENTHETICAL);

  auto *expr = node->As<AST::Parenthetical>()->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::TERNARY_EXPRESSION);

  auto *ternary = expr->As<AST::TernaryExpression>();
  auto *cond = ternary->condition.get();
  auto *true_ = ternary->true_expression.get();
  auto *false_ = ternary->false_expression.get();
  assert_identifier_is(cond, "x");
  assert_identifier_is(true_, "y");

  ASSERT_EQ(false_->type, AST::NodeType::TERNARY_EXPRESSION);

  ternary = false_->As<AST::TernaryExpression>();

  cond = ternary->condition.get();
  true_ = ternary->true_expression.get();
  false_ = ternary->false_expression.get();

  assert_identifier_is(cond, "z");
  assert_identifier_is(true_, "a");

  ASSERT_EQ(false_->type, AST::NodeType::PARENTHETICAL);
  expr = false_->As<AST::Parenthetical>()->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::FUNCTION_CALL);
  auto *function = expr->As<AST::FunctionCallExpression>();
  auto *called = function->function.get();
  auto *args = &function->arguments;

  ASSERT_EQ(called->type, AST::NodeType::BINARY_EXPRESSION);

  auto *bin = called->As<AST::BinaryExpression>();
  ASSERT_EQ(bin->operation, TokenType::TT_BEGINBRACKET);
  assert_identifier_is(bin->left.get(), "z");

  ASSERT_EQ(bin->right->type, AST::NodeType::LITERAL);
  auto *right = bin->right->As<AST::Literal>();
  ASSERT_EQ(std::get<std::string>(right->value.value), "5");

  ASSERT_EQ(args->size(), 1);
  auto *arg = (*args)[0].get();
  ASSERT_EQ(arg->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(arg->As<AST::Literal>()->value.value), "6");
}

TEST(ParserTest, SizeofExpression) {
  ParserTester test{"sizeof 5"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::SIZEOF);
  auto *sizeof_ = expr->As<AST::SizeofExpression>();
  ASSERT_EQ(sizeof_->kind, AST::SizeofExpression::Kind::EXPR);
  ASSERT_TRUE(std::holds_alternative<AST::PNode>(sizeof_->argument));

  auto &value = std::get<AST::PNode>(sizeof_->argument);
  ASSERT_EQ(value->type, AST::NodeType::LITERAL);
  auto *literal = value->As<AST::Literal>();
  ASSERT_EQ(std::get<std::string>(literal->value.value), "5");
}

TEST(ParserTest, SizeofVariadic) {
  ParserTester test{"sizeof...(ident)"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::SIZEOF);
  auto *sizeof_ = expr->As<AST::SizeofExpression>();
  ASSERT_EQ(sizeof_->kind, AST::SizeofExpression::Kind::VARIADIC);
  ASSERT_TRUE(std::holds_alternative<std::string>(sizeof_->argument));

  auto &value = std::get<std::string>(sizeof_->argument);
  ASSERT_EQ(value, "ident");
}

TEST(ParserTest, SizeofType) {
  ParserTester test{"sizeof(const volatile unsigned long long int **(*)[10])"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::SIZEOF);
  auto *sizeof_ = expr->As<AST::SizeofExpression>();
  ASSERT_EQ(sizeof_->kind, AST::SizeofExpression::Kind::TYPE);
  ASSERT_TRUE(std::holds_alternative<FullType>(sizeof_->argument));

  auto &value = std::get<FullType>(sizeof_->argument);
  auto has_value = [&value](jdi::typeflag *builtin) -> bool { return (value.flags & builtin->mask) == builtin->value; };

  ASSERT_TRUE(has_value(jdi::builtin_flag__const));
  ASSERT_TRUE(has_value(jdi::builtin_flag__volatile));
  ASSERT_TRUE(has_value(jdi::builtin_flag__unsigned));
  ASSERT_TRUE(has_value(jdi::builtin_flag__long_long));
  ASSERT_EQ(value.def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  ASSERT_EQ(value.def->name, "int");
  ASSERT_EQ(value.decl.components.size(), 3);
  jdi::ref_stack stack;
  value.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
}

TEST(ParserTest, AlignofType) {
  ParserTester test{"alignof(const volatile unsigned long long *)"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::ALIGNOF);
  auto *alignof_ = expr->As<AST::AlignofExpression>();
  auto &value = alignof_->ft;
  auto has_value = [&value](jdi::typeflag *builtin) -> bool { return (value.flags & builtin->mask) == builtin->value; };
  ASSERT_TRUE(has_value(jdi::builtin_flag__const));
  ASSERT_TRUE(has_value(jdi::builtin_flag__volatile));
  ASSERT_TRUE(has_value(jdi::builtin_flag__unsigned));
  ASSERT_TRUE(has_value(jdi::builtin_flag__long_long));
  ASSERT_EQ(value.def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  ASSERT_EQ(value.def->name, "int");
  ASSERT_EQ(value.decl.components.size(), 1);
  jdi::ref_stack stack;
  value.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
}

bool contains_flag(FullType *ft, std::size_t decflag) { return (ft->flags & decflag) == decflag; }

bool def_type_is(FullType *ft, std::size_t dectype) { return (ft->def->flags & dectype) == dectype; }

TEST(ParserTest, TypeSpecifierAndDeclarator) {
  ParserTester test{"const unsigned int ****(***)[10]"};
  FullType ft = test->TryParseTypeID();
  EXPECT_TRUE(def_type_is(&ft, jdi::DEF_TYPENAME));
  EXPECT_TRUE(contains_flag(&ft, jdi::builtin_flag__const->value));
  EXPECT_TRUE(contains_flag(&ft, jdi::builtin_flag__unsigned->value));
  jdi::ref_stack stack;
  ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_ARRAYBOUND);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);
}

/*
TEST(ParserTest, Declarator_1) {
  FullType ft2;
  ParserTester test2{"const unsigned int **(*var::*y)[10]"};
  test2->TryParseTypeSpecifierSeq(&ft2);
  test2->TryParseDeclarator(&ft2, AST::DeclaratorType::NON_ABSTRACT);

  jdi::ref_stack stack;
  ft2.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(ft2.decl.name.content, "y");
  ASSERT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ((first++)->type, jdi::ref_stack::RT_MEMBER_POINTER);
  ASSERT_EQ((first++)->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(test2.lexer.ReadToken().type, TT_ENDOFCODE);
}
*/

TEST(ParserTest, Declarator_2) {
  ParserTester test3{"int ((*a)(int (*x)(int x), int (*)[10]))(int);"};
  auto node = test3->TryParseStatement();
  EXPECT_EQ(test3.lexer.ReadToken().type, TT_ENDOFCODE);
}

TEST(ParserTest, Declarator_2_NoSemicolon) {
  ParserTester test3{"int ((*a)(int (*x)(int x), int (*)[10]))(int)"};
  auto node = test3->TryParseStatement();
  EXPECT_EQ(test3.lexer.ReadToken().type, TT_ENDOFCODE);
}

TEST(ParserTest, Declarator_3) {
  ParserTester test{"int *(*(*a)[10][12])[15];"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decls = node->As<AST::DeclarationStatement>();
  ASSERT_EQ(decls->declarations.size(), 1);

  ASSERT_EQ(decls->declarations[0].init, nullptr);
  auto &decl1 = decls->declarations[0].declarator->decl;
  ASSERT_EQ(decl1.name.content, "a");
  ASSERT_EQ(decl1.components.size(), 2);

  ASSERT_EQ(decl1.components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &ptr = decl1.components[0].as<PointerNode>();
  ASSERT_EQ(ptr.is_const, false);
  ASSERT_EQ(ptr.is_volatile, false);
  ASSERT_EQ(ptr.class_def, nullptr);

  ASSERT_EQ(decl1.components[1].kind, DeclaratorNode::Kind::NESTED);
  ASSERT_TRUE(decl1.components[1].as<NestedNode>().is<std::unique_ptr<Declarator>>());
  auto *nested = decl1.components[1].as<NestedNode>().as<std::unique_ptr<Declarator>>().get();
  ASSERT_EQ(nested->components.size(), 3);

  ASSERT_EQ(nested->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &nested_ptr = nested->components[0].as<PointerNode>();
  ASSERT_EQ(nested_ptr.is_const, false);
  ASSERT_EQ(nested_ptr.is_volatile, false);
  ASSERT_EQ(nested_ptr.class_def, nullptr);

  ASSERT_EQ(nested->components[1].kind, DeclaratorNode::Kind::NESTED);
  ASSERT_TRUE(nested->components[1].as<NestedNode>().is<std::unique_ptr<Declarator>>());
  auto *nested_nested = nested->components[1].as<NestedNode>().as<std::unique_ptr<Declarator>>().get();
  ASSERT_EQ(nested_nested->components.size(), 3);
  ASSERT_EQ(nested_nested->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &nested_nested_ptr = nested_nested->components[0].as<PointerNode>();
  ASSERT_EQ(nested_nested_ptr.is_const, false);
  ASSERT_EQ(nested_nested_ptr.is_volatile, false);
  ASSERT_EQ(nested_nested_ptr.class_def, nullptr);
  ASSERT_EQ(nested_nested->components[1].kind, DeclaratorNode::Kind::ARRAY_BOUND);
  ASSERT_EQ(nested_nested->components[2].kind, DeclaratorNode::Kind::ARRAY_BOUND);

  ASSERT_EQ(nested->components[2].kind, DeclaratorNode::Kind::ARRAY_BOUND);
}

TEST(ParserTest, Declarator_3_NoSemicolon) {
  ParserTester test{"int *(*(*a)[10][12])[15]"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decls = node->As<AST::DeclarationStatement>();
  ASSERT_EQ(decls->declarations.size(), 1);

  ASSERT_EQ(decls->declarations[0].init, nullptr);
  auto &decl1 = decls->declarations[0].declarator->decl;
  ASSERT_EQ(decl1.name.content, "a");
  ASSERT_EQ(decl1.components.size(), 2);

  ASSERT_EQ(decl1.components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &ptr = decl1.components[0].as<PointerNode>();
  ASSERT_EQ(ptr.is_const, false);
  ASSERT_EQ(ptr.is_volatile, false);
  ASSERT_EQ(ptr.class_def, nullptr);

  ASSERT_EQ(decl1.components[1].kind, DeclaratorNode::Kind::NESTED);
  ASSERT_TRUE(decl1.components[1].as<NestedNode>().is<std::unique_ptr<Declarator>>());
  auto *nested = decl1.components[1].as<NestedNode>().as<std::unique_ptr<Declarator>>().get();
  ASSERT_EQ(nested->components.size(), 3);

  ASSERT_EQ(nested->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &nested_ptr = nested->components[0].as<PointerNode>();
  ASSERT_EQ(nested_ptr.is_const, false);
  ASSERT_EQ(nested_ptr.is_volatile, false);
  ASSERT_EQ(nested_ptr.class_def, nullptr);

  ASSERT_EQ(nested->components[1].kind, DeclaratorNode::Kind::NESTED);
  ASSERT_TRUE(nested->components[1].as<NestedNode>().is<std::unique_ptr<Declarator>>());
  auto *nested_nested = nested->components[1].as<NestedNode>().as<std::unique_ptr<Declarator>>().get();
  ASSERT_EQ(nested_nested->components.size(), 3);
  ASSERT_EQ(nested_nested->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &nested_nested_ptr = nested_nested->components[0].as<PointerNode>();
  ASSERT_EQ(nested_nested_ptr.is_const, false);
  ASSERT_EQ(nested_nested_ptr.is_volatile, false);
  ASSERT_EQ(nested_nested_ptr.class_def, nullptr);
  ASSERT_EQ(nested_nested->components[1].kind, DeclaratorNode::Kind::ARRAY_BOUND);
  ASSERT_EQ(nested_nested->components[2].kind, DeclaratorNode::Kind::ARRAY_BOUND);

  ASSERT_EQ(nested->components[2].kind, DeclaratorNode::Kind::ARRAY_BOUND);
}

TEST(ParserTest, Declarator_4) {
  ParserTester test{"int *(*(*a)[10][12])[15];"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decls = node->As<AST::DeclarationStatement>();
  ASSERT_EQ(decls->declarations.size(), 1);

  ASSERT_EQ(decls->declarations[0].init, nullptr);
  auto &decl1 = decls->declarations[0].declarator->decl;
  ASSERT_EQ(decl1.name.content, "a");
  ASSERT_EQ(decl1.components.size(), 2);

  jdi::ref_stack stack;
  decl1.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
}

TEST(ParserTest, Declarator_4_NoSemicolon) {
  ParserTester test{"int *(*(*a)[10][12])[15]"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decls = node->As<AST::DeclarationStatement>();
  ASSERT_EQ(decls->declarations.size(), 1);

  ASSERT_EQ(decls->declarations[0].init, nullptr);
  auto &decl1 = decls->declarations[0].declarator->decl;
  ASSERT_EQ(decl1.name.content, "a");
  ASSERT_EQ(decl1.components.size(), 2);

  jdi::ref_stack stack;
  decl1.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
}

TEST(ParserTest, Declaration) {
  ParserTester test{"const unsigned *(*x)[10] = nullptr;"};
  auto node = test->TryParseStatement();
  EXPECT_EQ(test->current_token().type, TT_SEMICOLON);
  EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);
}

TEST(ParserTest, Declaration_NoSemicolon) {
  ParserTester test{"const unsigned *(*x)[10] = nullptr"};
  auto node = test->TryParseStatement();
  EXPECT_EQ(test->current_token().type, TT_ENDOFCODE);
  EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);
}

TEST(ParserTest, Declarations) {
  ParserTester test{"int *x = nullptr, y, (*z)(int x, int) = &y;"};

  auto node = test->TryParseStatement();
  EXPECT_EQ(test->current_token().type, TT_SEMICOLON);
  EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decls = reinterpret_cast<AST::DeclarationStatement *>(node.get());
  EXPECT_EQ(decls->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);

  EXPECT_EQ(decls->declarations.size(), 3);
  EXPECT_NE(decls->declarations[0].init, nullptr);
  EXPECT_EQ(decls->declarations[0].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  EXPECT_EQ(decls->declarations[0].declarator->decl.components.begin()->kind, DeclaratorNode::Kind::POINTER_TO);

  EXPECT_EQ(decls->declarations[1].init, nullptr);
  EXPECT_EQ(decls->declarations[1].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  EXPECT_EQ(decls->declarations[1].declarator->decl.components.size(), 0);

  EXPECT_NE(decls->declarations[2].init, nullptr);
  EXPECT_EQ(decls->declarations[2].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  EXPECT_EQ(decls->declarations[2].declarator->decl.components.size(), 1);
}

TEST(ParserTest, Declarations_NoSemicolon) {
  ParserTester test{"int *x = nullptr, y, (*z)(int x, int) = &y"};

  auto node = test->TryParseStatement();
  EXPECT_EQ(test->current_token().type, TT_ENDOFCODE);
  EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decls = reinterpret_cast<AST::DeclarationStatement *>(node.get());
  EXPECT_EQ(decls->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);

  EXPECT_EQ(decls->declarations.size(), 3);
  EXPECT_NE(decls->declarations[0].init, nullptr);
  EXPECT_EQ(decls->declarations[0].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  EXPECT_EQ(decls->declarations[0].declarator->decl.components.begin()->kind, DeclaratorNode::Kind::POINTER_TO);

  EXPECT_EQ(decls->declarations[1].init, nullptr);
  EXPECT_EQ(decls->declarations[1].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  EXPECT_EQ(decls->declarations[1].declarator->decl.components.size(), 0);

  EXPECT_NE(decls->declarations[2].init, nullptr);
  EXPECT_EQ(decls->declarations[2].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
  EXPECT_EQ(decls->declarations[2].declarator->decl.components.size(), 1);
}

void check_placement(AST::NewExpression *new_) {
  ASSERT_NE(new_->placement, nullptr);
  auto *placement = new_->placement.get();
  ASSERT_EQ(placement->kind, AST::Initializer::Kind::PLACEMENT_NEW);
  ASSERT_TRUE(std::holds_alternative<AST::BraceOrParenInitNode>(placement->initializer));
  auto *placement_args = std::get<AST::BraceOrParenInitNode>(placement->initializer).get();
  ASSERT_EQ(placement_args->kind, AST::BraceOrParenInitializer::Kind::PAREN_INIT);
  ASSERT_EQ(placement_args->values.size(), 1);
  ASSERT_EQ(placement_args->values[0].second->kind, AST::Initializer::Kind::ASSIGN_EXPR);
  ASSERT_TRUE(std::holds_alternative<AST::AssignmentInitNode>(placement_args->values[0].second->initializer));
  auto *placement_arg = std::get<AST::AssignmentInitNode>(placement_args->values[0].second->initializer).get();
  ASSERT_EQ(placement_arg->kind, AST::AssignmentInitializer::Kind::EXPR);
  auto *placement_expr = std::get<std::unique_ptr<AST::Node>>(placement_arg->initializer).get();
  assert_identifier_is(placement_expr, "nullptr");
}

void check_initializer(AST::NewExpression *new_, AST::BraceOrParenInitializer::Kind kind) {
  ASSERT_NE(new_->initializer, nullptr);
  auto *init = new_->initializer.get();
  ASSERT_EQ(init->kind, AST::Initializer::Kind::BRACE_INIT);
  ASSERT_TRUE(std::holds_alternative<AST::BraceOrParenInitNode>(init->initializer));
  auto *brace = std::get<AST::BraceOrParenInitNode>(init->initializer).get();
  ASSERT_EQ(brace->kind, kind);
  ASSERT_EQ(brace->values.size(), 5);
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(brace->values[i].first, "");
    ASSERT_EQ(brace->values[i].second->kind, AST::Initializer::Kind::ASSIGN_EXPR);
    ASSERT_TRUE(std::holds_alternative<AST::AssignmentInitNode>(brace->values[i].second->initializer));
    auto *assign = std::get<AST::AssignmentInitNode>(brace->values[i].second->initializer).get();
    ASSERT_EQ(assign->kind, AST::AssignmentInitializer::Kind::EXPR);
    ASSERT_TRUE(std::holds_alternative<std::unique_ptr<AST::Node>>(assign->initializer));
    auto *expr = std::get<std::unique_ptr<AST::Node>>(assign->initializer).get();
    ASSERT_EQ(expr->type, AST::NodeType::LITERAL);
    ASSERT_EQ(std::get<std::string>(expr->As<AST::Literal>()->value.value), std::to_string(i + 1));
  }
}

TEST(ParserTest, NewExpression_1) {
  ParserTester test{"new (nullptr) int[]{1, 2, 3, 4, 5};"};
  auto node = test->TryParseStatement();

  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_FALSE(new_->is_global);
  ASSERT_TRUE(new_->is_array);

  check_placement(new_);

  EXPECT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 1);
  ASSERT_EQ(new_->ft.decl.components.begin()->kind, DeclaratorNode::Kind::ARRAY_BOUND);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::BRACE_INIT);
}

TEST(ParserTest, NewExpression_1_NoSemicolon) {
  ParserTester test{"new (nullptr) int[]{1, 2, 3, 4, 5}"};
  auto node = test->TryParseStatement();

  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_FALSE(new_->is_global);
  ASSERT_TRUE(new_->is_array);

  check_placement(new_);

  EXPECT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 1);
  ASSERT_EQ(new_->ft.decl.components.begin()->kind, DeclaratorNode::Kind::ARRAY_BOUND);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::BRACE_INIT);
}

TEST(ParserTest, NewExpression_2) {
  ParserTester test{"::new int[][15]{1, 2, 3, 4, 5};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_TRUE(new_->is_global);
  ASSERT_TRUE(new_->is_array);

  ASSERT_EQ(new_->placement, nullptr);
  EXPECT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 2);
  jdi::ref_stack stack;
  new_->ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::BRACE_INIT);
}

TEST(ParserTest, NewExpression_2_NoSemiconlon) {
  ParserTester test{"::new int[][15]{1, 2, 3, 4, 5}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_TRUE(new_->is_global);
  ASSERT_TRUE(new_->is_array);

  ASSERT_EQ(new_->placement, nullptr);
  EXPECT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 2);
  jdi::ref_stack stack;
  new_->ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::BRACE_INIT);
}

TEST(ParserTest, NewExpression_3) {
  ParserTester test{"::new (nullptr) (int *(**)[10])(1, 2, 3, 4, 5);"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_TRUE(new_->is_global);
  ASSERT_FALSE(new_->is_array);

  check_placement(new_);

  ASSERT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 2);
  jdi::ref_stack stack;
  new_->ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::PAREN_INIT);
}

TEST(ParserTest, NewExpression_3_NoSemicolon) {
  ParserTester test{"::new (nullptr) (int *(**)[10])(1, 2, 3, 4, 5)"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_TRUE(new_->is_global);
  ASSERT_FALSE(new_->is_array);

  check_placement(new_);

  ASSERT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 2);
  jdi::ref_stack stack;
  new_->ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::PAREN_INIT);
}

TEST(ParserTest, NewExpression_4) {
  ParserTester test{"new (int *(**)[10]);"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_FALSE(new_->is_global);
  ASSERT_FALSE(new_->is_array);

  ASSERT_EQ(new_->placement, nullptr);
  ASSERT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 2);
  jdi::ref_stack stack;
  new_->ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
}

TEST(ParserTest, NewExpression_4_NoSemicolon) {
  ParserTester test{"new (int *(**)[10])"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::NEW);
  auto *new_ = reinterpret_cast<AST::NewExpression *>(node.get());
  ASSERT_FALSE(new_->is_global);
  ASSERT_FALSE(new_->is_array);

  ASSERT_EQ(new_->placement, nullptr);
  ASSERT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 2);
  jdi::ref_stack stack;
  new_->ft.decl.to_jdi_refstack(stack);
  auto first = stack.begin();
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
  ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
}

TEST(ParserTest, DeleteExpression_1) {
  ParserTester test{"delete x;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_FALSE(delete_->is_global);
  ASSERT_FALSE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_1_NoSemicolon) {
  ParserTester test{"delete x"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_FALSE(delete_->is_global);
  ASSERT_FALSE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_2) {
  ParserTester test{"::delete x;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_TRUE(delete_->is_global);
  ASSERT_FALSE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_2_NoSemicolon) {
  ParserTester test{"::delete x"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_TRUE(delete_->is_global);
  ASSERT_FALSE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_3) {
  ParserTester test{"delete[] x;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_FALSE(delete_->is_global);
  ASSERT_TRUE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_3_NoSemiColon) {
  ParserTester test{"delete[] x"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_FALSE(delete_->is_global);
  ASSERT_TRUE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_4) {
  ParserTester test{"::delete[] x;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_TRUE(delete_->is_global);
  ASSERT_TRUE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, DeleteExpression_4_NoSemicolon) {
  ParserTester test{"::delete[] x"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DELETE);
  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(node.get());
  ASSERT_TRUE(delete_->is_global);
  ASSERT_TRUE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, SwitchStatement_1) {
  ParserTester test{"switch (5 * 6) { case 1: return 2; break 13; case 2: return 3; break; default: break;};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 3);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::CASE);
  auto *case1 = switch_->body->statements[0]->As<AST::CaseStatement>();

  ASSERT_EQ(case1->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(case1->value->As<AST::Literal>()->value.value), "1");

  ASSERT_EQ(case1->statements->statements.size(), 2);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::RETURN);
  ASSERT_EQ(case1->statements->statements[1]->type, AST::NodeType::BREAK);

  ASSERT_EQ(switch_->body->statements[1]->type, AST::NodeType::CASE);
  auto *case2 = switch_->body->statements[1]->As<AST::CaseStatement>();

  ASSERT_EQ(case2->value->type, AST::NodeType::LITERAL);

  ASSERT_EQ(case2->statements->statements.size(), 2);
  ASSERT_EQ(case2->statements->statements[0]->type, AST::NodeType::RETURN);
  ASSERT_EQ(case2->statements->statements[1]->type, AST::NodeType::BREAK);

  ASSERT_EQ(switch_->body->statements[2]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[2]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::BREAK);
}

TEST(ParserTest, SwitchStatement_1_NoSemicolon) {
  ParserTester test{"switch (5 * 6) { case 1: return 2 break case 2: return 3 break default: break};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 3);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::CASE);
  auto *case1 = switch_->body->statements[0]->As<AST::CaseStatement>();
  ASSERT_EQ(case1->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(case1->value->As<AST::Literal>()->value.value), "1");
  ASSERT_EQ(case1->statements->statements.size(), 2);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::RETURN);
  ASSERT_EQ(case1->statements->statements[1]->type, AST::NodeType::BREAK);

  ASSERT_EQ(switch_->body->statements[1]->type, AST::NodeType::CASE);
  auto *case2 = switch_->body->statements[1]->As<AST::CaseStatement>();
  ASSERT_EQ(case2->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(case2->value->As<AST::Literal>()->value.value), "2");
  ASSERT_EQ(case2->statements->statements.size(), 2);
  ASSERT_EQ(case2->statements->statements[0]->type, AST::NodeType::RETURN);
  ASSERT_EQ(case2->statements->statements[1]->type, AST::NodeType::BREAK);

  ASSERT_EQ(switch_->body->statements[2]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[2]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::BREAK);
}

TEST(ParserTest, SwitchStatement_2) {
  ParserTester test{"switch (1) { case 1: return 2; default: return \"test\";};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 2);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::CASE);
  auto *case1 = switch_->body->statements[0]->As<AST::CaseStatement>();
  ASSERT_EQ(case1->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(case1->value->As<AST::Literal>()->value.value), "1");
  ASSERT_EQ(case1->statements->statements.size(), 1);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::RETURN);

  ASSERT_EQ(switch_->body->statements[1]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[1]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::RETURN);
}

TEST(ParserTest, SwitchStatement_2_NoSemicolon) {
  ParserTester test{"switch (1) { case 1: return 2 default: return \"test\"};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 2);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::CASE);
  auto *case1 = switch_->body->statements[0]->As<AST::CaseStatement>();
  ASSERT_EQ(case1->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(case1->value->As<AST::Literal>()->value.value), "1");
  ASSERT_EQ(case1->statements->statements.size(), 1);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::RETURN);

  ASSERT_EQ(switch_->body->statements[1]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[1]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::RETURN);
}

TEST(ParserTest, SwitchStatement_3) {
  ParserTester test{"switch (1) { default: continue 12;};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[0]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::CONTINUE);
}

TEST(ParserTest, SwitchStatement_3_NoSemicolon) {
  ParserTester test{"switch (1) { default: continue 12};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[0]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::CONTINUE);
}

TEST(ParserTest, SwitchStatement_4) {
  ParserTester test{"switch (1) { default: delete [] x; return \"new test\";};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[0]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 2);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::DELETE);
  ASSERT_EQ(default_->statements->statements[1]->type, AST::NodeType::RETURN);

  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(default_->statements->statements[0].get());
  ASSERT_FALSE(delete_->is_global);
  ASSERT_TRUE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, SwitchStatement_4_NoSemicolon) {
  ParserTester test{"switch (1) { default: delete [] x return \"new test\"};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[0]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 2);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::DELETE);
  ASSERT_EQ(default_->statements->statements[1]->type, AST::NodeType::RETURN);

  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(default_->statements->statements[0].get());
  // replace
  ASSERT_FALSE(delete_->is_global);
  ASSERT_TRUE(delete_->is_array);

  assert_identifier_is(delete_->expression.get(), "x");
}

TEST(ParserTest, SwitchStatement_5) {
  ParserTester test{"switch (1) { default: new (nullptr) int[]{1, 2, 3, 4, 5}; return \"new test\";};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[0]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 2);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::NEW);
  ASSERT_EQ(default_->statements->statements[1]->type, AST::NodeType::RETURN);

  auto *new_ = reinterpret_cast<AST::NewExpression *>(default_->statements->statements[0].get());
  ASSERT_FALSE(new_->is_global);
  ASSERT_TRUE(new_->is_array);

  check_placement(new_);

  EXPECT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 1);
  ASSERT_EQ(new_->ft.decl.components.begin()->kind, DeclaratorNode::Kind::ARRAY_BOUND);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::BRACE_INIT);
}

TEST(ParserTest, SwitchStatement_5_NoSemicolon) {
  ParserTester test{"switch (1) { default: new (nullptr) int[]{1, 2, 3, 4, 5} return \"new test\"};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = node->As<AST::SwitchStatement>();
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = switch_->body->statements[0]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 2);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::NEW);
  ASSERT_EQ(default_->statements->statements[1]->type, AST::NodeType::RETURN);

  auto *new_ = reinterpret_cast<AST::NewExpression *>(default_->statements->statements[0].get());
  ASSERT_FALSE(new_->is_global);
  ASSERT_TRUE(new_->is_array);

  check_placement(new_);

  EXPECT_EQ(new_->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(new_->ft.decl.components.size(), 1);
  ASSERT_EQ(new_->ft.decl.components.begin()->kind, DeclaratorNode::Kind::ARRAY_BOUND);

  check_initializer(new_, AST::BraceOrParenInitializer::Kind::BRACE_INIT);
}

TEST(ParserTest, CodeBlock_1) {
  ParserTester test{"{ int x = 5 const int y = 6 float *(*z)[10] = nullptr foo(bar) }"};
  auto node = test->ParseCodeBlock();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  ASSERT_EQ(block->statements.size(), 4);
  ASSERT_EQ(block->statements[0]->type, AST::NodeType::DECLARATION);
  ASSERT_EQ(block->statements[1]->type, AST::NodeType::DECLARATION);
  ASSERT_EQ(block->statements[2]->type, AST::NodeType::DECLARATION);
  ASSERT_EQ(block->statements[3]->type, AST::NodeType::FUNCTION_CALL);
}

TEST(ParserTest, CodeBlock_2) {
  ParserTester test{"{{{}}}"};
  auto node = test->ParseCodeBlock();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  ASSERT_EQ(block->statements.size(), 1);
  ASSERT_EQ(block->statements[0]->type, AST::NodeType::BLOCK);

  auto *inner_block = block->statements[0]->As<AST::CodeBlock>();
  ASSERT_EQ(inner_block->statements.size(), 1);
  ASSERT_EQ(inner_block->statements[0]->type, AST::NodeType::BLOCK);
}

TEST(ParserTest, IfStatement_1) {
  ParserTester test{"if(3>2) j++; else --k;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::Parenthetical>();
  ASSERT_TRUE(cond);

  auto *expr = cond->expression->As<AST::BinaryExpression>();
  ASSERT_TRUE(expr);
  ASSERT_EQ(expr->operation, TT_GREATER);
  ASSERT_EQ(expr->left->type, AST::NodeType::LITERAL);
  ASSERT_EQ(expr->right->type, AST::NodeType::LITERAL);

  auto *true_branch = if_->true_branch->As<AST::UnaryPostfixExpression>();
  ASSERT_TRUE(true_branch);
  ASSERT_EQ(true_branch->operation, TT_INCREMENT);
  ASSERT_EQ(true_branch->operand->type, AST::NodeType::IDENTIFIER);

  auto *false_branch = if_->false_branch->As<AST::UnaryPrefixExpression>();
  ASSERT_TRUE(false_branch);
  ASSERT_EQ(false_branch->operation, TT_DECREMENT);
  ASSERT_EQ(false_branch->operand->type, AST::NodeType::IDENTIFIER);
}

TEST(ParserTest, IfStatement_1_NoSemicolon) {
  ParserTester test{"if(3>2) j++ else --k"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::Parenthetical>();
  ASSERT_TRUE(cond);

  auto *expr = cond->expression->As<AST::BinaryExpression>();
  ASSERT_TRUE(expr);
  ASSERT_EQ(expr->operation, TT_GREATER);
  ASSERT_EQ(expr->left->type, AST::NodeType::LITERAL);
  ASSERT_EQ(expr->right->type, AST::NodeType::LITERAL);

  auto *true_branch = if_->true_branch->As<AST::UnaryPostfixExpression>();
  ASSERT_TRUE(true_branch);
  ASSERT_EQ(true_branch->operation, TT_INCREMENT);
  ASSERT_EQ(true_branch->operand->type, AST::NodeType::IDENTIFIER);

  auto *false_branch = if_->false_branch->As<AST::UnaryPrefixExpression>();
  ASSERT_TRUE(false_branch);
  ASSERT_EQ(false_branch->operation, TT_DECREMENT);
  ASSERT_EQ(false_branch->operand->type, AST::NodeType::IDENTIFIER);
}

TEST(ParserTest, IfStatement_2) {
  ParserTester test{"if k k++;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::IdentifierAccess>();
  ASSERT_TRUE(cond);
  ASSERT_EQ(cond->name.content, "k");

  auto *true_branch = if_->true_branch->As<AST::UnaryPostfixExpression>();
  ASSERT_TRUE(true_branch);
  ASSERT_EQ(true_branch->operation, TT_INCREMENT);
  ASSERT_EQ(true_branch->operand->type, AST::NodeType::IDENTIFIER);

  ASSERT_FALSE(if_->false_branch);
}

TEST(ParserTest, IfStatement_2_NoSemicolon) {
  ParserTester test{"if k k++"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::IdentifierAccess>();
  ASSERT_TRUE(cond);
  ASSERT_EQ(cond->name.content, "k");

  auto *true_branch = if_->true_branch->As<AST::UnaryPostfixExpression>();
  ASSERT_TRUE(true_branch);
  ASSERT_EQ(true_branch->operation, TT_INCREMENT);
  ASSERT_EQ(true_branch->operand->type, AST::NodeType::IDENTIFIER);

  ASSERT_FALSE(if_->false_branch);
}

TEST(ParserTest, IfStatement_3) {
  ParserTester test{"if (true) { return 1; } else { return 2; }"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::Parenthetical>();
  ASSERT_TRUE(cond);

  auto *expr = cond->expression->As<AST::IdentifierAccess>();
  ASSERT_TRUE(expr);
  ASSERT_EQ(expr->name.content, "true");

  auto *true_branch = if_->true_branch->As<AST::CodeBlock>();
  ASSERT_TRUE(true_branch);
  ASSERT_EQ(true_branch->statements.size(), 1);
  ASSERT_EQ(true_branch->statements[0]->type, AST::NodeType::RETURN);

  auto *return_1 = true_branch->statements[0]->As<AST::ReturnStatement>();
  ASSERT_EQ(return_1->expression->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(return_1->expression->As<AST::Literal>()->value.value), "1");

  auto *false_branch = if_->false_branch->As<AST::CodeBlock>();
  ASSERT_TRUE(false_branch);
  ASSERT_EQ(false_branch->statements.size(), 1);
  ASSERT_EQ(false_branch->statements[0]->type, AST::NodeType::RETURN);

  auto *return_2 = false_branch->statements[0]->As<AST::ReturnStatement>();
  ASSERT_EQ(return_2->expression->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(return_2->expression->As<AST::Literal>()->value.value), "2");
}

TEST(ParserTest, IfStatement_3_NoSemicolon) {
  ParserTester test{"if (true) { return 1 } else { return 2 }"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::Parenthetical>();
  ASSERT_TRUE(cond);

  auto *expr = cond->expression->As<AST::IdentifierAccess>();
  ASSERT_TRUE(expr);
  ASSERT_EQ(expr->name.content, "true");

  auto *true_branch = if_->true_branch->As<AST::CodeBlock>();
  ASSERT_TRUE(true_branch);
  ASSERT_EQ(true_branch->statements.size(), 1);
  ASSERT_EQ(true_branch->statements[0]->type, AST::NodeType::RETURN);

  auto *return_1 = true_branch->statements[0]->As<AST::ReturnStatement>();
  ASSERT_EQ(return_1->expression->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(return_1->expression->As<AST::Literal>()->value.value), "1");

  auto *false_branch = if_->false_branch->As<AST::CodeBlock>();
  ASSERT_TRUE(false_branch);
  ASSERT_EQ(false_branch->statements.size(), 1);
  ASSERT_EQ(false_branch->statements[0]->type, AST::NodeType::RETURN);

  auto *return_2 = false_branch->statements[0]->As<AST::ReturnStatement>();
  ASSERT_EQ(return_2->expression->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(return_2->expression->As<AST::Literal>()->value.value), "2");
}

TEST(ParserTest, IfStatement_4) {
  ParserTester test{
      "if (false) for(int i=0;i<12;i++) {k++;} else switch(i){ case 1 : k--; case 2 : k+=3; default : k=0; }"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::Parenthetical>();
  ASSERT_TRUE(cond);

  auto *expr = cond->expression->As<AST::IdentifierAccess>();
  ASSERT_TRUE(expr);
  ASSERT_EQ(expr->name.content, "false");

  auto *true_branch = if_->true_branch->As<AST::ForLoop>();
  ASSERT_TRUE(true_branch);

  vector<std::string> decls = {"i"};
  ASSERT_THAT(true_branch,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                    IsBinaryOperation(TT_LESS, IsIdentifier("i"), IsLiteral("12")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("i")), IsStatementBlock(1)));

  auto *false_branch = if_->false_branch->As<AST::SwitchStatement>();
  ASSERT_TRUE(false_branch);
  ASSERT_EQ(false_branch->body->statements.size(), 3);
  ASSERT_EQ(false_branch->body->statements[0]->type, AST::NodeType::CASE);
  ASSERT_EQ(false_branch->body->statements[1]->type, AST::NodeType::CASE);
  ASSERT_EQ(false_branch->body->statements[2]->type, AST::NodeType::DEFAULT);

  auto *case1 = false_branch->body->statements[0]->As<AST::CaseStatement>();
  ASSERT_EQ(case1->statements->statements.size(), 1);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::UNARY_POSTFIX_EXPRESSION);
  ASSERT_EQ(case1->statements->statements[0]->As<AST::UnaryPostfixExpression>()->operation, TT_DECREMENT);

  auto *case2 = false_branch->body->statements[1]->As<AST::CaseStatement>();
  ASSERT_EQ(case2->statements->statements.size(), 1);
  ASSERT_EQ(case2->statements->statements[0]->type, AST::NodeType::BINARY_EXPRESSION);
  ASSERT_EQ(case2->statements->statements[0]->As<AST::BinaryExpression>()->operation, TT_ASSOP);

  auto *default_ = false_branch->body->statements[2]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::BINARY_EXPRESSION);

  auto *assignment = default_->statements->statements[0]->As<AST::BinaryExpression>();
  ASSERT_EQ(assignment->operation, TT_EQUALS);
  ASSERT_EQ(assignment->left->type, AST::NodeType::IDENTIFIER);
  ASSERT_EQ(assignment->right->type, AST::NodeType::LITERAL);

  ASSERT_EQ(assignment->left->As<AST::IdentifierAccess>()->name.content, "k");
  ASSERT_EQ(std::get<std::string>(assignment->right->As<AST::Literal>()->value.value), "0");
}

TEST(ParserTest, IfStatement_4_NoSemicolon) {
  ParserTester test{
      "if (false) for(int i=0;i<12;i++) {k++} else switch(i){ case 1 : k-- case 2 : k+=3 default : k=0 }"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::IF);
  auto *if_ = node->As<AST::IfStatement>();
  ASSERT_TRUE(if_);

  auto *cond = if_->condition->As<AST::Parenthetical>();
  ASSERT_TRUE(cond);

  auto *expr = cond->expression->As<AST::IdentifierAccess>();
  ASSERT_TRUE(expr);
  ASSERT_EQ(expr->name.content, "false");

  auto *true_branch = if_->true_branch->As<AST::ForLoop>();
  ASSERT_TRUE(true_branch);

  vector<std::string> decls = {"i"};
  ASSERT_THAT(true_branch,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                    IsBinaryOperation(TT_LESS, IsIdentifier("i"), IsLiteral("12")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("i")), IsStatementBlock(1)));

  auto *false_branch = if_->false_branch->As<AST::SwitchStatement>();
  ASSERT_TRUE(false_branch);
  ASSERT_EQ(false_branch->body->statements.size(), 3);
  ASSERT_EQ(false_branch->body->statements[0]->type, AST::NodeType::CASE);
  ASSERT_EQ(false_branch->body->statements[1]->type, AST::NodeType::CASE);
  ASSERT_EQ(false_branch->body->statements[2]->type, AST::NodeType::DEFAULT);

  auto *case1 = false_branch->body->statements[0]->As<AST::CaseStatement>();
  ASSERT_EQ(case1->statements->statements.size(), 1);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::UNARY_POSTFIX_EXPRESSION);
  ASSERT_EQ(case1->statements->statements[0]->As<AST::UnaryPostfixExpression>()->operation, TT_DECREMENT);

  auto *case2 = false_branch->body->statements[1]->As<AST::CaseStatement>();
  ASSERT_EQ(case2->statements->statements.size(), 1);
  ASSERT_EQ(case2->statements->statements[0]->type, AST::NodeType::BINARY_EXPRESSION);
  ASSERT_EQ(case2->statements->statements[0]->As<AST::BinaryExpression>()->operation, TT_ASSOP);

  auto *default_ = false_branch->body->statements[2]->As<AST::DefaultStatement>();
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::BINARY_EXPRESSION);

  auto *assignment = default_->statements->statements[0]->As<AST::BinaryExpression>();
  ASSERT_EQ(assignment->operation, TT_EQUALS);
  ASSERT_EQ(assignment->left->type, AST::NodeType::IDENTIFIER);
  ASSERT_EQ(assignment->right->type, AST::NodeType::LITERAL);

  ASSERT_EQ(assignment->left->As<AST::IdentifierAccess>()->name.content, "k");
  ASSERT_EQ(std::get<std::string>(assignment->right->As<AST::Literal>()->value.value), "0");
}

TEST(ParserTest, TemporaryInitialization_1) {
  ParserTester test{"int((*x)[5] + 6)"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_NE(node.get(), nullptr);
  ASSERT_EQ(node->type, AST::NodeType::CAST);
  auto *cast = node->As<AST::CastExpression>();
  ASSERT_EQ(cast->kind, AST::CastExpression::Kind::FUNCTIONAL);
  ASSERT_EQ(cast->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(cast->ft.flags, 0);
  ASSERT_EQ(cast->ft.decl.components.size(), 0);
  ASSERT_EQ(cast->ft.decl.name.content, "");
  ASSERT_EQ(cast->ft.decl.has_nested_declarator, false);

  ASSERT_EQ(cast->expr->type, AST::NodeType::BINARY_EXPRESSION);
  auto *binary = cast->expr->As<AST::BinaryExpression>();
  ASSERT_EQ(binary->operation, TT_PLUS);

  ASSERT_EQ(binary->left->type, AST::NodeType::BINARY_EXPRESSION);
  auto *left = binary->left->As<AST::BinaryExpression>();
  ASSERT_EQ(left->operation, TT_BEGINBRACKET);
  ASSERT_EQ(left->left->type, AST::NodeType::PARENTHETICAL);
  auto *left_left_paren = left->left->As<AST::Parenthetical>();
  auto *left_left_unary = left_left_paren->expression->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(left_left_unary->operation, TT_STAR);
  ASSERT_EQ(left_left_unary->operand->type, AST::NodeType::LITERAL);
  auto *left_left_unary_operand = left_left_unary->operand->As<AST::Literal>();
  ASSERT_EQ(std::get<std::string>(left_left_unary_operand->value.value), "x");

  ASSERT_EQ(left->right->type, AST::NodeType::LITERAL);
  //  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(left->right.get())->value.value), "5");

  ASSERT_EQ(binary->right->type, AST::NodeType::LITERAL);
  //  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(binary->right.get())->value.value), "6");
}

TEST(ParserTest, TemporaryInitialization_2) {
  ParserTester test{"int(*(*a)[10]) = nullptr;"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
  auto *decl = node->As<AST::DeclarationStatement>();
  ASSERT_EQ(decl->declarations.size(), 1);
  ASSERT_EQ(decl->def, jdi::builtin_type__int);
  auto *decl1 = &decl->declarations[0];
  ASSERT_EQ(decl1->declarator->def, jdi::builtin_type__int);
  ASSERT_EQ(decl1->declarator->flags, 0);

  auto &declarator = decl1->declarator->decl;
  ASSERT_EQ(declarator.components.size(), 2);
  ASSERT_EQ(declarator.components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &decl_ptr = declarator.components[0].as<PointerNode>();
  ASSERT_EQ(decl_ptr.class_def, nullptr);
  ASSERT_EQ(decl_ptr.is_const, false);
  ASSERT_EQ(decl_ptr.is_volatile, false);

  ASSERT_EQ(declarator.components[1].kind, DeclaratorNode::Kind::NESTED);
  auto &nested = declarator.components[1].as<NestedNode>();
  ASSERT_TRUE(nested.is<std::unique_ptr<Declarator>>());
  auto &nested_decl = nested.as<std::unique_ptr<Declarator>>();
  ASSERT_EQ(nested_decl->components.size(), 2);
  ASSERT_EQ(nested_decl->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
  auto &nested_ptr = nested_decl->components[0].as<PointerNode>();
  ASSERT_EQ(nested_ptr.class_def, nullptr);
  ASSERT_EQ(nested_ptr.is_const, false);
  ASSERT_EQ(nested_ptr.is_volatile, false);

  ASSERT_EQ(nested_decl->components[1].kind, DeclaratorNode::Kind::ARRAY_BOUND);
  ASSERT_NE(decl1->init, nullptr);
  ASSERT_EQ(decl1->init->type, AST::NodeType::INITIALIZER);
  ASSERT_EQ(decl1->init->is_variadic, false);
  ASSERT_EQ(decl1->init->kind, AST::Initializer::Kind::ASSIGN_EXPR);
  auto *init = std::get<AST::AssignmentInitNode>(decl1->init->initializer).get();
  ASSERT_EQ(init->kind, AST::AssignmentInitializer::Kind::EXPR);
  auto *expr = std::get<std::unique_ptr<AST::Node>>(init->initializer).get();
  assert_identifier_is(expr, "nullptr");
}

TEST(ParserTest, TemporaryInitialization_3) {
  ParserTester test{"int(*(*a)[10] + b);"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::CAST);
  auto *cast = node->As<AST::CastExpression>();
  ASSERT_EQ(cast->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(cast->ft.flags, 0);
  ASSERT_EQ(cast->ft.decl.components.size(), 0);
  ASSERT_EQ(cast->ft.decl.name.content, "");
  ASSERT_EQ(cast->ft.decl.has_nested_declarator, false);

  ASSERT_EQ(cast->expr->type, AST::NodeType::BINARY_EXPRESSION);
  auto *binary = cast->expr->As<AST::BinaryExpression>();
  ASSERT_EQ(binary->operation, TT_PLUS);

  ASSERT_EQ(binary->left->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  auto *left = binary->left->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(left->operation, TT_STAR);
  ASSERT_EQ(left->operand->type, AST::NodeType::BINARY_EXPRESSION);
  auto *operand = left->operand->As<AST::BinaryExpression>();
  ASSERT_EQ(operand->operation, TT_BEGINBRACKET);
  ASSERT_EQ(operand->left->type, AST::NodeType::PARENTHETICAL);

  ASSERT_EQ(operand->left->type, AST::NodeType::PARENTHETICAL);
  auto *left_operand = (operand->left.get())->As<AST::Parenthetical>()->expression.get();
  ASSERT_EQ(left_operand->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  auto *left_unary = left_operand->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(left_unary->operation, TT_STAR);
  ASSERT_EQ(left_unary->operand->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(left_unary->operand->As<AST::Literal>()->value.value), "a");

  ASSERT_EQ(operand->right->type, AST::NodeType::LITERAL);
  //  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(operand->right.get())->value.value), "10");

  assert_identifier_is(binary->right.get(), "b");
}

TEST(ParserTest, TemporaryInitialization_4) {
  ParserTester test{"int(*(*(*(*x + 4))))"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::CAST);
  auto *cast = node->As<AST::CastExpression>();
  ASSERT_EQ(cast->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(cast->ft.flags, 0);
  ASSERT_EQ(cast->ft.decl.components.size(), 0);
  ASSERT_EQ(cast->ft.decl.name.content, "");
  ASSERT_EQ(cast->ft.decl.has_nested_declarator, false);

  ASSERT_EQ(cast->kind, AST::CastExpression::Kind::FUNCTIONAL);
  ASSERT_EQ(cast->expr->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  auto *unary = cast->expr->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  unary = unary->operand->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  unary = unary->operand->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::BINARY_EXPRESSION);
  auto *binary = unary->operand->As<AST::BinaryExpression>();
  ASSERT_EQ(binary->operation, TT_PLUS);
  ASSERT_EQ(binary->left->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  unary = binary->left->As<AST::UnaryPrefixExpression>();
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(unary->operand->As<AST::Literal>()->value.value), "x");

  ASSERT_EQ(binary->right->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(binary->right->As<AST::Literal>()->value.value), "4");
}

TEST(ParserTest, ForLoop_1) {
  ParserTester test{"for (int i = 0; i < 5; i++) {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i"};

  ASSERT_THAT(for_,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                    IsBinaryOperation(TT_LESS, IsIdentifier("i"), IsLiteral("5")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("i")), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_2) {
  ParserTester test{"for int i = 0, j=1; i >= 12; --i {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j"};

  ASSERT_THAT(for_, IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                          IsBinaryOperation(TT_GREATEREQUAL, IsIdentifier("i"), IsLiteral("12")),
                                          IsUnaryPrefixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_3) {
  ParserTester test{"for int i = 0, j=1, k=133 ;i != 12; --i {j ++;}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j", "k"};

  ASSERT_THAT(for_, IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                          IsBinaryOperation(TT_NOTEQUAL, IsIdentifier("i"), IsLiteral("12")),
                                          IsUnaryPrefixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(1)));
}

TEST(ParserTest, ForLoop_3_NoSemicolon) {
  ParserTester test{"for char i = '0', j='1', k='3' ;i != 12; --i {j ++}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j", "k"};

  ASSERT_THAT(for_, IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__char),
                                          IsBinaryOperation(TT_NOTEQUAL, IsIdentifier("i"), IsLiteral("12")),
                                          IsUnaryPrefixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(1)));
}

TEST(ParserTest, ForLoop_4) {
  ParserTester test{"for int i = 0, j=1, k=133, w=-99 ;w % 22; j++ {if(l) break; else continue;}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j", "k", "w"};

  ASSERT_THAT(for_,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                    IsBinaryOperation(TT_PERCENT, IsIdentifier("w"), IsLiteral("22")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("j")), IsStatementBlock(1)));
}

TEST(ParserTest, ForLoop_4_NoSemicolon) {
  ParserTester test{"for double i = 0, j=1, k=133, w=-99 ;w % 22; j++ {if(l) break else continue}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j", "k", "w"};

  ASSERT_THAT(for_,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__double),
                                    IsBinaryOperation(TT_PERCENT, IsIdentifier("w"), IsLiteral("22")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("j")), IsStatementBlock(1)));
}

TEST(ParserTest, ForLoop_5) {
  ParserTester test{"for int i = 0, j=1, k=133, w=44, u=-77 ;w % 22; w++ {f++; if(i) x = new int; else delete y;}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j", "k", "w", "u"};

  ASSERT_THAT(for_,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__int),
                                    IsBinaryOperation(TT_PERCENT, IsIdentifier("w"), IsLiteral("22")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("w")), IsStatementBlock(2)));
}

TEST(ParserTest, ForLoop_5_NoSemicolon) {
  ParserTester test{"for float i = 0, j=1, k=133, w=44, u=-77 ;w % 22; w++ {f++ if(i) x = new int else delete y}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  std::vector<std::string> decls = {"i", "j", "k", "w", "u"};

  ASSERT_THAT(for_,
              IsForLoopWithChildren(IsDeclaration(decls, jdi::builtin_type__float),
                                    IsBinaryOperation(TT_PERCENT, IsIdentifier("w"), IsLiteral("22")),
                                    IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("w")), IsStatementBlock(2)));
}

TEST(ParserTest, ForLoop_6) {
  ParserTester test{"for int(i = 5); i < 5; i++ {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  ASSERT_THAT(
      for_, IsForLoopWithChildren(
                IsCast(AST::CastExpression::Kind::FUNCTIONAL, AST::NodeType::BINARY_EXPRESSION, jdi::builtin_type__int),
                IsBinaryOperation(TT_LESS, IsIdentifier("i"), IsLiteral("5")),
                IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("i")), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_7) {
  ParserTester test{"for (int)(i = 0); i < 5; i++ {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  ASSERT_THAT(for_,
              IsForLoopWithChildren(
                  IsCast(AST::CastExpression::Kind::C_STYLE, AST::NodeType::PARENTHETICAL, jdi::builtin_type__int),
                  IsBinaryOperation(TT_LESS, IsIdentifier("i"), IsLiteral("5")),
                  IsUnaryPostfixOperator(TT_INCREMENT, IsIdentifier("i")), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_8) {
  ParserTester test{"for static_cast<int>(i = 10); i / 3; i-- {k++; return;}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  ASSERT_THAT(for_,
              IsForLoopWithChildren(
                  IsCast(AST::CastExpression::Kind::STATIC, AST::NodeType::BINARY_EXPRESSION, jdi::builtin_type__int),
                  IsBinaryOperation(TT_SLASH, IsIdentifier("i"), IsLiteral("3")),
                  IsUnaryPostfixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(2)));
}

TEST(ParserTest, ForLoop_8_NoSemicolon) {
  ParserTester test{"for static_cast<double>(i = 10222.2); i / 3; i-- {k++ return}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  ASSERT_THAT(
      for_, IsForLoopWithChildren(
                IsCast(AST::CastExpression::Kind::STATIC, AST::NodeType::BINARY_EXPRESSION, jdi::builtin_type__double),
                IsBinaryOperation(TT_SLASH, IsIdentifier("i"), IsLiteral("3")),
                IsUnaryPostfixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(2)));
}

TEST(ParserTest, ForLoop_9) {
  ParserTester test{"for static_cast<int>(i = 10, j=12); i mod 3; i-- {k--; return 12;}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  ASSERT_THAT(for_,
              IsForLoopWithChildren(
                  IsCast(AST::CastExpression::Kind::STATIC, AST::NodeType::BINARY_EXPRESSION, jdi::builtin_type__int),
                  IsBinaryOperation(TT_MOD, IsIdentifier("i"), IsLiteral("3")),
                  IsUnaryPostfixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(2)));
}

TEST(ParserTest, ForLoop_9_NoSemicolon) {
  ParserTester test{"for static_cast<float>(i = 10.2, j=12); i mod 3; i-- {k-- return 12}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = node->As<AST::ForLoop>();

  ASSERT_THAT(for_,
              IsForLoopWithChildren(
                  IsCast(AST::CastExpression::Kind::STATIC, AST::NodeType::BINARY_EXPRESSION, jdi::builtin_type__float),
                  IsBinaryOperation(TT_MOD, IsIdentifier("i"), IsLiteral("3")),
                  IsUnaryPostfixOperator(TT_DECREMENT, IsIdentifier("i")), IsStatementBlock(2)));
}
