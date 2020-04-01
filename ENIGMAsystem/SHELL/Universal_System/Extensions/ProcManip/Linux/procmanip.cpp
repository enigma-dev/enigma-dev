#include <proc/readproc.h>
#include <unistd.h>
#include <climits>
#include <string>

using std::string;

namespace enigma_user {

string path_from_pid(pid_t pid) {
  string path;
  char buffer[PATH_MAX];
  string link = string("/proc/") + std::to_string(pid) + string("/exe");
  ssize_t count = readlink(link.c_str(), buffer, PATH_MAX);
  if (count != -1) {
    buffer[count] = 0;
    path = buffer;
  }
  return path;
}

pid_t ppid_from_pid(pid_t pid) {
  pid_t ppid;
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));
  PROCTAB *pt_ptr = openproc(PROC_FILLSTATUS | PROC_PID, &pid);
  if (readproc(pt_ptr, &proc_info) != 0) { 
    ppid = proc_info.ppid;
  }
  closeproc(pt_ptr);
  return ppid;
}

}
