#include "TestHarness.hpp"
#include <gtest/gtest.h>

TEST(Game, dnd_generator_test) {
  TestConfig tc;
  tc.extensions = "Alarms,GTest";
  tc.audio = "None";
  auto test_harness = LAUNCH_HARNESS_FOR_GAME(tc, "gmx");

  if (!test_harness) FAIL() << "Game could not be run.";

  ASSERT_TRUE(test_harness->game_is_running())
      << "Game stopped running unexpectedly";

  // Close the window and verify that the game ends normally.
  test_harness->close_window();
  bool game_running = test_harness->game_is_running();
  for (int i = 0; game_running && i < 10; ++i) {
    test_harness->wait();
    game_running = test_harness->game_is_running();
  }
  ASSERT_FALSE(game_running) << "Game did not exit after window was closed!";
  EXPECT_EQ(test_harness->get_return(), 0) << "Game returned non-zero exit code!";

}
