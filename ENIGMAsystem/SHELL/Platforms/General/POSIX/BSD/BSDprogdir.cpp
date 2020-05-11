#include "Platforms/General/POSIX/POSIXprogdir.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cstddef>

using std::string;
using std::size_t;
using std::cout;
using std::endl;

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
  }
  return path;
}

namespace enigma {

  void initialize_program_directory() {
    string path;
    size_t length = 256;
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char *buffer = new char[length]();
    size_t cb = sizeof(buffer);
    if (sysctl(mib, 4, buffer, &cb, NULL, 0) == -1) {
      path = program_directory_helper(length);
    } else if (sysctl(mib, 4, buffer, &cb, NULL, 0) == 0) {
      path = enigma_user::filename_path(buffer);
    }
    path.shrink_to_fit();
    enigma_user::program_directory = path;
    delete[] buffer;
  }

} // namespace enigma
