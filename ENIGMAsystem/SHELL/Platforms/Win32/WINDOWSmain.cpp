/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have received a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/
#include <windows.h>
#include <time.h>
#include <string>
#include <sstream>
#include <algorithm>
using std::string;

#include "WINDOWScallback.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/estring.h"
#include "WINDOWSwindow.h"
#include "WINDOWSmain.h"

#include "Platforms/platforms_mandatory.h"

const int os_type = os_windows;

extern unsigned int game_id;
static HKEY registryCurrentRoot = HKEY_CURRENT_USER;

namespace enigma //TODO: Find where this belongs
{
  HINSTANCE hInstance;
  HWND hWndParent;
  HWND hWnd;
  LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
  HDC window_hDC;

  char** main_argv;
  int main_argc;

  void EnableDrawing (HGLRC *hRC);
  void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC);
}

namespace enigma {
  int initialize_everything();
  int ENIGMA_events();
} // TODO: synchronize with XLib by moving these declarations to a platform_includes header in the root.

extern double fps;
namespace enigma {
  int current_room_speed;
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
  long clamp(LONGLONG value, long min, long max)
  {
    if (value < min) {
      return min;
    }
    if (value > max) {
      return max;
    }
    return long(value);
  }
  void sleep_for_framerate(int rs)
  {
    current_room_speed = rs;
  }
  void initialize_timing()
  {
    use_pc = QueryPerformanceFrequency(&frequency_pc);
    if (use_pc) {
      QueryPerformanceCounter(&time_offset_pc);
      time_offset_slowing_pc.QuadPart = time_offset_pc.QuadPart;
    }
    else {
      FILETIME time_values;
      GetSystemTimeAsFileTime(&time_values);
      time_offset_ft.LowPart = time_values.dwLowDateTime;
      time_offset_ft.HighPart = time_values.dwHighDateTime;
      time_offset_slowing_ft.QuadPart = time_offset_ft.QuadPart;
    }
  }
  void update_current_time()
  {
    if (use_pc) {
      QueryPerformanceCounter(&time_current_pc);
    }
    else {
      FILETIME time_values;
      GetSystemTimeAsFileTime(&time_values);
      time_current_ft.LowPart = time_values.dwLowDateTime;
      time_current_ft.HighPart = time_values.dwHighDateTime;
    }
  }
  long get_current_offset_difference_mcs()
  {
    if (use_pc) {
      return clamp((time_current_pc.QuadPart - time_offset_pc.QuadPart)*1000000/frequency_pc.QuadPart, 0, 1000000);
    }
    else {
      return clamp((time_current_ft.QuadPart - time_offset_ft.QuadPart)/10, 0, 1000000);
    }
  }
  long get_current_offset_slowing_difference_mcs()
  {
    if (use_pc) {
      return clamp((time_current_pc.QuadPart - time_offset_slowing_pc.QuadPart)*1000000/frequency_pc.QuadPart, 0, 1000000);
    }
    else {
      return clamp((time_current_ft.QuadPart - time_offset_slowing_ft.QuadPart)/10, 0, 1000000);
    }
  }
  void increase_offset_slowing(long increase_mcs)
  {
    if (use_pc) {
      time_offset_slowing_pc.QuadPart += frequency_pc.QuadPart*increase_mcs/1000000;
    }
    else {
      time_offset_slowing_ft.QuadPart += 10*increase_mcs;
    }
  }
  void offset_modulus_one_second()
  {
    if (use_pc) {
      long passed_mcs = get_current_offset_difference_mcs();
      time_offset_pc.QuadPart += (passed_mcs/1000000)*frequency_pc.QuadPart;
      time_offset_slowing_pc.QuadPart = time_offset_pc.QuadPart;
    }
    else {
      long passed_mcs = get_current_offset_difference_mcs();
      time_offset_ft.QuadPart += (passed_mcs/1000000)*10000000;
      time_offset_slowing_ft.QuadPart = time_offset_ft.QuadPart;
    }
  }
}
#include <cstdio>
#include <mmsystem.h>
int WINAPI WinMain (HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow)
{
    int wid = (int)room_width, hgt = (int)room_height;
    enigma::hInstance = hInstance;
    //enigma::main_argc = argc;
    //enigma::main_argv = argv;

    //Create the window
        WNDCLASS wcontainer,wmain;
        HGLRC hRC;
        MSG msg;

        //Register window class
        wcontainer.style = CS_OWNDC;
        wcontainer.lpfnWndProc = enigma::WndProc;
        wcontainer.cbClsExtra = 0;
        wcontainer.cbWndExtra = 0;
        wcontainer.hInstance = hInstance;
        wcontainer.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wcontainer.hCursor = LoadCursor (NULL, IDC_ARROW);
        wcontainer.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
        wcontainer.lpszMenuName = NULL;
        wcontainer.lpszClassName = "TMain";
        RegisterClass (&wcontainer);

        //Register other window class
        wmain.style = 0;
        wmain.lpfnWndProc = enigma::WndProc;
        wmain.cbClsExtra = 0;
        wmain.cbWndExtra = 0;
        wmain.hInstance = hInstance;
        wmain.hIcon = LoadIcon (NULL, IDI_APPLICATION);
        wmain.hCursor = LoadCursor (NULL, IDC_ARROW);
        wmain.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
        wmain.lpszMenuName = NULL;
        wmain.lpszClassName = "TSub";
        RegisterClass (&wmain);


        //Create the parent window
        int screen_width = GetSystemMetrics(SM_CXSCREEN);
        int screen_height = GetSystemMetrics(SM_CYSCREEN);
        // TODO: Implement minimize button on both windows like GM
         enigma::hWndParent = CreateWindow ("TMain", "", WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_MINIMIZEBOX, (screen_width-wid)/2, (screen_height-hgt)/2, wid, hgt, NULL, NULL, hInstance, NULL);

        //Create a child window to put into that
        enigma::hWnd = CreateWindow ("TSub", NULL, WS_VISIBLE | WS_CHILD,0, 0, wid, hgt,enigma::hWndParent, NULL, hInstance, NULL);

    enigma::EnableDrawing (&hRC);
    enigma::initialize_everything();

    //Main loop

    // Initialize timing.

    UINT minimum_resolution = 1;
    TIMECAPS timer_resolution_info;
    if (timeGetDevCaps(&timer_resolution_info, sizeof(timer_resolution_info)) == MMSYSERR_NOERROR) {
      minimum_resolution = timer_resolution_info.wPeriodMin;
    }
    timeBeginPeriod(minimum_resolution);
    long speed_error_mcs = 0;
    enigma::initialize_timing();
    int frames_count = 0;

      char bQuit=0;
      while (!bQuit)
      {
          using enigma::current_room_speed;
          // Update current time.
          enigma::update_current_time();
          {
              // Find diff between current and offset.
              long passed_mcs = enigma::get_current_offset_difference_mcs();
              if (passed_mcs >= 1000000) { // Handle resetting.
                  // If more than one second has passed, update fps variable, reset frames count,
                  // and advance offset by difference in seconds, rounded down.
                  fps = frames_count;
                  frames_count = 0;
                  enigma::offset_modulus_one_second();
              }
          }

          if (current_room_speed > 0) {
              long spent_mcs = enigma::get_current_offset_slowing_difference_mcs();
              long remaining_mcs = 1000000 - spent_mcs;
              long needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
              const int catchup_limit_ms = 50;
              if (needed_mcs > remaining_mcs + catchup_limit_ms*1000) {
                // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
                // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
                // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
                // without any sleep.
                // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
                enigma::increase_offset_slowing(needed_mcs - (remaining_mcs + catchup_limit_ms*1000));
                long spent_mcs = enigma::get_current_offset_slowing_difference_mcs();
                long remaining_mcs = 1000000 - spent_mcs;
                long needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
              }
              if (remaining_mcs > needed_mcs) {
                  Sleep(1);
                  continue;
              }
          }
          if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
          {
              if (msg.message == WM_QUIT)
              {
                  bQuit=1;
                  break;
              }
              else
              {
                  TranslateMessage (&msg);
                  DispatchMessage (&msg);
              }
          }
          else
          {
              enigma::ENIGMA_events();
              enigma::input_push();

              frames_count++;
          }
      }

    timeEndPeriod(minimum_resolution);
    enigma::DisableDrawing (enigma::hWnd, enigma::window_hDC, hRC);
    DestroyWindow (enigma::hWnd);

    #if BUILDMODE
    buildmode::finishup();
    #endif

    return 0;
}

