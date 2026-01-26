#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>
#include <algorithm>

// Test pattern verification in generated code
// Verifies specific patterns match expected counts/usage

TEST(CodegenPatternVerificationTest, DoubleCastPattern) {
  // Test that (double) casts are NOT present in C++ output for division
  // (double) casts are only added in GML/script output, not C++ output
  std::string code = "a / b; c / d; e / f;";
  ParserTester test = ParserTester::CreateWithCpp(code);
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
  
  AST::CppPrettyPrinter v;
  std::cerr << "[DEBUG] Test: About to call VisitCode(*block), block=" << (void*)block << std::endl;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Count occurrences of " / (double)"
  size_t count = 0;
  size_t pos = 0;
  while ((pos = printed.find(" / (double)", pos)) != std::string::npos) {
    count++;
    pos += 11;  // Move past this occurrence
  }
  
  // C++ output should NOT have (double) casts
  EXPECT_EQ(count, 0) << "C++ output should NOT have (double) casts for division. Got: " << printed;
}

TEST(CodegenPatternVerificationTest, GlaccessPattern) {
  // Test that glaccess() is used for object access
  std::string code = "obj.x; obj.y; obj.z;";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Count occurrences of "glaccess"
  size_t count = std::count(printed.begin(), printed.end(), 'g');
  // More precise: count "glaccess" substring
  size_t glaccess_count = 0;
  size_t pos = 0;
  while ((pos = printed.find("glaccess", pos)) != std::string::npos) {
    glaccess_count++;
    pos += 8;  // Move past "glaccess"
  }
  
  // Should use glaccess for object member access
  EXPECT_GT(glaccess_count, 0) << "Should use glaccess() for object access. Got: " << printed;
}

TEST(CodegenPatternVerificationTest, IntCastPattern) {
  // Test that int() casts are present for object IDs and array indices
  std::string code = "obj_camera.x; arr[0]; arr[1];";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v(test.lexer.GetContext().language_fe);
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Count occurrences of "int("
  size_t count = 0;
  size_t pos = 0;
  while ((pos = printed.find("int(", pos)) != std::string::npos) {
    count++;
    pos += 4;  // Move past "int("
  }
  
  // Should have int() casts for object IDs and array indices
  EXPECT_GT(count, 0) << "Should have int() casts. Got: " << printed;
}

TEST(CodegenPatternVerificationTest, ModKeywordPreserved) {
  // Test that mod keyword is preserved (not converted to %)
  std::string code = "a mod b";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should contain "mod" keyword
  EXPECT_NE(printed.find("mod"), std::string::npos)
      << "mod keyword should be preserved. Got: " << printed;
  
  // Should NOT contain "% (variant )" pattern (old bug)
  EXPECT_EQ(printed.find("% (variant )"), std::string::npos)
      << "mod should NOT be converted to % (variant ). Got: " << printed;
}

TEST(CodegenPatternVerificationTest, ArgumentDirectParameter) {
  // Test that argument0-argument15 are direct parameters
  std::string code = "argument0; argument1; argument2;";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should contain argument0, argument1, argument2 directly
  EXPECT_NE(printed.find("argument0"), std::string::npos)
      << "argument0 should be direct parameter. Got: " << printed;
  EXPECT_NE(printed.find("argument1"), std::string::npos)
      << "argument1 should be direct parameter. Got: " << printed;
  EXPECT_NE(printed.find("argument2"), std::string::npos)
      << "argument2 should be direct parameter. Got: " << printed;
  
  // Should NOT use varaccess_argument0, etc.
  EXPECT_EQ(printed.find("varaccess_argument0"), std::string::npos)
      << "argument0 should NOT use varaccess. Got: " << printed;
}
