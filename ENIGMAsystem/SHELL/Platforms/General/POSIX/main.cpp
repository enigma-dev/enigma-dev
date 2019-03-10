#include "Platforms/General/PFmain.h"

#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <stdio.h>
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
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX + 1);
  if (count != -1) {
    buffer[count] = 0;
    enigma_user::program_directory = add_slash(dirname(buffer));
  }

  // Set the temp_directory
  char *env = getenv("TMPDIR");

  if (env != NULL)
    enigma_user::temp_directory = add_slash(env);
  else
    enigma_user::temp_directory = "/tmp/";
}

}

namespace enigma_user {

string shellscript_evaluate(string command, size_t buffer_size) {
  char buffer[buffer_size];
  FILE *file = popen(command.c_str(), "r");
  while (fgets(buffer, buffer_size, file) != NULL) {}
  pclose(file);

  if (buffer[strlen(buffer) - 1] == '\n')
    buffer[strlen(buffer) - 1] = '\0';

  char *result = (char *)malloc(strlen(buffer) + 1);
  strcpy(result, buffer);

  return string(result);
}

} // namespace enigma_user

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
