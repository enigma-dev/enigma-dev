#include "TestHarness.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <windows.h>
#include <direct.h>
#include <process.h>
#include <chrono>
#include <thread>
#include <cwchar>

namespace {
using std::string;
using std::wstring;
using std::size_t;
using std::to_string;
using std::unique_ptr;
using std::getenv;

wstring widen(string tstr) {
  size_t wchar_count = tstr.size() + 1;
  vector<wchar_t> buf(wchar_count);
  return wstring{ buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, tstr.c_str(), -1, buf.data(), (int)wchar_count) };
}

string narrow(wstring wstr) {
  int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
  vector<char> buf(nbytes);
  return string{ buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, NULL, NULL) };
}

string get_temp_directory() {
  wchar_t buffer[MAX_PATH + 1];
  GetTempPathW(MAX_PATH + 1, buffer);
  return (narrow(buffer).back() == '\\') ? narrow(buffer) : narrow(buffer) + "\\";
}

struct handle_data {
  DWORD process_id;
  HWND window_handle;
};

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam) {
  handle_data& data = *(handle_data*)lParam;
  DWORD process_id = 0;
  GetWindowThreadProcessId(handle, &process_id);
  if (data.process_id != process_id || !is_main_window(handle))
    return TRUE;
  data.window_handle = handle;
  return FALSE;
}

BOOL is_main_window(HWND handle) {
  return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

int pid_is_running(DWORD pid) {
  HANDLE hProcess;
  DWORD exitCode;
  if (pid == 0) {
    return 1;
  }
  if (pid < 0) {
    return 0;
  }
  hProcess = handle_from_pid(pid);
  if (NULL == hProcess) {
    if (GetLastError() == ERROR_INVALID_PARAMETER) {
      return 0;
    }
    return -1;
  }
  DWORD dwRetval = WaitForSingleObject(hProcess, 0);
  CloseHandle(hProcess);

  switch (dwRetval) {
    case WAIT_OBJECT_0;
      return 0;
      case WAIT_TIMEOUT;
        return 1;
      default:
        return -1;
  }
}

void gather_coverage(const TestConfig&);
bool config_supports_lcov(const TestConfig &tc) {
  return tc.compiler.empty() || tc.compiler == "TestHarness";
}

string get_window_caption(HWND win) {
  wchar_t res[256];
  GetWindowTextW(win, res, 256);
  return narrow(res);
}

DWORD get_window_pid(HWND win) {
  DWORD res;
  GetWindowThreadProcessId(win, &res);
  return res;
}

HWND find_window_by_pid(HWND win, DWORD pid) {
  if (get_window_pid(win) == pid) return win;
  handle_data data;
  data.process_id = pid;
  data.window_handle = 0;
  EnumWindows(enum_windows_callback, (LPARAM)&data);
  return data.window_handle;
}

class Win32_TestHarness final: public TestHarness {
 public:
  string get_caption() final {
    return get_window_caption(display, window_id);
  }
  void minimize_window() final {
    ShowWindow(window_id, SW_MINIMIZE);
  }
  void maximize_window() final {
    ShowWindow(window_id, SW_MAXIMIZE);
  }
  void fullscreen_window() final {
    DWORD style = GetWindowLongPtr(window_id, GWL_STYLE);
    style &= ~(WS_CAPTION | WS_BORDER);
    SetWindowLongPtr(window_id, GWL_STYLE, style);
    ShowWindow(window_id, SW_MAXIMIZE);
  }
  void unminimize_window() final {
    ShowWindow(window_id, SW_RESTORE);
  }
  void unmaximize_window() final {
    ShowWindow(window_id, SW_RESTORE);
  }
  void unfullscreen_window() final {
    DWORD style = GetWindowLongPtr(window_id, GWL_STYLE);
    style |= WS_CAPTION | WS_BORDER;
    SetWindowLongPtr(window_id, GWL_STYLE, style);
    ShowWindow(window_id, SW_RESTORE);
  }
  void close_window() final {
    DestroyWindow(window_id);
  }
  void screen_save(std::string fPath) final {
    RECT rc;
    GetClientRect(window_id, &rc);

    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, 
      rc.right - rc.left, rc.bottom - rc.top);

    SelectObject(hdc, hbmp);
    PrintWindow(window_id, hdc, PW_CLIENTONLY);

    Gdiplus::Bitmap bmp(hbmp,(HPALETTE)0);
    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);
    bmp.Save(widen(fPath).c_str(), &pngClsid,NULL);    

    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);
  }

  bool game_is_running() final {
    if (pid == -1) return false;
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    int status = 0, wr = WaitForSingleObject(hproc, INFINITE);
    CloseHandle(hproc);
    if (!wr) return true;
    if (wr != -1) {
      if (WIFEXITED(status)) {
        return_code = WEXITSTATUS(status);
        return false;
      }
      hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
      bool running = pid_is_running(hproc);
      CloseHandle(hproc);
      if (kill_success) {
        return_code = ErrorCodes::GAME_CRASHED;
        return false;
      }
    }
	hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	bool running = pid_is_running(hproc);
	CloseHandle(hproc);
    return !running;
  }
  void close_game() final {
    close_window();
    for (int i = 0; i < 16; ++i) { // Give the game a second to close
      if (!game_is_running()) return;
      std::this_thread::sleep_for(std::chrono::microseconds((62500));
    }
    std::cerr << "Warning: game did not close; terminated" << std::endl;
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    TerminateProcess(hproc, 1);
    CloseHandle(hproc);
    return_code = ErrorCodes::TIMED_OUT;
  }

  void wait() final {
    std::this_thread::sleep_for(std::chrono::microseconds((1000000));
  }
  int get_return() final {
    return return_code;
  }
  Win32_TestHarness(DWORD game_pid, HWND game_window,
                  const TestConfig &tc):
      pid(game_pid), window_id(game_window), display(disp), test_config(tc) {}
  ~Win32_TestHarness() {
    if (game_is_running()) {
      HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
      TerminateProcess(hproc, 1);
      CloseHandle(hproc);
      std::cerr << "Game still running; killed" << std::endl;
    }
    gather_coverage(test_config);
  }

 private:
  DWORD pid;
  HWND window_id;
  bool run_lcov;
  TestConfig test_config;
  int return_code = 0x10000;
};

constexpr const char *kDefaultExtensions =
    "Paths,DateTime,DataStructures,MotionPlanning,Alarms,Timelines,"
    "ParticleSystems";

int build_game(const string &game, const TestConfig &tc, const string &out) {
  if (DWORD emake = fork()) {
    int status = 0;
    if (emake == -1) return -1;
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    int status = 0, wr = WaitForSingleObject(hproc, INFINITE);
    CloseHandle(hproc);
    int ret = (wr == -1);
    system("./share_logs.sh");
    if (ret) return -1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    return -1;
  }

  // Invoke the compiler via emake
  using TC = TestConfig;
  string emake_cmd = "./emake";
  string compiler = "--compiler=" + tc.get_or(&TC::compiler, "TestHarness");
  string mode = "--mode=" + tc.get_or(&TC::mode, "Debug");
  string graphics = "--graphics=" + tc.get_or(&TC::graphics, "OpenGL1");
  string audio = "--audio=" + tc.get_or(&TC::audio, "OpenAL");
  string widgets = "--widgets=" + tc.get_or(&TC::widgets, "NULL");
  string network = "--network=" + tc.get_or(&TC::network, "NULL");
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

  _execvp(emake_cmd.c_str(), (char**)args);
  abort();
}

void gather_coverage(const TestConfig &config) {
  static int test_num = 0;
  test_num++;

  if (!config_supports_lcov(config)) {
    return;
  }

  DWORD child = fork();
  if (child) {
    if (child == -1) {
      ADD_FAILURE() << "Coverage failed to execute for test " << test_num << '!';
      return;
    }
    int status = -1;
  HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
  int status = 0, wr = WaitForSingleObject(hproc, INFINITE);
  CloseHandle(hproc);
    if (wr == -1) {
      ADD_FAILURE() << "Waiting on coverage report for test " << test_num
                    << " somehow failed...";
      return;
    }
    if (WIFEXITED(status)) {
      int code = WEXITSTATUS(status);
      if (code) {
        ADD_FAILURE() << "LCOV run for test " << test_num
                      << " exited with status " << code << "...";
        return;
      }
      std::cout << "Coverage completed successfully for test " << test_num
                << '.' << std::endl;
    }
    return;
  }

  string src_dir = "--directory=" + get_temp_directory() + "ENIGMA/.eobjs/Win32/Win32/TestHarness/"
                 + config.get_or(&TestConfig::mode, "Debug") + "/";
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

  _execvp("lcov", (char**) lcovArgs);
  abort();
}

}  // namespace

