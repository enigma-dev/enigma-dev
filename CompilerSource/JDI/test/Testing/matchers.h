#ifndef JDI_TESTING_MATCHERS_h
#define JDI_TESTING_MATCHERS_h

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "System/token.h"

namespace {

using ::testing::Matcher;
using ::testing::MatcherInterface;
using ::testing::MatchResultListener;

class HasTypeMatcher : public MatcherInterface<const jdi::token_t&> {
  jdi::TOKEN_TYPE type_;

 public:
  explicit HasTypeMatcher(jdi::TOKEN_TYPE type): type_(type) {}

  bool MatchAndExplain(const jdi::token_t &token,
                       MatchResultListener* /* listener */) const override {
    return token.type == type_;
  }

  void DescribeTo(::std::ostream* os) const override {
    *os << "token type is " << type_;
  }

  void DescribeNegationTo(::std::ostream* os) const override {
    *os << "token type is not " << type_;
  }
};

class HasContentMatcher : public MatcherInterface<const jdi::token_t&> {
  std::string_view content_;

 public:
  explicit HasContentMatcher(std::string_view content): content_(content) {}

  bool MatchAndExplain(const jdi::token_t &token,
                       MatchResultListener* /* listener */) const override {
    return token.content.view() == content_;
  }

  void DescribeTo(::std::ostream* os) const override {
    *os << "token content is `" << content_ << '`';
  }

  void DescribeNegationTo(::std::ostream* os) const override {
    *os << "token content is not `" << content_ << '`';
  }
};

inline Matcher<const jdi::token_t&> HasType(jdi::TOKEN_TYPE type) {
  return MakeMatcher(new HasTypeMatcher(type));
}

inline Matcher<const jdi::token_t&> HasContent(std::string_view type) {
  return MakeMatcher(new HasContentMatcher(type));
}

typedef MatcherInterface<const jdi::definition*> DefinitionMatcherInterface;
typedef Matcher<const jdi::definition*> DefinitionMatcher;
// GMock has no support for map operations, so we need to get intimate with our
// matcher interface.
class DefinitionMatcherImpl;
typedef std::unique_ptr<DefinitionMatcherImpl> DefinitionMatcherPtr;

class DefinitionMatcherImpl : public DefinitionMatcherInterface {
 public:
  const std::string &DefinitionName() const { return name_; }

  bool MatchAndExplain(const jdi::definition *def,
                       MatchResultListener* listener) const override {
    if (!def) return false;
    if (def->name != name_) return false;
    if ((def->flags & flags_) != flags_) return false;
    for (const auto &m : matchers_) {
      if (!m.MatchAndExplain(def, listener)) return false;
    }
    return true;
  }

  void DescribeTo(::std::ostream* os) const override {
    PrintGuts(os);
  }

  void PrintGuts(::std::ostream* os) const {
    *os << category_ << " named `" << name_ << '`';
    if (matchers_.size()) {
      *os << " (";
      std::string_view sep = "";
      for (const auto &m : matchers_) {
        *os << sep;
        m.DescribeTo(os);
        sep = ", ";
      }
      *os << ")";
    }
  }

  void DescribeNegationTo(::std::ostream* os) const override {
    *os << "no ";
    PrintGuts(os);
  }

  template<typename... T> DefinitionMatcherImpl(std::string name,
                                                std::string category,
                                                unsigned long flags,
                                                T... matchers):
      name_(name), category_(category), flags_(flags),
      matchers_{matchers...} {}

 private:
  /// Expected name of this definition.
  std::string name_;
  /// Human readable categorization of `flags_`.
  std::string category_;
  /// Actual flags to check against the definition.
  unsigned long flags_;

  std::vector<DefinitionMatcher> matchers_;
};

class HasMembersMatcher : public DefinitionMatcherInterface {
 public:
  bool MatchAndExplain(const jdi::definition *const def,
                       MatchResultListener* listener) const override {
    for (const DefinitionMatcherPtr &matcher_ptr : matchers_) {
      if (!def || !(def->flags & jdi::DEF_SCOPE)) return false;
      auto *const scope = (jdi::definition_scope*) def;
      jdi::definition *member = scope->look_up(matcher_ptr->DefinitionName());
      if (!member) return false;
      if (!matcher_ptr->MatchAndExplain(member, listener)) return false;
    }
    return true;
  }

  void DescribeTo(::std::ostream* os) const override {
    *os << "scope containing";
    std::string_view sep = " ";
    for (const DefinitionMatcherPtr &matcher_ptr : matchers_) {
      *os << sep;
      matcher_ptr->DescribeTo(os);
      sep = ", ";
    }
    if (!matchers_.size()) *os << " no members...?";
  }

  void DescribeNegationTo(::std::ostream* os) const override {
    *os << "scope not containing";
    std::string_view sep = " ";
    for (const DefinitionMatcherPtr &matcher_ptr : matchers_) {
      *os << sep;
      matcher_ptr->DescribeTo(os);
      sep = ", ";
    }
    if (!matchers_.size()) *os << " no members...?";
  }

  HasMembersMatcher(std::vector<DefinitionMatcherPtr> &&matchers):
      matchers_(std::move(matchers)) {}

 private:
  std::vector<DefinitionMatcherPtr> matchers_;
};

template<typename... T>
DefinitionMatcherPtr ClassDefinition(std::string name, T... matchers) {
  return std::make_unique<DefinitionMatcherImpl>(
      name, "class", jdi::DEF_CLASS, matchers...);
}

template<typename... T>
DefinitionMatcherPtr FunctionDefinition(std::string name, T... matchers) {
  return std::make_unique<DefinitionMatcherImpl>(
      name, "class", jdi::DEF_FUNCTION, matchers...);
}

template<typename... T>
Matcher<const jdi::definition*> HasMembers(T... matchers) {
  std::vector<DefinitionMatcherPtr> vec;
  (vec.emplace_back(std::move(matchers)), ...);
  return MakeMatcher(new HasMembersMatcher(std::move(vec)));
}

}  // namespace

#endif  // JDI_TESTING_MATCHERS_h
