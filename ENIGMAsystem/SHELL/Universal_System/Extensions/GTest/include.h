#ifndef ENIGMA_EXTENSION_GTEST_h
#define ENIGMA_EXTENSION_GTEST_h

#include "Universal_System/var4.h"
void gtest_assert_true(bool exp,  std::string message = "");
void gtest_assert_false(bool exp, std::string message = "");

#define DECLARE_BINARY_FOR_TYPE_G(FUNC, TYPE1, TYPE2)  \
  void FUNC(TYPE1 a, TYPE2 b, std::string message = "");
#define DECLARE_BINARY_FOR_TYPE(FUNC, TYPE)  \
  DECLARE_BINARY_FOR_TYPE_G(FUNC, TYPE, TYPE);

#define DECLARE_BINARY_FOR_ALL_TYPES(FUNC)     \
  DECLARE_BINARY_FOR_TYPE(FUNC, int8_t);       \
  DECLARE_BINARY_FOR_TYPE(FUNC, int16_t);      \
  DECLARE_BINARY_FOR_TYPE(FUNC, int32_t);      \
  DECLARE_BINARY_FOR_TYPE(FUNC, int64_t);      \
  DECLARE_BINARY_FOR_TYPE(FUNC, uint8_t);      \
  DECLARE_BINARY_FOR_TYPE(FUNC, uint16_t);     \
  DECLARE_BINARY_FOR_TYPE(FUNC, uint32_t);     \
  DECLARE_BINARY_FOR_TYPE(FUNC, uint64_t);     \
  DECLARE_BINARY_FOR_TYPE(FUNC, float);        \
  DECLARE_BINARY_FOR_TYPE(FUNC, double);       \
  DECLARE_BINARY_FOR_TYPE(FUNC, long double);  \
  DECLARE_BINARY_FOR_TYPE(FUNC, std::string);  \
  DECLARE_BINARY_FOR_TYPE(FUNC, const var&);   \
  DECLARE_BINARY_FOR_TYPE(FUNC, const variant&);

DECLARE_BINARY_FOR_ALL_TYPES(gtest_assert_eq);
DECLARE_BINARY_FOR_ALL_TYPES(gtest_assert_ne);
DECLARE_BINARY_FOR_ALL_TYPES(gtest_assert_lt);
DECLARE_BINARY_FOR_ALL_TYPES(gtest_assert_le);
DECLARE_BINARY_FOR_ALL_TYPES(gtest_assert_gt);
DECLARE_BINARY_FOR_ALL_TYPES(gtest_assert_ge);

DECLARE_BINARY_FOR_TYPE_G(gtest_assert_eq, float, int);
DECLARE_BINARY_FOR_TYPE_G(gtest_assert_eq, double, int);
DECLARE_BINARY_FOR_TYPE_G(gtest_assert_eq, int, double);

void gtest_assert_eq_eps(double a, double b, std::string m = "");
void gtest_assert_eq_eps(float a,  double b, std::string m = "");

#undef DECLARE_BINARY_FOR_ALL_TYPES
#undef DECLARE_BINARY_FOR_TYPE

#if defined INCLUDED_FROM_SHELLMAIN && !defined JUST_DEFINE_IT_RUN
namespace enigma {

class DefaultGtestErrorMessage {
  std::string default_message_;

 public:
  DefaultGtestErrorMessage(std::string default_message):
      default_message_(default_message) {}
  string operator()() { return default_message_; }
  string operator()(std::string msg) { return default_message_ + "\n" + msg; }
};

# define gtest_assert_eq(a, b, m...) \
         gtest_assert_eq(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_eq_eps(a, b, m...) \
         gtest_assert_eq_eps(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_ne(a, b, m...) \
         gtest_assert_ne(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_lt(a, b, m...) \
         gtest_assert_lt(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_le(a, b, m...) \
         gtest_assert_le(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_gt(a, b, m...) \
         gtest_assert_gt(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_ge(a, b, m...) \
         gtest_assert_ge(a, b,       \
             enigma::DefaultGtestErrorMessage("Where a = " #a ", b = " #b)(m))
# define gtest_assert_true(exp, m...) \
         gtest_assert_true(exp,       \
             enigma::DefaultGtestErrorMessage("False expression: " #exp)(m))
# define gtest_assert_false(exp, m...) \
         gtest_assert_false(exp,       \
             enigma::DefaultGtestErrorMessage("True expression: " #exp)(m))

}
#endif  // Included from SHELLmain, not JDI

#endif  // ENIGMA_EXTENSION_GTEST_h
