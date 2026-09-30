// Tests for the GML runtime helpers in ENIGMAsystem/SHELL/Universal_System/gml_ops.h,
// built as a GM8-compliance game would see them.
#include "../../ENIGMAsystem/SHELL/Universal_System/gml_ops.h"

#include <gtest/gtest.h>

#include <cmath>
#include <cstdint>
#include <limits>

const int enigma::gm_compatibility_version = 80;

namespace {
constexpr int64_t kMax = std::numeric_limits<int64_t>::max(), kMin = std::numeric_limits<int64_t>::min();
const double kInf = std::numeric_limits<double>::infinity(), kNan = std::nan("");
}  // namespace

TEST(GmlOps, ToIntRoundsHalfToEven) {
  EXPECT_EQ(enigma::gml_to_int(5.5), 6);
  EXPECT_EQ(enigma::gml_to_int(4.5), 4);
  EXPECT_EQ(enigma::gml_to_int(-5.5), -6);
  EXPECT_EQ(enigma::gml_to_int(5.7), 6);
}

TEST(GmlOps, ToIntOutOfRangeIsDefined) {
  EXPECT_EQ(enigma::gml_to_int(kNan), 0);
  EXPECT_EQ(enigma::gml_to_int(kInf), kMax);
  EXPECT_EQ(enigma::gml_to_int(-kInf), kMin);
  EXPECT_EQ(enigma::gml_to_int(1e300), kMax);
  EXPECT_EQ(enigma::gml_to_int(-1e300), kMin);
  EXPECT_EQ(enigma::gml_to_int(9223372036854775808.0), kMax);   // 2^63, one past the range
  EXPECT_EQ(enigma::gml_to_int(-9223372036854775808.0), kMin);  // -2^63, the lowest value
  EXPECT_EQ(enigma::gml_to_int(9007199254740992.0), 9007199254740992);  // 2^53 converts exactly
}

TEST(GmlOps, IndexOutOfRangeIsDefined) {
  EXPECT_EQ(enigma::gml_index(1.5), 2);
  EXPECT_EQ(enigma::gml_index(kNan), 0);
  EXPECT_EQ(enigma::gml_index(1e20), std::numeric_limits<int>::max());
  EXPECT_EQ(enigma::gml_index(-1e20), std::numeric_limits<int>::min());
}

TEST(GmlOps, BitwiseMatchesGm8) {
  EXPECT_EQ(enigma::gml_bitand(5.7, 1), 0);
  EXPECT_EQ(enigma::gml_bitand(-5.5, 255), 250);
  EXPECT_EQ(enigma::gml_shl(1, 40), 1099511627776.0);
  EXPECT_EQ(enigma::gml_shr(-8, 1), 9223372036854775804.0);  // logical: 2^63 - 4
  EXPECT_EQ(enigma::gml_bitnot(0), -1);
  EXPECT_EQ(enigma::gml_bitand(kNan, 255), 0);
  EXPECT_EQ(enigma::gml_bitor(kInf, 0), (double) kMax);
}