string parameter_string(int x)
{
  if (x < enigma::main_argc)
  	return enigma::main_argv[x];

  return "";
}

int parameter_count()
{
  return enigma::main_argc;
}

unsigned long long disk_size(std::string drive)
{
	DWORD sectorsPerCluster, bytesPerSector, totalClusters, freeClusters;

	if (drive.length() == 1)
		drive += ":\\";

	if (!GetDiskFreeSpace((drive == "") ? NULL : drive.c_str(), &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters))
		return 0;

	return (unsigned long long)(totalClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector;
}

unsigned long long disk_free(std::string drive)
{
	DWORD sectorsPerCluster, bytesPerSector, totalClusters, freeClusters;

	if (drive.length() == 1)
		drive += ":\\";

	if (!GetDiskFreeSpace((drive == "") ? NULL : drive.c_str(), &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters))
		return 0;

	return ((unsigned long long)(totalClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector) -
				((unsigned long long)(freeClusters * sectorsPerCluster) * (unsigned long long)bytesPerSector);
}

void set_program_priority(int value)
{
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

void execute_shell(std::string fname, std::string args)
{
	ShellExecute(enigma::hWndParent, NULL, fname.c_str(), args.c_str(), get_working_directory().c_str(), SW_SHOW);
}

void execute_program(std::string fname, std::string args, bool wait)
{
    SHELLEXECUTEINFO lpExecInfo;
      lpExecInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
      lpExecInfo.lpFile = fname.c_str();
      lpExecInfo.fMask=SEE_MASK_DOENVSUBST|SEE_MASK_NOCLOSEPROCESS;
      lpExecInfo.hwnd = enigma::hWndParent;
      lpExecInfo.lpVerb = "open";
      lpExecInfo.lpParameters = args.c_str();
      lpExecInfo.lpDirectory = get_working_directory().c_str();
      lpExecInfo.nShow = SW_SHOW;
      lpExecInfo.hInstApp = (HINSTANCE) SE_ERR_DDEFAIL ;   //WINSHELLAPI BOOL WINAPI result;
      ShellExecuteEx(&lpExecInfo);

      //wait until a file is finished printing
      if (wait && lpExecInfo.hProcess != NULL)
      {
        ::WaitForSingleObject(lpExecInfo.hProcess, INFINITE);
        ::CloseHandle(lpExecInfo.hProcess);
      }
}

std::string environment_get_variable(std::string name)
{
	std::string value(1024, '\x00');
	GetEnvironmentVariable((LPCTSTR)name.c_str(), (LPTSTR)value.data(), 1024);

	return value;
}

void registry_write_string(std::string name, std::string str)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Write to registry
	registry_write_string_ext(ss.str(), name, str);
}

void registry_write_real(std::string name, int x)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Write to registry
	registry_write_real_ext(ss.str(), name, x);
}

std::string registry_read_string(std::string name)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Read from registry
	return registry_read_string_ext(ss.str(), name);
}

