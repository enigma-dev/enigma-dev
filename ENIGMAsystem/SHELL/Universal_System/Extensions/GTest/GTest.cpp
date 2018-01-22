#include "include.h"
#include <gtest/gtest.h>
using std::string;

#define IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL)                          \
  static void FUNC##_impl(variant a, variant b, string msg, bool *survived) {  \
    if (msg.empty()) ASSERT_EQ(a, b);                                          \
    else GTEST_CALL(a, b) << msg;                                              \
    *survived = true;                                                          \
  }                                                                            \
  void FUNC(variant a, variant b, string msg) {                                \
    bool survived = false;                                                     \
    FUNC##_impl(a, b, msg, &survived);                                         \
    if (!survived) abort();                                                    \
  }
#define IMPLEMENT_GTEST_UNARY_FUNC(TYPE, FUNC, GTEST_CALL)         \
  static void FUNC##_impl(TYPE exp, string msg, bool *survived) {  \
    if (msg.empty()) GTEST_CALL(exp);                              \
    else GTEST_CALL(exp) << msg;                                   \
    *survived = true;                                              \
  }                                                                \
  void FUNC(TYPE exp, string message) {                            \
    bool survived = false;                                         \
    FUNC##_impl(exp, message, &survived);                          \
    if (!survived) abort();                                        \
  }

IMPLEMENT_GTEST_UNARY_FUNC(bool, gtest_assert_true, ASSERT_TRUE);
IMPLEMENT_GTEST_UNARY_FUNC(bool, gtest_assert_false, ASSERT_FALSE);

IMPLEMENT_GTEST_BINARY_FUNC(gtest_assert_eq, ASSERT_EQ);
IMPLEMENT_GTEST_BINARY_FUNC(gtest_assert_ne, ASSERT_NE);
IMPLEMENT_GTEST_BINARY_FUNC(gtest_assert_lt, ASSERT_LT);
IMPLEMENT_GTEST_BINARY_FUNC(gtest_assert_le, ASSERT_LE);
IMPLEMENT_GTEST_BINARY_FUNC(gtest_assert_gt, ASSERT_GT);
IMPLEMENT_GTEST_BINARY_FUNC(gtest_assert_ge, ASSERT_GE);
