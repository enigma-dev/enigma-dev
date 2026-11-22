// JDI2 T1: exercises clang_adapter::ClangContext directly, asserting that it
// populates a real jdi::Storage tree (definition/definition_scope/
// definition_class/definition_typed/definition_function) rather than the
// retired ClangDefinition* mirror. See CompilerSource/JDI/JDI2-DESIGN.md.
//
// The adapter itself is wired to nothing in the compile pipeline (lang_CPP
// is untouched); this suite is the only thing that drives it.

#include <languages/clang_adapter.h>
#include <parsing/macros.h>
#include <Storage/definition.h>
#include <System/builtins.h>
#include <clang-c/Index.h>

#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

// Small fixture covering the T1 population surface: a namespace with a
// nested class (two members + a method), a typedef, a two-value enum, a
// global variable, an overloaded free function (one overload with a
// defaulted parameter), a class template, and two macros.
constexpr char kFixtureSource[] = R"cpp(
namespace demo_ns {
  class Widget {
   public:
    int x;
    int y;
    int sum() const;
  };
}

typedef int demo_int_t;

enum Color { RED, GREEN };

int global_counter;

void greet(int x);
void greet(int x, int y = 5);

int printf_like(const char* fmt, ...);


template <typename T> T pick(T a, int n = 2);


#define DEMO_PAREN_VALUE (demo_ns::Widget())
#define DEMO_FUNC_MACRO(a, b) ((a) + (b))

template <typename T>
class Box {
 public:
  T value;
  int count;
  int size() { return count; }
  T& at(int i);
};
template <typename K, typename V = Box<K>, int N = 3, long M = 0>
class Holder {};
)cpp";

std::string WriteFixture() {
  std::filesystem::path path = std::filesystem::path("/tmp") / "clang_populator_test_fixture.hpp";
  std::ofstream out(path, std::ios::trunc);
  out << kFixtureSource;
  out.close();
  return path.string();
}

// This file lives at <repo-root>/CommandLine/emake-tests/Parsing/
// clang-populator-test.cpp; used only by the disabled full-engine probe.
std::filesystem::path EnigmaRoot() {
  return std::filesystem::path(__FILE__).parent_path().parent_path().parent_path().parent_path();
}

}  // namespace

class ClangPopulatorTest : public ::testing::Test {
 protected:
  clang_adapter::ClangContext ctx;
  static std::string fixture_path;

  static void SetUpTestSuite() { fixture_path = WriteFixture(); }

  void SetUp() override {
    ASSERT_FALSE(fixture_path.empty());
    ctx.parse_file(fixture_path);
  }
};
std::string ClangPopulatorTest::fixture_path;

TEST_F(ClangPopulatorTest, NamespaceAndNestedClass) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);

  // The class must not be directly visible at global scope...
  EXPECT_EQ(global->find_local("Widget"), nullptr);

  // ...it belongs to the namespace.
  jdi::definition* ns_def = global->find_local("demo_ns");
  ASSERT_NE(ns_def, nullptr);
  EXPECT_TRUE(ns_def->flags & jdi::DEF_NAMESPACE);
  auto* ns_scope = dynamic_cast<jdi::definition_scope*>(ns_def);
  ASSERT_NE(ns_scope, nullptr);

  jdi::definition* widget_def = ns_scope->find_local("Widget");
  ASSERT_NE(widget_def, nullptr);
  EXPECT_EQ(widget_def->flags & (jdi::DEF_CLASS | jdi::DEF_TYPENAME),
            (unsigned)(jdi::DEF_CLASS | jdi::DEF_TYPENAME));

  auto* widget_scope = dynamic_cast<jdi::definition_scope*>(widget_def);
  ASSERT_NE(widget_scope, nullptr);

  jdi::definition* x_def = widget_scope->find_local("x");
  ASSERT_NE(x_def, nullptr);
  EXPECT_TRUE(x_def->flags & jdi::DEF_TYPED);

  jdi::definition* y_def = widget_scope->find_local("y");
  EXPECT_NE(y_def, nullptr);

  jdi::definition* sum_def = widget_scope->find_local("sum");
  ASSERT_NE(sum_def, nullptr);
  EXPECT_TRUE(sum_def->flags & jdi::DEF_FUNCTION);
}

TEST_F(ClangPopulatorTest, CursorPopulation) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);
  jdi::definition* ns_def = global->find_local("demo_ns");
  ASSERT_NE(ns_def, nullptr);

  // A definition populated from a real cursor carries a non-null one...
  EXPECT_EQ(clang_Cursor_isNull(ns_def->cursor), 0);

  // ...whereas a default-constructed jdi::definition (JDI1's own parser
  // never touches `cursor`) stays null.
  jdi::definition default_def;
  EXPECT_NE(clang_Cursor_isNull(default_def.cursor), 0);
}

