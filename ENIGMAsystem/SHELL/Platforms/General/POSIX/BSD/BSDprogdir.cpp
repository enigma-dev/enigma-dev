#include "Platforms/General/POSIX/POSIXprogdir.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <cstddef>
#include <string>

using std::string;
using std::size_t;

static const size_t PATH_SIZE = 256;

static inline string program_directory_helper(string path, size_t length) {
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  path.resize(length, '\0');
  char *buffer = path.data();
  size_t cb = length;
  int result = sysctl(mib, 4, buffer, &cb, NULL, 0);
  if (result == -1) {
    length += PATH_SIZE;
    path = program_directory_helper(path, length);
  } else if (result == 0) {
    path = enigma_user::filename_path(buffer) + "\0";
    path.shrink_to_fit();
  }
  return path;
}

namespace enigma {

  void initialize_program_directory() {
    enigma_user::program_directory = program_directory_helper("", PATH_SIZE);
  }

} // namespace enigma
