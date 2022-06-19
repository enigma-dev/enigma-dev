#include "CommonTest.hpp"

TEST(Regression, dnd_generator_test) {
  if (!TestHarness::windowing_supported()) return;
  // Iterate only platforms and graphics systems
  for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    auto test_harness = LAUNCH_HARNESS_FOR_GAME(TestConfig(), "gmx");
    test_common(test_harness.get(), "dnd_generator_test" + tc.stringify());
  }
}
