#include "Platforms/General/PFmain.h"
#include "Universal_System/estring.h"
#include "progdir.h"
#include <cstdlib>
#include <climits>
#include <unistd.h>
#include <sys/sysctl.h>

namespace enigma {

void initialize_program_directory() {
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  char buffer[PATH_MAX];
  size_t cb = sizeof(buffer);
  sysctl(mib, 4, buffer, &cb, NULL, 0);
  enigma_user::program_directory = enigma_user::filename_path(buffer);
}

} // namespace enigma
