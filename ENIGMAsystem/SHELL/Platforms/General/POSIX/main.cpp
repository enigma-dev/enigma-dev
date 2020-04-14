#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Universal_System/estring.h"

#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <string>

using std::string;

namespace enigma {
  
static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}
  
void initialize_directory_globals() {
  // Set the working_directory
  char buffer[PATH_MAX];
  if (getcwd(buffer, PATH_MAX) != NULL)
    enigma_user::working_directory = add_slash(buffer);

  // Set the program_directory
  buffer[0] = 0;
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (count != -1) {
    buffer[count] = 0;
    enigma_user::program_directory = enigma_user::filename_path(buffer);
  }

  // Set the temp_directory
  char *env = getenv("TMPDIR");
  enigma_user::temp_directory = env ? add_slash(env) : "/tmp/";
  
  // Set the game_save_id
  enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
    string(".config/") + add_slash(std::to_string(enigma_user::game_id));
}

}

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
