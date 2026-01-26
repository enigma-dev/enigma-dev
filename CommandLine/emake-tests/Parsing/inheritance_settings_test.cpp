#include "../Parsing/parser-test-classes.h"
#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <unistd.h>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <parsing/parser.h>

using namespace ::enigma::parsing;

// Helper function to build YAML settings string
std::string BuildYamlSettings(const std::string& settings) {
  return settings;
}

// Helper function to parse code and get pretty-printed output
std::string ParseAndPrint(const std::string& code, const std::string& yaml_settings) {
  ParserTester test = ParserTester::CreateWithSettings(code, yaml_settings);
  auto node = test->ParseCode();
  EXPECT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  EXPECT_NE(block, nullptr);
  
  AST::CppPrettyPrinter v(test.context->language_fe);
  EXPECT_TRUE(v.VisitCode(*block));
  return v.GetPrintedCode();
}

// Helper function to parse code and get tokens
std::vector<Token> ParseTokens(const std::string& code, const std::string& yaml_settings) {
  ParserTester test = ParserTester::CreateWithSettings(code, yaml_settings);
  // builder->initialize() already consumed the first token, so we need to get tokens starting from the current one
  std::vector<Token> tokens;
  // Add the current token if it's not TT_ENDOFCODE
  Token tok = test.builder->current_token();
  int token_count = 0;
  if (tok.type != TT_ENDOFCODE && tok.type != TT_ERROR) {
    tokens.push_back(tok);
    token_count++;
  }
  // Continue reading tokens
  while (tok.type != TT_ENDOFCODE && tok.type != TT_ERROR) {
    tok = test.lexer.ReadToken();
    if (tok.type != TT_ENDOFCODE && tok.type != TT_ERROR) {
      tokens.push_back(tok);
      token_count++;
    }
    if (token_count > 100) {
      break;
    }
  }
  return tokens;
}

// ============================================================================
// inherit-strings-from Tests
// ============================================================================

TEST(InheritStringsFromTest, SingleQuoteInGmlModeIsString) {
  std::string code = "'A'";
  std::string yaml = "inherit-strings-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_STRINGLIT);
  EXPECT_EQ(tokens[0].content, "A");
}

TEST(InheritStringsFromTest, SingleQuoteInCppModeIsChar) {
  std::string code = "'A'";
  std::string yaml = "inherit-strings-from: 1\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_CHARLIT);
  EXPECT_EQ(tokens[0].content, "A");
}

TEST(InheritStringsFromTest, DoubleQuoteAlwaysString) {
  std::string code = "\"A\"";
  std::string yaml_gml = "inherit-strings-from: 0\n";
  std::string yaml_cpp = "inherit-strings-from: 1\n";
  
  auto tokens_gml = ParseTokens(code, yaml_gml);
  auto tokens_cpp = ParseTokens(code, yaml_cpp);
  
  ASSERT_GE(tokens_gml.size(), 1);
  ASSERT_GE(tokens_cpp.size(), 1);
  EXPECT_EQ(tokens_gml[0].type, TT_STRINGLIT);
  EXPECT_EQ(tokens_cpp[0].type, TT_STRINGLIT);
}

// ============================================================================
// inherit-escapes-from Tests
// ============================================================================

TEST(InheritEscapesFromTest, GmlModeHashBecomesNewline) {
  std::string code = "\"hello#world\"";
  std::string yaml = "inherit-escapes-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_STRINGLIT);
  // The string value should contain \n between hello and world
  std::string value = tokens[0].content;
  EXPECT_NE(value.find('\n'), std::string::npos) 
      << "Expected newline in string value, got: " << value;
}

TEST(InheritEscapesFromTest, GmlModeBackslashHashBecomesHash) {
  std::string code = "\"hello\\#world\"";
  std::string yaml = "inherit-escapes-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_STRINGLIT);
  // The string value should contain # (not \n)
  std::string value = tokens[0].content;
  EXPECT_NE(value.find('#'), std::string::npos) 
      << "Expected # in string value, got: " << value;
  EXPECT_EQ(value.find('\n'), std::string::npos) 
      << "Should not have newline, got: " << value;
}

TEST(InheritEscapesFromTest, GmlModeIgnoredBackslashesAreEscaped) {
  std::string code = "\"hello \\' \\\\ \\# # world\"";
  std::string yaml = "inherit-escapes-from: 0\n";
  
  std::string printed = ParseAndPrint(code, yaml);
  // Expected C++ output: "hello \\\' \\\\ # \n world"
  EXPECT_NE(printed.find("\\\\'"), std::string::npos) 
      << "Expected escaped backslash and quote, got: " << printed;
  EXPECT_NE(printed.find("\\\\\\\\"), std::string::npos) 
      << "Expected escaped backslashes, got: " << printed;
  EXPECT_NE(printed.find("\\n"), std::string::npos) 
      << "Expected newline escape, got: " << printed;
}

