#include "TestHarness.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <windows.h>
std::vector<TestConfig> GetValidConfigs(bool platforms, bool graphics, bool audio, bool collisions, bool widgets, bool network) {
  std::vector<TestConfig> tcs;
  
  for (std::string_view p : {"Win32", "SDL"} ) {
    // FIXME: glgetteximage is used in opengl-common and is unsupported by gles. This function currently is required to copy surfaces in some tests
    for (std::string_view g : {"OpenGL1", "OpenGL3"/*, "OpenGLES2", "OpenGLES3"*/ }) {
      // Invalid combos
      if (g == "OpenGLES2" && p != "SDL") continue;
      if (g == "OpenGLES3" && p != "SDL") continue;
      for (std::string_view a : {"OpenAL"}) {
        for (std::string_view c : {"Precise", "BBox" }) {
          for (std::string_view w : {"None"}) {
            for (std::string_view n : {"None", "BerkeleySockets", "Asynchronous" }) {
              TestConfig tc;
              tc.platform = p;
              tc.graphics = g;
              tc.audio = a;
              tc.collision = c;
              tc.widgets = w;
              tc.network = n;
              tcs.push_back(tc);
              if (!network) break;
            }
            if (!widgets) break;
          }
          if (!collisions) break;
        }
        if (!audio) break;
      }
      if (!graphics) break;
    }
    if (!platforms) break;
  }
  
  return tcs;
}

namespace {
using std::string;
using std::to_string;
using std::unique_ptr;
using std::getenv;

void gather_coverage(const TestConfig&);
bool config_supports_lcov(const TestConfig &tc) {
  return tc.compiler.empty() || tc.compiler == "TestHarness";
}

//support functions
//string get_window_caption(HWND win_hnd);

struct hwndPid{
  unsigned long pid;
  HWND whandle;
};

class ProcessData{
  bool islac;
  public:
  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  void meminit(){
    ZeroMemory( &si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi));
    return;
  }
  ProcessData(bool flag):islac(flag){
    meminit();
  }
  ProcessData(){
    meminit();
  }
  ~ProcessData(){
    if(!islac){
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
  }
};

BOOL CALLBACK EnumWindowCallback(HWND winhnd, LPARAM lparam) {
    (*(hwndPid *)lparam).whandle=winhnd;
    unsigned long int proc_id;

    GetWindowThreadProcessId(winhnd, &proc_id);
    if(proc_id==(*(hwndPid *)lparam).pid) return false;
    return true;
}

HWND find_window_by_pid(int proc_id) {
    hwndPid data;
    data.pid=proc_id;
    data.whandle=0;
    EnumWindows(EnumWindowCallback,(LPARAM)&data);
    return data.whandle;
}

class Win32_TestHarness final: public TestHarness {
 public:
 
