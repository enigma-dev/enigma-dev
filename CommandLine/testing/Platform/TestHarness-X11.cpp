#include "TestHarness.hpp"

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace {
using std::string;
using std::to_string;
using std::unique_ptr;
using std::getenv;

void gather_coverage();

string get_window_caption(Display *disp, Window win) {
  char *caption;
  XFetchName(disp, win, &caption);
  string res = caption ? caption : "";
  XFree(caption);
  return res;
}

pid_t get_window_pid(Display *disp, Window win) {
  static const Atom kWMPID = XInternAtom(disp, "_NET_WM_PID", False);
  static const Atom kCardinal = XInternAtom(disp, "CARDINAL", False);
  constexpr long kExpectItems = sizeof(pid_t) / 4;
  Atom type;
  int format;
  unsigned long items;
  unsigned long remainder;
  unsigned char *data = 0;
  pid_t res = 0;
  if (XGetWindowProperty(disp, win, kWMPID, 0, kExpectItems, False, kCardinal,
                         &type, &format, &items, &remainder, &data)
      == Success && data) {
    if (items < kExpectItems) {
      if (items == 1) {
        res = *(uint32_t*) data;
      }
    } else {
      res = *(pid_t*) data;
    }
    XFree(data);
  }
  return res;
}

Window find_window_by_pid(Display *disp, Window win, pid_t pid) {
  if (get_window_pid(disp, win) == pid) return win;
  Window root, parent, *child;
  unsigned children;
  if (XQueryTree(disp, win, &root, &parent, &child, &children)) {
    for (unsigned i = 0; i < children; i++) {
      Window recurse = find_window_by_pid(disp, child[i], pid);
      if (recurse != None) return recurse;
    }
  }
  return None;
}

class X11_TestHarness final: public TestHarness {
 public:
  string get_caption() final {
    return get_window_caption(display, window_id);
  }
  void minimize_window() final {
    system(("xdotool windowminimize " + to_string(window_id)).c_str());
  }
  void maximize_window() final {
    system(("wmctrl -i -r " + to_string(window_id)
                + " -b add,maximized_vert,maximized_horz").c_str());
  }
  void fullscreen_window() final {
    system(("wmctrl -i -r " + to_string(window_id)
                + " -b add,fullscreen").c_str());
  }
  void unminimize_window() final {
    system(("xdotool windowactivate " + to_string(window_id)).c_str());
  }
  void unmaximize_window() final {
    system(("wmctrl -i -r " + to_string(window_id)
                + " -b remove,maximized_vert,maximized_horz").c_str());
  }
  void unfullscreen_window() final {
    system(("wmctrl -i -r " + to_string(window_id)
                + " -b remove,fullscreen").c_str());
  }
  void close_window() final {
    system(("wmctrl -i -c " + to_string(window_id)).c_str());
  }

  bool game_is_running() final {
    if (pid == -1) return false;
    int status = 0, wr = waitpid(pid, &status, WNOHANG);
    if (!wr) return true;
    if (wr != -1) {
      if (WIFEXITED(status)) {
        return_code = WEXITSTATUS(status);
        return false;
      }
    }
    return !kill(pid, 0);
  }
  void close_game() final {
    close_window();
    for (int i = 0; i < 16; ++i) { // Give the game a second to close
      if (!game_is_running()) return;
      usleep(62500);
    }
    std::cerr << "Warning: game did not close; terminated" << std::endl;
    kill(pid, SIGTERM);
  }

  void wait() final {
    usleep(250000);
  }
  X11_TestHarness(Display *disp, pid_t game_pid, Window game_window, bool lcov):
      pid(game_pid), window_id(game_window), display(disp), run_lcov(lcov) {}
  ~X11_TestHarness() {
    if (game_is_running()) {
      kill(pid, SIGKILL);
      std::cerr << "Game still running; killed" << std::endl;
    }
    if (run_lcov) {
      gather_coverage();
    }
  }

 private:
  pid_t pid;
  Window window_id;
  Display *display;
  bool run_lcov;
  int return_code = 0x10000;
};

constexpr const char *kDefaultExtensions =
    "Paths,DateTime,DataStructures,MotionPlanning,Alarms,Timelines,"
    "ParticleSystems";

int build_game(const string &game, const TestConfig &tc, const string &out) {
  if (pid_t emake = fork()) {
    int status = 0;
    if (emake == -1) return -1;
    if (waitpid(emake, &status, 0) == -1) return -1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    return -1;
  }

  // Invoke the compiler via emake
  using TC = TestConfig;
  string emake_cmd = "./emake";
  string compiler = "--compiler=" + tc.get_or(&TC::compiler, "TestHarness");
  string mode = "--mode=" + tc.get_or(&TC::mode, "Debug");
  string graphics = "--graphics=" + tc.get_or(&TC::graphics, "OpenGL1");
  string audio = "--audio=" + tc.get_or(&TC::audio, "None");
  string widgets = "--widgets=" + tc.get_or(&TC::widgets, "None");
  string network = "--network=" + tc.get_or(&TC::network, "None");
  string collision = "--collision=" + tc.get_or(&TC::collision, "Precise");
  string extensions = "--extensions="
      + tc.get_or(&TC::extensions, kDefaultExtensions);

  const char *const args[] = {
    emake_cmd.c_str(),
    compiler.c_str(),
    mode.c_str(),
    graphics.c_str(),
    audio.c_str(),
    widgets.c_str(),
    network.c_str(),
    collision.c_str(),
    extensions.c_str(),
    game.c_str(),
    "-o",
    out.c_str(),
    nullptr
  };

  execvp(emake_cmd.c_str(), (char**) args);
  abort();
}

void gather_coverage() {
  static int test_num = 0;
  test_num++;

  pid_t child = fork();
  if (child) {
    if (child == -1) {
      std::cerr << "Coverage failed to execute for test " << test_num << "!\n";
      return;
    }
    int status = -1;
    if (waitpid(child, &status, 0) == -1) {
      std::cerr << "Waiting on coverage report for test " << test_num
                << " somehow failed..." << std::endl;
      return;
    }
    if (WIFEXITED(status)) {
      int code = WEXITSTATUS(status);
      if (code) {
        std::cerr << "LCOV run for test " << test_num << " exited with status "
                  << code << "..." << std::endl;
      }
      std::cout << "Coverage completed successfully for test " << test_num
                << '.' << std::endl;
    }
    return;
  }

  string src_dir = "--directory=" + string(getenv("HOME"))
                 + "/.enigma/.eobjs/Linux/Linux/TestHarness/Debug";
  string out_file = "--output-file=coverage_" + to_string(test_num) + ".info";

  const char *const lcovArgs[] = {
    "lcov",
    "--quiet",
    "--no-external",
    "--base-directory=ENIGMAsystem/SHELL/",
    "--capture",
    src_dir.c_str(),
    out_file.c_str(),
    nullptr
  };

  execvp("lcov", (char**) lcovArgs);
  abort();
}

}  // namespace

bool TestHarness::windowing_supported() {
  return true;
}

unique_ptr<TestHarness> launch(const string &game, const TestConfig &tc) {
  string out = "/tmp/test-game";
  if (int retcode = build_game(game, tc, out)) {
    if (retcode != -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return nullptr;
  }

  pid_t pid = fork();
  if (!pid) execl(out.c_str(), out.c_str(), nullptr);
  if (pid == -1) return nullptr;
  usleep(250000); // Give the window a quarter second to load and display.

  bool gen_cov_report = tc.compiler.empty() || tc.compiler == "TestHarness";

  Display *display = XOpenDisplay(0);
  Window root = XDefaultRootWindow(display);
  for (int i = 0; i < 50; ++i) {  // Try for over ten seconds to grab the window
    Window win = find_window_by_pid(display, root, pid);
    if (win != None)
      return std::unique_ptr<X11_TestHarness>(
          new X11_TestHarness(display, pid, win, gen_cov_report));
    usleep(250000);
  }

  return nullptr;
}