TEST(InheritEscapesFromTest, CppModeStandardEscapes) {
  std::string code = "\"hello\\nworld\"";
  std::string yaml = "inherit-escapes-from: 1\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_STRINGLIT);
  std::string value = tokens[0].content;
  EXPECT_NE(value.find('\n'), std::string::npos) 
      << "Expected newline in string value, got: " << value;
}

TEST(InheritEscapesFromTest, OutputAlwaysUsesCppEscapes) {
  std::string code = "\"hello#world\"";
  std::string yaml_gml = "inherit-escapes-from: 0\n";
  
  std::string printed = ParseAndPrint(code, yaml_gml);
  // Output should use C++ escapes (\n) even though input used GML (#)
  EXPECT_NE(printed.find("\\n"), std::string::npos) 
      << "Output should use C++ escape sequences, got: " << printed;
  EXPECT_EQ(printed.find("#"), std::string::npos) 
      << "Output should not contain raw #, got: " << printed;
}

// ============================================================================
// inherit-literals-from Tests
// ============================================================================

TEST(InheritLiteralsFromTest, GmlModeDollarHex) {
  std::string code = "$DEADBEEF";
  std::string yaml = "inherit-literals-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_HEXLITERAL);
  EXPECT_EQ(tokens[0].content, "DEADBEEF");
}

TEST(InheritLiteralsFromTest, CppMode0xHex) {
  std::string code = "0xDEADBEEF";
  std::string yaml = "inherit-literals-from: 1\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_HEXLITERAL);
  EXPECT_EQ(tokens[0].content, "DEADBEEF");
}

TEST(InheritLiteralsFromTest, GmlModeNoOctal) {
  std::string code = "0777";
  std::string yaml = "inherit-literals-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  // In GML mode, 0777 should be parsed as decimal, not octal
  EXPECT_EQ(tokens[0].type, TT_DECLITERAL);
  EXPECT_EQ(tokens[0].content, "0777");
}

TEST(InheritLiteralsFromTest, CppModeOctalSupport) {
  std::string code = "0o777";
  std::string yaml = "inherit-literals-from: 1\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_OCTLITERAL);
  EXPECT_EQ(tokens[0].content, "777");
}

TEST(InheritLiteralsFromTest, CppModeBinarySupport) {
  std::string code = "0b111000";
  std::string yaml = "inherit-literals-from: 1\n";
  
  auto tokens = ParseTokens(code, yaml);
  ASSERT_GE(tokens.size(), 1);
  EXPECT_EQ(tokens[0].type, TT_BINLITERAL);
  EXPECT_EQ(tokens[0].content, "111000");
}

// ============================================================================
// inherit-increment-from Tests
// ============================================================================

TEST(InheritIncrementFromTest, GmlModeRejectsIncrement) {
  std::string code = "x++";
  std::string yaml = "inherit-increment-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  // In GML mode, ++ should be parsed as two separate + tokens, not TT_INCREMENT
  bool found_increment = false;
  for (const auto& tok : tokens) {
    if (tok.type == TT_INCREMENT) {
      found_increment = true;
      break;
    }
  }
  EXPECT_FALSE(found_increment) 
      << "GML mode should not recognize ++ as increment operator";
  
  // Check for two plus tokens instead
  int plus_count = 0;
  for (const auto& tok : tokens) {
    if (tok.type == TT_PLUS) {
      plus_count++;
    }
  }
  EXPECT_GE(plus_count, 2) 
      << "GML mode should parse ++ as two + tokens";
}

TEST(InheritIncrementFromTest, CppModeAcceptsIncrement) {
  std::string code = "x++";
  std::string yaml = "inherit-increment-from: 1\n";
  
  auto tokens = ParseTokens(code, yaml);
  bool found_increment = false;
  for (const auto& tok : tokens) {
    if (tok.type == TT_INCREMENT) {
      found_increment = true;
      break;
    }
  }
  EXPECT_TRUE(found_increment) 
      << "C++ mode should recognize ++ as increment operator";
}

TEST(InheritIncrementFromTest, GmlModeRejectsDecrement) {
  std::string code = "x--";
  std::string yaml = "inherit-increment-from: 0\n";
  
  auto tokens = ParseTokens(code, yaml);
  // In GML mode, -- should be parsed as two separate - tokens, not TT_DECREMENT
  bool found_decrement = false;
  for (const auto& tok : tokens) {
    if (tok.type == TT_DECREMENT) {
      found_decrement = true;
      break;
    }
  }
  EXPECT_FALSE(found_decrement) 
      << "GML mode should not recognize -- as decrement operator";
}

