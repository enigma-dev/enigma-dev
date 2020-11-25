#include <parsing/lexer.h>

#include <gtest/gtest.h>

using namespace ::enigma::parsing;

namespace testing {
std::string PrintToString(TokenType tt) {
  return ToString(tt);
}
}

class TestFailureErrorHandler : public ErrorHandler {
 public:
  void ReportError(CodeSnippet snippet, std::string_view error) final {
    ADD_FAILURE() << "Test reported an error at line " << snippet.line
                  << ", position " << snippet.position << ": " << error;
  }
  void ReportWarning(CodeSnippet snippet, std::string_view warning) final {
    ADD_FAILURE() << "Test reported a warning at line " << snippet.line
                  << ", position " << snippet.position << ": " << warning;
  }
};

struct LexerTester {
  TestFailureErrorHandler herr;
  ParseContext context;
  Lexer lexer;

  Lexer *operator->() { return &lexer; }

  LexerTester(std::string code):
      context(nullptr),
      lexer(std::move(code), &context, &herr) {}
};

TEST(LexerTest, NumericLiterals) {
  LexerTester lex("cool+++beans");
  EXPECT_EQ(lex->ReadToken().type, TT_VARNAME);
  EXPECT_EQ(lex->ReadToken().type, TT_INCREMENT);
  EXPECT_EQ(lex->ReadToken().type, TT_PLUS);
  EXPECT_EQ(lex->ReadToken().type, TT_VARNAME);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}
