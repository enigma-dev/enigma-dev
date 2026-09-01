#include "PFmain.h"

#include "PFwindow.h"
#include "PFsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/mathnc.h" // enigma_user::clamp

#include "Universal_System/Extensions/Steamworks/steamworks.h"

#include <chrono> // std::chrono::microseconds
#include <thread> // sleep_for

#include "Platforms/General/PFmain.h"
#include "Platforms/General/fileio.h"
#include "Platforms/General/PFfilemanip.h"
#include "Universal_System/estring.h"
#include "exepath/exepath.hpp"

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#include <shlobj.h>
#include <knownfolders.h>
#elif (defined(__APPLE__) && defined(__MACH__))
#include <sysdir.h>
#include <climits>
#include <cstdlib>
#include <cstdio>
#else
#include <climits>
#include <cstdlib>
#include <cstdio>
#endif

#include <filesystem>

#if !defined(os_unknown)
#define os_unknown -1
#endif
#if !defined(os_win32)
#define os_win32 0
#endif
#if !defined(os_win64)
#define os_win64 1
#endif
#if !defined(os_macosx)
#define os_macosx 2
#endif
#if !defined(os_linux)
#define os_linux 3
#endif
#if !defined(os_anroid)
#define os_android 4
#endif
#if !defined(os_freebsd)
#define os_freebsd 5
#endif
#if !defined(os_dragonfly)
#define os_dragonfly 6
#endif
#if (defined(_WIN32) && !defined(_WIN64) && !defined(os_windows))
#define os_windows os_win32
#elif (defined(_WIN32) && defined(_WIN64) && !defined(os_windows))
#define os_windows os_win64
#elif !defined(os_windows)
#define os_windows os_unknown
#endif
#if !defined(os_type)
#if (defined(_WIN32) && !defined(_WIN64))
#define os_type os_win32
#elif (defined(_WIN32) && defined(_WIN64))
#define os_type os_win64
#elif (defined(__APPLE__) && defined(__MACH__))
#define os_type os_macosx
#elif defined(__linux__) && !defined(__ANDROID__)
#define os_type os_linux
#elif defined(__linux__) && defined(__ANDROID__)
#define os_type os_android
#elif defined(__FreeBSD__)
#define os_type os_freebsd
#elif defined(__DragonFly__)
#define os_type os_dragonfly
#else
#define os_type os_unknown
#endif
#endif

