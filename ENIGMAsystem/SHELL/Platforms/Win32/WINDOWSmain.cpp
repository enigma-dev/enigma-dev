/** Copyright (C) 2008-2011 Josh Ventura
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

#include <time.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <vector>
using std::string;
using std::vector;

#include "WINDOWScallback.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/estring.h"
#include "../General/PFwindow.h"
#include "WINDOWSmain.h"

#include "Platforms/platforms_mandatory.h"

namespace enigma_user {

const int os_type = os_windows;

}

namespace enigma //TODO: Find where this belongs
{
  HINSTANCE hInstance;
  HWND hWnd;
  LRESULT CALLBACK WndProc (HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
  HDC window_hDC;
  HANDLE mainthread;
  extern bool gameWindowFocused, freezeOnLoseFocus;
  unsigned int pausedSteps = 0;

  vector<string> main_argv;
  int main_argc;

  void EnableDrawing (HGLRC *hRC);
  void DisableDrawing (HWND hWnd, HDC hDC, HGLRC hRC);

  void windowsystem_write_exename(char* exenamehere)
  {
    GetModuleFileName(NULL, exenamehere, 1024);
  }
}

void enigma_catchmouse_backend(bool x) {
  if (x) SetCapture(enigma::hWnd); else ReleaseCapture();
}

namespace enigma {
  int initialize_everything();
  int ENIGMA_events();
  int game_ending();
} // TODO: synchronize with XLib by moving these declarations to a platform_includes header in the root.

//TODO: Implement pause events
unsigned long current_time_mcs = 0; // microseconds since the start of the game

namespace enigma_user {
  std::string working_directory = "";
  std::string program_directory = "";
  extern double fps;
  unsigned long current_time = 0; // milliseconds since the start of the game
  unsigned long delta_time = 0; // microseconds since the last step event
}

namespace enigma {
  int current_room_speed;
  int game_return = 0;
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
  void set_room_speed(int rs)
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
    int wid = (int)enigma_user::room_width, hgt = (int)enigma_user::room_height;
    if (!wid || !hgt) wid = 640, hgt = 480;
    enigma::hInstance = hInstance;
    enigma::mainthread = GetCurrentThread();

    // Set the working_directory
    char buffer[MAX_PATH];
    GetCurrentDirectory( MAX_PATH, buffer );
    enigma_user::working_directory = string( buffer );

    // Set the program_directory
    memset(&buffer[0], 0, MAX_PATH);
    GetModuleFileName( NULL, buffer, MAX_PATH );
    enigma_user::program_directory = string( buffer );
    enigma_user::program_directory = enigma_user::program_directory.substr( 0, enigma_user::program_directory.find_last_of( "\\/" ));

    LPWSTR *argv;
    if ((argv = CommandLineToArgvW(GetCommandLineW(), &enigma::main_argc)))
    {
        for (int i = 0; i < enigma::main_argc; i++){
            char buffer[256];
            snprintf( buffer, 256, "%ls", argv[i]); //Maybe we should check and show some warning if the argument was longer than 256
            string param = buffer;
            enigma::main_argv.push_back(param);
        }
        LocalFree(argv);
    }

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
    wcontainer.hIcon = LoadIcon (hInstance, "IDI_MAIN_ICON");
    wcontainer.hCursor = LoadCursor (NULL, IDC_ARROW);
    wcontainer.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wcontainer.lpszMenuName = NULL;
    wcontainer.lpszClassName = "TMain";
    RegisterClass (&wcontainer);
    
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
     enigma::hWnd = CreateWindow ("TMain", "", WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX, (screen_width-wid)/2, (screen_height-hgt)/2, wid, hgt, NULL, NULL, hInstance, NULL);

    enigma::EnableDrawing (&hRC);
    //Do not set the parent window visible until we have initialized the graphics context.
    ShowWindow(enigma::hWnd, iCmdShow);
    enigma::initialize_everything();

    //Main loop

    // Initialize timing.

    UINT minimum_resolution = 1;
    TIMECAPS timer_resolution_info;
    if (timeGetDevCaps(&timer_resolution_info, sizeof(timer_resolution_info)) == MMSYSERR_NOERROR) {
      minimum_resolution = timer_resolution_info.wPeriodMin;
    }
    timeBeginPeriod(minimum_resolution);
    enigma::initialize_timing();
    int frames_count = 0;

    char bQuit = 0;
    long last_mcs = 0;
    long spent_mcs = 0;
    long remaining_mcs = 0;
    long needed_mcs = 0;
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

                enigma_user::fps = frames_count;
                frames_count = 0;
                enigma::offset_modulus_one_second();
            }
        }

        if (current_room_speed > 0) {
            spent_mcs = enigma::get_current_offset_slowing_difference_mcs();

            remaining_mcs = 1000000 - spent_mcs;
            needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
            const int catchup_limit_ms = 50;
            if (needed_mcs > remaining_mcs + catchup_limit_ms*1000) {
              // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
              // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
              // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
              // without any sleep.
              // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
              enigma::increase_offset_slowing(needed_mcs - (remaining_mcs + catchup_limit_ms*1000));

              spent_mcs = enigma::get_current_offset_slowing_difference_mcs();
              remaining_mcs = 1000000 - spent_mcs;
              needed_mcs = long((1.0 - 1.0*frames_count/current_room_speed)*1e6);
            }
            if (remaining_mcs > needed_mcs) {
                const long sleeping_time = std::min((remaining_mcs - needed_mcs)/5, long(999999));
                usleep(std::max(long(1), sleeping_time));
                continue;
            }
        }
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                enigma::game_return = msg.wParam;
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
          if (!enigma::gameWindowFocused && enigma::freezeOnLoseFocus) {
            if (enigma::pausedSteps < 1) {
              enigma::pausedSteps += 1;
            } else {
              usleep(100000); continue;
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

          enigma::ENIGMA_events();
          enigma::input_push();

          frames_count++;
        }
    }

    enigma::game_ending();
    timeEndPeriod(minimum_resolution);
    enigma::DisableDrawing (enigma::hWnd, enigma::window_hDC, hRC);
    DestroyWindow (enigma::hWnd);

    #if BUILDMODE
    buildmode::finishup();
    #endif

    return enigma::game_return;
}

namespace enigma_user
{
unsigned long get_timer() {  // microseconds since the start of the game
    enigma::update_current_time();

    LARGE_INTEGER time;
    if (enigma::use_pc) {
        time.QuadPart = enigma::time_current_pc.QuadPart*1000000/enigma::frequency_pc.QuadPart;
    } else {
    time.QuadPart = enigma::time_current_ft.QuadPart/10;
    }

    return time.QuadPart;
}

int sleep(int millis)
{
  Sleep(millis);
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
  TCHAR cDir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, cDir);
  ShellExecute(enigma::hWnd, NULL, fname.c_str(), args.c_str(), cDir, SW_SHOW);
}

void execute_shell(std::string operation, std::string fname, std::string args)
{
  TCHAR cDir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, cDir);
  ShellExecute(enigma::hWnd, operation.c_str(), fname.c_str(), args.c_str(), cDir, SW_SHOW);
}

void execute_program(std::string operation, std::string fname, std::string args, bool wait)
{
  SHELLEXECUTEINFO lpExecInfo;
  lpExecInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
  lpExecInfo.lpFile = fname.c_str();
  lpExecInfo.fMask=SEE_MASK_DOENVSUBST|SEE_MASK_NOCLOSEPROCESS;
  lpExecInfo.hwnd = enigma::hWnd;
  lpExecInfo.lpVerb = operation.c_str();
  lpExecInfo.lpParameters = args.c_str();
  TCHAR cDir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, cDir);
  lpExecInfo.lpDirectory = cDir;
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

void execute_program(std::string fname, std::string args, bool wait)
{
	execute_program("open", fname, args, wait);
}

std::string environment_get_variable(std::string name)
{
	char buffer[1024];
	GetEnvironmentVariable(name.c_str(), (LPTSTR)&buffer, 1024);

	return buffer;
}

void game_end(int ret) { PostQuitMessage(ret); }

void action_end_game() { game_end(); }

void action_webpage(const std::string &url)
{
	ShellExecute (NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}


}

