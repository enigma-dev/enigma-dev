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

#include <cstdlib>
#include <cstddef>
#include <cstdint>

#include "../procinfo.h"

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

namespace procinfo {

string path_from_pid(process_t pid) {
  string path;
  size_t length;
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = pid;
  if (sysctl(mib, 4, NULL, &length, NULL, 0) == 0) {
    path.resize(length, '\0');
    char *buffer = path.data();
    if (sysctl(mib, 4, buffer, &length, NULL, 0) == 0) {
      path = string(buffer) + "\0";
    }
  }
  return path;
}

string cmd_from_pid(process_t pid) {
  string cmd;
  if (!pid_exists(pid)) { return ""; }
  struct procstat *proc_stat = procstat_open_sysctl();
  struct kinfo_proc *proc_info = kinfo_getproc(pid);
  char **args = procstat_getargv(proc_stat, proc_info, 0);
  if (args != NULL) {
    for (unsigned i = 0; args[i] != NULL; i++) {
      if (string_has_whitespace(args[i])) {
        cmd += "\"" + string_replace_all(args[i], "\"", "\\\"") + "\"";
      } else {
        cmd += args[i];
      }
      if (args[i + 1] != NULL) {
        cmd += " ";
      }
    }
  }
  free(proc_info);
  free(proc_stat);
  return cmd;
}

string env_from_pid(process_t pid) {
  string env;
  if (!pid_exists(pid)) { return ""; }
  struct procstat *proc_stat = procstat_open_sysctl();
  struct kinfo_proc *proc_info = kinfo_getproc(pid);
  char **envs = procstat_getenvv(proc_stat, proc_info, 0);
  if (envs != NULL) {
    unsigned j = 0;
    for (unsigned i = 0; envs[i] != NULL; i++) {
      if (string_has_whitespace(envs[i]) || string_count_equalssigns(envs[i]) > 1) {
        vector<string> envVec = string_split_by_first_equalssign(envs[i]);
        for (const string &environ : envVec) {
          if (j == 0) { env += environ; }
          else { env += "=\"" + string_replace_all(environ, "\"", "\\\"") + "\""; }
          j++;
        }
        j = 0;
      } else {
        env += envs[i];
      }
      if (envs[i + 1] != NULL) {
        env += "\n";
      }
    }
  }
  free(proc_info);
  free(proc_stat);
  return env;
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
