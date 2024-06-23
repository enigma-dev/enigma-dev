#ifndef PARSER_TESTING_UTILS
#define PARSER_TESTING_UTILS

#include <gtest/gtest.h>
#include <languages/lang_CPP.h>
#include <parsing/ast.h>
#include <parsing/parser.h>
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
  const ParseContext *context;
  Lexer lexer;
  lang_CPP cpp{};
  AstBuilder builder;

  AstBuilder *operator->() { return &builder; }

  explicit ParserTester(std::string code, bool use_cpp = false)
      : context(&ParseContext::ForTesting(use_cpp)), lexer(std::move(code), context, &herr), builder{&lexer, &herr} {}
};

#endif
