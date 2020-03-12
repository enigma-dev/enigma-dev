#include "progdir.h"

namespace enigma {

initialize_program_directory() {
  buffer[0] = 0;
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (count != -1) {
    buffer[count] = 0;
    enigma_user::program_directory = enigma_user::filename_path(buffer);
  }
}

} // namespace enigma
