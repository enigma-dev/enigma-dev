#include "common_test.hpp"

TEST(Game, window_test) {
  if (!TestHarness::windowing_supported()) return;
  TestConfig tc;
  tc.extensions = "Paths,GTest";
  auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
  test_common(test_harness, "window_test", false);
}
