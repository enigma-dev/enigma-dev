#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/POSIX/POSIXprogdir.h"
#include "Universal_System/estring.h"
#include "Universal_System/filesystem.h"

#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <climits>
#include <string>

using std::string;

namespace enigma {
  
static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}
  
void initialize_directory_globals() {
  // Set the working_directory
  enigma_user::working_directory = enigma_user::get_working_directory();

  // Set the program_directory
  initialize_program_directory();

  // Set the temp_directory
  enigma_user::temp_directory = enigma_user::get_temp_directory();
  
  // Set the game_save_id
  enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
    string(".config/") + add_slash(std::to_string(enigma_user::game_id));
}

} // namespace enigma

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
