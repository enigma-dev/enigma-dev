#include "include.h"
#include <gtest/gtest.h>
using std::string;

#define IMPLEMENT_GTEST_BINARY_FUNC_G(FUNC, GTEST_CALL, TYPE1, TYPE2)     \
  static void FUNC##_impl(TYPE1 a, TYPE2 b, string msg, bool *survived) { \
    if (msg.empty()) ASSERT_EQ(a, b);                                     \
    else GTEST_CALL(a, b) << msg;                                         \
    *survived = true;                                                     \
  }                                                                       \
  void FUNC(TYPE1 a, TYPE2 b, string msg) {                               \
    bool survived = false;                                                \
    FUNC##_impl(a, b, msg, &survived);                                    \
    if (!survived) abort();                                               \
  }
#define IMPLEMENT_GTEST_BINARY_FUNC(F, C, T)                              \
        IMPLEMENT_GTEST_BINARY_FUNC_G(F, C, T, T)
#define IMPLEMENT_GTEST_UNARY_FUNC(TYPE, FUNC, GTEST_CALL)                \
  static void FUNC##_impl(TYPE exp, string msg, bool *survived) {         \
    if (msg.empty()) GTEST_CALL(exp);                                     \
    else GTEST_CALL(exp) << msg;                                          \
    *survived = true;                                                     \
  }                                                                       \
  void FUNC(TYPE exp, string message) {                                   \
    bool survived = false;                                                \
    FUNC##_impl(exp, message, &survived);                                 \
    if (!survived) abort();                                               \
  }

#define IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(FUNC, GTEST_CALL)       \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, int8_t);                  \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, int16_t);                 \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, int32_t);                 \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, int64_t);                 \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, uint8_t);                 \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, uint16_t);                \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, uint32_t);                \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, uint64_t);                \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, float);                   \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, double);                  \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, long double);             \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, std::string);             \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, const var&);              \
  IMPLEMENT_GTEST_BINARY_FUNC(FUNC, GTEST_CALL, const variant&);

IMPLEMENT_GTEST_UNARY_FUNC(bool, gtest_assert_true, ASSERT_TRUE);
IMPLEMENT_GTEST_UNARY_FUNC(bool, gtest_assert_false, ASSERT_FALSE);

IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(gtest_assert_eq, ASSERT_EQ);
IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(gtest_assert_ne, ASSERT_NE);
IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(gtest_assert_lt, ASSERT_LT);
IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(gtest_assert_le, ASSERT_LE);
IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(gtest_assert_gt, ASSERT_GT);
IMPLEMENT_GTEST_BINARY_FUNC_FOR_ALL_TYPES(gtest_assert_ge, ASSERT_GE);

IMPLEMENT_GTEST_BINARY_FUNC_G(gtest_assert_eq, ASSERT_EQ, float, int);
IMPLEMENT_GTEST_BINARY_FUNC_G(gtest_assert_eq, ASSERT_EQ, double, int);
IMPLEMENT_GTEST_BINARY_FUNC_G(gtest_assert_eq, ASSERT_EQ, int, double);

IMPLEMENT_GTEST_BINARY_FUNC_G(gtest_assert_eq_eps, ASSERT_FLOAT_EQ, float, double);
IMPLEMENT_GTEST_BINARY_FUNC_G(gtest_assert_eq_eps, ASSERT_DOUBLE_EQ, double, double);
