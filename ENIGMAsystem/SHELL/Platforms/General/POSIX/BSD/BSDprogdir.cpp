#include "Platforms/General/POSIX/POSIXprogdir.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cstddef>
#include <string>

using std::string;
using std::size_t;

namespace enigma {

  void initialize_program_directory() {
    string path;
    size_t length;
    int mib[4] { 0 };
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    int result = sysctl(mib, 4, NULL, &length, NULL, 0);
    if (result == 0) {
      path.resize(length, '\0');
      char *buffer = path.data();
      result = sysctl(mib, 4, buffer, &length, NULL, 0);
      if (result == 0) {
        path = buffer;
        path.resize(length + 1, '\0');
      }
    }
    if (!path.empty()) {
      if (print) {
        cout << "program_pathname = \"" << path << "\"" << endl;
      }
    }
    enigma_user::program_directory = path;
  }

} // namespace enigma
