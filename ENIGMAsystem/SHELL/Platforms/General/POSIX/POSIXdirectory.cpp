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

} // namespace enigma_user
