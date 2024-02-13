#include <languages/lang_CPP.h>
#include <parsing/ast.h>
#include <parsing/parser.cpp>

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
  ASSERT_EQ(dynamic_cast<AST::IdentifierAccess *>(node)->name.content, name);
}

TEST(ParserTest, Basics) {
  ParserTester test{"(x ? y : z ? a : (z[5](6)))"};

  auto node = test->TryParseStatement();
  ASSERT_EQ(node->type, AST::NodeType::PARENTHETICAL);

  auto *expr = dynamic_cast<AST::Parenthetical *>(node.get())->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::TERNARY_EXPRESSION);

  auto *ternary = dynamic_cast<AST::TernaryExpression *>(expr);
  auto *cond = ternary->condition.get();
  auto *true_ = ternary->true_expression.get();
  auto *false_ = ternary->false_expression.get();
  assert_identifier_is(cond, "x");
  assert_identifier_is(true_, "y");

  ASSERT_EQ(false_->type, AST::NodeType::TERNARY_EXPRESSION);

  ternary = dynamic_cast<AST::TernaryExpression *>(false_);
  cond = ternary->condition.get();
  true_ = ternary->true_expression.get();
  false_ = ternary->false_expression.get();

  assert_identifier_is(cond, "z");
  assert_identifier_is(true_, "a");

  ASSERT_EQ(false_->type, AST::NodeType::PARENTHETICAL);
  expr = dynamic_cast<AST::Parenthetical *>(false_)->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::FUNCTION_CALL);
  auto *function = dynamic_cast<AST::FunctionCallExpression *>(expr);
  auto *called = function->function.get();
  auto *args = &function->arguments;

  ASSERT_EQ(called->type, AST::NodeType::BINARY_EXPRESSION);

  auto *bin = dynamic_cast<AST::BinaryExpression *>(called);
  ASSERT_EQ(bin->operation, TokenType::TT_BEGINBRACKET);
  assert_identifier_is(bin->left.get(), "z");

  ASSERT_EQ(bin->right->type, AST::NodeType::LITERAL);
  auto *right = dynamic_cast<AST::Literal *>(bin->right.get());
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(right)->value.value), "5");

  ASSERT_EQ(args->size(), 1);
  auto *arg = (*args)[0].get();
  ASSERT_EQ(arg->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(arg)->value.value), "6");
}

TEST(ParserTest, SizeofExpression) {
  ParserTester test{"sizeof 5"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::SIZEOF);
  auto *sizeof_ = dynamic_cast<AST::SizeofExpression *>(expr.get());
  ASSERT_EQ(sizeof_->kind, AST::SizeofExpression::Kind::EXPR);
  ASSERT_TRUE(std::holds_alternative<AST::PNode>(sizeof_->argument));

  auto &value = std::get<AST::PNode>(sizeof_->argument);
  ASSERT_EQ(value->type, AST::NodeType::LITERAL);
  auto *literal = dynamic_cast<AST::Literal *>(value.get());
  ASSERT_EQ(std::get<std::string>(literal->value.value), "5");
}

TEST(ParserTest, SizeofVariadic) {
  ParserTester test{"sizeof...(ident)"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::SIZEOF);
  auto *sizeof_ = dynamic_cast<AST::SizeofExpression *>(expr.get());
  ASSERT_EQ(sizeof_->kind, AST::SizeofExpression::Kind::VARIADIC);
  ASSERT_TRUE(std::holds_alternative<std::string>(sizeof_->argument));

  auto &value = std::get<std::string>(sizeof_->argument);
  ASSERT_EQ(value, "ident");
}

