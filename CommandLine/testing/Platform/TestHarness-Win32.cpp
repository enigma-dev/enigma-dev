#include "TestHarness.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <thread>
#include <memory>
#include <string>
#include <string_view>
#include <windows.h>

std::vector<TestConfig> GetValidConfigs(bool platforms, bool graphics, bool audio, bool collisions, bool widgets, bool network) {
  std::vector<TestConfig> tcs;
  
  for (std::string_view p : {"Win32", "SDL"} ) {
    // FIXME: glgetteximage is used in opengl-common and is unsupported by gles. This function currently is required to copy surfaces in some tests
    for (std::string_view g : {"OpenGL1", "OpenGL3", "Direct3D9"/*, "OpenGLES2", "OpenGLES3"*/ }) {
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

#define BUFSIZE 4096

void gather_coverage(const TestConfig&);
bool config_supports_lcov(const TestConfig &tc) {
  return tc.compiler.empty() || tc.compiler == "TestHarness";
}

//support functions
//string get_window_caption(HWND win_hnd);
struct hwndPid{                         //Structure to store Process ID and Window handle for use in EnumWindows
  unsigned long pid;
  HWND whandle;
};
void CreateShellProc_Wait(string);
void CaptureStdOut( const HANDLE stdOut) {
  char tmpPath[MAX_PATH];
  GetEnvironmentVariable("tmp",tmpPath,MAX_PATH);
  HANDLE parentStd_Out = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwRead,dwWritten;
  CHAR buf[BUFSIZE];
  BOOL bSuccess = FALSE;
  string savelog = string(tmpPath)+"/enigma_game.log";
  HANDLE game_log = CreateFile(savelog.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
  if (game_log == INVALID_HANDLE_VALUE) {
    std::cerr<<"Failed to create game log."<<std::endl;
    CloseHandle(stdOut);
    return;
  }
  for (;;) {
    bSuccess = ReadFile(stdOut,buf,BUFSIZE,&dwRead,NULL);
    if(!bSuccess || dwRead == 0) break;
    if(!WriteFile(game_log, buf,dwRead,&dwWritten,NULL)) break;
    if(!WriteFile(parentStd_Out, buf,dwRead,&dwWritten,NULL)) break;
  }
  CloseHandle(game_log);
  CloseHandle(stdOut);
  return;
}
class ProcessData{                      // A class to manage process handle closing for normal and harness attached processes seperately
  bool share_log=false;
  string logdir;
  HANDLE childStd_Out_Wr = NULL;
  HANDLE childStd_Out_Rd = NULL;
  string game_name;
  void meminit(bool ispiped){                       //Initialise STARTUPINFO and PROCESS_INFORMATION
    ZeroMemory( &pi, sizeof(pi));
    ZeroMemory( &si, sizeof(si)); 
    si.cb = sizeof(si);
    if (ispiped) {
      SECURITY_ATTRIBUTES sAttr;
      sAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
      sAttr.bInheritHandle = TRUE;
      sAttr.lpSecurityDescriptor = NULL;
      if(!CreatePipe(&childStd_Out_Rd, &childStd_Out_Wr, &sAttr, 0)) {
        std::cerr<<"Failed to create pipe"<<std::endl;
        return;
      }
      if(!SetHandleInformation(childStd_Out_Rd, HANDLE_FLAG_INHERIT, 0)){
        std::cerr<<"Failed to set handle information"<<std::endl;
        return;
      }
      si.hStdError = childStd_Out_Wr;
      si.hStdOutput = childStd_Out_Wr;
      si.dwFlags |= STARTF_USESTDHANDLES;
    }
    return;
  }
  public:
  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  ProcessData() {}
  ProcessData(string arg) : logdir(arg) {}
  ~ProcessData(){                       //If islac == true, then the process handles (hProcess, hThread) needs to be closed explicitly by the harness when it gets destroyed
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    if(share_log){
      CreateShellProc_Wait(("mv \"/tmp/enigma_game.log\" \"test-harness-out/"+logdir+"/enigma_game.log\"").c_str());;
    }
  }
  void CaptureThread(){
    std::thread capThread(CaptureStdOut,childStd_Out_Rd);
    capThread.detach();
  }
  bool CreateGameProc(string out) {
    meminit(true);
    share_log = true;
    if(!CreateProcess(out.c_str(),&out[0],NULL,NULL,TRUE,0,NULL,NULL,&si,&pi))
      return false;
    CloseHandle(childStd_Out_Wr);
    CaptureThread();
    return true;
  }
  bool CreateBuildProc(string emake,string arguments) {
    meminit(false);
    if(!CreateProcess(emake.c_str(),&arguments[0],NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
      return false;
    return true;
  }
  bool CreateShellProc(string arguments) {
    meminit(false);
    char shellPath[MAX_PATH];
    GetEnvironmentVariable( "SHELL", shellPath, MAX_PATH);
    string shell_args = string(shellPath) + " -lc \""+arguments+"\"";
    if(!CreateProcess(shellPath,&shell_args[0],NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
      return false;
    return true;
  }
};
void CreateShellProc_Wait(string arguments) {
  ProcessData shellProc;
  shellProc.CreateShellProc(arguments);
  WaitForSingleObject(shellProc.pi.hProcess,INFINITE);
  return;
}

BOOL CALLBACK EnumWindowCallback(HWND winhnd, LPARAM lparam) {      //Callback function that checks if the PID we have matches the PID of the Top-level window being enumerated by EnumWindows
    (*(hwndPid *)lparam).whandle = winhnd;
    unsigned long int proc_id;

    GetWindowThreadProcessId(winhnd, &proc_id);
    if(proc_id==(*(hwndPid *)lparam).pid) return false;
    return true;  
}

HWND find_window_by_pid(int proc_id) {                              //Find the window, if it exists, by the process ID
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
    ShowWindow(window_handle,SW_MINIMIZE);                          // ShowWindow passes window manipulation messages
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
    PostMessage( window_handle, WM_CLOSE, (WPARAM)nullptr, (LPARAM)nullptr);    // Posts WM_CLOSE to the window and returns without waiting for a reply
  }
  void screen_save(std::string fPath) final {
    RECT clientRect;
    POINT clientAbsCoor;
    GetClientRect(window_handle,&clientRect);
    clientAbsCoor.x=clientRect.left;
    clientAbsCoor.y=clientRect.top;
    ClientToScreen(window_handle,&clientAbsCoor);  // ClientToScreen converts client space coordinates to screen space coordinates, GetClientRect isnt actually necessary as client origin is always at 0,0

    SetWindowPos(window_handle,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); // Locks the game window as the topmost window, so that other windows do not overlap while capturing
    system(("py ./screenshot.py \""+fPath+"\" "+                            // Uses python Pillow module to capture and crop only the client area
              to_string(clientAbsCoor.x)+" "+
              to_string(clientAbsCoor.y)+" "+
              to_string(clientAbsCoor.x+clientRect.right)+" "+
              to_string(clientAbsCoor.y+clientRect.bottom)
              ).c_str());
    return;
  }

  bool game_is_running() final {
    unsigned long int wr = WaitForSingleObject(process_info->pi.hProcess,0) , exitcode;
    GetExitCodeProcess(process_info->pi.hProcess,&exitcode);
    if(exitcode != STILL_ACTIVE && wr != WAIT_TIMEOUT){                                          // If game process is still running then exitcode stays at 259.
      if(exitcode<0xC0000001 || exitcode>0xCFFFFFFF){
        return_code=exitcode; 
        return false;
      }
      return_code=ErrorCodes::GAME_CRASHED;
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
    TerminateProcess(process_info->pi.hProcess,-1);                  // Terminates the game process and makes it return -1.
    return_code = ErrorCodes::TIMED_OUT;
  }

  void wait() final {
    Sleep(1000);
  }
  int get_return() final {
    return return_code;
  }
  Win32_TestHarness(ProcessData* game_process_info, HWND game_window,
                    const TestConfig &tc):
      process_info(game_process_info), window_handle(game_window), test_config(tc) {}
  ~Win32_TestHarness() {
    if (game_is_running()) {
      TerminateProcess(process_info->pi.hProcess,-1);
      std::cerr << "Game still running; killed" << std::endl;
    }
    gather_coverage(test_config);
    delete process_info;
  }

private:
  ProcessData* process_info;
  HWND window_handle;
  bool run_lcov;
  TestConfig test_config;
  int return_code = 0x10000;
};

constexpr const char *kDefaultExtensions =
    "Paths,DateTime,DataStructures,MotionPlanning,Alarms,Timelines,"
    "ParticleSystems";

int build_game(const string &game, const TestConfig &tc, const string &out) {
  TestHarness::configpool += TestHarness::configpool == "" ? tc.stringify() : "," + tc.stringify();
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

  string args = emake_cmd + compiler + mode + platform + graphics + audio + widgets + network + collision + extensions +" -j4"
                " \"" + game + "\" -o \"" + out + "\" ";

  ProcessData emakeProcess;

  if(!emakeProcess.CreateBuildProc(emake_cmd,args)) return -1;
  
  //If Waitfor..Object returns WAIT_FAILED, something was wrong, process handle may not exist
  if(WaitForSingleObject(emakeProcess.pi.hProcess,INFINITE)!=WAIT_OBJECT_0) {
    return -1;
  }
  
  string slogs_arg = "./share_logs.sh " + game.substr(game.find_last_of("\\/")+1)+tc.stringify();
  CreateShellProc_Wait(slogs_arg);
  
  unsigned long int exitcode;
  if(GetExitCodeProcess(emakeProcess.pi.hProcess,&exitcode)) {                                 // TODO: Add checks to GetExitCodeProcess so that we dont deal with bogus exitcodes
    if(exitcode!=STILL_ACTIVE && (exitcode<0xC0000001 || exitcode>0xCFFFFFFF)) {            // If exitcode isnt 259 then emake has returned
      return exitcode;
    }
  }
  return -1;
}

void gather_coverage(const TestConfig &config) {
  SCOPED_TRACE(config.stringify());
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

  string lcovArgs =
    "lcov"
    " --quiet"
    " --no-external"
    " --base-directory=ENIGMAsystem/SHELL/"
    " --capture "+
    src_dir+" "+
    out_file;

  ProcessData lcovProcess;
  unsigned long int exitcode;

  if(!lcovProcess.CreateShellProc(lcovArgs)) {
    ADD_FAILURE() << "Coverage failed to execute for test " << test_num << '!';
    return;
  }
  if(WaitForSingleObject(lcovProcess.pi.hProcess,INFINITE)!=WAIT_OBJECT_0){
    ADD_FAILURE() << "Waiting on coverage report for test " << test_num
                  << " somehow failed...";
    return;
  }
  if(GetExitCodeProcess(lcovProcess.pi.hProcess,&exitcode)){        // Enter block only if GetExitCodeProcess succeeds in fetching a return
    if(exitcode!=STILL_ACTIVE && (exitcode<0xC0000001 || exitcode>0xCFFFFFFF)){ //STILL_ACTIVE = 259
        if(exitcode){
            ADD_FAILURE() << "LCOV run for test " << test_num
                          << " exited with status " << exitcode << "...";
            return;
        }
        std::cout << "Coverage completed successfully for test " << test_num
                  << '.' << std::endl;
    }
    return;              // lcov still running, there was a failure to wait for the process
  }
  return;                // Could not get exit code
}

}  // namespace

bool TestHarness::windowing_supported() {
  return true;
}

unique_ptr<TestHarness>
TestHarness::launch_and_attach(const string &game, const TestConfig &tc) {
  char tmpPath[MAX_PATH];
  GetEnvironmentVariable("tmp",tmpPath,MAX_PATH);
  string out = string(tmpPath)+"/test-game.exe";
  if (int retcode = build_game(game, tc, out)) {
    if (retcode == -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return nullptr;
  }

  ProcessData* lacPprocess =  new ProcessData(game.substr(game.find_last_of("\\/")+1,game.find_last_of("."))+tc.stringify());

  if(!lacPprocess->CreateGameProc(out)){
        return nullptr;
  }
  Sleep(5000); // Give the window 5 seconds to load and display.

  for (int i = 0; i < 50; ++i) {  // Try for over ten seconds to grab the window
    HWND win = find_window_by_pid( (lacPprocess->pi).dwProcessId);
    unsigned long int pid;
    GetWindowThreadProcessId(win,&pid);
    if (pid == (lacPprocess->pi).dwProcessId){
          return std::unique_ptr<Win32_TestHarness>(
          new Win32_TestHarness(lacPprocess, win, tc));
    };
    Sleep(250);
  }
  // game failed to get a window
  return nullptr;
}

constexpr int operator"" _million(unsigned long long x) {
  return x * 1000 * 1000;
}

int TestHarness::run_to_completion(const string &game, const TestConfig &tc) {
  char tmpPath[MAX_PATH];
  GetEnvironmentVariable("tmp",tmpPath,MAX_PATH);                 //Expand msys2 tmp path
  string out = string(tmpPath)+"/test-game.exe";

  

  if (int retcode = build_game(game, tc, out)) {
    if (retcode == -1) {
      std::cerr << "Failed to run emake." << std::endl;
    } else {
      std::cerr << "emake returned " << retcode << "; abort" << std::endl;
    }
    return ErrorCodes::BUILD_FAILED;
  }

  ProcessData rtcProcess(game.substr(game.find_last_of("\\/")+1)+tc.stringify());
  char currentdir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH,currentdir);                        // Store current working directory to switch back to later
  SetCurrentDirectory(game.substr(0, game.find_last_of("\\/")).c_str());              //Set current directory to the game's dir

  if(!rtcProcess.CreateGameProc(out)){
    return ErrorCodes::LAUNCH_FAILED;
  }
  SetCurrentDirectory(currentdir);        // Switch back

  for (int i = 0; i < 30000000; i += 12500) {
    unsigned long int wr = WaitForSingleObject(rtcProcess.pi.hProcess,0) , exitcode;
    GetExitCodeProcess(rtcProcess.pi.hProcess,&exitcode);
    if (exitcode != STILL_ACTIVE && wr != WAIT_TIMEOUT) {                  // 259 denotes that the game has not exited yet
      if (wr == WAIT_OBJECT_0) {
        if (exitcode < 0xC0000001 || exitcode > 0xCFFFFFFF) {
          gather_coverage(tc);
          return exitcode;
        }
        return ErrorCodes::GAME_CRASHED;
      }
      // Ignore the error for now...
      std::cerr << "Warning: Waiting failed." << std::endl;
    }
    Sleep(12);
  }
  std::cerr << "ERROR: game still running after 30 seconds; killed" << std::endl;
  TerminateProcess(rtcProcess.pi.hProcess,-1);    // Terminates process and makes it return -1.
  return ErrorCodes::TIMED_OUT;
}
