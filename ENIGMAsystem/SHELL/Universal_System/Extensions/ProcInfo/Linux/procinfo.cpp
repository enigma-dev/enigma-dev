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
#include <proc/readproc.h>
#include <cstdlib>
#include <cstring>

using std::string;
using std::to_string;

namespace procinfo {

string path_from_pid(process_t pid) {
  string path;
  string link = string("/proc/") + to_string(pid) + string("/exe");
  char *buffer = realpath(link.c_str(), NULL);
  path = buffer ? : "";
  free(buffer);
  return path;
}

string pids_enum(bool trim_dir, bool trim_empty) {
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));
  PROCTAB *proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
  string pids = "PID\tPPID\t";
  pids += trim_dir ? "NAME\n" : "PATH\n";
  while (readproc(proc, &proc_info) != 0) {
    string exe = trim_dir ? 
      name_from_pid(proc_info.tid) :
      path_from_pid(proc_info.tid);
    if (!trim_empty || !exe.empty()) {
      pids += to_string(proc_info.tid) + "\t";
      pids += to_string(proc_info.ppid) + "\t";
      pids += exe + "\n";
    }
  }
  if (pids.back() == '\n')
    pids.pop_back();
  pids += "\0";
  closeproc(proc);
  return pids;
}

process_t ppid_from_pid(process_t pid) {
  process_t ppid;
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));
  PROCTAB *proc = openproc(PROC_FILLSTATUS | PROC_PID, &pid);
  if (readproc(proc, &proc_info) != 0) { 
    ppid = proc_info.ppid;
  }
  closeproc(proc);
  return ppid;
}

string pids_from_ppid(process_t ppid) {
  string pids;
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));
  PROCTAB *proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
  while (readproc(proc, &proc_info) != 0) {
    if (proc_info.ppid == ppid) {
      pids += to_string(proc_info.tid) + "|";
    }
  }
  if (pids.back() == '|')
    pids.pop_back();
  pids += "\0";
  closeproc(proc);
  return pids;
}

} // namespace procinfo
