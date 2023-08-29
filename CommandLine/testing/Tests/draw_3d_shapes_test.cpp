#include "CommonTest.hpp"

TEST(Regression, draw_3d_shapes_test) {
  if (!TestHarness::windowing_supported()) return;
  // Iterate only platforms and graphics systems
  //for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    TestConfig tc = TestConfig();
    auto test_harness = LAUNCH_HARNESS_FOR_GAME(tc, "gmx");
    test_common(test_harness.get(), "draw_3d_shapes_test" + tc.stringify());
  //}
}
