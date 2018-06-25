#include <gtest/gtest.h>
#include "TestHarness.hpp"

TEST(Game, draw_test) {
  if (!TestHarness::windowing_supported()) return;
  auto test_harness = LAUNCH_HARNESS_FOR_SOG(TestConfig());
  if (!test_harness) FAIL() << "Game could not be run.";

  ASSERT_TRUE(test_harness->game_is_running())
      << "Game stopped running unexpectedly";

  test_harness->screen_save("/tmp/test.png");
  // Test our image compare returns correct values (first 2 should fail)
  ASSERT_NE(test_harness->image_compare("CommandLine/testing/data/draw_test.png", "CommandLine/testing/data/sprite.png", "/tmp/diff.png"), 0);
  ASSERT_NE(test_harness->image_compare("CommandLine/testing/data/draw_test.png", "CommandLine/testing/data/noexisto.png", "/tmp/diff.png"), 0);
  ASSERT_EQ(test_harness->image_compare("CommandLine/testing/data/draw_test.png", "/tmp/test.png", "/tmp/diff.png"), 0);

  test_harness->close_window();
  bool game_running = test_harness->game_is_running();
  for (int i = 0; game_running && i < 10; ++i) {
    test_harness->wait();
    game_running = test_harness->game_is_running();
  }
  ASSERT_FALSE(game_running) << "Game did not exit after window was closed!";
}