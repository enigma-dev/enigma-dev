#include <cstdlib>
#include <cstddef>
#include <cstdint>

#include "Universal_System/Extensions/VideoPlayer/insecure_bullshit.h"

#include "Universal_System/estring.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/user.h>
#include <libprocstat.h>
#include <libutil.h>

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

string pids_from_ppid(process_t ppid) {
  string pids; int cntp;
  struct kinfo_proc *proc_info = kinfo_getallproc(&cntp);
  if (proc_info) {
    for (size_t i = 0; i < cntp; i++) {
      if (proc_info[i].ki_ppid == ppid) {
        pids += to_string(proc_info[i].ki_pid) + "|";
      }
    }
  }
  if (pids.back() == '|')
    pids.pop_back();
  pids += "\0";
  free(proc_info);
  return pids;
}

// namespace enigma_insecure
