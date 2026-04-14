#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/PFprogdir.h"
#include "Universal_System/estring.h"

#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <climits>
#include <filesystem>
#include <string>

using std::string;

static inline string add_slash(const string& dir) {
  if (!dir.empty() && *dir.rbegin() != '/') return dir + '/';
  return dir;
}

namespace enigma_user {
  bool set_working_directory(string dname) {
    std::error_code ec;
    std::filesystem::current_path(dname, ec);
    if (ec.value() == 0) {
      working_direcory = add_slash(std::filesystem::current_path(ec).u8string());
      return (ec.value() == 0);
    }
    return false;
  }
} // namespace enigma_user

namespace enigma {
  void initialize_directory_globals() {
    std::error_code ec;
    enigma_user::working_directory = add_slash(std::filesystem::current_path(ec).u8string());
    enigma_user::program_directory = enigma_user::filename_path(get_executable_path());
    enigma_user::temp_directory = add_slash(std::filesystem::temp_directory_path(ec).u8string());
    enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
      string(".config/") + add_slash(std::to_string(enigma_user::game_id));
    std::filesystem::create_directories(enigma_user::game_save_id, ec);
  }
} // namespace enigma

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