TEST_F(ClangPopulatorTest, TypedefAndEnum) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);

  jdi::definition* typedef_def = global->find_local("demo_int_t");
  ASSERT_NE(typedef_def, nullptr);
  EXPECT_EQ(typedef_def->flags & (jdi::DEF_TYPENAME | jdi::DEF_TYPED),
            (unsigned)(jdi::DEF_TYPENAME | jdi::DEF_TYPED));

  jdi::definition* enum_def = global->find_local("Color");
  ASSERT_NE(enum_def, nullptr);
  EXPECT_EQ(enum_def->flags & (jdi::DEF_ENUM | jdi::DEF_TYPENAME),
            (unsigned)(jdi::DEF_ENUM | jdi::DEF_TYPENAME));

  // Enum constants of an unscoped enum land as siblings in the enclosing
  // scope (the enum cursor itself isn't a DEF_SCOPE -- see cursor_kind_to_flags).
  jdi::definition* red_def = global->find_local("RED");
  ASSERT_NE(red_def, nullptr);
  EXPECT_TRUE(red_def->flags & jdi::DEF_TYPED);
  EXPECT_NE(global->find_local("GREEN"), nullptr);
}

TEST_F(ClangPopulatorTest, GlobalVariable) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);

  jdi::definition* counter_def = global->find_local("global_counter");
  ASSERT_NE(counter_def, nullptr);
  EXPECT_TRUE(counter_def->flags & jdi::DEF_TYPED);
}

TEST_F(ClangPopulatorTest, FunctionOverloads) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);

  jdi::definition* greet_def = global->find_local("greet");
  ASSERT_NE(greet_def, nullptr);
  EXPECT_TRUE(greet_def->flags & jdi::DEF_FUNCTION);

  auto* greet_func = dynamic_cast<jdi::definition_function*>(greet_def);
  ASSERT_NE(greet_func, nullptr);
  ASSERT_EQ(greet_func->overloads.size(), 2u);

  // Each overload's ref_stack holds exactly one RT_FUNCTION node; its
  // paramcount() is the parameter count for that overload (see
  // walk_declarator_expr in parsing/ast.cpp for the same construction idiom).
  std::vector<size_t> param_counts;
  for (auto& entry : greet_func->overloads) {
    ASSERT_FALSE(entry.second->referencers.empty());
    param_counts.push_back(entry.second->referencers.top().paramcount());
  }
  std::sort(param_counts.begin(), param_counts.end());
  ASSERT_EQ(param_counts.size(), 2u);
  EXPECT_EQ(param_counts[0], 1u);
  EXPECT_EQ(param_counts[1], 2u);
}

// Defaulted parameters carry JDI's presence sentinel so bounds minima count
// them optional: greet(int, int = 5) must yield min 1, max 2 through the
// same params[i].default_value read iterate_overloads performs.
TEST_F(ClangPopulatorTest, DefaultedParameterSentinel) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);
  auto* greet_func =
      dynamic_cast<jdi::definition_function*>(global->find_local("greet"));
  ASSERT_NE(greet_func, nullptr);

  bool saw_two_param_overload = false;
  for (auto& entry : greet_func->overloads) {
    ASSERT_FALSE(entry.second->referencers.empty());
    auto& node = entry.second->referencers.top();
    if (node.paramcount() != 2) continue;
    saw_two_param_overload = true;
    const jdi::ref_stack::parameter_ct& params =
        ((jdi::ref_stack::node_func*)&node)->params;
    EXPECT_EQ(params[0].default_value, nullptr);
    EXPECT_NE(params[1].default_value, nullptr);
  }
  EXPECT_TRUE(saw_two_param_overload);
}

// C-style varargs mark the overload DEF_VARIADIC; non-variadic overloads
// (greet's) never carry it. Parameter packs are deliberately NOT this flag.
TEST_F(ClangPopulatorTest, CStyleVariadicFlag) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);

  auto* variadic_func =
      dynamic_cast<jdi::definition_function*>(global->find_local("printf_like"));
  ASSERT_NE(variadic_func, nullptr);
  ASSERT_EQ(variadic_func->overloads.size(), 1u);
  EXPECT_TRUE(variadic_func->overloads.begin()->second->flags & jdi::DEF_VARIADIC);

  auto* plain_func =
      dynamic_cast<jdi::definition_function*>(global->find_local("greet"));
  ASSERT_NE(plain_func, nullptr);
  for (auto& entry : plain_func->overloads) {
    EXPECT_FALSE(entry.second->flags & jdi::DEF_VARIADIC);
  }
}

