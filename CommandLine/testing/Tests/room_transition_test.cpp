#include "CommonTest.hpp"

TEST(Regression, room_transition_test) {
  if (!TestHarness::windowing_supported()) return;
    // Iterate only platforms and graphics systems
  for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    tc.extensions = "Paths,GTest";
    auto test_harness = LAUNCH_HARNESS_FOR_GAME(tc, "gmx");
    test_common(test_harness.get(), "room_transition_test" + tc.stringify());
  }
}
