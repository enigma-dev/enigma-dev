#include "CommonTest.hpp"

TEST(Regression, draw_3d_fog_test) {
  if (!TestHarness::windowing_supported()) return;
  // Iterate only platforms and graphics systems
  for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    auto test_harness = LAUNCH_HARNESS_FOR_GAME(TestConfig(), "gmx");
    test_common(test_harness.get(), "draw_3d_fog_test" + tc.stringify());
  }
}
