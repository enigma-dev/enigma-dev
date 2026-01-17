#include <parsing/lexer.h>
#include "parser-test-classes.h"

#include <gtest/gtest.h>

using namespace ::enigma::parsing;

namespace testing {

std::string PrintToString(TokenType tt) {
  return ToString(tt);
}

}  // namespace testing

struct LexerTester {
  TestFailureErrorHandler herr;
  const ParseContext *context;
  Lexer lexer;

  Lexer *operator->() { return &lexer; }

  LexerTester(std::string code, bool use_cpp = false):
      context(&ParseContext::ForTesting(use_cpp)),
      lexer(std::move(code), context, &herr) {}
};

#define first_vararg(x, ...) x
#define second_vararg(x, y, ...) y
#define ExpectToken(tok_exp, tt, lnum, ...) {                     \
    const Token tok = (tok_exp);                                  \
    const int pos = first_vararg(__VA_ARGS__, {});                \
    string_view content = second_vararg(__VA_ARGS__, {}, {});     \
    EXPECT_EQ(tok.type, (tt));                                    \
    EXPECT_EQ(tok.line, (lnum));                                  \
    EXPECT_EQ(tok.position, (pos));                               \
    if (!content.empty()) { EXPECT_EQ(tok.content, content); }    \
  }

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

TEST(LexerTest, EmptyCppCommentEndOfCode) {
  LexerTester lex("{//");
  EXPECT_EQ(lex->ReadToken().type, TT_BEGINBRACE);
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
// Make sure .1 is a single literal
TEST(LexerTest, DecimalOnlyNumbers) {
  LexerTester lex(".1", false);
  ExpectToken(lex->ReadToken(), TT_DECLITERAL, 1, 1, ".1");
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
}
// Make sure a.b is an expression
TEST(LexerTest, DotOperatorLexing) {
  LexerTester lex("a.b = c . d\nd .* e = e.*f", false);
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 1, 1, "a");
  ExpectToken(lex->ReadToken(), TT_DOT,        1, 2, ".");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 1, 3, "b");
  ExpectToken(lex->ReadToken(), TT_EQUALS,     1, 5, "=");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 1, 7, "c");
  ExpectToken(lex->ReadToken(), TT_DOT,        1, 9, ".");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 1, 11, "d");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 2, 1, "d");
  ExpectToken(lex->ReadToken(), TT_DOT_STAR,   2, 3, ".*");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 2, 6, "e");
  ExpectToken(lex->ReadToken(), TT_EQUALS,     2, 8, "=");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 2, 10, "e");
  ExpectToken(lex->ReadToken(), TT_DOT_STAR,   2, 11, ".*");
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 2, 13, "f");
  EXPECT_EQ(lex->ReadToken().type, TT_ENDOFCODE);
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

// Test that verifies token type correction for tokens from macro expansion
// This tests the CorrectTokenTypeFromContent function directly
TEST(LexerTest, TokenTypeCorrection_FromMacroExpansion) {
  // Create a macro that expands to operators and punctuation
  LexerTester lex("MACRO_TEST(5);", true);
  
  // Create a macro that expands to: for (int x = (5); x > 0; x--)
  // This contains operators that were being mis-categorized: =, ;, >, --
  std::string macro_body = "for (int x = (5); x > 0; x--)";
  AddMacro(lex, Macro("MACRO_TEST", {"arg"}, false, macro_body, &lex.herr));
  
  // Read tokens and verify they have correct types
  // The macro should expand and tokens should have proper types, not TT_IDENTIFIER
  Token t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_S_FOR) << "Expected 'for' keyword, got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_BEGINPARENTH) << "Expected '(', got type " << (int)t.type << " content '" << t.content << "'";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_TYPE_NAME) << "Expected 'int' type, got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_IDENTIFIER) << "Expected identifier 'x', got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_EQUALS) << "Expected '=' operator (TT_EQUALS), got type " << (int)t.type << " content '" << t.content << "'";
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token '=' should not be TT_IDENTIFIER";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_BEGINPARENTH) << "Expected '(', got type " << (int)t.type;
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token '(' should not be TT_IDENTIFIER";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_DECLITERAL) << "Expected literal '5', got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_ENDPARENTH) << "Expected ')', got type " << (int)t.type;
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token ')' should not be TT_IDENTIFIER";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_SEMICOLON) << "Expected ';' (TT_SEMICOLON), got type " << (int)t.type << " content '" << t.content << "'";
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token ';' should not be TT_IDENTIFIER";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_IDENTIFIER) << "Expected identifier 'x', got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_GREATER) << "Expected '>' operator (TT_GREATER), got type " << (int)t.type << " content '" << t.content << "'";
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token '>' should not be TT_IDENTIFIER";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_DECLITERAL) << "Expected literal '0', got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_SEMICOLON) << "Expected ';', got type " << (int)t.type;
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token ';' should not be TT_IDENTIFIER";
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_IDENTIFIER) << "Expected identifier 'x', got type " << (int)t.type;
  
  t = lex->ReadToken();
  ASSERT_EQ(t.type, TT_DECREMENT) << "Expected '--' operator (TT_DECREMENT), got type " << (int)t.type << " content '" << t.content << "'";
  ASSERT_NE(t.type, TT_IDENTIFIER) << "Token '--' should not be TT_IDENTIFIER";
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