TEST(ParserTest, SizeofType) {
  ParserTester test{"sizeof(const volatile unsigned long long int **(*)[10])"};
  auto expr = test->TryParseStatement();

  ASSERT_EQ(expr->type, AST::NodeType::SIZEOF);
  auto *sizeof_ = dynamic_cast<AST::SizeofExpression *>(expr.get());
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
  auto *alignof_ = dynamic_cast<AST::AlignofExpression *>(expr.get());
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

// TEST(ParserTest, Declarator_1) {
//   FullType ft2;
//   ParserTester test2{"const unsigned int **(*var::*y)[10]"};
//   test2->TryParseTypeSpecifierSeq(&ft2);
//   test2->TryParseDeclarator(&ft2, AST::DeclaratorType::NON_ABSTRACT);

//   jdi::ref_stack stack;
//   ft2.decl.to_jdi_refstack(stack);
//   auto first = stack.begin();
//   EXPECT_EQ(ft2.decl.name.content, "y");
//   EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
//   EXPECT_EQ((first++)->type, jdi::ref_stack::RT_MEMBER_POINTER);
//   EXPECT_EQ((first++)->type, jdi::ref_stack::RT_ARRAYBOUND);
//   EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
//   EXPECT_EQ((first++)->type, jdi::ref_stack::RT_POINTERTO);
//   EXPECT_EQ(test2.lexer.ReadToken().type, TT_ENDOFCODE);
// }

// TEST(ParserTest, Declarator_2) {
//   FullType ft3;
//   ParserTester test3{"int ((*a)(int (*x)(int x), int (*)[10]))(int)"};
//   auto node = test3->TryParseStatement();

//   EXPECT_EQ(test3.lexer.ReadToken().type, TT_ENDOFCODE);
// }

// TEST(ParserTest, Declarator_3) {
//   ParserTester test{"int *(*(*a)[10][12])[15]"};
//   auto node = test->TryParseStatement();
//   ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
//   auto *decls = dynamic_cast<AST::DeclarationStatement *>(node.get());
//   ASSERT_EQ(decls->declarations.size(), 1);

//   ASSERT_EQ(decls->declarations[0].init, nullptr);
//   auto &decl1 = decls->declarations[0].declarator->decl;
//   ASSERT_EQ(decl1.name.content, "a");
//   ASSERT_EQ(decl1.components.size(), 2);

//   ASSERT_EQ(decl1.components[0].kind, DeclaratorNode::Kind::POINTER_TO);
//   auto &ptr = decl1.components[0].as<PointerNode>();
//   ASSERT_EQ(ptr.is_const, false);
//   ASSERT_EQ(ptr.is_volatile, false);
//   ASSERT_EQ(ptr.class_def, nullptr);

//   ASSERT_EQ(decl1.components[1].kind, DeclaratorNode::Kind::NESTED);
//   ASSERT_TRUE(decl1.components[1].as<NestedNode>().is<std::unique_ptr<Declarator>>());
//   auto *nested = decl1.components[1].as<NestedNode>().as<std::unique_ptr<Declarator>>().get();
//   ASSERT_EQ(nested->components.size(), 3);

//   ASSERT_EQ(nested->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
//   auto &nested_ptr = nested->components[0].as<PointerNode>();
//   ASSERT_EQ(nested_ptr.is_const, false);
//   ASSERT_EQ(nested_ptr.is_volatile, false);
//   ASSERT_EQ(nested_ptr.class_def, nullptr);

//   ASSERT_EQ(nested->components[1].kind, DeclaratorNode::Kind::NESTED);
//   ASSERT_TRUE(nested->components[1].as<NestedNode>().is<std::unique_ptr<Declarator>>());
//   auto *nested_nested = nested->components[1].as<NestedNode>().as<std::unique_ptr<Declarator>>().get();
//   ASSERT_EQ(nested_nested->components.size(), 3);
//   ASSERT_EQ(nested_nested->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
//   auto &nested_nested_ptr = nested_nested->components[0].as<PointerNode>();
//   ASSERT_EQ(nested_nested_ptr.is_const, false);
//   ASSERT_EQ(nested_nested_ptr.is_volatile, false);
//   ASSERT_EQ(nested_nested_ptr.class_def, nullptr);
//   ASSERT_EQ(nested_nested->components[1].kind, DeclaratorNode::Kind::ARRAY_BOUND);
//   ASSERT_EQ(nested_nested->components[2].kind, DeclaratorNode::Kind::ARRAY_BOUND);

//   ASSERT_EQ(nested->components[2].kind, DeclaratorNode::Kind::ARRAY_BOUND);
// }

// TEST(ParserTest, Declarator_4) {
//   ParserTester test{"int *(*(*a)[10][12])[15]"};
//   auto node = test->TryParseStatement();
//   ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
//   auto *decls = dynamic_cast<AST::DeclarationStatement *>(node.get());
//   ASSERT_EQ(decls->declarations.size(), 1);

//   ASSERT_EQ(decls->declarations[0].init, nullptr);
//   auto &decl1 = decls->declarations[0].declarator->decl;
//   ASSERT_EQ(decl1.name.content, "a");
//   ASSERT_EQ(decl1.components.size(), 2);

//   jdi::ref_stack stack;
//   decl1.to_jdi_refstack(stack);
//   auto first = stack.begin();
//   ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
//   ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
//   ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
//   ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
//   ASSERT_EQ(first++->type, jdi::ref_stack::RT_ARRAYBOUND);
//   ASSERT_EQ(first++->type, jdi::ref_stack::RT_POINTERTO);
// }

// TEST(ParserTest, Declaration) {
//   ParserTester test{"const unsigned *(*x)[10] = nullptr;"};
//   auto node = test->TryParseStatement();
//   EXPECT_EQ(test->current_token().type, TT_SEMICOLON);
//   EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);
// }

// TEST(ParserTest, Declarations) {
//   ParserTester test{"int *x = nullptr, y, (*z)(int x, int) = &y;"};

//   auto node = test->TryParseStatement();
//   EXPECT_EQ(test->current_token().type, TT_SEMICOLON);
//   EXPECT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

//   ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
//   auto *decls = reinterpret_cast<AST::DeclarationStatement *>(node.get());
//   EXPECT_EQ(decls->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);

//   EXPECT_EQ(decls->declarations.size(), 3);
//   EXPECT_NE(decls->declarations[0].init, nullptr);
//   EXPECT_EQ(decls->declarations[0].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
//   EXPECT_EQ(decls->declarations[0].declarator->decl.components.begin()->kind, DeclaratorNode::Kind::POINTER_TO);

//   EXPECT_EQ(decls->declarations[1].init, nullptr);
//   EXPECT_EQ(decls->declarations[1].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
//   EXPECT_EQ(decls->declarations[1].declarator->decl.components.size(), 0);

//   EXPECT_NE(decls->declarations[2].init, nullptr);
//   EXPECT_EQ(decls->declarations[2].declarator->def->flags & jdi::DEF_TYPENAME, jdi::DEF_TYPENAME);
//   EXPECT_EQ(decls->declarations[2].declarator->decl.components.size(), 1);
// }

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
    ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(expr)->value.value), std::to_string(i + 1));
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