namespace enigma_user {

std::string filename_addslash(const std::string& dir) {
  #if (defined(_WIN32) || defined(_WIN64))
  if (!dir.empty() && *dir.rbegin() != '\\') return dir + '\\';
  #else
  if (!dir.empty() && *dir.rbegin() != '/') return dir + '/';
  #endif
  return dir;
}

std::string filename_join(std::string prefix, std::string suffix) {
  return filename_addslash(prefix) + suffix;
}

std::string filename_absolute(std::string fname, bool must_exist) {
  std::string result;
  std::error_code ec;
  if (must_exist) {
    result = std::filesystem::canonical(fname, ec).u8string();
  } else {
    result = std::filesystem::weakly_canonical(fname, ec).u8string();
  }
  if (ec.value() == 0) {
    return result;
  }
  return "";
}

namespace {

std::string directory_get_special_path(int dtype) {
  std::string result;
  #if (defined(_WIN32) || defined(_WIN64))
  wchar_t *ptr = nullptr;
  KNOWNFOLDERID fid;
  switch (dtype) {
    case  0: { fid = FOLDERID_Desktop;   break; }
    case  1: { fid = FOLDERID_Documents; break; }
    case  2: { fid = FOLDERID_Downloads; break; }
    case  3: { fid = FOLDERID_Music;     break; }
    case  4: { fid = FOLDERID_Pictures;  break; }
    case  5: { fid = FOLDERID_Videos;    break; }
    case  6: { fid = FOLDERID_Public;    break; }
    default: { fid = FOLDERID_Desktop;   break; }
  }
  if (SUCCEEDED(SHGetKnownFolderPath(fid, KF_FLAG_CREATE | KF_FLAG_DONT_UNEXPAND, nullptr, &ptr))) {
    result = shorten(ptr); 
    result = filename_addslash(filename_absolute(result));
  }
  CoTaskMemFree(ptr); 
  #elif (defined(__APPLE__) && defined(__MACH__))
  char buf[PATH_MAX];
  sysdir_search_path_directory_t fid;
  sysdir_search_path_enumeration_state state;
  switch (dtype) {
    case  0: { fid = SYSDIR_DIRECTORY_DESKTOP;       break; }
    case  1: { fid = SYSDIR_DIRECTORY_DOCUMENT;      break; }
    case  2: { fid = SYSDIR_DIRECTORY_DOWNLOADS;     break; }
    case  3: { fid = SYSDIR_DIRECTORY_MUSIC;         break; }
    case  4: { fid = SYSDIR_DIRECTORY_PICTURES;      break; }
    case  5: { fid = SYSDIR_DIRECTORY_MOVIES;        break; }
    case  6: { fid = SYSDIR_DIRECTORY_SHARED_PUBLIC; break; }
    default: { fid = SYSDIR_DIRECTORY_DESKTOP;       break; }
  }
  state = sysdir_start_search_path_enumeration(fid, SYSDIR_DOMAIN_MASK_USER);
  while ((state = sysdir_get_next_search_path_enumeration(state, buf))) {
    if (buf[0] == '~') {
      result = buf; 
      result.replace(0, 1, environment_get_variable("HOME"));
      result = filename_addslash(filename_absolute(result));
      break;
    }
  }
  #elif !defined(__ANDROID__)
  std::string fid;
  switch (dtype) {
    case  0: { fid = "XDG_DESKTOP_DIR=";     break; }
    case  1: { fid = "XDG_DOCUMENTS_DIR=";   break; }
    case  2: { fid = "XDG_DOWNLOAD_DIR=";    break; }
    case  3: { fid = "XDG_MUSIC_DIR=";       break; }
    case  4: { fid = "XDG_PICTURES_DIR=";    break; }
    case  5: { fid = "XDG_VIDEOS_DIR=";      break; }
    case  6: { fid = "XDG_PUBLICSHARE_DIR="; break; }
    default: { fid = "XDG_DESKTOP_DIR=";     break; }
  }
  if (filename_absolute(environment_get_variable("HOME")).empty()) return result;
  std::string conf = filename_addslash(filename_absolute(environment_get_variable("HOME"))) + ".config/user-dirs.dirs";
  if (file_exists(conf)) {
    int dirs = file_text_open_read(conf);
    if (dirs != -1) {
      while (!file_text_eof(dirs)) {
        std::string line = file_text_read_string(dirs);
        file_text_readln(dirs);
        size_t pos = line.find(fid, 0);
        if (pos != std::string::npos) {
          FILE *fp = popen(("echo " + line.substr(pos + fid.length())).c_str(), "r");
          if (fp) {
            char buf[PATH_MAX];
            if (fgets(buf, sizeof(buf), fp)) {
              std::string str = buf;
              size_t pos = str.find("\n", strlen(buf) - 1);
              if (pos != std::string::npos) {
                str.replace(pos, 1, "");
              }
              if (!directory_exists(str)) {
                directory_create(str);
              }
              result = filename_addslash(filename_absolute(str));
            }
            pclose(fp);
          }
        }
      }
      file_text_close(dirs);
    }
  }
  #endif
  return result;
}

std::string directory_get_desktop_path() {
  return directory_get_special_path(0);
}

std::string directory_get_documents_path() {
  return directory_get_special_path(1);
}

std::string directory_get_downloads_path() {
  return directory_get_special_path(2);
}

std::string directory_get_music_path() {
  return directory_get_special_path(3);
}

std::string directory_get_pictures_path() {
  return directory_get_special_path(4);
}

std::string directory_get_videos_path() {
  return directory_get_special_path(5);
}

std::string directory_get_public_path() {
  return directory_get_special_path(6);
}

} // anonymous namespace

std::string environment_get_variable(std::string name) {
  #if (defined(_WIN32) || defined(_WIN64))
  std::string value; 
  DWORD length = 0;
  std::wstring u8name = widen(name);
  if ((length = GetEnvironmentVariableW(u8name.c_str(), nullptr, 0)) != 0) {
    wchar_t *buffer = new wchar_t[length]();
    if (GetEnvironmentVariableW(u8name.c_str(), buffer, length) != 0) {
      value = shorten(buffer);
    }
    delete[] buffer;
  }
  return value;
  #else
  char *value = getenv(name.c_str());
  return value ? value : "";
  #endif
}

bool environment_get_variable_exists(std::string name) {
  #if (defined(_WIN32) || defined(_WIN64))
  std::wstring u8name = widen(name);
  return (!(GetEnvironmentVariableW(u8name.c_str(), nullptr, 0) == 0 && 
    GetLastError() == ERROR_ENVVAR_NOT_FOUND));
  #else
  return (getenv(name.c_str()) != nullptr);
  #endif
}

bool environment_set_variable(std::string name, std::string value) {
  #if (defined(_WIN32) || defined(_WIN64))
  std::wstring u8name = widen(name); 
  std::wstring u8value = widen(value);
  return (SetEnvironmentVariableW(u8name.c_str(), u8value.c_str()) != 0);
  #else
  return (setenv(name.c_str(), value.c_str(), 1) == 0);
  #endif
}

bool environment_unset_variable(std::string name) {
  #if (defined(_WIN32) || defined(_WIN64))
  std::wstring u8name = widen(name);
  return (SetEnvironmentVariableW(u8name.c_str(), nullptr) != 0);
  #else
  return (unsetenv(name.c_str()) == 0);
  #endif
}

std::string environment_expand_variables(std::string str) {
  if (str.find("${") == std::string::npos) return str;
  std::string pre = str.substr(0, str.find("${"));
  std::string post = str.substr(str.find("${") + 2);
  if (post.find('}') == std::string::npos) return str;
  std::string variable = post.substr(0, post.find('}'));
  size_t pos = post.find('}') + 1; post = post.substr(pos);
  std::string value = environment_get_variable(variable);
  if (!environment_get_variable_exists(variable))
    return str.substr(0, pos) + environment_expand_variables(str.substr(pos));
  return environment_expand_variables(pre + value + post);
}

bool set_working_directory(std::string dname) {
  std::error_code ec;
  std::filesystem::current_path(dname, ec);
  if (ec.value() == 0) {
    working_directory = filename_addslash(std::filesystem::current_path(ec).u8string());
    return (ec.value() == 0);
  }
  return false;
}

} // namespace enigma_user

