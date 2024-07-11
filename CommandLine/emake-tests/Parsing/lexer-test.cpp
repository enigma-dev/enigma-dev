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

TEST(LexerTest, GreedyTokenization) {
  LexerTester lex("cool+++beans");
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_INCREMENT);
  EXPECT_EQ(lex->ReadToken().type, TT_PLUS);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, OneCharIdentifiers) {
  LexerTester lex("a b");
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_IDENTIFIER);
  EXPECT_EQ(t.content, "a");

  t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_IDENTIFIER);
  EXPECT_EQ(t.content, "b");

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
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
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
  EXPECT_EQ(lex->ReadToken().type, TT_STRINGLIT);
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
  lex->UseCppOptions();
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_HEXLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDBRACE);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

void AddMacro(LexerTester &lex, Macro macro) {
  // FIXME: this is a horrible hack
  const_cast<MacroMap&>(lex.context->macro_map)
      .insert({macro.name, macro});
}

TEST(LexerTest, MacroFunctions) {
  LexerTester lex("MACRO_FUNC(ident);", true);
  AddMacro(lex, Macro("MACRO_FUNC", {"arg"}, false, "(arg)", &lex.herr));
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, VariadicMacroFunctions) {
  LexerTester lex("VAR_FUNC(ident, 123);", true);
  AddMacro(lex, Macro("VAR_FUNC", {"arg"}, true, "(arg)", &lex.herr));
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_COMMA);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}