  string get_caption() final {
    return "TEST";
  }
  void minimize_window() final {
    ShowWindow(window_handle,SW_MINIMIZE);
  }
  void maximize_window() final {
    ShowWindow(window_handle,SW_MAXIMIZE);
  }
  void fullscreen_window() final {
    ShowWindow(window_handle,SW_MAXIMIZE);
  }
  void unminimize_window() final {
    ShowWindow(window_handle,SW_RESTORE);
  }
  void unmaximize_window() final {
    ShowWindow(window_handle,SW_SHOWDEFAULT);
  }
  void unfullscreen_window() final {
    ShowWindow(window_handle,SW_RESTORE);
  }
  void close_window() final {
    PostMessage( window_handle, WM_CLOSE, (WPARAM)nullptr, (LPARAM)nullptr);
  }
  void screen_save(std::string fPath) final {
    RECT clientRect;
    POINT clientAbsCoor;
    GetClientRect(window_handle,&clientRect);
    clientAbsCoor.x=clientRect.left;
    clientAbsCoor.y=clientRect.top;
    ClientToScreen(window_handle,&clientAbsCoor);

    SetWindowPos(window_handle,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
    system(("py ./screenshot.py \""+fPath+"\" "+
              to_string(clientAbsCoor.x)+" "+
              to_string(clientAbsCoor.y)+" "+
              to_string(clientAbsCoor.x+clientRect.right)+" "+
              to_string(clientAbsCoor.y+clientRect.bottom)
              ).c_str());
    return;
  }

  bool game_is_running() final {
    unsigned long int exitcode;
    GetExitCodeProcess(process_info.hProcess,&exitcode);
    if(exitcode!=259){
      return_code=exitcode; 
      return false;
    }
    return true;
    
  }
  void close_game() final {
    close_window();
    for (int i = 0; i < 20; ++i) { // Give the game a second to close
      if (!game_is_running()) return;
      Sleep(50);
    }
    std::cerr << "Warning: game did not close; terminated" << std::endl;
    TerminateProcess(process_info.hProcess,-1);
    return_code = ErrorCodes::TIMED_OUT;
  }

  void wait() final {
    Sleep(1000);
  }
  int get_return() final {
    return return_code;
  }
  Win32_TestHarness(PROCESS_INFORMATION game_process_info, HWND game_window,
                  const TestConfig &tc):
      process_info(game_process_info), window_handle(game_window), test_config(tc) {}
  ~Win32_TestHarness() {
    if (game_is_running()) {
      TerminateProcess(process_info.hProcess,-1);
      std::cerr << "Game still running; killed" << std::endl;
    }
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
    gather_coverage(test_config);
  }

private:
  PROCESS_INFORMATION process_info;
  HWND window_handle;
  bool run_lcov;
  TestConfig test_config;
  int return_code = 0x10000;
};

constexpr const char *kDefaultExtensions =
    "Paths,DateTime,DataStructures,MotionPlanning,Alarms,Timelines,"
    "ParticleSystems";

int build_game(const string &game, const TestConfig &tc, const string &out) {
  // Invoke the compiler via emake
  using TC = TestConfig;
  string emake_cmd = "emake.exe";
  string compiler = " --compiler=" + tc.get_or(&TC::compiler, "TestHarness");
  string mode = " --mode=" + tc.get_or(&TC::mode, "Debug");
  string platform = " --platform=" + tc.get_or(&TC::platform, "Win32");
  string graphics = " --graphics=" + tc.get_or(&TC::graphics, "OpenGL1");
  string audio = " --audio=" + tc.get_or(&TC::audio, "OpenAL");
  string widgets = " --widgets=" + tc.get_or(&TC::widgets, "None");
  string network = " --network=" + tc.get_or(&TC::network, "None");
  string collision = " --collision=" + tc.get_or(&TC::collision, "Precise");
  string extensions = " --extensions="
      + tc.get_or(&TC::extensions, kDefaultExtensions);

  string args = emake_cmd + compiler + mode + platform + graphics + audio + widgets + network + collision + extensions +
                " \"" + game + "\" -o \"" + out + "\" ";

  ProcessData emakeProcess;

  if(!CreateProcess(emake_cmd.c_str(),&args[0],NULL,NULL,FALSE,0,NULL,NULL,&emakeProcess.si,&emakeProcess.pi)) return -1;

  bool ret=(WaitForSingleObject(emakeProcess.pi.hProcess,INFINITE)==(DWORD)0xFFFFFFFF);
  //system("./share_logs.sh");
  if(ret) {
    return -1;
  }

  unsigned long int exitcode;
  GetExitCodeProcess(emakeProcess.pi.hProcess,&exitcode);
  if(exitcode!=259) {
    return exitcode;
  }
  return -1;
}

void gather_coverage(const TestConfig &config) {
  static int test_num = 0;
  test_num++;

  if (!config_supports_lcov(config)) {
    return;
  }

  //char localappdata_path[MAX_PATH];
  //GetEnvironmentVariable("LOCALAPPDATA",localappdata_path,MAX_PATH);

  string src_dir = "--directory=$LOCALAPPDATA/ENIGMA/.eobjs/Windows/Windows/TestHarness/"
                 + config.get_or(&TestConfig::mode, "Debug") + "/";
  string out_file = "--output-file=coverage_" + to_string(test_num) + ".info";

  char shellpath[MAX_PATH]; 
  GetEnvironmentVariable( "SHELL", shellpath, MAX_PATH);

  string lcovArgs =
    string(shellpath)+
    " -l -c"
    " \"lcov"
    " --quiet"
    " --no-external"
    " --base-directory=ENIGMAsystem/SHELL/"
    " --capture "+
    src_dir+" "+
    out_file+" \"";

  ProcessData lcovProcess;

  if(!CreateProcess(shellpath,&lcovArgs[0],NULL,NULL,FALSE,0,NULL,NULL,&lcovProcess.si,&lcovProcess.pi)){
    ADD_FAILURE() << "Coverage failed to execute for test " << test_num << '!';
    return;
  }
  if(WaitForSingleObject(lcovProcess.pi.hProcess,INFINITE)==WAIT_FAILED){
    ADD_FAILURE() << "Waiting on coverage report for test " << test_num
                  << " somehow failed...";
    return;
  }
  unsigned long int exitcode;
  if(GetExitCodeProcess(lcovProcess.pi.hProcess,&exitcode)){
    if(exitcode!=259){ //STILL_ACTIVE = 259
        if(exitcode){
            ADD_FAILURE() << "LCOV run for test " << test_num
                          << " exited with status " << exitcode << "...";
            return;
        }
        std::cout << "Coverage completed successfully for test " << test_num
                  << '.' << std::endl;
    }
    return;
  }
  return;
}

}  // namespace

bool TestHarness::windowing_supported() {
  return true;
}

unique_ptr<TestHarness>
TestHarness::launch_and_attach(const string &game, const TestConfig &tc) {
  char tmp_path[MAX_PATH];
  GetEnvironmentVariable("tmp",tmp_path,MAX_PATH);
  string out = string(tmp_path)+"/test-game.exe";
  if (int retcode = build_game(game, tc, out)) {
    if (retcode == -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return nullptr;
  }

  ProcessData lacProcess(true);
  if(!CreateProcess(out.c_str(),&out[0],NULL,NULL,FALSE,0,NULL,NULL,&lacProcess.si,&lacProcess.pi)){
        std::cerr<<"Failed to launch";
        return nullptr;
  }
  Sleep(5000); // Give the window a 5 seconds to load and display.

  for (int i = 0; i < 50; ++i) {  // Try for over ten seconds to grab the window
    HWND win = find_window_by_pid(lacProcess.pi.dwProcessId);
    unsigned long int pid;
    GetWindowThreadProcessId(win,&pid);
    if (pid==lacProcess.pi.dwProcessId){
          return std::unique_ptr<Win32_TestHarness>(
          new Win32_TestHarness(lacProcess.pi, win, tc));
    };
    Sleep(250);
  }
  std::cerr<<"Did not get window";
  return nullptr;
}

constexpr int operator"" _million(unsigned long long x) {
  return x * 1000 * 1000;
}

int TestHarness::run_to_completion(const string &game, const TestConfig &tc) {
  char tmp_path[MAX_PATH];
  GetEnvironmentVariable("tmp",tmp_path,MAX_PATH);
  string out = string(tmp_path)+"/test-game.exe";
  if (int retcode = build_game(game, tc, out)) {
    if (retcode == -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return ErrorCodes::BUILD_FAILED;
  }

  ProcessData rtcProcess;
  char currentdir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH,currentdir);
  SetCurrentDirectory(game.substr(0, game.find_last_of("\\/")).c_str());

  if(!CreateProcess(out.c_str(),&out[0],NULL,NULL,FALSE,0,NULL,NULL,&rtcProcess.si,&rtcProcess.pi)){
    return ErrorCodes::LAUNCH_FAILED;
  }
  SetCurrentDirectory(currentdir);

  for (int i = 0; i < 30000000; i += 12500) {
    unsigned long int wr = WaitForSingleObject(rtcProcess.pi.hProcess,0),exitcode;
    GetExitCodeProcess(rtcProcess.pi.hProcess,&exitcode);
    if (exitcode!=259) {
      if (wr != WAIT_FAILED) {
        if (exitcode==0) {
          gather_coverage(tc);
          return 0;
        }
        return ErrorCodes::GAME_CRASHED;
      }
      // Ignore the error for now...
      std::cerr << "Warning: ignoring waitpid being dumb." << std::endl;
    }
    Sleep(12);
  }
  std::cerr << "ERROR: game still running after 30 seconds; killed" << std::endl;
  TerminateProcess(rtcProcess.pi.hProcess,-1); // We're not dicking around with this.
  return ErrorCodes::TIMED_OUT;
}
