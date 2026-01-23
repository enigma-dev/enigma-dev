#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <cstdio>

// Test that argument[N] in scripts uses direct array access, not varaccess_argument
// The argument array is a local array in script functions, not an instance variable.
// See CompilerSource/docs/script-with-self-and-argument-handling.md

TEST(CodegenArgumentVaraccessTest, ArgumentArrayAccessInScriptUsesDirectAccess) {
  std::string code = "argument[0]";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  // Test as script (is_script=true, is_object_script=false)
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_1.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  // Read from the temp file since GetPrintedCode() reads from a hardcoded path
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should use argument[int(N)] directly, NOT varaccess_argument
  EXPECT_NE(printed.find("argument["), std::string::npos)
      << "Should use argument array directly. Got: " << printed;
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Array index should be wrapped in int(). Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_argument"), std::string::npos)
      << "Should NOT use varaccess_argument. Got: " << printed;
}

TEST(CodegenArgumentVaraccessTest, ArgumentArrayAccessInObjectScriptUsesDirectAccess) {
  std::string code = "argument[1]";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  // Test as object script (is_script=true, is_object_script=true)
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_2.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, true);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should use argument[int(N)] directly, same as global scripts
  EXPECT_NE(printed.find("argument["), std::string::npos)
      << "Should use argument array directly. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_argument"), std::string::npos)
      << "Should NOT use varaccess_argument. Got: " << printed;
}

TEST(CodegenArgumentVaraccessTest, ArgumentArrayAccessWithExpression) {
  std::string code = "argument[i + 1]";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_3.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should use argument array directly with int() cast on index expression
  EXPECT_NE(printed.find("argument["), std::string::npos)
      << "Should use argument array. Got: " << printed;
  EXPECT_NE(printed.find("int("), std::string::npos)
      << "Index expression should be wrapped in int(). Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_argument"), std::string::npos)
      << "Should NOT use varaccess_argument. Got: " << printed;
}

TEST(CodegenArgumentVaraccessTest, ArgumentArrayAccessInExpression) {
  std::string code = "x = argument[0] + 10";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_4.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should use argument array directly in expression
  EXPECT_NE(printed.find("argument["), std::string::npos)
      << "Should use argument array. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_argument"), std::string::npos)
      << "Should NOT use varaccess_argument. Got: " << printed;
}
