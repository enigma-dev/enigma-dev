#include <cstdlib>
#include <cstddef>
#include <cstdint>

#include "Uniersal_System/Extensions/VideoPlayer/insecure_bullshit.h"

#include "Uniersal_System/estring.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/user.h>
#include <libprocstat.h>

using std::string;
using std::vector;
using std::to_string;
using std::size_t;

namespace enigma_insecure {

string cmd_from_pid(process_t pid) {
  string cmd;
  unsigned cntp;
  if (!pid_exists(pid)) { return ""; }
  struct procstat *proc_stat = procstat_open_sysctl();
  struct kinfo_proc *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, pid, &cntp);
  char **args = procstat_getargv(proc_stat, proc_info, 0);
  if (args != NULL) {
    for (size_t i = 0; args[i] != NULL; i++) {
      if (enigma::string_has_whitespace(args[i])) {
        cmd += "\"" + enigma_user::string_replace_all(args[i], "\"", "\\\"") + "\"";
      } else {
        cmd += args[i];
      }
      if (args[i + 1] != NULL) {
        cmd += " ";
      }
    }
  }
  procstat_freeargv(proc_stat);
  procstat_freeprocs(proc_stat, proc_info);
  procstat_close(proc_stat);
  return cmd;
}

// namespace enigma_insecure
