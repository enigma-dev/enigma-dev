#include "progdir.h"
#include <unistd.h>
#include <libproc.h>

namespace enigma {

initialize_program_directory() {
  int result; pid_t pid = getpid();
  char buffer[PROC_PIDPATHINFO_MAXSIZE];
  result = proc_pidpath (pid, buffer, sizeof(buffer));
  if ( result > 0 )
    enigma_user::program_directory = enigma_user::filename_path(buffer);
}

} // namespace enigma


