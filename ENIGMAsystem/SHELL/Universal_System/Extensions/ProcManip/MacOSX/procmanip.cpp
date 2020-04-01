#include <libproc.h>
#include <string>

using std::string;

extern "C" unsigned long cocoa_pid_from_wid(unsigned long wid);

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

pid_t pid_from_wid(string wid) {
  return cocoa_pid_from_wid(stoul(wid, nullptr, 10));
}

}