namespace enigma {

void initialize_directory_globals() {
  std::error_code ec;
  enigma_user::program_pathname  = exepath::exepath();
  enigma_user::program_directory = ((!enigma_user::program_pathname.empty()) ? enigma_user::filename_path(enigma_user::program_pathname) : "");
  enigma_user::program_filename  = ((!enigma_user::program_pathname.empty()) ? enigma_user::filename_name(enigma_user::program_pathname) : "");
  
  enigma_user::desktop_directory = enigma_user::directory_get_desktop_path();
  enigma_user::documents_directory = enigma_user::directory_get_documents_path();
  enigma_user::downloads_directory = enigma_user::directory_get_downloads_path();
  enigma_user::music_directory = enigma_user::directory_get_music_path();
  enigma_user::pictures_directory = enigma_user::directory_get_pictures_path();
  enigma_user::videos_directory = enigma_user::directory_get_videos_path();
  enigma_user::public_directory = enigma_user::directory_get_public_path();
  
  #if (defined(__APPLE__) && defined(__MACH__))

  /*
    This function will set the working directory to the app bundle's Resources folder 
    like GM4Mac 7.5, GMStudio 1.4, GMS 2.x and most Mac apps do, if the executable is in
    an app bundle. If the executable is not in an app bundle, use unix working directory
    
    ONLY use working_directory for loading read-only included files! When SAVING, use game_save_id
    
    *_bname = base name - removes the full path from the string leaving just the file or folder name
    *_dname = directory name - removes final slash and base name from full path to file or folder name
    *_pname = path name - removes the base name from a full path while keeping the dir and final slash
    *_ename = extension name - includes everything in bname at and following the period if one exists
  */

  const std::string exe_pname = enigma_user::program_directory;                  // = "/Path/To/YourAppBundle.app/Contents/MacOS/";
  const std::string macos_dname = enigma_user::filename_dir(exe_pname);          // = "/Path/To/YourAppBundle.app/Contents/MacOS";
  const std::string macos_bname = enigma_user::filename_name(macos_dname);       // = "MacOS";
  const std::string contents_dname = enigma_user::filename_dir(macos_dname);     // = "/Path/To/YourAppBundle.app/Contents";
  const std::string contents_bname = enigma_user::filename_name(contents_dname); // = "Contents";
  const std::string app_dname = enigma_user::filename_dir(contents_dname);       // = "/Path/To/YourAppBundle.app";
  const std::string app_ename = enigma_user::filename_ext(app_dname);            // = ".app";
  const std::string contents_pname = enigma_user::filename_path(macos_dname);    // = "/Path/To/YourAppBundle.app/Contents/";
  const std::string resources_pname = contents_pname + "Resources/";             // = "/Path/To/YourAppBundle.app/Contents/Resources/";

  // if "/Path/To/YourAppBundle.app/Contents/MacOS/YourExe" and "/Path/To/YourAppBundle.app/Contents/Resources/" exists
  if (macos_bname == "MacOS" && contents_bname == "Contents" && app_ename == ".app" && enigma_user::directory_exists(enigma_user::filename_dir(resources_pname))) {
    // set working directory to "/Path/To/YourAppBundle.app/Contents/Resources/" and allow loading normal included files
    enigma_user::set_working_directory(enigma_user::filename_absolute(enigma_user::filename_dir(resources_pname)));
  }

  #endif
  
  enigma_user::working_directory = enigma_user::filename_addslash(enigma_user::filename_absolute(std::filesystem::current_path(ec).u8string()));
  enigma_user::temp_directory = enigma_user::filename_addslash(enigma_user::filename_absolute(std::filesystem::temp_directory_path(ec).u8string()));
  
  #if (defined(_WIN32) || defined(_WIN64))
  std::string localappdata = enigma_user::filename_absolute(enigma_user::environment_get_variable("LOCALAPPDATA"));
  if (localappdata.empty()) return; while (!localappdata.empty() && (*localappdata.rbegin() == '\\' || *localappdata.rbegin() == '/')) { localappdata.pop_back(); } 
  std::filesystem::create_directories(localappdata, ec); enigma_user::game_save_id = enigma_user::filename_addslash(localappdata) + 
    enigma_user::filename_addslash(std::to_string(enigma_user::game_id));
  #else
  std::string home = enigma_user::filename_addslash(enigma_user::filename_absolute(enigma_user::environment_get_variable("HOME"))); if (home.empty()) return; 
  std::filesystem::create_directories(home + std::string(".config"), ec); enigma_user::game_save_id = home + std::string(".config/") + 
    enigma_user::filename_addslash(std::to_string(enigma_user::game_id));
  #endif
}

std::queue<std::map<std::string, variant>> posted_async_events;

std::mutex posted_async_events_mutex;

std::vector<std::function<void()> > extension_update_hooks;

bool game_isending = false;
int game_return = 0;
int pausedSteps = 0;
int current_room_speed;
std::string* parameters;
int parameterc;
int frames_count = 0;
// Monotic non-wall clock timer is required for accurate frame limiting.
// https://github.com/enigma-dev/enigma-dev/pull/2259
std::chrono::steady_clock::time_point timer_start;
std::chrono::steady_clock::time_point timer_offset;
std::chrono::steady_clock::time_point timer_offset_slowing;
std::chrono::steady_clock::time_point timer_current;
unsigned long current_time_mcs = 0;
bool game_window_focused = true;

void platform_focus_gained() {
  game_window_focused = true;
  pausedSteps = 0;
  input_initialize();
}

void platform_focus_lost() {
  game_window_focused = false;
  for (int i = 0; i < 255; i++) {
    last_keybdstatus[i] = keybdstatus[i];
    keybdstatus[i] = 0;
  }
  for (int i=0; i < 3; i++) {
    last_mousestatus[i] = mousestatus[i];
    mousestatus[i] = 0;
  }
}

int gameWait() {
  if (enigma_user::os_is_paused()) {
    if (pausedSteps < 1) {
      pausedSteps += 1;
    } else {
      std::this_thread::sleep_for(std::chrono::microseconds(100000));
      return -1;
    }
  }

  frames_count++;

  return 0;
}

void set_room_speed(int rs) { current_room_speed = rs; }

void set_program_args(int argc, char** argv) {
  parameters = new std::string[argc];
  parameterc = argc;
  for (int i = 0; i < argc; i++) parameters[i] = argv[i];
}

void initTimer() {
  timer_start = std::chrono::steady_clock::now();
  timer_offset = timer_start;
  timer_offset_slowing = timer_start;
  timer_current = timer_start;
}

void update_current_time() {
  timer_current = std::chrono::steady_clock::now();
}

long get_current_offset_difference_mcs() {
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_current - timer_offset).count();
  return enigma_user::clamp(delta, 0, 1000000);
}

