#include <sys/user.h>
#include <libutil.h>
#include <string>

using std::string;

namespace enigma_user {

string path_from_pid(pid_t pid) {
  string path;
  struct kinfo_proc *proc_info = kinfo_getproc(pid);
  if (proc_info) {
    path = proc_info->ki_comm;
  }
  free(proc_info);
  return path;
}

pid_t ppid_from_pid(pid_t pid) {
  pid_t ppid;
  struct kinfo_proc *proc_info = kinfo_getproc(pid);
  if (proc_info) {
    ppid = proc_info->ki_ppid;
  }
  free(proc_info);
  return ppid;
}

}
