/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include "../procinfo.h"
#include <sys/user.h>
#include <sys/sysctl.h>
#include <libutil.h>
#include <cstdlib>
#include <cstddef>

using std::string;
using std::to_string;
using std::size_t;

namespace procinfo {

string path_from_pid(process_t pid) {
  string path;
  size_t length;
  // CTL_KERN::KERN_PROC::KERN_PROC_PATHNAME(pid)
  int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, pid };
  if (sysctl(mib, 4, NULL, &length, NULL, 0) == 0) {
    path.resize(length, '\0');
    char *buffer = path.data();
    if (sysctl(mib, 4, buffer, &length, NULL, 0) == 0) {
      path = string(buffer) + "\0";
    }
  }
  return path;
}

string pids_enum(bool trim_dir, bool trim_empty) {
  int cntp;
  string pids = "PID\tPPID\t";
  pids += trim_dir ? "NAME\n" : "PATH\n";
  struct kinfo_proc *proc_info = kinfo_getallproc(&cntp);
  if (proc_info) {
    for (unsigned i = 0; i < cntp; i++) {
      string exe = trim_dir ? 
        name_from_pid(proc_info[i].ki_tid) :
        path_from_pid(proc_info[i].ki_tid);
      if (!trim_empty || !exe.empty()) {
        pids += to_string(proc_info[i].ki_tid) + "\t";
        pids += to_string(proc_info[i].ki_ppid) + "\t";
        pids += exe + "\n";
      }
    }
  }
  if (pids.back() == '\n')
    pids.pop_back();
  pids += "\0";
  free(proc_info);
  return pids;
}

process_t ppid_from_pid(process_t pid) {
  process_t ppid;
  struct kinfo_proc *proc_info = kinfo_getproc(pid);
  if (proc_info) {
    ppid = proc_info->ki_ppid;
  }
  free(proc_info);
  return ppid;
}

string pids_from_ppid(process_t ppid) {
  string pids; int cntp;
  struct kinfo_proc *proc_info = kinfo_getallproc(&cntp);
  if (proc_info) {
    for (unsigned i = 0; i < cntp; i++) {
      if (proc_info[i].ki_ppid == ppid) {
        pids += to_string(proc_info[i].ki_tid) + "|";
      }
    }
  }
  if (pids.back() == '|')
    pids.pop_back();
  pids += "\0";
  free(proc_info);
  return pids;
}

} // namespace procinfo
