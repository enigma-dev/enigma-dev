#ifndef PARSER_TESTING_UTILS
#define PARSER_TESTING_UTILS

#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <parsing/ast.h>
#include <parsing/parser.h>
#include "../../../CompilerSource/OS_Switchboard.h"
#include "matchers.h"

using namespace ::enigma::parsing;

class TestFailureErrorHandler : public ErrorHandler {
 public:
  void ReportError(CodeSnippet snippet, std::string_view error) final {
    ADD_FAILURE() << "Test reported an error at line " << snippet.line << ", position " << snippet.position << ": "
                  << error;
  }
  void ReportWarning(CodeSnippet snippet, std::string_view warning) final {
    ADD_FAILURE() << "Test reported a warning at line " << snippet.line << ", position " << snippet.position << ": "
                  << warning;
  }
};

struct ParserTester {
  TestFailureErrorHandler herr;
  const ParseContext* context;
  Lexer lexer;
  lang_CPP cpp{};
  AstBuilderTestAPI* builder = CreateBuilder();

  AstBuilderTestAPI* operator->() { return builder; }

  explicit ParserTester(std::string code, bool use_cpp = false)
      : context(&ParseContext::ForTesting(use_cpp)), lexer(std::move(code), context, &herr) {
    builder->initialize(&lexer, &herr);
  }

  void SetUp() {
    cpp.definitionsModified(NULL, ((string) "%e-yaml\n"
    "---\n"
    "target-windowing: " +  (CURRENT_PLATFORM_ID==OS_WINDOWS ? "Win32" : CURRENT_PLATFORM_ID==OS_MACOSX ? "Cocoa" : "xlib")  + "\n"
    /* "target-graphics: OpenGL\n"
    "target-audio: OpenAL\n"
    "target-collision: BBox\n" */
    "treat-literals-as: 0\n"
    "sample-lots-of-radios: 0\n"
    "inherit-equivalence-from: 0\n"
    "sample-checkbox: on\n"
    "sample-edit: DEADBEEF\n"
    "sample-combobox: 0\n"
    "inherit-strings-from: 0\n"
    "inherit-escapes-from: 0\n"
    "inherit-increment-from: 0\n"
    " \n"
    "target-audio: OpenAL\n"
    "target-windowing: xlib\n"
    "target-compiler: gcc\n"
    "target-graphics: OpenGL\n"
    "target-widget: None\n"
    "target-collision: BBox\n"
    "target-networking: None\n"
    ).c_str());
  }

  void TearDown() {
    delete main_context;
    delete builder;
    delete current_language;
    jdi::clean_up();
  }
};

#endif
