#include "CommonTest.hpp"

TEST(Regression, draw_test) {
  if (!TestHarness::windowing_supported()) return;
  // Iterate only platforms and graphics systems
  for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    tc.extensions = "Paths,libpng,GTest";
    auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
    test_common(test_harness.get(), "draw_test" + tc.stringify());
  }
}
