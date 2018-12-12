#include "Platforms/General/PFmain.h"

#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <cstdlib>
#include <string>

using std::string;

namespace enigma_user {
  double set_working_directory(string dname) {
    if (!dname.empty()) {
      while (*dname.rbegin() == '/') {
        dname.erase(dname.size() - 1);
      }
    }

    struct stat sb;
    if (stat((char *)dname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
      return (chdir((char *)dname.c_str()) == 0);

    return 0;
  }
}  // namespace enigma_user

namespace enigma {
void set_working_directory() {
  // Set the working_directory
  char buffer[PATH_MAX + 1];
  if (getcwd(buffer, PATH_MAX + 1) != NULL)
    enigma_user::working_directory = buffer;

  // Set the program_directory
  buffer[0] = 0;
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX + 1);
  if (count !=  -1)
    enigma_user::program_directory = dirname(buffer) + string("/");

  // Set the temp_directory
  char const *env = getenv("TMPDIR");

  if (env == 0)
    env = "/tmp/";

  enigma_user::temp_directory = env; 
}


int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
