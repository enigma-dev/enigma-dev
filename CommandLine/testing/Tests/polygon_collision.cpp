#include "CommonTest.hpp"

TEST(Game, polygon_collision) {
  TestConfig tc;
  tc.extensions = "Alarms,GTest";
  tc.collision = "Polygon";
  auto test_harness = LAUNCH_HARNESS_FOR_SOG(tc);
  test_common(test_harness.get(), "polygon_collision" + tc.stringify(), false);
}
