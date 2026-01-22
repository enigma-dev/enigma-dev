#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>

// Test that code after return is not generated
// Issue #2: Unreachable code after return should not be printed

TEST(CodegenReturnStatementTest, CodeAfterReturnIsNotGenerated) {
  std::string code = "return 0; x = 10;";
  std::cerr << "[TEST] Parsing code: " << code << std::endl;
  
  ParserTester test = ParserTester::CreateWithCpp(code);
  std::cerr << "[TEST] ParserTester created" << std::endl;
  
  auto node = test->ParseCode();
  std::cerr << "[TEST] ParseCode completed, node type: " << (int)node->type << std::endl;
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  ASSERT_NE(block, nullptr);
  
  std::cerr << "[TEST] CodeBlock has " << block->statements.size() << " statements" << std::endl;
  for (size_t i = 0; i < block->statements.size(); ++i) {
    if (block->statements[i]) {
      std::cerr << "[TEST]   Statement " << i << ": type=" << (int)block->statements[i]->type 
                << " ptr=" << (void*)block->statements[i].get() << std::endl;
    } else {
      std::cerr << "[TEST]   Statement " << i << ": NULL" << std::endl;
    }
  }
  
  AST::CppPrettyPrinter v;
  std::cerr << "[TEST] CppPrettyPrinter created, starting VisitCode..." << std::endl;
  
  bool visit_result = v.VisitCode(*block);
  std::cerr << "[TEST] VisitCode completed, result: " << (visit_result ? "true" : "false") << std::endl;
  
  ASSERT_TRUE(visit_result);
  std::string printed = v.GetPrintedCode();
  std::cerr << "[TEST] GetPrintedCode completed, output length: " << printed.length() << std::endl;
  std::cerr << "[TEST] Printed output: [" << printed << "]" << std::endl;
  
  // Should contain return 0
  EXPECT_NE(printed.find("return 0"), std::string::npos)
      << "Return statement should be present. Got: " << printed;
  
  // Should NOT contain x = 10 (unreachable code after return)
  EXPECT_EQ(printed.find("x = 10"), std::string::npos)
      << "Code after return should NOT be generated. Got: " << printed;
}

TEST(CodegenReturnStatementTest, ReturnInBlockStopsGeneration) {
  std::string code = "{ x = 5; return 0; y = 10; }";
  std::cerr << "[TEST] Parsing code: " << code << std::endl;
  
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  std::cerr << "[TEST] ParseCode completed, node type: " << (int)node->type << std::endl;
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  ASSERT_NE(block, nullptr);
  
  std::cerr << "[TEST] CodeBlock has " << block->statements.size() << " statements" << std::endl;
  
  AST::CppPrettyPrinter v;
  std::cerr << "[TEST] Starting VisitCode..." << std::endl;
  ASSERT_TRUE(v.VisitCode(*block));
  std::cerr << "[TEST] VisitCode completed" << std::endl;
  
  std::string printed = v.GetPrintedCode();
  std::cerr << "[TEST] Printed output: [" << printed << "]" << std::endl;
  
  // Should contain x = 5 and return 0
  EXPECT_NE(printed.find("x = 5"), std::string::npos)
      << "Code before return should be present. Got: " << printed;
  EXPECT_NE(printed.find("return 0"), std::string::npos)
      << "Return statement should be present. Got: " << printed;
  
  // Should NOT contain y = 10 (unreachable after return)
  EXPECT_EQ(printed.find("y = 10"), std::string::npos)
      << "Code after return should NOT be generated. Got: " << printed;
}

TEST(CodegenReturnStatementTest, ReturnWithValueStopsGeneration) {
  std::string code = "return 42; z = 99;";
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
  
  // Should contain return 42
  EXPECT_NE(printed.find("return 42"), std::string::npos)
      << "Return statement should be present. Got: " << printed;
  
  // Should NOT contain z = 99
  EXPECT_EQ(printed.find("z = 99"), std::string::npos)
      << "Code after return should NOT be generated. Got: " << printed;
}

TEST(CodegenReturnStatementTest, MultipleReturnsOnlyFirstGenerated) {
  std::string code = "return 1; return 2; return 3;";
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  AST::CppPrettyPrinter v;
  ASSERT_TRUE(v.VisitCode(*block));
  std::string printed = v.GetPrintedCode();
  
  // Should contain first return
  EXPECT_NE(printed.find("return 1"), std::string::npos)
      << "First return should be present. Got: " << printed;
  
  // Should NOT contain subsequent returns
  EXPECT_EQ(printed.find("return 2"), std::string::npos)
      << "Second return should NOT be generated. Got: " << printed;
  EXPECT_EQ(printed.find("return 3"), std::string::npos)
      << "Third return should NOT be generated. Got: " << printed;
}
