#include "Platforms/General/POSIX/POSIXprogdir.h"
#include <libproc.h>
#include <unistd.h>

namespace enigma {

  void initialize_program_directory() {
    char exe[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(getpid(), exe, sizeof(exe)) > 0) {
      enigma_user::program_directory = exe;
    }
  }
  
} // namespace enigma
