#ifndef ENIGMA_EXTENSION_GTEST_h
#define ENIGMA_EXTENSION_GTEST_h

#include "libEGMstd.h"

#ifdef JUST_DEFINE_IT_RUN
namespace enigma_user {

void gtest_assert_true(bool exp,  std::string message = "");
void gtest_assert_false(bool exp, std::string message = "");

void gtest_assert_eq_eps(var a, var b, std::string message = "");

void gtest_assert_eq(var a, var b, std::string message = "");
void gtest_assert_eq(var a, var b, std::string message = "");
void gtest_assert_ne(var a, var b, std::string message = "");
void gtest_assert_lt(var a, var b, std::string message = "");
void gtest_assert_le(var a, var b, std::string message = "");
void gtest_assert_gt(var a, var b, std::string message = "");
void gtest_assert_ge(var a, var b, std::string message = "");


void gtest_expect_true(bool exp,  std::string message = "");
void gtest_expect_false(bool exp, std::string message = "");

void gtest_expect_eq_eps(var a, var b, std::string message = "");

void gtest_expect_eq(var a, var b, std::string message = "");
void gtest_expect_eq(var a, var b, std::string message = "");
void gtest_expect_ne(var a, var b, std::string message = "");
void gtest_expect_lt(var a, var b, std::string message = "");
void gtest_expect_le(var a, var b, std::string message = "");
void gtest_expect_gt(var a, var b, std::string message = "");
void gtest_expect_ge(var a, var b, std::string message = "");

} //enigma_user

#else

namespace enigma {

void gtest_binary(std::string expression, std::string left_value, std::string right_value,
                  std::string left_exp, std::string right_exp, std::string operator_english,
                  bool pass, bool assert, std::string user_message,
                  std::string script, int line);

void gtest_unary(std::string expression, std::string value, std::string expected_value,
                 bool pass, bool assert, std::string user_message,
                 std::string script, int line);

template<typename T, typename U> bool within_five_ulp(T a, U b) {
  if (a == b) return true;
  T next = nexttoward(a, b);
  return  (a < b) != (a + 5 * (next - a) < b);
}

}  // namespace enigma

namespace enigma_user {

template<typename T, typename U>
void gtest_check_eq(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  enigma::gtest_binary("(" + aE + ") == (" + bE + ")", toString(a), toString(b), aE, bE,
                       "is equal to",                  a == b, assert, message, "EDL", 1);
}
template<typename T, typename U>
void gtest_check_ne(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  enigma::gtest_binary("(" + aE + ") != (" + bE + ")", toString(a), toString(b), aE, bE,
                       "is not equal to",              a != b, assert, message, "EDL", 1);
}
template<typename T, typename U>
void gtest_check_lt(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  enigma::gtest_binary("(" + aE + ") < ("  + bE + ")", toString(a), toString(b), aE, bE,
                       "is less than",                 a <  b, assert, message, "EDL", 1);
}
template<typename T, typename U>
void gtest_check_le(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  enigma::gtest_binary("(" + aE + ") <= (" + bE + ")", toString(a), toString(b), aE, bE,
                       "is less than or equal to",     a <= b, assert, message, "EDL", 1);
}
template<typename T, typename U>
void gtest_check_gt(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  enigma::gtest_binary("(" + aE + ") > ("  + bE + ")", toString(a), toString(b), aE, bE,
                       "is greater than",              a >  b, assert, message, "EDL", 1);
}
template<typename T, typename U>
void gtest_check_ge(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  enigma::gtest_binary("(" + aE + ") >= (" + bE + ")", toString(a), toString(b), aE, bE,
                       "is greater than or equal to",  a >= b, assert, message, "EDL", 1);
}

template<typename T, typename U>
void gtest_check_eq_eps(T a, U b, std::string message, std::string aE, std::string bE, bool assert) {
  bool approx_eq = enigma::within_five_ulp(a, b);
  enigma::gtest_binary("(" + aE + ") == (" + bE + ")", toString(a), toString(b), aE, bE,
                       "is approximately equal to", approx_eq, assert, message, "EDL", 1);
}

static inline void gtest_check_true(bool exp,  std::string message, std::string exp_str, bool assert) {
  enigma::gtest_unary(exp_str, (exp ? "true" : "false"), "true", exp, assert, message, "EDL", 1);
}
static inline void gtest_check_false(bool exp, std::string message, std::string exp_str, bool assert) {
  enigma::gtest_unary(exp_str, (exp ? "true" : "false"), "false", !exp, assert, message, "EDL", 1);
}

}  // namespace enigma_user

#ifdef INCLUDED_FROM_SHELLMAIN
# define gtest_assert_eq_eps(a, b, m...) gtest_check_eq_eps(a, b, std::string{m}, #a, #b, true)
# define gtest_assert_eq(a, b, m...)     gtest_check_eq    (a, b, std::string{m}, #a, #b, true)
# define gtest_assert_ne(a, b, m...)     gtest_check_ne    (a, b, std::string{m}, #a, #b, true)
# define gtest_assert_lt(a, b, m...)     gtest_check_lt    (a, b, std::string{m}, #a, #b, true)
# define gtest_assert_le(a, b, m...)     gtest_check_le    (a, b, std::string{m}, #a, #b, true)
# define gtest_assert_gt(a, b, m...)     gtest_check_gt    (a, b, std::string{m}, #a, #b, true)
# define gtest_assert_ge(a, b, m...)     gtest_check_ge    (a, b, std::string{m}, #a, #b, true)
# define gtest_assert_true(exp, m...)    gtest_check_true  (exp,  std::string{m}, #exp,   true)
# define gtest_assert_false(exp, m...)   gtest_check_false (exp,  std::string{m}, #exp,   true)

# define gtest_expect_eq_eps(a, b, m...) gtest_check_eq_eps(a, b, std::string{m}, #a, #b, false)
# define gtest_expect_eq(a, b, m...)     gtest_check_eq    (a, b, std::string{m}, #a, #b, false)
# define gtest_expect_ne(a, b, m...)     gtest_check_ne    (a, b, std::string{m}, #a, #b, false)
# define gtest_expect_lt(a, b, m...)     gtest_check_lt    (a, b, std::string{m}, #a, #b, false)
# define gtest_expect_le(a, b, m...)     gtest_check_le    (a, b, std::string{m}, #a, #b, false)
# define gtest_expect_gt(a, b, m...)     gtest_check_gt    (a, b, std::string{m}, #a, #b, false)
# define gtest_expect_ge(a, b, m...)     gtest_check_ge    (a, b, std::string{m}, #a, #b, false)
# define gtest_expect_true(exp, m...)    gtest_check_true  (exp,  std::string{m}, #exp,   false)
# define gtest_expect_false(exp, m...)   gtest_check_false (exp,  std::string{m}, #exp,   false)
#endif  // Included from SHELLmain
#endif  // Not JDI

#endif  // ENIGMA_EXTENSION_GTEST_h
