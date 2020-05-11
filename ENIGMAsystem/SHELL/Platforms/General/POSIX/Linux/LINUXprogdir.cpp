#include "Platforms/General/POSIX/POSIXprogdir.h"
#include <cstdlib>

namespace enigma {

  void initialize_program_directory() {
    char *buffer = realpath("/proc/self/exe", NULL);
    path = buffer ? enigma_user::filename_path(buffer) : "";
    free(buffer);
  }

} // namespace enigma
