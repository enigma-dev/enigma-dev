#include "CommonTest.hpp"

TEST(Regression, draw_tiles_test) {
  if (!TestHarness::windowing_supported()) return;
<<<<<<< HEAD
  // Iterate only platforms and graphics systems
  for (TestConfig tc : GetValidConfigs(true, true, false, false, false, false)) {
    tc.extensions = "Paths,libpng";
    auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
    test_common(test_harness.get(), "draw_tiles_test" + tc.stringify());
  }
=======
  TestConfig tc;
  tc.extensions = "Paths,libpng";
  auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
  test_common(test_harness.get(), "draw_tiles_test");
>>>>>>> origin/master
}
