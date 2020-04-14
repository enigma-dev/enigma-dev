#include "CommonTest.hpp"

TEST(Game, window_test) {
  if (!TestHarness::windowing_supported()) return;
  // Iterate only platforms and graphics systems
  for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    tc.extensions = "Paths,GTest";
    auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
    test_common(test_harness.get(), "window_test" + tc.stringify(), false);
  }
}