TEST_F(ClangPopulatorTest, ClassTemplate) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);

  jdi::definition* box_def = global->find_local("Box");
  ASSERT_NE(box_def, nullptr);
  EXPECT_TRUE(box_def->flags & jdi::DEF_TEMPLATE);
  EXPECT_EQ(box_def->flags & (jdi::DEF_CLASS | jdi::DEF_SCOPE),
            (unsigned)(jdi::DEF_CLASS | jdi::DEF_SCOPE));
  // No DEF_TYPENAME on the wrapper: the lexer retypes DEF_TYPENAME names to
  // TT_TYPE_NAME, which skips the identifier path that parses `<args>`.
  EXPECT_FALSE(box_def->flags & jdi::DEF_TYPENAME);

  // T4: a real definition_template -- the parser casts to it, reads params,
  // and calls instantiate(). One type parameter carrying DEF_TYPENAME (the
  // parser's type-vs-value kind test), and the member lives in the pattern
  // class that instantiate() duplicates, not the template's own scope.
  auto* tmpl = dynamic_cast<jdi::definition_template*>(box_def);
  ASSERT_NE(tmpl, nullptr);
  ASSERT_EQ(tmpl->params.size(), 1u);
  EXPECT_TRUE(tmpl->params[0]->flags & jdi::DEF_TYPENAME);
  auto* pattern = dynamic_cast<jdi::definition_class*>(tmpl->def.get());
  ASSERT_NE(pattern, nullptr);
  EXPECT_NE(pattern->find_local("value"), nullptr);
  EXPECT_EQ(tmpl->find_local("value"), nullptr);
}

// The parser's TryParseTemplateArgs endgame: build an arg_key the way it
// does (sized ctor + mirror_types + put_final_type) and instantiate().
// The result must be a cached definition_class clone of the pattern with
// its members intact.
TEST_F(ClangPopulatorTest, ClassTemplateInstantiate) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);
  auto* tmpl = dynamic_cast<jdi::definition_template*>(global->find_local("Box"));
  ASSERT_NE(tmpl, nullptr);

  jdi::definition* color = global->find_local("Color");
  ASSERT_NE(color, nullptr);

  jdi::arg_key key(tmpl->params.size());
  key.mirror_types(tmpl);
  key.put_final_type(0, jdi::full_type(color));

  jdi::ErrorContext errc(jdi::default_error_handler,
                         jdi::SourceLocation{"test", 0, 0});
  jdi::definition* inst = tmpl->instantiate(key, errc);
  ASSERT_NE(inst, nullptr);

  auto* inst_class = dynamic_cast<jdi::definition_class*>(inst);
  ASSERT_NE(inst_class, nullptr);
  EXPECT_EQ(inst_class->instance_of, tmpl);
  EXPECT_NE(inst_class->find_local("value"), nullptr);
  EXPECT_NE(inst_class->find_local("count"), nullptr);
  EXPECT_NE(inst_class->find_local("size"), nullptr);

  // The dependent member's type must be remapped to the parameter alias
  // (a typedef-style definition_typed the instantiation owns) whose type
  // is the argument.
  auto* value_member =
      dynamic_cast<jdi::definition_typed*>(inst_class->find_local("value"));
  ASSERT_NE(value_member, nullptr);
  auto* subst = dynamic_cast<jdi::definition_typed*>(value_member->type);
  ASSERT_NE(subst, nullptr);
  EXPECT_EQ(subst->type, color);

  // Same key, same instantiation (the cache the parser relies on).
  jdi::arg_key key2(tmpl->params.size());
  key2.mirror_types(tmpl);
  key2.put_final_type(0, jdi::full_type(color));
  EXPECT_EQ(tmpl->instantiate(key2, errc), inst);
}

