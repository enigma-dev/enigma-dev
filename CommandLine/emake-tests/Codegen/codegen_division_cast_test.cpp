#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>

// Test that division operations do NOT add (double) cast in C++ output
// (double) casts are only added in GML/script output, not C++ output
// Issue #3: Missing (double) casts cause integer division with variant types in GML

TEST(CodegenDivisionCastTest, SimpleDivision) {
  std::string code = "a / b";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  std::cerr << "[DEBUG] Test: node.get()=" << (void*)node.get() 
            << ", node->type=" << (int)node->type << std::endl;
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  
  std::cerr << "[DEBUG] Test: About to call As<CodeBlock>()" << std::endl;
  auto *block = node->As<AST::CodeBlock>();
  std::cerr << "[DEBUG] Test: As<CodeBlock>() returned block=" << (void*)block 
            << ", block->statements.size()=" << (block ? block->statements.size() : 0) << std::endl;
  
  if (block) {
    std::cerr << "[DEBUG] Test: block->type=" << (int)block->type 
              << ", &block->statements=" << (void*)&block->statements << std::endl;
    for (size_t i = 0; i < block->statements.size(); ++i) {
      std::cerr << "[DEBUG] Test: block->statements[" << i << "]=" 
                << (void*)block->statements[i].get() << std::endl;
    }
  }
  
  AST::CppPrettyPrinter v;
  std::cerr << "[DEBUG] Test: About to call VisitCode(*block)" << std::endl;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // C++ output should NOT contain " / (double)" pattern
  EXPECT_EQ(printed.find(" / (double)"), std::string::npos) 
      << "Division in C++ output should NOT cast right operand to double. Got: " << printed;
  // Should contain plain division
  EXPECT_NE(printed.find(" / "), std::string::npos) 
      << "Division should be present. Got: " << printed;
}

TEST(CodegenDivisionCastTest, DivisionInExpression) {
  std::string code = "x = a / b + c";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // C++ output should NOT contain " / (double)" pattern
  EXPECT_EQ(printed.find(" / (double)"), std::string::npos)
      << "Division in C++ output should NOT cast right operand. Got: " << printed;
}

TEST(CodegenDivisionCastTest, NestedDivision) {
  std::string code = "x = a / b / c";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // C++ output should NOT contain " / (double)" patterns
  EXPECT_EQ(printed.find(" / (double)"), std::string::npos)
      << "Nested division in C++ output should NOT cast. Got: " << printed;
}

TEST(CodegenDivisionCastTest, DivisionWithParentheses) {
  std::string code = "x = (a + b) / (c + d)";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // C++ output should NOT contain " / (double)" pattern
  EXPECT_EQ(printed.find(" / (double)"), std::string::npos)
      << "Division with parentheses in C++ output should NOT cast. Got: " << printed;
}

TEST(CodegenDivisionCastTest, DivisionInAssignment) {
  std::string code = "velocity = distance / time";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // C++ output should NOT contain " / (double)" pattern
  EXPECT_EQ(printed.find(" / (double)"), std::string::npos)
      << "Division in assignment in C++ output should NOT cast. Got: " << printed;
}
