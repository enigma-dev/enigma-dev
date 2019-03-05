#include "Platforms/General/PFmain.h"

#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
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
  char buffer[PATH_MAX + 1];
  if (getcwd(buffer, PATH_MAX + 1) != NULL)
    enigma_user::working_directory = add_slash(buffer);

  // Set the program_directory
  buffer[0] = 0;
  char path[PATH_MAX + 1];
  memset(buffer, 0, sizeof(buffer));
  
  pid_t pid = getpid();
  sprintf(path, "/proc/%d/exe", pid);

  if (readlink(path, buffer, PATH_MAX + 1) != -1)
    enigma_user::program_directory = add_slash(dirname(buffer));

  // Set the temp_directory
  char *env = getenv("TMPDIR");

  if (env != NULL)
    enigma_user::temp_directory = add_slash(env);
  else
    enigma_user::temp_directory = "/tmp/";
}

}

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
