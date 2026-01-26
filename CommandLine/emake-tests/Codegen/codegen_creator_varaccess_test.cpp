#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>

// Test that creator variable accessed via dot notation generates varaccess_creator
// Example: object.creator = value should generate varaccess_creator function

TEST(CodegenCreatorVaraccessTest, ObjectCreatorDotAccess) {
  std::string code = "obj.creator";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use varaccess_creator for dot access
  EXPECT_NE(printed.find("varaccess_creator"), std::string::npos)
      << "Should use varaccess_creator for object.creator. Got: " << printed;
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Object ID should be wrapped in int(). Got: " << printed;
}

TEST(CodegenCreatorVaraccessTest, ObjectCreatorAssignment) {
  std::string code = "obj.creator = 5";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use varaccess_creator for assignment
  EXPECT_NE(printed.find("varaccess_creator"), std::string::npos)
      << "Should use varaccess_creator for object.creator assignment. Got: " << printed;
}

TEST(CodegenCreatorVaraccessTest, SelfCreatorAccess) {
  std::string code = "self.creator";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use varaccess_creator with int(self)
  EXPECT_NE(printed.find("varaccess_creator"), std::string::npos)
      << "Should use varaccess_creator for self.creator. Got: " << printed;
  EXPECT_NE(printed.find("int(self)"), std::string::npos)
      << "Should use int(self) for self.creator. Got: " << printed;
}
