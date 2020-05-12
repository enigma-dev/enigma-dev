#include "Platforms/General/POSIX/POSIXprogdir.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cstddef>
#include <string>

using std::string;
using std::size_t;

static inline string program_directory_helper(size_t length) {
  string path;
  length++;
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  char *buffer = path.data();
  size_t cb = length;
  if (sysctl(mib, 4, buffer, &cb, NULL, 0) == -1) {
    path = program_directory_helper(length);
  } else if (sysctl(mib, 4, buffer, &cb, NULL, 0) == 0) {
    path = enigma_user::filename_path(buffer);
    path.shrink_to_fit();
  }
  return path;
}

namespace enigma {

  void initialize_program_directory() {
    enigma_user::program_directory = program_directory_helper(256);
  }

} // namespace enigma
