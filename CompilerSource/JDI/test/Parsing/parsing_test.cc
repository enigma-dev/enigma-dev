#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <System/lex_cpp.h>
#include <System/builtins.h>
#include <Testing/error_handler.h>
#include <Testing/matchers.h>

using ::testing::Eq;
using ::testing::AllOf;

namespace jdi {
namespace {

Context Parse(const char *test_case) {
  Context ctex(error_constitutes_failure);
  macro_map no_macros;
  llreader read("test_input", test_case, false);
  ctex.parse_stream(read);
  return ctex;
}

TEST(ParsingTest, VariousDeclarations) {
  auto ctex = Parse(R"cpp(
    int integer;
    char *cstring;
    int intfn(int x);
    int (*intfn_ptr)(int);
  )cpp");
  EXPECT_EQ(ctex.get_global()->look_up("integer")->toString(), "int integer;");
}

TEST(ParsingTest, HighlyDecoratedIntegers) {
  Parse("long long ago;                     ");
  Parse("const long unsigned long int etc;  ");
}

TEST(ParsingTest, HighlyDecoratedIntegerTypedefs) {
  // FIXME: brittle-ass test; add a better way of matching these attributes.
  EXPECT_EQ(Parse("typedef long long ago;").get_global()
                ->look_up("ago")->toString(),
            "typedef long long int ago;");
  Parse("typedef const long unsigned long int etc;  ");
}

TEST(ParsingTest, SimpleClassDefinition) {
  // FIXME: brittle-ass test; add a better way of matching these attributes.
  auto ctex = Parse("class my_class { public: void do_something(); };");
  EXPECT_THAT(
      ctex.get_global(),
      HasMembers(ClassDefinition(
          "my_class", HasMembers(FunctionDefinition("do_something")))));
}

}  // namespace
}  // namespace jdi
