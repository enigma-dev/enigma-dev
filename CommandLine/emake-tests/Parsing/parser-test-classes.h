#ifndef PARSER_TESTING_UTILS
#define PARSER_TESTING_UTILS

#include <gtest/gtest.h>
#include <memory>
#include <languages/lang_CPP.h>
#include <languages/clang_adapter.h>
#include <parsing/ast.h>
#include <parsing/parser.h>
#include "../../../CompilerSource/OS_Switchboard.h"
#include "matchers.h"

using namespace ::enigma::parsing;

static const NameSet kNoNames;

// Global test environment to ensure consistent state before any tests run.
// Tests using CreateWithCpp use NullLanguageFrontend which returns nullptr for all lookups.
// Tests using CreateWithSetUp initialize main_context and builtins.
// 
// The key insight: tests using CreateWithCpp expect nullptr types (jdi::builtin_type__int is nullptr
// when tests run in isolation). Tests using CreateWithSetUp expect initialized types.
// 
// To maintain consistency, we DON'T initialize types here. Instead, we ensure that:
// 1. Tests using CreateWithCpp work with nullptr types
// 2. Tests using CreateWithSetUp initialize types through SetUp()
// 3. After a SetUp() test runs, subsequent CreateWithCpp tests can still pass if they
//    compare against jdi::builtin_type__int (which is now set)
//
// Initialize main_context at startup so builtin types are always available.
// This ensures both ParserTests and PrinterTests work correctly:
// - ParserTests can find types via main_context fallback
// - PrinterTests can access type names via ft.def->name

class BuiltinTypesEnvironment : public ::testing::Environment {
 public:
  void SetUp() override {
    // Initialize main_context with builtin types so they're always available
    if (!main_context) {
      main_context = new clang_adapter::ClangContext();
    }
  }
  
  void TearDown() override {
    // Clean up global state
    delete main_context;
    main_context = nullptr;
    jdi::builtin_type__int = nullptr;
  }
};

// Register the environment
static ::testing::Environment* const builtin_types_env = 
    ::testing::AddGlobalTestEnvironment(new BuiltinTypesEnvironment());

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

struct ParserTester {
  TestFailureErrorHandler herr;
  const ParseContext* context;
  Lexer lexer;
  lang_CPP cpp{};
  AstBuilderTestAPI* builder = CreateBuilder();
  // Store lang_CPP instance for custom settings to manage lifetime
  std::unique_ptr<lang_CPP> custom_cpp;
  std::unique_ptr<ParseContext> custom_context;

  AstBuilderTestAPI* operator->() { return builder; }

  explicit ParserTester(std::string code, bool use_cpp)
      : context(&ParseContext::ForTesting(use_cpp)), lexer(std::move(code), context, &herr) {
    builder->initialize(&lexer, &herr);
  }

  explicit ParserTester(std::string code) : context(&SetUp()), lexer(std::move(code), context, &herr) {
    builder->initialize(&lexer, &herr);
  }

  // Private constructor for CreateWithSettings
  explicit ParserTester(std::string code, const ParseContext* ctx)
      : context(ctx), lexer(std::move(code), context, &herr) {
    builder->initialize(&lexer, &herr);
  }
  
  // Copy constructor - ensure lexer is copied correctly
  // Note: Lexer has a reference member, so we need to be careful when copying
  // The default copy should work since owned_code is a shared_ptr
  ParserTester(const ParserTester& other) = default;
  
  // Move constructor
  ParserTester(ParserTester&& other) noexcept = default;

  static ParserTester CreateWithCpp(std::string code) { 
    return ParserTester(std::move(code), true); 
  }

  static ParserTester CreateWithoutCpp(std::string code) { 
    return ParserTester(std::move(code), false); 
  }

  static ParserTester CreateWithSetUp(std::string code) { return ParserTester(std::move(code)); }