bool TestHarness::windowing_supported() {
  return true;
}

unique_ptr<TestHarness>
TestHarness::launch_and_attach(const string &game, const TestConfig &tc) {
  string out = get_temp_directory() + "test-game.exe";
  if (int retcode = build_game(game, tc, out)) {
    if (retcode != -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return nullptr;
  }

  DWORD pid = fork();
  if (!pid) {
    _execl(out.c_str(), out.c_str(), nullptr);
    abort();
  }
  if (pid == -1) return nullptr;
  std::this_thread::sleep_for(std::chrono::microseconds((250000)); // Give the window a quarter second to load and display.

  for (int i = 0; i < 50; ++i) {  // Try for over ten seconds to grab the window
    HWND win = find_window_by_pid(display, pid);
    if (win != NULL)
      return std::unique_ptr<Win32_TestHarness>(
          new Win32_TestHarness(display, pid, win, tc));
    std::this_thread::sleep_for(std::chrono::microseconds((250000));
  }

  return nullptr;
}

constexpr int operator"" _million(unsigned long long x) {
  return x * 1000 * 1000;
}

int TestHarness::run_to_completion(const string &game, const TestConfig &tc) {
  string out = get_temp_directory() + "test-game";
  if (int retcode = build_game(game, tc, out)) {
    if (retcode != -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return ErrorCodes::BUILD_FAILED;
  }

  DWORD pid = fork();
  if (!pid) {
    _chdir(game.substr(0, game.find_last_of("\\/")).c_str());
    _execl(out.c_str(), out.c_str(), nullptr);
    abort();
  }
  if (pid == -1) {
    return ErrorCodes::LAUNCH_FAILED;
  }
  for (int i = 0; i < 30000000; i += 12500) {
    HANDLE hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    int status = 0, wr = WaitForSingleObject(hproc, INFINITE);
    CloseHandle(hproc);
    if (wr) {
      if (wr != -1) {
        if (WIFEXITED(status)) {
          gather_coverage(tc);
          return WEXITSTATUS(status);
        }
        return ErrorCodes::GAME_CRASHED;
      }
      // Ignore the error for now...
      std::cerr << "Warning: ignoring waitpid being dumb." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::microseconds((12500));
  }
  std::cerr << "ERROR: game still running after 30 seconds; killed" << std::endl;
  hproc = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
  TerminateProcess(hproc, 1); // We're not dicking around with this.
  CloseHandle(hproc);
  return ErrorCodes::TIMED_OUT;
}