int registry_read_real(std::string name)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	// Read from registry
	return registry_read_real_ext(ss.str(), name);
}

bool registry_exists(std::string name)
{
	std::stringstream ss;
	ss << "Software\\EnigmaGM\\" << game_id;

	return registry_exists_ext(ss.str(), name);
}

void registry_write_string_ext(std::string key, std::string name, std::string str)
{
	HKEY hKey;

	// Open registry key
	if (RegCreateKeyEx(registryCurrentRoot, key.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		return;

	// Write file and close key
	RegSetValueEx(hKey, name.c_str(), 0, REG_SZ, (LPBYTE)str.c_str(), str.length() + 1);
	RegCloseKey(hKey);
}

void registry_write_real_ext(std::string key, std::string name, int x)
{
	HKEY hKey;

	// Open registry key
	if (RegCreateKeyEx(registryCurrentRoot, key.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		return;

	// Write value and close key
	RegSetValueEx(hKey, name.c_str(), 0, REG_DWORD, (LPBYTE)&x, sizeof(int));
	RegCloseKey(hKey);
}

std::string registry_read_string_ext(std::string key, std::string name)
{
	std::string value(1024, '\x00');
	DWORD type = REG_SZ, len = 1024;
	HKEY hKey;

	// Open registry key
	if (RegOpenKeyEx(registryCurrentRoot, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return "";

	// Read value and close key
	RegQueryValueEx(hKey, (LPCTSTR)name.c_str(), 0, &type, (LPBYTE)value.data(), &len);
	RegCloseKey(hKey);

	return value;
}

int registry_read_real_ext(std::string key, std::string name)
{
	DWORD type = REG_DWORD, len = sizeof(int);
	HKEY hKey;
	int value;

	// Open registry key
	if (RegOpenKeyEx(registryCurrentRoot, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return 0;

	// Read value and close key
	RegQueryValueEx(hKey, (LPCTSTR)name.c_str(), 0, &type, (LPBYTE)&value, &len);
	RegCloseKey(hKey);

	return value;
}

bool registry_exists_ext(std::string key, std::string name)
{
	HKEY hKey;
	bool value;

	// Open registry key
	if (RegOpenKeyEx(registryCurrentRoot, key.c_str(), 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;

	// Read value and close key
	value = RegQueryValueEx(hKey, (LPCTSTR)name.c_str(), 0, NULL, NULL, NULL) != ERROR_FILE_NOT_FOUND;
	RegCloseKey(hKey);

	return value;
}

void registry_set_root(int root)
{
	const HKEY keyLookup[4] = { HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_CLASSES_ROOT, HKEY_USERS };
	if (root >= 0 && root < 4)
		registryCurrentRoot = keyLookup[root];
}

unsigned long long window_handle()
{
    return (unsigned long long)enigma::hWnd;
}