// One-time manual probe, not part of the default suite (SHELLmain.cpp is a
// full engine translation unit; parsing it is slow and this isn't something
// we want gating every build). Its own test suite (not ClangPopulatorTest --
// gtest forbids mixing TEST_F and TEST under one suite name). Run manually
// with:
//   ./emake-tests --gtest_also_run_disabled_tests --gtest_filter='*FullEngineProbe*'
TEST(ClangPopulatorFullEngineProbe, DISABLED_FullEngineProbe) {
  std::filesystem::path root = EnigmaRoot();
  std::filesystem::path shellmain = root / "ENIGMAsystem" / "SHELL" / "SHELLmain.cpp";
  ASSERT_TRUE(std::filesystem::exists(shellmain)) << shellmain;

  clang_adapter::ClangContext ctx;
  ctx.parse_file(shellmain.string(),
                  {(root / "ENIGMAsystem" / "SHELL").string(), (root / "shared").string()},
                  {"JUST_DEFINE_IT_RUN"});

  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);
  for (const char* name : {"variant", "var", "enigma", "enigma_user"}) {
    jdi::definition* def = global->find_local(name);
    EXPECT_NE(def, nullptr) << "expected " << name << " to be resolvable in the global scope";
    if (def) {
      std::cout << name << " flags: " << jdi::flagnames(def->flags) << std::endl;
    }
  }
}

// An object-like macro whose body starts with a paren is not function-like.
TEST_F(ClangPopulatorTest, ParenBodiedObjectMacroIsNotFunctionLike) {
  auto macros = ctx.get_macros();
  auto value = macros.find("DEMO_PAREN_VALUE");
  ASSERT_NE(value, macros.end());
  EXPECT_FALSE(value->second->parameters.has_value());
  EXPECT_FALSE(value->second->value.empty());

  auto func = macros.find("DEMO_FUNC_MACRO");
  ASSERT_NE(func, macros.end());
  ASSERT_TRUE(func->second->parameters.has_value());
  EXPECT_EQ(*func->second->parameters, (std::vector<std::string>{"a", "b"}));
}

// Function templates expose no arguments through clang_Cursor_getArgument;
// their parameters and defaults still populate, from the ParmDecl children.
TEST_F(ClangPopulatorTest, FunctionTemplateParameters) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);
  auto* pick = dynamic_cast<jdi::definition_function*>(global->find_local("pick"));
  ASSERT_NE(pick, nullptr);
  ASSERT_EQ(pick->overloads.size(), 1u);
  auto& node = pick->overloads.begin()->second->referencers.top();
  ASSERT_EQ(node.paramcount(), 2u);
  const jdi::ref_stack::parameter_ct& params =
      ((jdi::ref_stack::node_func*)&node)->params;
  EXPECT_EQ(params[0].default_value, nullptr);
  EXPECT_NE(params[1].default_value, nullptr);
}

// System search dirs come from the active compiler descriptor and can change
// between parses (the IDE switching descriptors); each parse reads them anew.
TEST(ClangSearchDirsTest, LaterParsesSeeNewSearchDirs) {
  namespace fs = std::filesystem;
  fs::path dir = fs::temp_directory_path() / "clang_search_dir_probe";
  fs::create_directories(dir);
  std::ofstream(dir / "search_dir_probe.h") << "int from_new_search_dir;\n";
  fs::path src = fs::temp_directory_path() / "clang_search_dir_probe_main.hpp";
  std::ofstream(src) << "#include <search_dir_probe.h>\n";

  clang_adapter::ClangContext before;
  before.parse_file(src.string());
  ASSERT_NE(before.get_global(), nullptr);
  EXPECT_EQ(before.get_global()->find_local("from_new_search_dir"), nullptr);

  jdi::builtin_context().add_search_directory(dir.string());
  clang_adapter::ClangContext after;
  after.parse_file(src.string());
  ASSERT_NE(after.get_global(), nullptr);
  EXPECT_NE(after.get_global()->find_local("from_new_search_dir"), nullptr);
}

// Template parameter defaults populate as presence sentinels; a value
// parameter's type reference is not mistaken for a default.
TEST_F(ClangPopulatorTest, TemplateParameterDefaults) {
  jdi::definition_scope* global = ctx.get_global();
  ASSERT_NE(global, nullptr);
  auto* holder = dynamic_cast<jdi::definition_template*>(global->find_local("Holder"));
  ASSERT_NE(holder, nullptr);
  ASSERT_EQ(holder->params.size(), 4u);
  EXPECT_EQ(holder->params[0]->default_assignment, nullptr);
  EXPECT_NE(holder->params[1]->default_assignment, nullptr);
  EXPECT_NE(holder->params[2]->default_assignment, nullptr);
  EXPECT_NE(holder->params[3]->default_assignment, nullptr);

  auto* box = dynamic_cast<jdi::definition_template*>(global->find_local("Box"));
  ASSERT_NE(box, nullptr);
  ASSERT_EQ(box->params.size(), 1u);
  EXPECT_EQ(box->params[0]->default_assignment, nullptr);
}
