#include "procmanip.h"
#include <string>

using std::string;

namespace enigma_user {

string path_from_pid(pid_t pid) {
  return proc_manip::path_from_pid(pid);
}

pid_t ppid_from_pid(pid_t pid) {
  return proc_manip::ppid_from_pid(pid);
}

pid_t pid_from_wid(string wid) {
  return proc_manip::pid_from_wid(wid);
}

}
