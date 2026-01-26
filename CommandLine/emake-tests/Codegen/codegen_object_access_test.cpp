#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>

// Test that object access uses glaccess() and int() casts
// Issue #1: Direct dereference causes null pointer hangs
// Issue #18: Object IDs need int() cast for varaccess functions

TEST(CodegenObjectAccessTest, ObjectMemberAccessUsesGlaccess) {
  std::string code = "obj.x";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  std::cerr << "[DEBUG] Test: node.get()=" << (void*)node.get() 
            << ", node->type=" << (int)node->type << std::endl;
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  
  std::cerr << "[DEBUG] Test: About to call As<CodeBlock>() on node at " << (void*)node.get() << std::endl;
  auto *block = node->As<AST::CodeBlock>();
  std::cerr << "[DEBUG] Test: As<CodeBlock>() returned block=" << (void*)block 
            << ", block->statements.size()=" << (block ? block->statements.size() : 0) << std::endl;
  
  if (block) {
    std::cerr << "[DEBUG] Test: block->type=" << (int)block->type 
              << ", &block->statements=" << (void*)&block->statements 
              << ", block address=" << (void*)block << std::endl;
    std::cerr << "[DEBUG] Test: About to dereference block with *block, &(*block)=" << (void*)&(*block) << std::endl;
    for (size_t i = 0; i < block->statements.size(); ++i) {
      std::cerr << "[DEBUG] Test: block->statements[" << i << "]=" 
                << (void*)block->statements[i].get() << std::endl;
    }
  }
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  std::cerr << "[DEBUG] Test: About to call VisitCode(*block), block=" << (void*)block << std::endl;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use glaccess() instead of direct cast
  EXPECT_NE(printed.find("glaccess"), std::string::npos)
      << "Object access should use glaccess(). Got: " << printed;
  
  // Should wrap object in int()
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Object should be wrapped in int(). Got: " << printed;
}

TEST(CodegenObjectAccessTest, ObjectMemberAccessWithIntCast) {
  std::string code = "obj_camera.x";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use glaccess with int() cast (Issue #1 fix)
  EXPECT_NE(printed.find("glaccess"), std::string::npos)
      << "Should use glaccess. Got: " << printed;
  
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Object ID should be wrapped in int(). Got: " << printed;
}

TEST(CodegenObjectAccessTest, ObjectAccessInExpression) {
  std::string code = "y = obj.x + 10";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use glaccess
  EXPECT_NE(printed.find("glaccess"), std::string::npos)
      << "Object access in expression should use glaccess(). Got: " << printed;
}

TEST(CodegenObjectAccessTest, MultipleObjectAccess) {
  std::string code = "obj1.x + obj2.y";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should use glaccess for both
  size_t first_glaccess = printed.find("glaccess");
  EXPECT_NE(first_glaccess, std::string::npos)
      << "First object access should use glaccess(). Got: " << printed;
  
  size_t second_glaccess = printed.find("glaccess", first_glaccess + 1);
  EXPECT_NE(second_glaccess, std::string::npos)
      << "Second object access should also use glaccess(). Got: " << printed;
}
