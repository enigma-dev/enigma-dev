#ifndef EMAKE_TESTHARNESS_HPP
#define EMAKE_TESTHARNESS_HPP

#include <memory>
#include <string>

const std::string kGamesDir = "CommandLine/testing/Tests/";

struct TestConfig {
  std::string workdir;
  std::string codegen;
  std::string compiler;
  std::string mode;
  std::string platform;
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

  virtual void screen_save(std::string fPath) = 0;

  /// Wait for an arbitrary amount of time to let things settle.
  virtual void wait() = 0;

  /// Close the game by conventional means.
  virtual void close_game() = 0;
  /// Check if the game process is still running.
  virtual bool game_is_running() = 0;
  /// Check the game process's exit code.
  virtual int get_return() = 0;

  virtual ~TestHarness() {}

  static std::string swap_extension(std::string file, std::string ext) {
    const size_t slash = file.find_last_of("/\\");
    if (slash != std::string::npos) file.erase(0, slash + 1);
    const size_t dot = file.find_last_of('.');
    if (dot != 0 && dot != std::string::npos) file.erase(dot);
    return file + "." + ext;
  }

  /// Launch a game's executable file and "attach" to it.
  /// Not in a debugger sense, but in a "grabbing the window" sense.
  static std::unique_ptr<TestHarness>
      launch_and_attach(const std::string &game, const TestConfig &tc);

  /// Launch a game's executable file and let it run to completion.
  /// Return its exit code.
  static int run_to_completion(const std::string &game, const TestConfig &tc);

  enum ErrorCodes {
    BUILD_FAILED = -1,      ///< Used if the game failed to build.
    LAUNCH_FAILED = -2,    ///< Used if the game built, but could not be run.
    SYSCALL_FAILED = -3,  ///< Used if the operating system threw up.
    GAME_CRASHED = -4,   ///< Used if the game did not exit normally.
    TIMED_OUT = -5,     ///< Used if the game is killed for not exiting in time.
  };
};

/// Construct a test harness attached to the SOG with the same name as the
/// calling source file.
#define LAUNCH_HARNESS_FOR_GAME(config, ext)                           \
  (TestHarness::launch_and_attach(                                     \
      kGamesDir + TestHarness::swap_extension(__FILE__, ext), config))
#define LAUNCH_HARNESS_FOR_SOG(config)                                 \
  LAUNCH_HARNESS_FOR_GAME(config, "sog")

#endif  // EMAKE_TESTHARNESS_HPP