// ============================================================================
// inherit-equivalence-from Tests
// ============================================================================

TEST(InheritEquivalenceFromTest, GmlModeEqualsIsComparison) {
  std::string code = "if a = b { }";
  std::string yaml = "inherit-equivalence-from: 0\n";
  
  ParserTester test = ParserTester::CreateWithSettings(code, yaml);
  auto node = test->ParseCode();
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  
  // In GML mode, = in conditionals should be converted to ==
  // We need to traverse the AST to find the binary expression and check its operator
  AST::CppPrettyPrinter v(test.context->language_fe);
  EXPECT_TRUE(v.VisitCode(*node->As<AST::CodeBlock>()));
  std::string printed = v.GetPrintedCode();
  
  // Check if == is used (GML mode) or = is used (C++ mode)
  EXPECT_NE(printed.find("=="), std::string::npos) 
      << "GML mode should convert = to == in conditionals, got: " << printed;
}

TEST(InheritEquivalenceFromTest, CppModeEqualsIsAssignment) {
  std::string code = "if a = b { }";
  std::string yaml = "inherit-equivalence-from: 1\n";
  
  ParserTester test = ParserTester::CreateWithSettings(code, yaml);
  auto node = test->ParseCode();
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  
  AST::CppPrettyPrinter v(test.context->language_fe);
  EXPECT_TRUE(v.VisitCode(*node->As<AST::CodeBlock>()));
  std::string printed = v.GetPrintedCode();
  
  // In C++ mode, = should remain as assignment (though compiler may warn)
  EXPECT_NE(printed.find(" = "), std::string::npos) 
      << "C++ mode should keep = as assignment, got: " << printed;
}

// ============================================================================
// automatic-semicolons Tests
// ============================================================================

// Error handler that allows errors to be reported without failing the test
class AllowErrorsErrorHandler : public ErrorHandler {
 public:
  void ReportError(CodeSnippet snippet, std::string_view error) final {
    // Allow errors to be reported without failing the test
    // This is used for tests where errors are expected
  }
  void ReportWarning(CodeSnippet snippet, std::string_view warning) final {
    // Allow warnings to be reported without failing the test
  }
};

TEST(AutomaticSemicolonsTest, StrictModeRequiresSemicolons) {
  std::string code = "x = 5 y = 10";
  std::string yaml = "automatic-semicolons: false\n";
  
  // In strict mode, this should error or fail to parse
  // Create a custom ParserTester with an error handler that allows errors
  AllowErrorsErrorHandler allow_errors_herr;
  
  // Build full YAML with required fields
  std::string full_yaml = "%e-yaml\n---\n";
  full_yaml += "target-windowing: None\n";
  full_yaml += "target-audio: None\n";
  full_yaml += "target-compiler: gcc\n";
  full_yaml += "target-graphics: None\n";
  full_yaml += "target-widget: None\n";
  full_yaml += "target-collision: None\n";
  full_yaml += "target-networking: None\n";
  full_yaml += "inherit-strings-from: 0\n";
  full_yaml += "inherit-escapes-from: 0\n";
  full_yaml += "inherit-literals-from: 0\n";
  full_yaml += "inherit-increment-from: 0\n";
  full_yaml += "inherit-equivalence-from: 0\n";
  full_yaml += "automatic-semicolons: true\n";  // Default
  full_yaml += yaml;  // Override with user settings
  
  // Initialize context if needed
  if (!main_context) {
    main_context = new clang_adapter::ClangContext();
  }
  
  // Create a new lang_CPP instance for this test
  auto custom_cpp = std::make_unique<lang_CPP>();
  syntax_error* err = custom_cpp->definitionsModified(NULL, full_yaml.c_str());
  
  // Create ParseContext with the configured language frontend
  auto custom_context = std::make_unique<ParseContext>(custom_cpp.get(), kNoNames);
  const ParseContext* ctx = custom_context.get();
  
  // Create lexer and builder with the allow-errors handler
  Lexer lexer(code, ctx, &allow_errors_herr);
  AstBuilderTestAPI* builder = CreateBuilder();
  builder->initialize(&lexer, &allow_errors_herr);
  
  auto node = builder->ParseCode();
  
  // Strict mode should either error or parse as single statement
  // We check that it doesn't parse as two separate statements
  EXPECT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  if (block) {
    // In strict mode, this should fail or have only one statement
    EXPECT_LE(block->statements.size(), 1) 
        << "Strict mode should not infer semicolons";
  }
  
  delete builder;
}

