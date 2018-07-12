#include "TestHarness.hpp"
#include "Utility.hpp"

#include <gtest/gtest.h>

TEST(Regression, draw_test) {
  if (!TestHarness::windowing_supported()) return;
  auto test_harness = LAUNCH_HARNESS_FOR_SOG(TestConfig());
  if (!test_harness) FAIL() << "Game could not be run.";

  test_harness->wait();  // Let the game render a frame first.
  ASSERT_TRUE(test_harness->game_is_running())
      << "Game stopped running unexpectedly";

  test_harness->screen_save("./enigma_draw_test.png");

  test_harness->close_window();
  bool game_running = test_harness->game_is_running();
  for (int i = 0; game_running && i < 10; ++i) {
    test_harness->wait();
    game_running = test_harness->game_is_running();
  }
  ASSERT_FALSE(game_running) << "Game did not exit after window was closed!";
}

TEST(RegressionCompare, draw_compare_test) {
  // Test our image compare returns correct values (first 2 should fail)
  //ASSERT_NE(image_compare("/tmp/enigma_master_test.png", "CommandLine/testing/data/sprite.png", "/tmp/enigma_draw_diff.png"), 0);
  ASSERT_NE(image_compare("./enigma_draw_test.png", "CommandLine/testing/data/noexisto.png", "/tmp/enigma_draw_diff.png"), 0);
  ASSERT_EQ(image_compare("./enigma_draw_test.png", "/tmp/enigma-master/enigma_draw_test.png", "/tmp/enigma_draw_diff.png"), 0);
  // if the image comparison did not fail, then we can delete the files
  // so our Travis CI script knows not to upload them
  file_delete("./enigma_draw_test.png");
  file_delete("/tmp/enigma-master/enigma_draw_test.png");
  file_delete("/tmp/enigma_draw_diff.png");
}
