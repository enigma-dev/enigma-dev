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
  const ParseContext *context;
  Lexer lexer;

  Lexer *operator->() { return &lexer; }

  LexerTester(std::string code, bool use_cpp = false):
      context(&ParseContext::ForTesting(use_cpp)),
      lexer(std::move(code), context, &herr) {}
};

TEST(LexerTest, NumericLiterals) {
  LexerTester lex("cool+++beans");
  EXPECT_EQ(lex->ReadToken().type, TT_VARNAME);
  EXPECT_EQ(lex->ReadToken().type, TT_INCREMENT);
  EXPECT_EQ(lex->ReadToken().type, TT_PLUS);
  EXPECT_EQ(lex->ReadToken().type, TT_VARNAME);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, Comments) {
  LexerTester lex("// This is a \"comment' /*\n{/* more // */}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, AnnoyingCommentsA) {
  LexerTester lex("test/*'*/endtest");
  EXPECT_EQ(lex->ReadToken().type, TT_VARNAME);
  EXPECT_EQ(lex->ReadToken().type, TT_VARNAME);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, AnnoyingCommentsB) {
  LexerTester lex("{}/* comment */{}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, EmptyComment) {
  LexerTester lex("{/**/}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, EmptyCppComment) {
  LexerTester lex("{//\n}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, StringThenComment) {
  LexerTester lex("{\"string\"/**/}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_STRING);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

// Zero is special, because we look for 0x, 0b, etc.
TEST(LexerTest, ZeroThenComment) {
  LexerTester lex("{0/**/}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

// Zero is special, because we look for 0x, 0b, etc.
TEST(LexerTest, CppZeroThenComment) {
  LexerTester lex("{0/**/}", true);
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}
// Catch OOB reads with zero
TEST(LexerTest, ZeroAtEndOfString) {
  LexerTester lex("0", false);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
  LexerTester lex2("0", true);
  EXPECT_EQ(lex2->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex2->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, IntegerThenComment) {
  LexerTester lex("{12345/**/}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, DoubleThenComment) {
  LexerTester lex("{12.345/**/}");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, HexThenComment) {
  LexerTester lex("{0x1234ABC/**/}", true);
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_HEXLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}
