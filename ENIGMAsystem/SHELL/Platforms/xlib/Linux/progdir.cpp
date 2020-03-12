#include "progdir.h"
#include <unistd.h>
#include <climits>

namespace enigma {

void initialize_program_directory() {
  char buffer[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (count != -1) {
    buffer[count] = 0;
    enigma_user::program_directory = enigma_user::filename_path(buffer);
  }
}

} // namespace enigma
