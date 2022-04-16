#include <API/error_reporting.h>

#include <string>
#include <vector>
#include <iostream>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#define TOKEN(type) token_t(type, __FILE__, __LINE__, 0)

using ::testing::Eq;
using ::testing::SizeIs;

namespace {

struct TestErrorHandler : jdi::ErrorHandler {
  std::vector<std::string> errors;
  void error(std::string_view msg, jdi::SourceLocation) override {
    errors.emplace_back(msg);
  }

  std::vector<std::string> warnings;
  void warning(std::string_view msg, jdi::SourceLocation) override {
    warnings.emplace_back(msg);
  }

  std::vector<std::string> infos;
  void info(std::string_view msg, int, jdi::SourceLocation) override {
    infos.emplace_back(msg);
  }
};

TEST(ErrorHandlerTest, CorrectErrorCounts) {
  TestErrorHandler err_impl;
  jdi::ErrorHandler *err = &err_impl;
  err->error({"hi", 0, 0})
      << "Test message " << 1 << ", " << 2 << ", " << 3 << "...";
  err->error(jdi::SourceLocation{"hi", 0, 0}) << "Second error.";
  ASSERT_THAT(err_impl.errors, testing::SizeIs(2));
  EXPECT_THAT(err_impl.errors[0], testing::Eq("Test message 1, 2, 3..."));
  EXPECT_THAT(err_impl.errors[1], testing::Eq("Second error."));
}


struct A { std::string to_string() const { return "Hello"; } };
struct B { std::string toString() const { return ", "; } };
struct C {};
std::ostream &operator<<(std::ostream &os, C const&) { return os << "world!"; }

TEST(ErrorHandlerTest, UniversalConversionHandling) {
  TestErrorHandler err_impl;
  jdi::ErrorHandler *err = &err_impl;
  jdi::ErrorContext errc = err->at({"hi", 0, 0});

  A a; B b; C c;
  errc.error() << a << b << c;
  ASSERT_THAT(err_impl.errors, testing::SizeIs(1));
  EXPECT_THAT(err_impl.errors[0], testing::Eq("Hello, world!"));
}

}  // namespace
