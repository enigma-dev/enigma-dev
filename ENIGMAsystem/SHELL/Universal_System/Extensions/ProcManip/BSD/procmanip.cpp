#include <sys/user.h>
#include <libutil.h>
#include <string>

using std::string;

static inline void path_and_ppid_from_pid(pid_t pid, char **path, pid_t *ppid) {
  struct kinfo_proc *proc_info = kinfo_getproc(pid);
  if (proc_info) {
    *(path) = proc_info->ki_comm;
    *(ppid) = proc_info->ki_ppid;
  }
  free(proc_info);
}

namespace enigma_user {

string path_from_pid(pid_t pid) {
  char *path; pid_t ppid;
  path_and_ppid_from_pid(pid, &path, &ppid);
  return path;
}

pid_t ppid_from_pid(pid_t pid) {
  char *path; pid_t ppid;
  path_and_ppid_from_pid(pid, &path, &ppid);
  return ppid;
}

}