long get_current_offset_slowing_difference_mcs() {
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(timer_current - timer_offset_slowing).count();
  return enigma_user::clamp(delta, 0, 1000000);
}

void increase_offset_slowing(long increase_mcs) {
  timer_offset_slowing += std::chrono::microseconds(increase_mcs);
}

void offset_modulus_one_second() {
  long passed_mcs = get_current_offset_difference_mcs();
  // rounds towards 0
  timer_offset += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(passed_mcs));
  timer_offset_slowing = timer_offset;
}

long last_mcs = 0;
long spent_mcs = 0;
long remaining_mcs = 0;
long needed_mcs = 0;

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

void fireEventsFromQueue() {
  // Acquire lock and release it when out of scope of fireEventsFromQueue().
  std::lock_guard<std::mutex> guard(posted_async_events_mutex);
  while (!posted_async_events.empty()) {
    enigma_user::ds_map_clear(enigma_user::async_load);

    std::map<std::string, variant> event = posted_async_events.front();

    posted_async_events.pop();

    for (auto& [key, value] : event) {
      enigma_user::ds_map_add(enigma_user::async_load, key, value);
    }

    enigma::fireSteamworksEvent();
  }
}

int enigma_main(int argc, char** argv) {
  enigma_user::async_load = enigma_user::ds_map_create();

  // Initialize directory globals
  initialize_directory_globals();
  
  // Copy our parameters
  set_program_args(argc, argv);

  if (!initGameWindow()) {
    DEBUG_MESSAGE("Failed to create game window", MESSAGE_TYPE::M_FATAL_ERROR);
    return -4;
  }

  initTimer();
  initInput();

  EnableDrawing(nullptr);

  // Call ENIGMA system initializers; sprites, audio, and what have you
  initialize_everything();

  while (!game_isending) {

    if (!((std::string)enigma_user::room_caption).empty())
      enigma_user::window_set_caption(enigma_user::room_caption);
    update_mouse_variables();

    if (updateTimer() != 0) continue;
    if (handleEvents() != 0) break;
    if (gameWait() != 0) continue;

    // if any extensions need updated, update them now
    // just before we fire off user events like step
    for (auto update_hook : extension_update_hooks)
      update_hook();

    enigma::fireEventsFromQueue();

    ENIGMA_events();
    handleInput();
  }

  game_ending();
  DisableDrawing(nullptr);
  destroyWindow();
  return game_return;
}

}  //namespace enigma

namespace enigma_user {

int async_load;

const int os_browser = browser_not_a_browser;
std::string working_directory = "";
std::string program_directory = "";
std::string program_pathname = "";
std::string program_filename = "";
std::string desktop_directory = "";
std::string documents_directory = "";
std::string downloads_directory = "";
std::string music_directory = "";
std::string pictures_directory = "";
std::string videos_directory = "";
std::string public_directory = "";
std::string temp_directory = "";
std::string game_save_id = "";
std::string keyboard_string = "";
double fps = 0;
unsigned long delta_time = 0;
unsigned long current_time = 0;

bool os_is_paused() { return !enigma::game_window_focused && enigma::freezeOnLoseFocus; }

std::string parameter_string(int num) { return num < enigma::parameterc ? enigma::parameters[num] : ""; }

int parameter_count() { return enigma::parameterc; }

void sleep(int ms) { enigma::Sleep(ms); }

unsigned long get_timer() {
  enigma::update_current_time();
  return std::chrono::duration_cast<std::chrono::microseconds>(enigma::timer_current - enigma::timer_start).count();
}

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void game_end() {
  enigma::game_isending = true;
}

void action_end_game() { return game_end(); }

}  //namespace enigma_user
