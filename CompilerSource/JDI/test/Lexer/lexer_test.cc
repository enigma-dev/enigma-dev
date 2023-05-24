#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <System/lex_cpp.h>
#include <System/builtins.h>
#include <Testing/error_handler.h>
#include <Testing/matchers.h>

#define TOKEN(type) token_t(type, __FILE__, __LINE__, 0)

using ::testing::Eq;
using ::testing::AllOf;

namespace jdi {

TEST(LexerTest, BasicTokenization) {
  macro_map no_macros;
  llreader read("test_input", R"cpp(const char *my_identifier = "hello, world!";
      )cpp", false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECFLAG));       // const
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // char
  EXPECT_THAT(lex.get_token(), HasType(TT_STAR));          // *
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // my_identifier
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL)); // "hello, world!"
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, FlagTokenization) {
  macro_map no_macros;
  llreader read("test_input", R"cpp(
      const unsigned long long int a = 10;
      )cpp", false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECFLAG));     // const
  EXPECT_THAT(lex.get_token(), HasType(TT_DECFLAG));     // unsigned
  EXPECT_THAT(lex.get_token(), HasType(TT_DECFLAG));     // long
  EXPECT_THAT(lex.get_token(), HasType(TT_DECFLAG));     // long
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));  // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));  // a
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));       // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));  // 10
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));   // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, StringLiteralBehavior) {
  macro_map no_macros;
  constexpr char kTestCase[] = R"cpp("hello,"    ""    " world!")cpp";
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

// Ridiculously directive-heavy code that says `int identifier = 4;`
TEST(LexerTest, BasicPreprocessors) {
  constexpr char kCppWithIfDirectives[] = R"cpp(
#if 1 + 1 == 2
  int
#else
  wrong
#endif

#if 2 + 2 == 5
  decltype
#elif 2 + 2 == 4
  identifier =
#else
  sizeof
#endif

4;
)cpp";
  macro_map no_macros;
  llreader read("test_input", kCppWithIfDirectives, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // identifier
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));    // 4
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

// Macro-heavy code that says `int identifier = 100;`
TEST(LexerTest, BasicMacroFunction) {
  constexpr char kTestCase[] = R"cpp(
#define MACRO_FUNC(x, y, type) type x = y
MACRO_FUNC(identifier, 100, int);
)cpp";
  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // identifier
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));    // 100
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

// Macro-heavy code that says `int x;`
TEST(LexerTest, ConditionalWithMacroExpansion) {
  constexpr char kTestCase[] = R"cpp(
#define butts 12345
#if 2 > 1 && butts == 12345
  int x;
#endif
)cpp";
  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // x
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, ConditionalWithMacroFunction) {
  constexpr char kTestCase[] = R"cpp(
# define TEST_LT(left, right) \
	left > right
#if TEST_LT (4,3)
  "success"
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

// Tests a simple not expression with a macro.
TEST(LexerTest, ConditionalWithNot) {
  constexpr char kCppWithNot[] = R"cpp(
#define butts 1
#if ! butts
uh_oh
#endif
)cpp";
  macro_map no_macros;
  llreader read("test_input", kCppWithNot, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, ConcatenationInObjectLikeMacros) {
  constexpr char kTestCase[] = R"(
#define type in ## t
type name;
  )";
  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // name
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, UncalledMacroFuncLeftAlone) {
  constexpr char kTestCase[] = R"(
    #define macro_func(x)
    int macro_func = 10;
  )";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // macro_func
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));    // 10
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, ISO_n4800_14_3_3) {
  constexpr char kTestCase[] = R"(
    #define hash_hash # ## #
    #define mkstr(a) # a
    #define in_between(a) mkstr(a)
    #define join(c, d) in_between(c hash_hash d)
    join(x, y)
  )";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(),
              AllOf(HasType(TT_STRINGLITERAL), HasContent("\"x ## y\"")));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, AnnoyingSubstitutionOrder) {
  constexpr char kTestCase[] = R"(
    #define cat1(x, y) x ## y
    #define cat2(x, y) cat1(x, y)
    #define identifier cat1(id, __LINE__)
    cat1(id, __LINE__)
    identifier
    cat2(id, __LINE__)
  )";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  std::string line_6 = std::to_string(6 + llreader::kFirstLine);
  EXPECT_THAT(lex.get_token(),
              AllOf(HasType(TT_IDENTIFIER), HasContent("id__LINE__")));
  EXPECT_THAT(lex.get_token(),
              AllOf(HasType(TT_IDENTIFIER), HasContent("id__LINE__")));
  EXPECT_THAT(lex.get_token(),
              AllOf(HasType(TT_IDENTIFIER), HasContent("id" + line_6)));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, InducesTears) {
  constexpr char kTestCase[] = R"(
    #define a(x) # x
    #define b(x) a ( # x )
    #define c(x) b ( # x )
    #define d(x) c ( x )
    #define e() d ( __FILE__ )
    e()
  )";
  constexpr char kTragedy[] = R"("\"\\\"\\\\\\\"test_input\\\\\\\"\\\"\"")";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  ASSERT_THAT(lex.get_token(),
              AllOf(HasType(TT_STRINGLITERAL), HasContent(kTragedy)));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, LineNumbering) {
  constexpr char kTestCase[] = R"(// Comment
1/*

multiline comment

*/2/*/

antagonistic multiline comment

/*/3/**

multiline doc comment

**/4/**/5)";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("1")));
  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("2")));
  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("3")));
  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("4")));
  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("5")));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, StringLiterals) {
  constexpr char kTestCase[] = R"cpp("string\
'\"literal\"'"1'c'R"raaw(
raw "string literal")"
gotcha, bitch)raaw";R"(normal raw string") with a twist)";"":R"()";
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_CHARLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_COLON));
  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, DefinedExpressions) {
  constexpr char kTestCase[] = R"cpp(#define foo
#if defined foo && ! defined bar
#if defined bar
fail
#else
100
#endif
#else
"fail"
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("100")));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, DefinedExpressionsWithParenths) {
  constexpr char kTestCase[] = R"cpp(#define foo
#if defined ( foo ) && ! defined ( bar )
#if defined ( bar )
fail
#else
100
#endif
#else
"fail"
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_DECLITERAL), HasContent("100")));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, DefinedExpressionsChain) {
  constexpr char kTestCase[] = R"cpp(#define foo
#if defined ( one ) || defined ( two ) || defined ( red ) || defined ( foo )
pass
#endif
;
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), AllOf(HasType(TT_IDENTIFIER), HasContent("pass")));
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

