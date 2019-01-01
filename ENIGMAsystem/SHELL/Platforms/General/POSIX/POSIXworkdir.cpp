#include "Platforms/General/PFmain.h"

#include <sys/stat.h>
#include <unistd.h>
#include <string>

using std::string;

namespace enigma {

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}
  
} // namespace enigma

namespace enigma_user {

bool set_working_directory(string dname) {
  if (!dname.empty()) {
    while (*dname.rbegin() == '/') {
      dname.erase(dname.size() - 1);
    }
  }

  struct stat sb;
  if (stat((char *)dname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
    if (chdir((char *)dname.c_str()) == 0) {
      working_directory = enigma::add_slash(dname);
      return true;
    }
  }

  return false;
}

} // namespace enigma_user