TEST(ParserTest, NewExpression_2) {
  ParserTester test{"::new int[][15]{1, 2, 3, 4, 5};"};
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

TEST(ParserTest, NewExpression_4) {
  ParserTester test{"new (int *(**)[10]);"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

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

TEST(ParserTest, SwitchStatement_1) {
  ParserTester test{"switch (5 * 6) { case 1: return 2 break 13; case 2: return 3 break default: break };"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = dynamic_cast<AST::SwitchStatement *>(node.get());
  ASSERT_EQ(switch_->body->statements.size(), 3);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::CASE);
  auto *case1 = dynamic_cast<AST::CaseStatement *>(switch_->body->statements[0].get());
  ASSERT_EQ(case1->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(case1->value.get())->value.value), "1");
  ASSERT_EQ(case1->statements->statements.size(), 2);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::RETURN);
  ASSERT_EQ(case1->statements->statements[1]->type, AST::NodeType::BREAK);

  ASSERT_EQ(switch_->body->statements[1]->type, AST::NodeType::CASE);
  auto *case2 = dynamic_cast<AST::CaseStatement *>(switch_->body->statements[1].get());
  ASSERT_EQ(case2->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(case2->value.get())->value.value), "2");
  ASSERT_EQ(case2->statements->statements.size(), 2);
  ASSERT_EQ(case2->statements->statements[0]->type, AST::NodeType::RETURN);
  ASSERT_EQ(case2->statements->statements[1]->type, AST::NodeType::BREAK);

  ASSERT_EQ(switch_->body->statements[2]->type, AST::NodeType::DEFAULT);
  auto *default_ = dynamic_cast<AST::DefaultStatement *>(switch_->body->statements[2].get());
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::BREAK);
}

TEST(ParserTest, SwitchStatement_2) {
  ParserTester test{"switch (1) { case 1: return 2; default: return \"test\"; };"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = dynamic_cast<AST::SwitchStatement *>(node.get());
  ASSERT_EQ(switch_->body->statements.size(), 2);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::CASE);
  auto *case1 = dynamic_cast<AST::CaseStatement *>(switch_->body->statements[0].get());
  ASSERT_EQ(case1->value->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(case1->value.get())->value.value), "1");
  ASSERT_EQ(case1->statements->statements.size(), 1);
  ASSERT_EQ(case1->statements->statements[0]->type, AST::NodeType::RETURN);

  ASSERT_EQ(switch_->body->statements[1]->type, AST::NodeType::DEFAULT);
  auto *default_ = dynamic_cast<AST::DefaultStatement *>(switch_->body->statements[1].get());
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::RETURN);
}

TEST(ParserTest, SwitchStatement_3) {
  ParserTester test{"switch (1) { default: continue 12;};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = dynamic_cast<AST::SwitchStatement *>(node.get());
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = dynamic_cast<AST::DefaultStatement *>(switch_->body->statements[0].get());
  ASSERT_EQ(default_->statements->statements.size(), 1);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::CONTINUE);
}