// FIXME: this isn't an odd place. A lot of bits of JDI just assumed that
// skipping past string literals would be easy peasy lemon squeezy, but then
// C++11 made it difficult difficult lemon difficult
TEST(LexerTest, StringsInOddPlaces) {
  constexpr char kTestCase[] = R"cpp(
#define foo ""
foo
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_STRINGLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, BoundariesInComparisonTests) {
  constexpr char kTestCase[] = R"cpp(
#if 0x7fffffffffffffffL < 0xffffffffffffffffUL
	hooray
#else
	"fail"
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, HasIncludeBasics) {
  constexpr char kTestCase[] = R"cpp(
#if __has_include(<success.h>)
	hooray
#else
	"fail"
#endif

#if __has_include(<made_up_header.h>)
  "double fail"
#else
  1337
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  builtin_context().add_search_directory("test/test_data");

  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, HasIncludeBasicsWithQuotes) {
  constexpr char kTestCase[] = R"cpp(
#if __has_include("success.h")
	hooray
#else
	"fail"
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  builtin_context().add_search_directory("test/test_data");

  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, HasIncludeInsideMacro) {
  constexpr char kTestCase[] = R"cpp(
#define success failure  // Try to trip up the tokenizer.
#define expr1 __has_include(<success.h>)
#define expr2 __has_include(<made_up_header.h>)

#if expr1
	hooray
#else
	"fail"
#endif

#if expr2
  "double fail"
#else
  1337
#endif
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  builtin_context().add_search_directory("test/test_data");

  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, DeferredInsanity) {
  constexpr char kTestCase[] = R"cpp(
#define foo defined bar

#if foo
  "fail"
#else
	hooray
#endif

#define bar

#if foo
	1337
#else
  "double fail"
#endif

)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, ExpressInsanity) {
  // This test verifies that the lexer isn't handling Cpp.Cond expressions
  // before preprocessing #if expressions or after, but instead *during*.
  // Handling them before will cause expr1 to fail, either because `defined foo`
  // was false when expr1 was declared. Handling them after will fail because
  // `foo` will be preprocessed to nothing.
  constexpr char kTestCase[] = R"cpp(
#define expr1 defined foo
#define expr2 __has_include(<success.h>)
#define foo

#if expr1
	hooray
#else
  "fail"
#endif

#if expr2
	1337
#else
  "double fail"
#endif

)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);
  builtin_context().add_search_directory("test/test_data");

  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, VariadicMacros) {
  constexpr char kTestCase[] = R"cpp(
#define my_macro(x, y, ...) \
   y x __VA_ARGS__
my_macro(id, int, = 10);
my_macro(id2, float);
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // id
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));    // 10
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // float
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // id2
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, VariadicMacrosGNU) {
  constexpr char kTestCase[] = R"cpp(
#define my_macro(x, y, z...) \
   y x z
my_macro(id, int, = 10);
my_macro(id2, float);
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // id
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));    // 10
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // float
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // id2
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

TEST(LexerTest, NoArgsVariadicMacros) {
  constexpr char kTestCase[] = R"cpp(
#define my_macro1(...) __VA_ARGS__
#define my_macro2(x...) __VA_ARGS__
my_macro1( int x )
my_macro2( = 10  );
)cpp";

  macro_map no_macros;
  llreader read("test_input", kTestCase, false);
  lexer lex(read, no_macros, error_constitutes_failure);

  EXPECT_THAT(lex.get_token(), HasType(TT_DECLARATOR));    // int
  EXPECT_THAT(lex.get_token(), HasType(TT_IDENTIFIER));    // x
  EXPECT_THAT(lex.get_token(), HasType(TT_EQUAL));         // =
  EXPECT_THAT(lex.get_token(), HasType(TT_DECLITERAL));    // 10
  EXPECT_THAT(lex.get_token(), HasType(TT_SEMICOLON));     // ;
  EXPECT_THAT(lex.get_token(), HasType(TT_ENDOFCODE));
}

}  // namespace jdi
