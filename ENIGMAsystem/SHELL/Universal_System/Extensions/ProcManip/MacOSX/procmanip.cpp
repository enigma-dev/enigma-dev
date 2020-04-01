#include <libproc.h>
#include <string>

using std::string;

namespace enigma_user {

string path_from_pid(pid_t pid) {
  string path;
  char buffer[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(pid, buffer, sizeof(buffer)) > 0) {
    path = buffer;
  }
  return path;
}

pid_t ppid_from_pid(pid_t pid) {
  pid_t ppid;
  proc_bsdinfo proc_info;
  if (proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) > 0) {
    ppid = proc_info.pbi_ppid;
  }
  return ppid;
}

}
