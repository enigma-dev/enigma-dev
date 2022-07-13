#include <parsing/ast.h>
#include <parsing/parser.cpp>

#include <gtest/gtest.h>

using namespace ::enigma::parsing;

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
  AstBuilder builder;

  AstBuilder *operator->() { return &builder; }

  ParserTester(std::string code, bool use_cpp = false)
      : context(&ParseContext::ForTesting(use_cpp)),
        lexer(std::move(code), context, &herr),
        builder{&lexer, &herr, SyntaxMode::STRICT} {}
};

TEST(ParserTest, Basics) {
  ParserTester test{"(x ? y : z ? a : (z[5](6)))"};

  auto node = test->TryParseExpression(Precedence::kAll);
  ASSERT_EQ(node->type, AST::NodeType::PARENTHETICAL);

  auto *expr = dynamic_cast<AST::Parenthetical *>(node.get())->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::TERNARY_EXPRESSION);

  auto *ternary = dynamic_cast<AST::TernaryExpression *>(expr);
  auto *cond = ternary->condition.get();
  auto *true_ = ternary->true_expression.get();
  auto *false_ = ternary->false_expression.get();
  ASSERT_EQ(cond->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(cond)->value.value), "x");

  ASSERT_EQ(true_->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(true_)->value.value), "y");

  ASSERT_EQ(false_->type, AST::NodeType::TERNARY_EXPRESSION);

  ternary = dynamic_cast<AST::TernaryExpression *>(false_);
  cond = ternary->condition.get();
  true_ = ternary->true_expression.get();
  false_ = ternary->false_expression.get();

  ASSERT_EQ(cond->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(cond)->value.value), "z");

  ASSERT_EQ(true_->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(true_)->value.value), "a");

  ASSERT_EQ(false_->type, AST::NodeType::PARENTHETICAL);
  expr = dynamic_cast<AST::Parenthetical *>(false_)->expression.get();
  ASSERT_EQ(expr->type, AST::NodeType::FUNCTION_CALL);
  auto *function = dynamic_cast<AST::FunctionCallExpression *>(expr);
  auto *called = function->function.get();
  auto *args = &function->arguments;

  ASSERT_EQ(called->type, AST::NodeType::BINARY_EXPRESSION);

  auto *bin = dynamic_cast<AST::BinaryExpression*>(called);
  ASSERT_EQ(bin->operation, TokenType::TT_BEGINBRACKET);
  ASSERT_EQ(bin->left->type, AST::NodeType::LITERAL);
  auto *left = dynamic_cast<AST::Literal*>(bin->left.get());
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(left)->value.value), "z");

  ASSERT_EQ(bin->right->type, AST::NodeType::LITERAL);
  auto *right = dynamic_cast<AST::Literal*>(bin->right.get());
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(right)->value.value), "5");

  ASSERT_EQ(args->size(), 1);
  auto *arg = (*args)[0].get();
  ASSERT_EQ(arg->type, AST::NodeType::LITERAL);
  ASSERT_EQ(std::get<std::string>(dynamic_cast<AST::Literal *>(arg)->value.value), "6");
}