TEST(LexerTest, TokenPositions) {
  LexerTester lex(R"edl({
// start the game

if (keyboard_check_pressed(vk_space) && view_xview[0]+view_wview[0]/2 == x) { // If you press space an the player is in the center of the view ...

    gravity = .25;              // ... set the gravity, ...

    gravity_direction = 270;    // ... its direction, ...

    hspeed = 7.5;               // ... and let the player go forward in hor-direction.

}



// fly upwards

if (keyboard_check(vk_space)) { // If you press space ...

    vspeed -= .75;              // ... decrease the vspeed by .75.

}



// limits

if (vspeed > 12) {              // If vpeed is larger than 12 ...

    vspeed = 12;                // ... set it back to 12.

} else if (vspeed < -12) {      // And if it's smaller than -12 ...

    vspeed = -12;               // ... set it back to -12.

}



// rotate sprite

image_angle = 0-vspeed*3;       // 0-vspeed, because vspeed have to be under 0. Otherwise the rotation will act in the wrong direction.

                                // *3, because I wanted to increase the rotation-effect.

// create line instances

if (hspeed > 0) {                   // If the player is moving ...

    instance_create(x,y,obj_line);  // ... create a way-marker.

} else {

    // if the player don\'t move forward he can\'t move up or down!

    vspeed = 0;                     // Stop the up-down-movement.

}



// update score

if (x-320 > global.max_x) {     // If the score is larger than the old one, update it. -320, because 320 is the start position!

    global.max_x = floor(x)-320; // 320 = view_wview[0]/2

}


/**/
}
)edl", false);
  ExpectToken(lex->ReadToken(), TT_BEGINBRACE, 1, 1);
  ExpectToken(lex->ReadToken(), TT_S_IF, 4, 1);
  ExpectToken(lex->ReadToken(), TT_BEGINPARENTH, 4, 4);
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 4, 5, "keyboard_check_pressed");
  ExpectToken(lex->ReadToken(), TT_BEGINPARENTH, 4, 27);
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 4, 28, "vk_space");
  ExpectToken(lex->ReadToken(), TT_ENDPARENTH, 4, 36);
  ExpectToken(lex->ReadToken(), TT_AND, 4, 38);
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 4, 41, "view_xview");
  ExpectToken(lex->ReadToken(), TT_BEGINBRACKET, 4, 51);
  ExpectToken(lex->ReadToken(), TT_DECLITERAL, 4, 52, "0");
  ExpectToken(lex->ReadToken(), TT_ENDBRACKET, 4, 53);
  ExpectToken(lex->ReadToken(), TT_PLUS, 4, 54);
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 4, 55, "view_wview");
  ExpectToken(lex->ReadToken(), TT_BEGINBRACKET, 4, 65);
  ExpectToken(lex->ReadToken(), TT_DECLITERAL, 4, 66, "0");
  ExpectToken(lex->ReadToken(), TT_ENDBRACKET, 4, 67);
  ExpectToken(lex->ReadToken(), TT_SLASH, 4, 68);
  ExpectToken(lex->ReadToken(), TT_DECLITERAL, 4, 69, "2");
  ExpectToken(lex->ReadToken(), TT_EQUALTO, 4, 71);
  ExpectToken(lex->ReadToken(), TT_IDENTIFIER, 4, 74, "x");
  ExpectToken(lex->ReadToken(), TT_ENDPARENTH, 4, 75);
  ExpectToken(lex->ReadToken(), TT_BEGINBRACE, 4, 77);

  for (const auto &[lnum, pos1, pos2, pos3, pos4, name, value] : std::initializer_list<std::tuple<int,int,int,int,int,const char*,const char*>>{
      {6, 5, 13, 15, 18, "gravity", ".25"},
      {8, 5, 23, 25, 28, "gravity_direction", "270"},
      }) {
    ExpectToken(lex->ReadToken(), TT_IDENTIFIER, lnum, pos1, name);
    ExpectToken(lex->ReadToken(), TT_EQUALS,     lnum, pos2);
    ExpectToken(lex->ReadToken(), TT_DECLITERAL, lnum, pos3, value);
    ExpectToken(lex->ReadToken(), TT_SEMICOLON,  lnum, pos4);
  }
}
