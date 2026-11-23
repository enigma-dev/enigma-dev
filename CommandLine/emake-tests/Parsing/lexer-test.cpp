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

// Names inside a macro expansion get the same keyword/definition typing as
// names lexed straight from the code; `f(true)` through a wrapper macro must
// not demote `true` to an identifier.
TEST(LexerTest, MacroExpansionTranslatesNames) {
  LexerTester lex("WRAP(true);", true);
  AddMacro(lex, Macro("WRAP", {"arg"}, false, "(0, arg)", &lex.herr));
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_COMMA);
  EXPECT_EQ(lex->ReadToken().type, TT_BOOLLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

// A macro parameter may be named after an EDL keyword: the definition is
// C++, where `repeat` is an ordinary identifier. GStextures.h's
// `#define texture_set_repeat(repeat) texture_set_repeat_ext(0, repeat)`
// is the live example; the keyword-typed token must still substitute.
TEST(LexerTest, MacroParameterNamedAfterEdlKeyword) {
  LexerTester lex("SETREP(1);", true);
  AddMacro(lex, Macro("SETREP", {"repeat"}, false, "wrap(0, repeat)", &lex.herr));
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_COMMA);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

// Other EDL words (GML's brace alias `begin`, word operators) are names in
// a C++ macro definition too, and substitute like any parameter.
TEST(LexerTest, MacroParametersNamedAfterEdlWords) {
  for (const char *param : {"begin", "end", "div", "mod", "not", "then"}) {
    SCOPED_TRACE(param);
    LexerTester lex("PICK(7);", true);
    AddMacro(lex, Macro("PICK", {param}, false,
                        std::string("pick(") + param + ")", &lex.herr));
    EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
    EXPECT_EQ(lex->ReadToken().type, TT_BEGINPARENTH);
    EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
    EXPECT_EQ(lex->ReadToken().type, TT_ENDPARENTH);
    EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
    EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
  }
}

// A parameter named after another macro shadows it inside the body.
// OUTER is defined first: AddMacro writes into the shared C++ context that
// Macro's string constructor lexes with, so INNER would expand at definition.
TEST(LexerTest, MacroParameterShadowsMacroName) {
  LexerTester lex("OUTER(5);", true);
  AddMacro(lex, Macro("OUTER", {"INNER"}, false, "(INNER)", &lex.herr));
  AddMacro(lex, Macro("INNER", "999", &lex.herr));
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINPARENTH);
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_DECLITERAL);
  EXPECT_EQ(t.content, "5");
  EXPECT_EQ(lex->ReadToken().type, TT_ENDPARENTH);
  EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

// Stringizing a keyword-named parameter yields the argument's spelling.
TEST(LexerTest, StringizeKeywordNamedParameter) {
  LexerTester lex("STR(abc);", true);
  AddMacro(lex, Macro("STR", {"repeat"}, false, "#repeat", &lex.herr));
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_STRINGLIT);
  EXPECT_EQ(t.content, "\"abc\"");
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

TEST(LexerTest, LambdaExpressions) {
  LexerTester lex("y = x => x+10;", true);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_EQUALS);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_JS_ARROW);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_PLUS);
  EXPECT_EQ(lex->ReadToken().type, TT_DECLITERAL);
  EXPECT_EQ(lex->ReadToken().type, TT_SEMICOLON);
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}

TEST(LexerTest, Literals_1) {
  LexerTester lex("y = \" \\n \" ", true);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_EQUALS);
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_STRINGLIT);
  EXPECT_EQ(t.content, " \n ");
}

TEST(LexerTest, Literals_2) {
  LexerTester lex("y = \"#\" ", false);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_EQUALS);
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_STRINGLIT);
  EXPECT_EQ(t.content, "\n");
}

TEST(LexerTest, SingleQuoteIsStringInGml) {
  LexerTester lex("y = ' ' ", false);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_EQUALS);
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_STRINGLIT);
  EXPECT_EQ(t.content, " ");
}

TEST(LexerTest, SingleQuoteIsCharInCpp) {
  LexerTester lex("y = 'a' ", true);
  EXPECT_EQ(lex->ReadToken().type, TT_IDENTIFIER);
  EXPECT_EQ(lex->ReadToken().type, TT_EQUALS);
  Token t = lex->ReadToken();
  EXPECT_EQ(t.type, TT_CHARLIT);
  EXPECT_EQ(t.content, "a");
}
