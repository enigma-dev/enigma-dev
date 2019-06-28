#include "Platforms/General/PFmain.h"

#include <limits.h>
#include <unistd.h>
#include <string>

using std::string;

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

namespace enigma_user {

bool set_working_directory(string dname) {
  if (chdir((char *)dname.c_str()) == 0) {
    char buffer[PATH_MAX + 1]; 
    if (getcwd(buffer, PATH_MAX + 1) != NULL) {
      working_directory = add_slash(buffer);
      return true;
    }
  }

  return false;
}

string environment_get_variable(string name) {
  char *env = getenv((char *)name.c_str());
  return env ? env : "";
}

// deletes the environment variable if set to empty string
bool environment_set_variable(const string &name, const string &value) {
  if (value == "") return (unsetenv(name.c_str()) == 0);
  return (setenv(name.c_str(), value.c_str(), 1) == 0);
}

} // namespace enigma_user
