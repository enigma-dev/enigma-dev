#include "TestHarness.hpp"

#include <gtest/gtest.h>

TEST(Regression, draw_3d_shapes_test) {
  if (!TestHarness::windowing_supported()) return;
  auto test_harness = LAUNCH_HARNESS_FOR_GAME(TestConfig(), "gmx");
  if (!test_harness) FAIL() << "Game could not be run.";

  test_harness->wait();  // Let the game render a frame first.
  ASSERT_TRUE(test_harness->game_is_running())
      << "Game stopped running unexpectedly";

  test_harness->screen_save("./test-harness-out/enigma_draw_3d_shapes_test.png");

  test_harness->close_window();
  bool game_running = test_harness->game_is_running();
  for (int i = 0; game_running && i < 10; ++i) {
    test_harness->wait();
    game_running = test_harness->game_is_running();
  }
  ASSERT_FALSE(game_running) << "Game did not exit after window was closed!";
  EXPECT_EQ(test_harness->get_return(), 0) << "Game returned non-zero exit code!";
}
