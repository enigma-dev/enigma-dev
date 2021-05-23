#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <climits>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>

#include "Platforms/General/PFmain.h"
#include "Universal_System/estring.h"
#include "Universal_System/mathnc.h"

#ifndef _WIN32
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/POSIX/POSIXprogdir.h"
#else
#define byte __windows_byte_workaround
#include <windows.h>
#undef byte
#endif

using std::string;

namespace enigma {

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

#ifndef _WIN32
void initialize_directory_globals() {
  // Set the working_directory
  char buffer[PATH_MAX];
  if (getcwd(buffer, PATH_MAX) != NULL)
    enigma_user::working_directory = add_slash(buffer);

  // Set the program_directory
  initialize_program_directory();

  // Set the temp_directory
  char *env = getenv("TMPDIR");
  enigma_user::temp_directory = env ? add_slash(env) : "/tmp/";
  
  // Set the game_save_id
  enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
    string(".config/") + add_slash(std::to_string(enigma_user::game_id));
}

} // namespace enigma
#else
	
HWND hWnd;
HINSTANCE hInstance;

void Sleep(int ms) { ::Sleep(ms); }
  
void windowsystem_write_exename(char *exenamehere) { GetModuleFileName(NULL, exenamehere, 1024); }

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
}

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

} // namespace enigma

namespace enigma_user {
	
bool set_working_directory(string dname) {
  tstring tstr_dname = widen(dname);
  std::replace(tstr_dname.begin(), tstr_dname.end(), '/', '\\');
  if (SetCurrentDirectoryW(tstr_dname.c_str()) != 0) {
    WCHAR wstr_buffer[MAX_PATH];
    if (GetCurrentDirectoryW(MAX_PATH, wstr_buffer) != 0) {
      working_directory = enigma::add_slash(shorten(wstr_buffer));
      return true;
    }
  }
  
  return false;
}

} // namespace enigma_user

#endif

#ifndef _WIN32
} // namespace enigma
#endif

int main(int argc, char** argv) {
  #ifdef _WIN32
  enigma::hWnd = GetDesktopWindow();
  enigma::hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(nullptr));
  #endif

  return enigma::enigma_main(argc, argv);
}
