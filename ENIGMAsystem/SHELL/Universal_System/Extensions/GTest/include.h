#ifndef ENIGMA_EXTENSION_GTEST_h
#define ENIGMA_EXTENSION_GTEST_h

#include "Universal_System/var4.h"
void gtest_assert_true(bool exp,  std::string message = "");
void gtest_assert_false(bool exp, std::string message = "");

void gtest_assert_eq(variant a, variant b, std::string message = "");
void gtest_assert_ne(variant a, variant b, std::string message = "");
void gtest_assert_lt(variant a, variant b, std::string message = "");
void gtest_assert_le(variant a, variant b, std::string message = "");
void gtest_assert_gt(variant a, variant b, std::string message = "");
void gtest_assert_ge(variant a, variant b, std::string message = "");

#endif  // ENIGMA_EXTENSION_GTEST_h
