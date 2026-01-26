#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>

// Test that argument0-argument15 are direct parameters and argument[N] uses array
// Issue #19, #20: argument0 should be direct parameter, argument[0] should use array access

TEST(CodegenArgumentHandlingTest, Argument0IsDirectParameter) {
  std::string code = "argument0";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should output "argument0" directly, not "varaccess_argument0"
  EXPECT_NE(printed.find("argument0"), std::string::npos)
      << "argument0 should be output directly. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_argument0"), std::string::npos)
      << "argument0 should NOT use varaccess. Got: " << printed;
}

TEST(CodegenArgumentHandlingTest, Argument1IsDirectParameter) {
  std::string code = "argument1";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should output "argument1" directly
  EXPECT_NE(printed.find("argument1"), std::string::npos)
      << "argument1 should be output directly. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_argument1"), std::string::npos)
      << "argument1 should NOT use varaccess. Got: " << printed;
}

TEST(CodegenArgumentHandlingTest, ArgumentArrayAccessHasIntCast) {
  std::string code = "argument[0]";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Array index should be wrapped in int()
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Array index should be wrapped in int(). Got: " << printed;
  EXPECT_NE(printed.find("argument["), std::string::npos)
      << "Should use argument array. Got: " << printed;
}

TEST(CodegenArgumentHandlingTest, ArgumentArrayAccessWithExpression) {
  std::string code = "argument[i + 1]";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Index expression should be wrapped in int()
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Index expression should be wrapped in int(). Got: " << printed;
}
