#include "common_test.hpp"

TEST(Regression, room_transition_test) {
  if (!TestHarness::windowing_supported()) return;
  TestConfig tc;
  tc.extensions = "Paths,GTest";
  auto test_harness = LAUNCH_HARNESS_FOR_GAME(tc, "gmx");
  test_common(test_harness, "room_transition_test");
}
