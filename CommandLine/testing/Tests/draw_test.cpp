#include "common_test.hpp"

TEST(Regression, draw_test) {
  if (!TestHarness::windowing_supported()) return;
  TestConfig tc;
  tc.extensions = "Paths,libpng,GTest";
  auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
  test_common(test_harness, "draw_test");
}
