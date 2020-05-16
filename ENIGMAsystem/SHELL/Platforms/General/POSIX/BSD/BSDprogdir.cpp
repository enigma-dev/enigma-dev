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
    // CTL_KERN::KERN_PROC::KERN_PROC_PATHNAME(-1 (= getpid()))
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    if (sysctl(mib, 4, NULL, &length, NULL, 0) == 0) {
      path.resize(length, '\0');
      char *buffer = path.data();
      if (sysctl(mib, 4, buffer, &length, NULL, 0) == 0) {
        path = string(buffer) + "\0";
      }
    }
    enigma_user::program_directory = path;
  }

} // namespace enigma
