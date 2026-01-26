#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <cstdio>
#include <unistd.h>

// Test that part_type_* functions are called directly, not via varaccess_* functions
// Functions should never be in dot_accessed_locals - they're called directly, not accessed as variables.

TEST(CodegenFunctionNotVaraccessTest, PartTypeCreateIsFunctionCall) {
  std::string code = "part_type_create()";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  // Test as script (is_script=true) to ensure functions aren't treated as variables
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_func1.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should call part_type_create() directly, NOT varaccess_part_type_create
  EXPECT_NE(printed.find("part_type_create"), std::string::npos)
      << "Should call part_type_create function. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_part_type_create"), std::string::npos)
      << "Should NOT use varaccess_part_type_create. Got: " << printed;
}

TEST(CodegenFunctionNotVaraccessTest, PartTypeSizeIsFunctionCall) {
  std::string code = "part_type_size(id, 1, 2, 0, 0)";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_func2.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should call part_type_size() directly
  EXPECT_NE(printed.find("part_type_size"), std::string::npos)
      << "Should call part_type_size function. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_part_type_size"), std::string::npos)
      << "Should NOT use varaccess_part_type_size. Got: " << printed;
}

TEST(CodegenFunctionNotVaraccessTest, PartTypeLifeIsFunctionCall) {
  std::string code = "part_type_life(id, 10, 20)";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_func3.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should call part_type_life() directly
  EXPECT_NE(printed.find("part_type_life"), std::string::npos)
      << "Should call part_type_life function. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_part_type_life"), std::string::npos)
      << "Should NOT use varaccess_part_type_life. Got: " << printed;
}

TEST(CodegenFunctionNotVaraccessTest, PartTypeBlendIsFunctionCall) {
  std::string code = "part_type_blend(id, true)";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_func4.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should call part_type_blend() directly
  EXPECT_NE(printed.find("part_type_blend"), std::string::npos)
      << "Should call part_type_blend function. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_part_type_blend"), std::string::npos)
      << "Should NOT use varaccess_part_type_blend. Got: " << printed;
}

TEST(CodegenFunctionNotVaraccessTest, PartTypeOrientationIsFunctionCall) {
  std::string code = "part_type_orientation(id, 0, 360, 0, 0, true)";
  ParserTester test = ParserTester::CreateWithSetUp(code);
  auto node = test->ParseCode();
  
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto *block = node->As<AST::CodeBlock>();
  
  std::string tmp_file = "/tmp/enigma_test_" + std::to_string(getpid()) + "_func5.txt";
  std::ofstream dummy_ofs(tmp_file, std::ios::out | std::ios::trunc);
  AST::CppPrettyPrinter v(dummy_ofs, test.lexer.GetContext().language_fe, true, false);
  ASSERT_TRUE(v.VisitCode(*block));
  dummy_ofs.close();
  std::ifstream ifs(tmp_file);
  std::string printed((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
  ifs.close();
  std::remove(tmp_file.c_str());
  
  // Should call part_type_orientation() directly
  EXPECT_NE(printed.find("part_type_orientation"), std::string::npos)
      << "Should call part_type_orientation function. Got: " << printed;
  EXPECT_EQ(printed.find("varaccess_part_type_orientation"), std::string::npos)
      << "Should NOT use varaccess_part_type_orientation. Got: " << printed;
}
