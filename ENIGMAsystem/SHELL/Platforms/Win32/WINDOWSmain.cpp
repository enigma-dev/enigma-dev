/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#define NOMINMAX // for windows.h because we use std::min/max
#include "WINDOWSmain.h"

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/platforms_mandatory.h"

#include "Universal_System/mathnc.h" // enigma_user::clamp
#include "Universal_System/estring.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/var4.h"

#include <mmsystem.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>
using std::string;
using std::vector;

using enigma_user::file_exists;
using enigma_user::directory_exists;

namespace enigma_user {

const int os_type = os_windows;
}  // namespace enigma_user

namespace enigma
{

HINSTANCE hInstance;
HWND hWnd;
HDC window_hDC;
HANDLE mainthread;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void (*touch_extension_register)(HWND hWnd);

void windowsystem_write_exename(char *exenamehere) { GetModuleFileName(NULL, exenamehere, 1024); }

void Sleep(int ms) { ::Sleep(ms); }

void initInput(){};

} // namespace enigma

static inline string add_slash(const string& dir) {
  if (dir.empty() || dir.back() != '\\') return dir + '\\';
  return dir;
}

namespace enigma_user {

bool set_working_directory(string dname) {
  tstring tstr_dname = widen(dname);
  replace(tstr_dname.begin(), tstr_dname.end(), '/', '\\');
  if (SetCurrentDirectoryW(tstr_dname.c_str()) != 0) {
    WCHAR wstr_buffer[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, wstr_buffer) != 0) {
      working_directory = add_slash(shorten(wstr_buffer));
      return true;
    }
  }

  return false;
}

} // enigma_user

