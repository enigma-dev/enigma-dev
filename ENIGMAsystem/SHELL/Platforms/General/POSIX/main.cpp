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
      working_directory = add_slash(std::filesystem::current_path(ec).u8string());
      return (ec.value() == 0);
    }
    return false;
  }
} // namespace enigma_user

namespace enigma {
  void initialize_directory_globals() {
    std::error_code ec;
    const char *execname = __getexecname();
    enigma_user::program_directory = enigma_user::filename_path(execname ? execname : "");
    enigma_user::working_directory = add_slash(std::filesystem::current_path(ec).u8string());
    enigma_user::temp_directory = add_slash(std::filesystem::temp_directory_path(ec).u8string());
    std::filesystem::create_directories(add_slash(enigma_user::environment_get_variable("HOME")) + std::string(".config"), ec);
    enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
    std::string(".config/") + add_slash(std::to_string(enigma_user::game_id));
  }
} // namespace enigma

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