TEST(AutomaticSemicolonsTest, AutomaticModeInfersSemicolons) {
  std::string code = "x = 5 y = 10";
  std::string yaml = "automatic-semicolons: true\n";
  
  // In automatic mode, this should parse as two statements
  ParserTester test = ParserTester::CreateWithSettings(code, yaml);
  auto node = test->ParseCode();
  
  EXPECT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  if (block) {
    // In automatic mode, this should parse as two statements
    EXPECT_GE(block->statements.size(), 2) 
        << "Automatic mode should infer semicolons and create two statements";
  }
}

// ============================================================================
// Division Casting Tests
// ============================================================================

TEST(DivisionCastingTest, ScriptModeAddsDoubleCast) {
  std::string code = "a / b";
  
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  
  // Create pretty printer with is_script=true
  // Use a temporary file for output
  char tmpname[] = "/tmp/enigma_test_XXXXXX";
  int fd = mkstemp(tmpname);
  ASSERT_NE(fd, -1) << "Failed to create temporary file";
  close(fd);
  
  std::ofstream tmp_file(tmpname);
  AST::CppPrettyPrinter v(tmp_file, test.context->language_fe, true, false);
  EXPECT_TRUE(v.VisitCode(*block));
  tmp_file.close();
  
  // Read the file
  std::ifstream in(tmpname);
  std::string printed((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  in.close();
  unlink(tmpname);
  
  // Script mode should add (double) cast
  EXPECT_NE(printed.find(" / (double)"), std::string::npos) 
      << "Script mode should add (double) cast, got: " << printed;
}

TEST(DivisionCastingTest, CppModeNoDoubleCast) {
  std::string code = "a / b";
  
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  
  // Create pretty printer with is_script=false
  char tmpname[] = "/tmp/enigma_test_XXXXXX";
  int fd = mkstemp(tmpname);
  ASSERT_NE(fd, -1) << "Failed to create temporary file";
  close(fd);
  
  std::ofstream tmp_file(tmpname);
  AST::CppPrettyPrinter v(tmp_file, test.context->language_fe, false, false);
  EXPECT_TRUE(v.VisitCode(*block));
  tmp_file.close();
  
  std::ifstream in(tmpname);
  std::string printed((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  in.close();
  unlink(tmpname);
  
  // C++ mode should NOT add (double) cast
  EXPECT_EQ(printed.find(" / (double)"), std::string::npos) 
      << "C++ mode should NOT add (double) cast, got: " << printed;
  EXPECT_NE(printed.find(" / "), std::string::npos) 
      << "Division should be present, got: " << printed;
}

TEST(DivisionCastingTest, NestedDivisionCasting) {
  std::string code = "x = a / b / c";
  
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  
  // Create pretty printer with is_script=true
  char tmpname[] = "/tmp/enigma_test_XXXXXX";
  int fd = mkstemp(tmpname);
  ASSERT_NE(fd, -1) << "Failed to create temporary file";
  close(fd);
  
  std::ofstream tmp_file(tmpname);
  AST::CppPrettyPrinter v(tmp_file, test.context->language_fe, true, false);
  EXPECT_TRUE(v.VisitCode(*block));
  tmp_file.close();
  
  std::ifstream in(tmpname);
  std::string printed((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  in.close();
  unlink(tmpname);
  
  // Both divisions should have (double) casts
  size_t count = 0;
  size_t pos = 0;
  while ((pos = printed.find(" / (double)", pos)) != std::string::npos) {
    count++;
    pos += 11; // length of " / (double)"
  }
  EXPECT_GE(count, 2) 
      << "Nested divisions should both have (double) casts, got: " << printed;
}

TEST(DivisionCastingTest, DivisionInExpressionCasting) {
  std::string code = "x = a / b + c";
  
  ParserTester test = ParserTester::CreateWithCpp(code);
  auto node = test->ParseCode();
  ASSERT_EQ(node->type, AST::NodeType::BLOCK);
  auto* block = node->As<AST::CodeBlock>();
  
  // Create pretty printer with is_script=true
  char tmpname[] = "/tmp/enigma_test_XXXXXX";
  int fd = mkstemp(tmpname);
  ASSERT_NE(fd, -1) << "Failed to create temporary file";
  close(fd);
  
  std::ofstream tmp_file(tmpname);
  AST::CppPrettyPrinter v(tmp_file, test.context->language_fe, true, false);
  EXPECT_TRUE(v.VisitCode(*block));
  tmp_file.close();
  
  std::ifstream in(tmpname);
  std::string printed((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  in.close();
  unlink(tmpname);
  
  // Division should have (double) cast even in expression
  EXPECT_NE(printed.find(" / (double)"), std::string::npos) 
      << "Division in expression should have (double) cast, got: " << printed;
}