namespace enigma {
bool use_pc;
// Filetime.
ULARGE_INTEGER time_offset_ft;
ULARGE_INTEGER time_offset_slowing_ft;
ULARGE_INTEGER time_current_ft;
// High-resolution performance counter.
LARGE_INTEGER time_offset_pc;
LARGE_INTEGER time_offset_slowing_pc;
LARGE_INTEGER time_current_pc;
LARGE_INTEGER frequency_pc;
// Timing functions.

void initialize_timing() {
  use_pc = QueryPerformanceFrequency(&frequency_pc);
  if (use_pc) {
    QueryPerformanceCounter(&time_offset_pc);
    time_offset_slowing_pc.QuadPart = time_offset_pc.QuadPart;
  } else {
    FILETIME time_values;
    GetSystemTimeAsFileTime(&time_values);
    time_offset_ft.LowPart = time_values.dwLowDateTime;
    time_offset_ft.HighPart = time_values.dwHighDateTime;
    time_offset_slowing_ft.QuadPart = time_offset_ft.QuadPart;
  }
}
void update_current_time() {
  if (use_pc) {
    QueryPerformanceCounter(&time_current_pc);
  } else {
    FILETIME time_values;
    GetSystemTimeAsFileTime(&time_values);
    time_current_ft.LowPart = time_values.dwLowDateTime;
    time_current_ft.HighPart = time_values.dwHighDateTime;
  }
}
long get_current_offset_difference_mcs() {
  if (use_pc) {
    return enigma_user::clamp((time_current_pc.QuadPart - time_offset_pc.QuadPart) * 1000000 / frequency_pc.QuadPart, 0, 1000000);
  } else {
    return enigma_user::clamp((time_current_ft.QuadPart - time_offset_ft.QuadPart) / 10, 0, 1000000);
  }
}
long get_current_offset_slowing_difference_mcs() {
  if (use_pc) {
    return enigma_user::clamp((time_current_pc.QuadPart - time_offset_slowing_pc.QuadPart) * 1000000 / frequency_pc.QuadPart, 0,
                 1000000);
  } else {
    return enigma_user::clamp((time_current_ft.QuadPart - time_offset_slowing_ft.QuadPart) / 10, 0, 1000000);
  }
}
void increase_offset_slowing(long increase_mcs) {
  if (use_pc) {
    time_offset_slowing_pc.QuadPart += frequency_pc.QuadPart * increase_mcs / 1000000;
  } else {
    time_offset_slowing_ft.QuadPart += 10 * increase_mcs;
  }
}
void offset_modulus_one_second() {
  if (use_pc) {
    long passed_mcs = get_current_offset_difference_mcs();
    time_offset_pc.QuadPart += (passed_mcs / 1000000) * frequency_pc.QuadPart;
    time_offset_slowing_pc.QuadPart = time_offset_pc.QuadPart;
  } else {
    long passed_mcs = get_current_offset_difference_mcs();
    time_offset_ft.QuadPart += (passed_mcs / 1000000) * 10000000;
    time_offset_slowing_ft.QuadPart = time_offset_ft.QuadPart;
  }
}

static LONG_PTR ComputeInitialWindowStyle() {
  LONG_PTR newlong = 0;

  if (showIcons) newlong |= WS_SYSMENU;

  if (isFullScreen) {
    newlong |= WS_POPUP;
  } else if (enigma::showBorder) {
    newlong |= WS_CAPTION | WS_MINIMIZEBOX;
    if (isSizeable) newlong |= WS_SIZEBOX | WS_MAXIMIZEBOX;
  }

  // these two flags are necessary for extensions like Ultimate3D and GMOgre to render on top of the window
  return newlong | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

WNDCLASS wcontainer;
HGLRC hRC;
MSG msg;

bool initGameWindow() {
  int wid = (int)enigma_user::room_width, hgt = (int)enigma_user::room_height;
  if (!wid || !hgt) wid = 640, hgt = 480;
  enigma::mainthread = GetCurrentThread();

  //Register window class
  wcontainer.style = CS_OWNDC;
  wcontainer.lpfnWndProc = enigma::WndProc;
  wcontainer.cbClsExtra = 0;
  wcontainer.cbWndExtra = 0;
  wcontainer.hInstance = hInstance;
  wcontainer.hIcon = LoadIcon(hInstance, "IDI_MAIN_ICON");
  wcontainer.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcontainer.hbrBackground = (HBRUSH)CreateSolidBrush(enigma::windowColor);
  wcontainer.lpszMenuName = NULL;
  wcontainer.lpszClassName = "EnigmaDevGameMainWindow";
  RegisterClass(&wcontainer);

  //Create the parent window
  int screen_width = GetSystemMetrics(SM_CXSCREEN);
  int screen_height = GetSystemMetrics(SM_CYSCREEN);
  // By default if the room is too big instead of creating a gigantic ass window
  // make it not bigger than the screen to full screen it, this is what 8.1 and Studio
  // do, if the user wants to manually override this they can using
  // views/screen_set_viewport or window_set_size/window_set_region_size
  // We won't limit those functions like GM, just the default.
  if (wid > screen_width) wid = screen_width;
  if (hgt > screen_height) hgt = screen_height;
  // Create the window initially without the WS_VISIBLE flag until we've loaded all of the resources.
  // This will be handled by game_start in roomsystem.cpp where window_default(true) sets the initial
  // fullscreen state of the window before showing it.
  enigma::hWnd = CreateWindow("EnigmaDevGameMainWindow", "", ComputeInitialWindowStyle(), (screen_width - wid) / 2,
                              (screen_height - hgt) / 2, wid, hgt, NULL, NULL, hInstance, NULL);

  if (enigma::touch_extension_register != NULL) {
    enigma::touch_extension_register(enigma::hWnd);
  }

  return true;
}

long last_mcs = 0;
long spent_mcs = 0;
long remaining_mcs = 0;
long needed_mcs = 0;

void initTimer() {
  UINT minimum_resolution = 1;
  TIMECAPS timer_resolution_info;
  if (timeGetDevCaps(&timer_resolution_info, sizeof(timer_resolution_info)) == MMSYSERR_NOERROR) {
    minimum_resolution = timer_resolution_info.wPeriodMin;
  }
  timeBeginPeriod(minimum_resolution);
  enigma::initialize_timing();
}

int updateTimer() {
  // Update current time.
  update_current_time();
  {
    // Find diff between current and offset.

    long passed_mcs = enigma::get_current_offset_difference_mcs();
    if (passed_mcs >= 1000000) {  // Handle resetting.
      // If more than one second has passed, update fps variable, reset frames count,
      // and advance offset by difference in seconds, rounded down.

      enigma_user::fps = frames_count;
      frames_count = 0;
      enigma::offset_modulus_one_second();
    }
  }

  if (current_room_speed > 0) {
    spent_mcs = enigma::get_current_offset_slowing_difference_mcs();

    remaining_mcs = 1000000 - spent_mcs;
    needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
    const int catchup_limit_ms = 50;
    if (needed_mcs > remaining_mcs + catchup_limit_ms * 1000) {
      // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
      // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
      // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
      // without any sleep.
      // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
      enigma::increase_offset_slowing(needed_mcs - (remaining_mcs + catchup_limit_ms * 1000));

      spent_mcs = enigma::get_current_offset_slowing_difference_mcs();
      remaining_mcs = 1000000 - spent_mcs;
      needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
    }
    if (remaining_mcs > needed_mcs) {
      const long sleeping_time = std::min((remaining_mcs - needed_mcs) / 5, long(999999));
      std::this_thread::sleep_for(std::chrono::microseconds(std::max(long(1), sleeping_time)));
      return -1;
    }
  }

  //TODO: The placement of this code is inconsistent with XLIB because events are handled before, ask Josh.
  unsigned long dt = 0;
  if (spent_mcs > last_mcs) {
    dt = (spent_mcs - last_mcs);
  } else {
    //TODO: figure out what to do here this happens when the fps is reached and the timers start over
    dt = enigma_user::delta_time;
  }
  last_mcs = spent_mcs;
  enigma_user::delta_time = dt;
  current_time_mcs += enigma_user::delta_time;
  enigma_user::current_time += enigma_user::delta_time / 1000;

  return 0;
}

int handleEvents() {
  if (enigma::game_isending) PostQuitMessage(enigma::game_return);

  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    if (msg.message == WM_QUIT) {
      enigma::game_return = msg.wParam;
      return 1;
    } else {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return 0;
}

void handleInput() { input_push(); }

void destroyWindow() { DestroyWindow(enigma::hWnd); }

void initialize_directory_globals() {
  // Set the working_directory
  WCHAR buffer[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, buffer);
  enigma_user::working_directory = add_slash(shorten(buffer));

  // Set the program_directory
  buffer[0] = 0;
  GetModuleFileNameW(NULL, buffer, MAX_PATH);
  enigma_user::program_directory = shorten(buffer);
  enigma_user::program_directory = enigma_user::filename_path(enigma_user::program_directory);

  // Set the temp_directory
  buffer[0] = 0;
  GetTempPathW(MAX_PATH, buffer);
  enigma_user::temp_directory = add_slash(shorten(buffer));
  
  // Set the game_save_id
  enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("LOCALAPPDATA")) + 
    add_slash(std::to_string(enigma_user::game_id));
}

}  // namespace enigma

namespace enigma_user {

unsigned long get_timer() {  // microseconds since the start of the game
  enigma::update_current_time();

  LARGE_INTEGER time;
  if (enigma::use_pc) {
    time.QuadPart = enigma::time_current_pc.QuadPart * 1000000 / enigma::frequency_pc.QuadPart;
  } else {
    time.QuadPart = enigma::time_current_ft.QuadPart / 10;
  }

  return time.QuadPart;
}

unsigned long long disk_size(std::string drive) {
  DWORD sectorsPerCluster, bytesPerSector, totalClusters, freeClusters;

  tstring tstr_drive = widen(drive);
  tstring tstr_colonslash = widen(":\\");
  tstring tstr_empty = widen("");

  if (tstr_drive.length() == 1) tstr_drive += tstr_colonslash;

  if (!GetDiskFreeSpaceW((tstr_drive == tstr_empty) ? NULL : tstr_drive.c_str(), &sectorsPerCluster, &bytesPerSector,
                         &freeClusters, &totalClusters))
    return 0;

  return (unsigned long long)(totalClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector;
}

unsigned long long disk_free(std::string drive) {
  DWORD sectorsPerCluster, bytesPerSector, totalClusters, freeClusters;

  tstring tstr_drive = widen(drive);
  tstring tstr_colonslash = widen(":\\");
  tstring tstr_empty = widen("");

  if (tstr_drive.length() == 1) tstr_drive += tstr_colonslash;

  if (!GetDiskFreeSpaceW((tstr_drive == tstr_empty) ? NULL : tstr_drive.c_str(), &sectorsPerCluster, &bytesPerSector,
                         &freeClusters, &totalClusters))
    return 0;

  return ((unsigned long long)(totalClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector) -
         ((unsigned long long)(freeClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector);
}

void set_program_priority(int value) {
  // Need to add PROCESS_SET_INFORMATION permission to thread's access rights, not sure how

  DWORD priorityValue = NORMAL_PRIORITY_CLASS;
  if (value == -1 || value == -2)
    priorityValue = BELOW_NORMAL_PRIORITY_CLASS;
  else if (value == -3)
    priorityValue = IDLE_PRIORITY_CLASS;
  else if (value == 1)
    priorityValue = ABOVE_NORMAL_PRIORITY_CLASS;
  else if (value == 2)
    priorityValue = HIGH_PRIORITY_CLASS;
  else if (value == 3)
    priorityValue = REALTIME_PRIORITY_CLASS;

  SetPriorityClass(GetCurrentThread(), priorityValue);
}

void execute_shell(std::string fname, std::string args) {
  WCHAR cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  ShellExecuteW(enigma::hWnd, NULL, tstr_fname.c_str(), tstr_args.c_str(), cDir, SW_SHOW);
}

void execute_shell(std::string operation, std::string fname, std::string args) {
  WCHAR cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  tstring tstr_operation = widen(operation);
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  ShellExecuteW(enigma::hWnd, tstr_operation.c_str(), tstr_fname.c_str(), tstr_args.c_str(), cDir, SW_SHOW);
}

std::string execute_shell_for_output(const std::string &command) {
  tstring tstr_command = widen(command);
  wchar_t ctstr_command[32768];
  wcsncpy(ctstr_command, tstr_command.c_str(), 32768);
  BOOL ok = TRUE;
  HANDLE hStdInPipeRead = NULL;
  HANDLE hStdInPipeWrite = NULL;
  HANDLE hStdOutPipeRead = NULL;
  HANDLE hStdOutPipeWrite = NULL;
  SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
  ok = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
  if (ok == FALSE) return "";
  ok = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);
  if (ok == FALSE) return "";
  STARTUPINFOW si = { };
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = hStdOutPipeWrite;
  si.hStdOutput = hStdOutPipeWrite;
  si.hStdInput = hStdInPipeRead;
  PROCESS_INFORMATION pi = { };
  if (CreateProcessW(NULL, ctstr_command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
    while (WaitForSingleObject(pi.hProcess, 5) == WAIT_TIMEOUT) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    char buf[4096] = { };
    DWORD dwRead = 0;
    DWORD dwAvail = 0;
    ok = ReadFile(hStdOutPipeRead, buf, 4095, &dwRead, NULL);
    string str_buf = buf; tstring output = widen(str_buf);
    while (ok == TRUE) {
      buf[dwRead] = '\0';
      OutputDebugStringW(output.c_str());
      _putws(output.c_str());
      ok = ReadFile(hStdOutPipeRead, buf, 4095, &dwRead, NULL);
      str_buf = buf; output += widen(str_buf);
    }
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return shorten(output);
  }
  return "";
}

void execute_program(std::string operation, std::string fname, std::string args, bool wait) {
  SHELLEXECUTEINFOW lpExecInfo;
  tstring tstr_operation = widen(operation);
  tstring tstr_fname = widen(fname);
  tstring tstr_args = widen(args);
  lpExecInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
  lpExecInfo.lpFile = tstr_fname.c_str();
  lpExecInfo.fMask = SEE_MASK_DOENVSUBST | SEE_MASK_NOCLOSEPROCESS;
  lpExecInfo.hwnd = enigma::hWnd;
  lpExecInfo.lpVerb = tstr_operation.c_str();
  lpExecInfo.lpParameters = tstr_args.c_str();
  WCHAR cDir[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, cDir);
  lpExecInfo.lpDirectory = cDir;
  lpExecInfo.nShow = SW_SHOW;
  lpExecInfo.hInstApp = (HINSTANCE)SE_ERR_DDEFAIL;  //WINSHELLAPI BOOL WINAPI result;
  ShellExecuteExW(&lpExecInfo);

  //wait until a file is finished printing
  if (wait && lpExecInfo.hProcess != NULL) {
    while (WaitForSingleObject(lpExecInfo.hProcess, 5) == WAIT_TIMEOUT) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }
  if (lpExecInfo.hProcess != NULL)
    CloseHandle(lpExecInfo.hProcess);
}

void execute_program(std::string fname, std::string args, bool wait) { execute_program("open", fname, args, wait); }

// converts a relative path to absolute if the path exists
std::string filename_absolute(std::string fname) {
  if (string_replace_all(fname, " ", "") == "") fname = ".";
  wchar_t rpath[MAX_PATH];
  tstring tstr_fname = widen(fname);
  tstring result(rpath, GetFullPathNameW(tstr_fname.c_str(), MAX_PATH, rpath, NULL));
  if (directory_exists(shorten(result))) return add_slash(shorten(result));
  if (file_exists(shorten(result))) return shorten(result);
  return "";
}

std::string filename_join(std::string prefix, std::string suffix) {
  return add_slash(prefix) + suffix;
}

std::string environment_get_variable(std::string name) {
  WCHAR buffer[1024];
  tstring tstr_name = widen(name);
  GetEnvironmentVariableW(tstr_name.c_str(), (LPWSTR)&buffer, 1024);
  return shorten(buffer);
}

// deletes the environment variable if set to empty string
bool environment_set_variable(const std::string &name, const std::string &value) {
  tstring tstr_name = widen(name);
  tstring tstr_value = widen(value);
  if (value == "") return (SetEnvironmentVariableW(tstr_name.c_str(), NULL) != 0);
  return (SetEnvironmentVariableW(tstr_name.c_str(), tstr_value.c_str()) != 0);
}

void action_webpage(const std::string &url) {
  tstring tstr_url = widen(url);
  tstring tstr_open = widen("open");
  ShellExecuteW(NULL, tstr_open.c_str(), tstr_url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

}  // namespace enigma_user

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow) {
  // DO NOT move this out of WinMain like a fucking idiot
  // we are setting the hInstance in the enigma namespace
  // to the local one passed as a parameter here so that
  // we can use it for the CreateWindow call and icon
  enigma::hInstance = hInstance;

  int argc = 0;
  std::vector<const char*> argv;
  std::vector<string> shortened;
  if (LPWSTR *_argv = CommandLineToArgvW(GetCommandLineW(), &argc)) {
    for (int i = 0; i < argc; ++i) {
      shortened.push_back(shorten(_argv[i]));
      argv.push_back(shortened[i].c_str());
    }
    LocalFree (_argv);
  }

  //Main loop
  return enigma::enigma_main(argc, const_cast<char**>(argv.data()));
}
