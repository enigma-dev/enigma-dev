#include "common_test.hpp"

TEST(Regression, draw_3d_shapes_test) {
  if (!TestHarness::windowing_supported()) return;
  auto test_harness = LAUNCH_HARNESS_FOR_GAME(TestConfig(), "gmx");
  test_common(test_harness.get(), "draw_3d_shapes_test");
}
