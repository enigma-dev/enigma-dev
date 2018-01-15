#ifndef EMAKE_TESTHARNESS_HPP
#define EMAKE_TESTHARNESS_HPP

#include <memory>
#include <string>

const std::string kGamesDir = "CommandLine/testing/Tests/";

class TestHarness {
 public:
  virtual std::string get_caption() = 0;

  // Window manipulation
  static bool windowing_supported();
  virtual void minimize_window() = 0;
  virtual void maximize_window() = 0;
  virtual void fullscreen_window() = 0;
  virtual void unminimize_window() = 0;
  virtual void unmaximize_window() = 0;
  virtual void unfullscreen_window() = 0;
  virtual void close_window() = 0;

  /// Wait for an arbitrary amount of time to let things settle.
  virtual void wait() = 0;

  /// Close the game by conventional means.
  virtual void close_game() = 0;
  /// Check if the game process is still running.
  virtual bool game_is_running() = 0;

  virtual ~TestHarness() {}
};

struct TestConfig {
  std::string mode;
  std::string graphics;
  std::string audio;
  std::string widgets;
  std::string network;
  std::string collision;
  std::string extensions;

  std::string get_or(std::string(TestConfig::*option), std::string alt) const {
    std::string mine = this->*option;
    return mine.empty() ? alt : mine;
  }
};

/// Launch a game's executable file and "attach" to it.
/// Not in a debugger sense, but in a "grabbing the window" sense.
std::unique_ptr<TestHarness>
    launch(const std::string &game, const TestConfig &tc);

#endif  // EMAKE_TESTHARNESS_HPP