TEST(ParserTest, SwitchStatement_4) {
  ParserTester test{"switch (1) { default: delete [] x; return \"new test\";};"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::SWITCH);
  auto *switch_ = dynamic_cast<AST::SwitchStatement *>(node.get());
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = dynamic_cast<AST::DefaultStatement *>(switch_->body->statements[0].get());
  ASSERT_EQ(default_->statements->statements.size(), 2);
  ASSERT_EQ(default_->statements->statements[0]->type, AST::NodeType::DELETE);
  ASSERT_EQ(default_->statements->statements[1]->type, AST::NodeType::RETURN);

  auto *delete_ = reinterpret_cast<AST::DeleteExpression *>(default_->statements->statements[0].get());
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
  auto *switch_ = dynamic_cast<AST::SwitchStatement *>(node.get());
  ASSERT_EQ(switch_->body->statements.size(), 1);

  ASSERT_EQ(switch_->body->statements[0]->type, AST::NodeType::DEFAULT);
  auto *default_ = dynamic_cast<AST::DefaultStatement *>(switch_->body->statements[0].get());
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

// TEST(ParserTest, CodeBlock) {
//   ParserTester test{"{ int x = 5 const int y = 6 float *(*z)[10] = nullptr foo(bar) }"};
//   auto node = test->TryParseDeclarations(true);
//   ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

//   ASSERT_EQ(node->type, AST::NodeType::BLOCK);
//   auto *block = dynamic_cast<AST::CodeBlock *>(node.get());
//   ASSERT_EQ(block->statements.size(), 4);
//   ASSERT_EQ(block->statements[0]->type, AST::NodeType::DECLARATION);
//   ASSERT_EQ(block->statements[1]->type, AST::NodeType::DECLARATION);
//   ASSERT_EQ(block->statements[2]->type, AST::NodeType::DECLARATION);
//   ASSERT_EQ(block->statements[3]->type, AST::NodeType::FUNCTION_CALL);
// }

TEST(ParserTest, TemporaryInitialization_1) {
  ParserTester test{"int((*x)[5] + 6)"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_NE(node.get(), nullptr);
  ASSERT_EQ(node->type, AST::NodeType::CAST);
  auto *cast = dynamic_cast<AST::CastExpression *>(node.get());
  ASSERT_EQ(cast->kind, AST::CastExpression::Kind::FUNCTIONAL);
  ASSERT_EQ(cast->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(cast->ft.flags, 0);
  ASSERT_EQ(cast->ft.decl.components.size(), 0);
  ASSERT_EQ(cast->ft.decl.name.content, "");
  ASSERT_EQ(cast->ft.decl.has_nested_declarator, false);

  ASSERT_EQ(cast->expr->type, AST::NodeType::BINARY_EXPRESSION);
  auto *binary = dynamic_cast<AST::BinaryExpression *>(cast->expr.get());
  ASSERT_EQ(binary->operation, TT_PLUS);

  ASSERT_EQ(binary->left->type, AST::NodeType::BINARY_EXPRESSION);
  auto *left = dynamic_cast<AST::BinaryExpression *>(binary->left.get());
  ASSERT_EQ(left->operation, TT_BEGINBRACKET);
  ASSERT_EQ(left->left->type, AST::NodeType::PARENTHETICAL);
  auto *left_left_paren = dynamic_cast<AST::Parenthetical *>(left->left.get());
  auto *left_left_unary = dynamic_cast<AST::UnaryPrefixExpression *>(left_left_paren->expression.get());
  ASSERT_EQ(left_left_unary->operation, TT_STAR);
  ASSERT_EQ(left_left_unary->operand->type, AST::NodeType::LITERAL);
  auto *left_left_unary_operand = dynamic_cast<AST::Literal *>(left_left_unary->operand.get());
  ASSERT_EQ(std::get<std::string>(left_left_unary_operand->value.value), "x");

  ASSERT_EQ(left->right->type, AST::NodeType::LITERAL);
  //  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(left->right.get())->value.value), "5");

  ASSERT_EQ(binary->right->type, AST::NodeType::LITERAL);
  //  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(binary->right.get())->value.value), "6");
}

// TEST(ParserTest, TemporaryInitialization_2) {
//   ParserTester test{"int(*(*a)[10]) = nullptr;"};
//   auto node = test->TryParseStatement();
//   ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
//   ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

//   ASSERT_EQ(node->type, AST::NodeType::DECLARATION);
//   auto *decl = dynamic_cast<AST::DeclarationStatement *>(node.get());
//   ASSERT_EQ(decl->declarations.size(), 1);
//   ASSERT_EQ(decl->def, jdi::builtin_type__int);
//   auto *decl1 = &decl->declarations[0];
//   ASSERT_EQ(decl1->declarator->def, jdi::builtin_type__int);
//   ASSERT_EQ(decl1->declarator->flags, 0);

//   auto &declarator = decl1->declarator->decl;
//   ASSERT_EQ(declarator.components.size(), 2);
//   ASSERT_EQ(declarator.components[0].kind, DeclaratorNode::Kind::POINTER_TO);
//   auto &decl_ptr = declarator.components[0].as<PointerNode>();
//   ASSERT_EQ(decl_ptr.class_def, nullptr);
//   ASSERT_EQ(decl_ptr.is_const, false);
//   ASSERT_EQ(decl_ptr.is_volatile, false);

//   ASSERT_EQ(declarator.components[1].kind, DeclaratorNode::Kind::NESTED);
//   auto &nested = declarator.components[1].as<NestedNode>();
//   ASSERT_TRUE(nested.is<std::unique_ptr<Declarator>>());
//   auto &nested_decl = nested.as<std::unique_ptr<Declarator>>();
//   ASSERT_EQ(nested_decl->components.size(), 2);
//   ASSERT_EQ(nested_decl->components[0].kind, DeclaratorNode::Kind::POINTER_TO);
//   auto &nested_ptr = nested_decl->components[0].as<PointerNode>();
//   ASSERT_EQ(nested_ptr.class_def, nullptr);
//   ASSERT_EQ(nested_ptr.is_const, false);
//   ASSERT_EQ(nested_ptr.is_volatile, false);

//   ASSERT_EQ(nested_decl->components[1].kind, DeclaratorNode::Kind::ARRAY_BOUND);
//   ASSERT_NE(decl1->init, nullptr);
//   ASSERT_EQ(decl1->init->type, AST::NodeType::INITIALIZER);
//   ASSERT_EQ(decl1->init->is_variadic, false);
//   ASSERT_EQ(decl1->init->kind, AST::Initializer::Kind::ASSIGN_EXPR);
//   auto *init = std::get<AST::AssignmentInitNode>(decl1->init->initializer).get();
//   ASSERT_EQ(init->kind, AST::AssignmentInitializer::Kind::EXPR);
//   auto *expr = std::get<std::unique_ptr<AST::Node>>(init->initializer).get();
//   assert_identifier_is(expr, "nullptr");
// }

TEST(ParserTest, TemporaryInitialization_3) {
  ParserTester test{"int(*(*a)[10] + b);"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_SEMICOLON);
  ASSERT_EQ(test.lexer.ReadToken().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::CAST);
  auto *cast = dynamic_cast<AST::CastExpression *>(node.get());
  ASSERT_EQ(cast->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(cast->ft.flags, 0);
  ASSERT_EQ(cast->ft.decl.components.size(), 0);
  ASSERT_EQ(cast->ft.decl.name.content, "");
  ASSERT_EQ(cast->ft.decl.has_nested_declarator, false);

  ASSERT_EQ(cast->expr->type, AST::NodeType::BINARY_EXPRESSION);
  auto *binary = dynamic_cast<AST::BinaryExpression *>(cast->expr.get());
  ASSERT_EQ(binary->operation, TT_PLUS);

  ASSERT_EQ(binary->left->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  auto *left = dynamic_cast<AST::UnaryPrefixExpression *>(binary->left.get());
  ASSERT_EQ(left->operation, TT_STAR);
  ASSERT_EQ(left->operand->type, AST::NodeType::BINARY_EXPRESSION);
  auto *operand = dynamic_cast<AST::BinaryExpression *>(left->operand.get());
  ASSERT_EQ(operand->operation, TT_BEGINBRACKET);
  ASSERT_EQ(operand->left->type, AST::NodeType::PARENTHETICAL);

  ASSERT_EQ(operand->left->type, AST::NodeType::PARENTHETICAL);
  auto *left_operand = dynamic_cast<AST::Parenthetical *>(operand->left.get())->expression.get();
  ASSERT_EQ(left_operand->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  auto *left_unary = dynamic_cast<AST::UnaryPrefixExpression *>(left_operand);
  ASSERT_EQ(left_unary->operation, TT_STAR);
  ASSERT_EQ(left_unary->operand->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(left_unary->operand.get())->value.value), "a");

  ASSERT_EQ(operand->right->type, AST::NodeType::LITERAL);
  //  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(operand->right.get())->value.value), "10");

  assert_identifier_is(binary->right.get(), "b");
}

TEST(ParserTest, TemporaryInitialization_4) {
  ParserTester test{"int(*(*(*(*x + 4))))"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::CAST);
  auto *cast = dynamic_cast<AST::CastExpression *>(node.get());
  ASSERT_EQ(cast->ft.def, jdi::builtin_type__int);
  ASSERT_EQ(cast->ft.flags, 0);
  ASSERT_EQ(cast->ft.decl.components.size(), 0);
  ASSERT_EQ(cast->ft.decl.name.content, "");
  ASSERT_EQ(cast->ft.decl.has_nested_declarator, false);

  ASSERT_EQ(cast->kind, AST::CastExpression::Kind::FUNCTIONAL);
  ASSERT_EQ(cast->expr->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  auto *unary = dynamic_cast<AST::UnaryPrefixExpression *>(cast->expr.get());
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  unary = dynamic_cast<AST::UnaryPrefixExpression *>(unary->operand.get());
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  unary = dynamic_cast<AST::UnaryPrefixExpression *>(unary->operand.get());
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::BINARY_EXPRESSION);
  auto *binary = dynamic_cast<AST::BinaryExpression *>(unary->operand.get());
  ASSERT_EQ(binary->operation, TT_PLUS);
  ASSERT_EQ(binary->left->type, AST::NodeType::UNARY_PREFIX_EXPRESSION);
  unary = dynamic_cast<AST::UnaryPrefixExpression *>(binary->left.get());
  ASSERT_EQ(unary->operation, TT_STAR);
  ASSERT_EQ(unary->operand->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(unary->operand.get())->value.value), "x");

  ASSERT_EQ(binary->right->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(binary->right.get())->value.value), "4");
}

std::string ExpectedMsg = "";

vector<std::string> NodeTypes = {"ERROR",
                                 "BLOCK",
                                 "BINARY_EXPRESSION",
                                 "UNARY_PREFIX_EXPRESSION",
                                 "UNARY_POSTFIX_EXPRESSION",
                                 "TERNARY_EXPRESSION",
                                 "SIZEOF",
                                 "ALIGNOF",
                                 "CAST",
                                 "NEW",
                                 "DELETE",
                                 "PARENTHETICAL",
                                 "ARRAY",
                                 "IDENTIFIER",
                                 "SCOPE_ACCESS",
                                 "LITERAL",
                                 "FUNCTION_CALL",
                                 "IF",
                                 "FOR",
                                 "WHILE",
                                 "DO",
                                 "WITH",
                                 "REPEAT",
                                 "SWITCH",
                                 "CASE",
                                 "DEFAULT",
                                 "BREAK",
                                 "CONTINUE",
                                 "RETURN",
                                 "DECLARATION",
                                 "INITIALIZER"};
vector<std::string> StorageClasses = {"TEMPORARY", "LOCAL", "GLOBAL"};
vector<std::string> CastKinds = {"C_STYLE", "STATIC", "DYNAMIC", "REINTERPRET", "CONST", "FUNCTIONAL"};

MATCHER_P(IsDeclaration, decls, "") {
  auto *decl = dynamic_cast<AST::DeclarationStatement *>(arg);

  bool b1 = arg->type == AST::NodeType::DECLARATION,
       b2 = decl->storage_class == enigma::parsing::AST::DeclarationStatement::StorageClass::TEMPORARY,
       b3 = decl->declarations.size() == decls.size();

  if (!b1 || !b2 || !b3) {
    ExpectedMsg = "From IsDeclaration Matcher: ";

    if (!b1) {
      ExpectedMsg += "NodeType = DECLARATION\n";
      *result_listener << "got NodeType = " << NodeTypes[(int)arg->type] << "\n";
    }
    if (!b2) {
      ExpectedMsg += "StorageClass = TEMPORARY\n";
      *result_listener << "got StorageClass = " << StorageClasses[(int)decl->storage_class] << "\n";
    }
    if (!b3) {
      ExpectedMsg += "DeclarationsSize = " + to_string(decls.size()) + "\n";
      *result_listener << "got DeclarationsSize = " << to_string(decl->declarations.size()) << "\n";
    }
  }

  bool b4 = 1;
  for (size_t i = 0; i < decls.size(); i++) {
    auto &decli = decl->declarations[i].declarator->decl;
    b4 = b4 && decl->declarations[i].init != nullptr;
    b4 = b4 && decl->declarations[i].declarator->def == jdi::builtin_type__int;  // need to send the type
    b4 = b4 && decl->declarations[i].declarator->flags == 0;
    b4 = b4 && decli.name.content == decls[i];
    b4 = b4 && decli.components.size() == 0;
    if (!b4) {
      if (ExpectedMsg == "") ExpectedMsg = "From IsDeclaration Matcher: ";
      ExpectedMsg += "Declaration [" + to_string(i) +
                     "] has init != nullptr, def = jdi::builtin_type__int, flags = 0, name.content = " + decls[i] +
                     ", components.size() = 0\n";
      *result_listener << " got Declaration [" << to_string(i) << "] has init "
                       << ((decl->declarations[i].init) ? "!=" : "=")
                       << " nullptr, def = jdi::builtin_type__int, flags = "
                       << to_string(decl->declarations[i].declarator->flags)
                       << ", name.content = " << decli.name.content
                       << ", components.size() = " << to_string(decli.components.size()) << "\n";
    }
  }

  return b1 && b2 && b3 && b4;
}

MATCHER_P2(IsCast, cast_kind, expr_type, "") {
  auto *cast = dynamic_cast<AST::CastExpression *>(arg);
  auto *expr = dynamic_cast<AST::Node *>(cast->expr.get());

  bool b1 = arg->type == AST::NodeType::CAST, b2 = cast->ft.def == jdi::builtin_type__int, b3 = cast->ft.flags == 0,
       b4 = cast->ft.decl.components.size() == 0, b5 = cast->ft.decl.name.content == "",
       b6 = cast->ft.decl.has_nested_declarator == false, b7 = cast->kind == cast_kind, b8 = expr->type == expr_type;

  bool res = b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8;

  if (!res) {
    ExpectedMsg = "From IsCast Matcher: ";

    if (!b1) {
      ExpectedMsg += "NodeType = CAST\n";
      *result_listener << "got NodeType = " << NodeTypes[(int)arg->type] << "\n";
    }
    if (!b3) {
      ExpectedMsg += "ft.flags = 0\n";
      *result_listener << "got ft.flags = " << to_string(cast->ft.flags) << "\n";
    }
    if (!b4) {
      ExpectedMsg += "ft.decl.components.size() = 0\n";
      *result_listener << "got ft.decl.components.size() = " << to_string(cast->ft.decl.components.size()) << "\n";
    }
    if (!b5) {
      ExpectedMsg += "ft.decl.name.content = \"\"\n";
      *result_listener << "got ft.decl.name.content = " << cast->ft.decl.name.content << "\n";
    }
    if (!b6) {
      ExpectedMsg += "ft.decl.has_nested_declarator = false\n";
      *result_listener << "got ft.decl.has_nested_declarator = " << to_string(cast->ft.decl.has_nested_declarator)
                       << "\n";
    }
    if (!b7) {
      ExpectedMsg += "cast->kind = " + CastKinds[(int)cast_kind] + "\n";
      *result_listener << "got cast->kind = " << CastKinds[(int)cast->kind] << "\n";
    }
    if (!b8) {
      ExpectedMsg += "expr->type = " + NodeTypes[(int)expr_type] + "\n";
      *result_listener << "got expr->type = " << NodeTypes[(int)expr->type] << "\n";
    }
  }

  return res;
}

MATCHER_P5(IsBinaryOperation, op, left_, right_, iden, num, "") {
  auto *right = dynamic_cast<AST::Literal *>(arg->right.get());

  bool b1 = arg->type == AST::NodeType::BINARY_EXPRESSION, b2 = arg->operation == op, b3 = arg->left->type == left_,
       b4 = dynamic_cast<AST::IdentifierAccess *>(arg->left.get())->name.content == iden,
       b5 = arg->right->type == right_,
       b6 = std::get<std::string>(dynamic_cast<AST::Literal *>(right)->value.value) == num;

  bool res = b1 && b2 && b3 && b4 && b5 && b6;
  if (!res) {
    ExpectedMsg = "From IsBinaryOperation Matcher: ";

    if (!b1) {
      ExpectedMsg += "NodeType = BINARY_EXPRESSION\n";
      *result_listener << "got NodeType = " << NodeTypes[(int)arg->type] << "\n";
    }
    if (!b2) {
      ExpectedMsg += "Operation = " + enigma::parsing::ToString(op) + "\n";
      *result_listener << "got Operation = " << enigma::parsing::ToString(arg->operation) << "\n";
    }
    if (!b3) {
      ExpectedMsg += "Left Type = " + NodeTypes[(int)left_] + "\n";
      *result_listener << "got Left Type = " << NodeTypes[(int)arg->left->type] << "\n";
    }
    if (!b4) {
      ExpectedMsg += "Left Identifier = " + PrintToString(iden) + "\n";
      *result_listener << "got Left Identifier = "
                       << dynamic_cast<AST::IdentifierAccess *>(arg->left.get())->name.content << "\n";
    }
    if (!b5) {
      ExpectedMsg += "Right Type = " + NodeTypes[(int)right_] + "\n";
      *result_listener << "got Right Type = " << NodeTypes[(int)arg->right->type] << "\n";
    }
    if (!b6) {
      ExpectedMsg += "Right Literal = " + PrintToString(num) + "\n";
      *result_listener << "got Right Literal = "
                       << std::get<std::string>(dynamic_cast<AST::Literal *>(right)->value.value) << "\n";
    }
  }

  return res;
}

MATCHER_P2(IsUnaryPostfixOperator, op, iden, "") {
  auto *unary = dynamic_cast<AST::UnaryPostfixExpression *>(arg);

  bool b1 = unary->type == AST::NodeType::UNARY_POSTFIX_EXPRESSION, b2 = unary->operation == op,
       b3 = unary->operand->type == AST::NodeType::IDENTIFIER,
       b4 = dynamic_cast<AST::IdentifierAccess *>(unary->operand.get())->name.content == iden;

  bool res = b1 && b2 && b3 && b4;
  if (!res) {
    ExpectedMsg = "From IsUnaryPostfixOperator Matcher: ";

    if (!b1) {
      ExpectedMsg += "NodeType = UNARY_POSTFIX_EXPRESSION\n";
      *result_listener << "got NodeType = " << NodeTypes[(int)unary->type] << "\n";
    }
    if (!b2) {
      ExpectedMsg += "Operation = " + enigma::parsing::ToString(op) + "\n";
      *result_listener << "got Operation = " << enigma::parsing::ToString(unary->operation) << "\n";
    }
    if (!b3) {
      ExpectedMsg += "Operand Type = IDENTIFIER \n";
      *result_listener << "got Operand Type = " << NodeTypes[(int)unary->operand->type] << "\n";
    }
    if (!b4) {
      ExpectedMsg += "Operand Identifier = " + PrintToString(iden) + "\n";
      *result_listener << "got Operand Identifier = "
                       << dynamic_cast<AST::IdentifierAccess *>(unary->operand.get())->name.content << "\n";
    }
  }

  return res;
}

MATCHER_P2(IsUnaryPrefixOperator, op, iden, "") {
  auto *unary = dynamic_cast<AST::UnaryPrefixExpression *>(arg);

  bool b1 = unary->type == AST::NodeType::UNARY_PREFIX_EXPRESSION, b2 = unary->operation == op,
       b3 = unary->operand->type == AST::NodeType::IDENTIFIER,
       b4 = dynamic_cast<AST::IdentifierAccess *>(unary->operand.get())->name.content == iden;

  bool res = b1 && b2 && b3 && b4;
  if (!res) {
    ExpectedMsg = "From IsUnaryPrefixOperator Matcher: ";

    if (!b1) {
      ExpectedMsg += "NodeType = UNARY_PREFIX_EXPRESSION\n";
      *result_listener << "got NodeType = " << NodeTypes[(int)unary->type] << "\n";
    }
    if (!b2) {
      ExpectedMsg += "Operation = " + enigma::parsing::ToString(op) + "\n";
      *result_listener << "got Operation = " << enigma::parsing::ToString(unary->operation) << "\n";
    }
    if (!b3) {
      ExpectedMsg += "Operand Type = IDENTIFIER \n";
      *result_listener << "got Operand Type = " << NodeTypes[(int)unary->operand->type] << "\n";
    }
    if (!b4) {
      ExpectedMsg += "Operand Identifier = " + PrintToString(iden) + "\n";
      *result_listener << "got Operand Identifier = "
                       << dynamic_cast<AST::IdentifierAccess *>(unary->operand.get())->name.content << "\n";
    }
  }

  return res;
}

MATCHER_P(IsStatementBlock, stateSize, "") {
  bool b1 = arg->type == AST::NodeType::BLOCK, b2 = arg->statements.size() == size_t(stateSize);

  if (!b1 || !b2) {
    ExpectedMsg = "From IsStatementBlock Matcher: ";

    if (!b1) {
      ExpectedMsg += "NodeType = BLOCK";
      *result_listener << "got NodeType = " << NodeTypes[(int)arg->type] << "\n";
    }
    if (!b2) {
      ExpectedMsg = "IsStatementBlock Matcher: Statements Size = " + to_string(stateSize);
      *result_listener << "Statements Size = " << to_string(arg->statements.size()) << "\n";
    }
  }

  return b1 && b2;
}

MATCHER_P4(IsForLoopWithChildren, M1, M2, M3, M4, ExpectedMsg) {
  auto *assign = (arg->assignment.get());
  auto *binary = dynamic_cast<AST::BinaryExpression *>(arg->condition.get());
  auto *unary = (arg->increment.get());
  auto *block = dynamic_cast<AST::CodeBlock *>(arg->body.get());

  return ExplainMatchResult(M1, assign, result_listener) && ExplainMatchResult(M2, binary, result_listener) &&
         ExplainMatchResult(M3, unary, result_listener) && ExplainMatchResult(M4, block, result_listener);
}

TEST(ParserTest, ForLoop_1) {
  ParserTester test{"for (int i = 0; i < 5; i++) {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = dynamic_cast<AST::ForLoop *>(node.get());

  std::vector<std::string> decls = {"i"};

  ASSERT_THAT(for_, IsForLoopWithChildren(
                        IsDeclaration(decls),
                        IsBinaryOperation(TT_LESS, AST::NodeType::IDENTIFIER, AST::NodeType::LITERAL, "i", "5"),
                        IsUnaryPostfixOperator(TT_INCREMENT, "i"), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_2) {
  ParserTester test{"for int i = 0, j=1; i >= 12; --i {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = dynamic_cast<AST::ForLoop *>(node.get());

  std::vector<std::string> decls = {"i", "j"};

  ASSERT_THAT(for_, IsForLoopWithChildren(IsDeclaration(decls),
                                          IsBinaryOperation(TT_GREATEREQUAL, AST::NodeType::IDENTIFIER,
                                                            AST::NodeType::LITERAL, "i", "12"),
                                          IsUnaryPrefixOperator(TT_DECREMENT, "i"), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_3) {
  ParserTester test{"for (int)(i = 0); i < 5; i++ {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = dynamic_cast<AST::ForLoop *>(node.get());

  ASSERT_THAT(for_, IsForLoopWithChildren(
                        IsCast(AST::CastExpression::Kind::C_STYLE, AST::NodeType::PARENTHETICAL),
                        IsBinaryOperation(TT_LESS, AST::NodeType::IDENTIFIER, AST::NodeType::LITERAL, "i", "5"),
                        IsUnaryPostfixOperator(TT_INCREMENT, "i"), IsStatementBlock(0)));
}

TEST(ParserTest, ForLoop_4) {
  ParserTester test{"for int(i = 5); i < 5; i++ {}"};
  auto node = test->TryParseStatement();
  ASSERT_EQ(test->current_token().type, TT_ENDOFCODE);

  ASSERT_EQ(node->type, AST::NodeType::FOR);
  auto *for_ = dynamic_cast<AST::ForLoop *>(node.get());

  ASSERT_THAT(for_, IsForLoopWithChildren(
                        IsCast(AST::CastExpression::Kind::FUNCTIONAL, AST::NodeType::BINARY_EXPRESSION),
                        IsBinaryOperation(TT_LESS, AST::NodeType::IDENTIFIER, AST::NodeType::LITERAL, "i", "5"),
                        IsUnaryPostfixOperator(TT_INCREMENT, "i"), IsStatementBlock(0)));
}