  static ParserTester CreateWithSettings(std::string code, std::string yaml_settings) {
    // Build full YAML with required fields
    std::string full_yaml = "%e-yaml\n---\n";
    full_yaml += "target-windowing: None\n";
    full_yaml += "target-audio: None\n";
    full_yaml += "target-compiler: gcc\n";
    full_yaml += "target-graphics: None\n";
    full_yaml += "target-widget: None\n";
    full_yaml += "target-collision: None\n";
    full_yaml += "target-networking: None\n";
    
    // Add default values for settings not specified
    if (yaml_settings.find("inherit-strings-from") == std::string::npos) {
      full_yaml += "inherit-strings-from: 0\n";
    }
    if (yaml_settings.find("inherit-escapes-from") == std::string::npos) {
      full_yaml += "inherit-escapes-from: 0\n";
    }
    if (yaml_settings.find("inherit-literals-from") == std::string::npos) {
      full_yaml += "inherit-literals-from: 0\n";
    }
    if (yaml_settings.find("inherit-increment-from") == std::string::npos) {
      full_yaml += "inherit-increment-from: 0\n";
    }
    if (yaml_settings.find("inherit-equivalence-from") == std::string::npos) {
      full_yaml += "inherit-equivalence-from: 0\n";
    }
    if (yaml_settings.find("automatic-semicolons") == std::string::npos) {
      full_yaml += "automatic-semicolons: true\n";
    }
    
    // Append user-provided settings (they will override defaults)
    full_yaml += yaml_settings;
    
    // Initialize context if needed
    if (!main_context) {
      main_context = new clang_adapter::ClangContext();
    }
    
    // Create a new lang_CPP instance for this test
    auto custom_cpp = std::make_unique<lang_CPP>();
    
    // Apply settings to the language frontend
    custom_cpp->definitionsModified(NULL, full_yaml.c_str());
    
    // Create ParseContext with the configured language frontend
    auto custom_context = std::make_unique<ParseContext>(custom_cpp.get(), kNoNames);
    const ParseContext* ctx = custom_context.get();
    
    // Create ParserTester with the custom context
    ParserTester tester(std::move(code), ctx);
    tester.custom_cpp = std::move(custom_cpp);
    tester.custom_context = std::move(custom_context);
    
    return tester;
  }

  const ParseContext& SetUp() {
    static lang_CPP cpp{};
    static bool initialized = false;

    if (!initialized) {
      // Initialize global context and language
      if (!main_context) {
        main_context = new clang_adapter::ClangContext();
      }
      if (!current_language) {
        current_language = &cpp;
      }
      
      // Parse headers to load type definitions
      // This should parse SHELLmain.cpp which includes standard headers
      // Use "None" for all subsystems to minimize dependencies for testing
      cpp.definitionsModified(NULL, ((string) "%e-yaml\n"
      "---\n"
      "target-windowing: None\n"
      "inherit-literals-from: 0\n"
      "sample-lots-of-radios: 0\n"
      "inherit-equivalence-from: 0\n"
      "sample-checkbox: on\n"
      "sample-edit: DEADBEEF\n"
      "sample-combobox: 0\n"
      "inherit-strings-from: 0\n"
      "inherit-escapes-from: 0\n"
      "inherit-increment-from: 0\n"
      " \n"
      "target-audio: None\n"
      "target-compiler: gcc\n"
      "target-graphics: None\n"
      "target-widget: None\n"
      "target-collision: None\n"
      "target-networking: None\n"
      ).c_str());

      initialized = true;
    }
    
    // Always restore jdi::builtin_type__int from main_context
    // This handles the case where a CreateWithCpp test reset it to nullptr
    if (main_context && main_context->get_global() && !jdi::builtin_type__int) {
      jdi::builtin_type__int = main_context->get_global()->look_up("int");
    }
    
    static ParseContext context(&cpp, kNoNames);
    return context;
  }

  void TearDown() {
    delete main_context;
    delete builder;
    delete current_language;
    // jdi::clean_up(); // TODO: Fix after JDI restoration
  }
};

#endif
