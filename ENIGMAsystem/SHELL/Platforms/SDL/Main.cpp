#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/PFprogdir.h"
#include "Universal_System/estring.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

#include <filesystem>

static inline std::string add_slash(const std::string& dir) {
  #if defined(_WIN32)
  if (!dir.empty() && *dir.rbegin() != '\\') return dir + '\\';
  #else
  if (!dir.empty() && *dir.rbegin() != '/') return dir + '/';
  #endif
  return dir;
}

namespace enigma_user {
  // This function is defined elsewhere for POSIX on the SDL platform
  #if defined(_WIN32)
  std::string environment_get_variable(std::string name) {
    std::string value; 
    DWORD length = 0;
    tstring u8name = widen(name);
    if ((length = GetEnvironmentVariableW(u8name.c_str(), nullptr, 0)) != 0) {
      wchar_t *buffer = new wchar_t[length]();
      if (buffer) {
        if (GetEnvironmentVariableW(u8name.c_str(), buffer, length) != 0) {
          value = shorten(buffer);
        }
        delete[] buffer;
      }
    }
    return value;
  }
  #endif

  bool set_working_directory(std::string dname) {
    std::error_code ec;
    std::filesystem::current_path(dname, ec);
    if (ec.value() == 0) {
      working_directory = add_slash(std::filesystem::current_path(ec).u8string());
      return (ec.value() == 0);
    }
    return false;
  }
} // namespace enigma_user

namespace enigma {
  void initialize_directory_globals() {
    std::error_code ec;
    const char *basepath = __getbasepath();
    const char *execname = __getexecname();
    const char *progname = __getprogname();
    enigma_user::program_directory = basepath ? basepath : ""; // C:/Path/To/Executable.exe, /path/to/executable
    enigma_user::program_pathname  = execname ? execname : ""; // C:/Path/To/, /path/to/
    enigma_user::program_filename  = progname ? progname : ""; // Executable.exe, executable
    enigma_user::working_directory = add_slash(std::filesystem::current_path(ec).u8string());
    enigma_user::temp_directory = add_slash(std::filesystem::temp_directory_path(ec).u8string());
    #if defined(_WIN32)
    std::string localappdata = enigma_user::environment_get_variable("LOCALAPPDATA");
    while (!localappdata.empty() && (*localappdata.rbegin() == '\\' || *localappdata.rbegin() == '/')) 
    { localappdata.pop_back(); } std::filesystem::create_directories(localappdata, ec);
    enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("LOCALAPPDATA")) + 
    add_slash(std::to_string(enigma_user::game_id));
    #else
    std::filesystem::create_directories(add_slash(enigma_user::environment_get_variable("HOME")) + std::string(".config"), ec);
    enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
    std::string(".config/") + add_slash(std::to_string(enigma_user::game_id));
    #endif
  }
} // namespace enigma

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
