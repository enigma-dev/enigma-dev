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
#include <sys/proc_info.h>
#include <libproc.h>
#include <cstdint>

typedef void NSWindow;
typedef unsigned long CGWindowID;

using std::string;
using std::to_string;

extern "C" NSWindow *cocoa_window_from_wid(CGWindowID wid);
extern "C" CGWindowID cocoa_wid_from_window(NSWindow *window);
extern "C" bool cocoa_wid_exists(CGWindowID wid);
extern "C" pid_t cocoa_pid_from_wid(CGWindowID wid);
extern "C" unsigned long cocoa_get_wid_or_pid(bool wid);
extern "C" void cocoa_wid_to_top(CGWindowID wid);

namespace procinfo {

string path_from_pid(process_t pid) {
  string path;
  char buffer[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(pid, buffer, sizeof(buffer)) > 0) {
    path = string(buffer) + "\0";
  }
  return path;
}

bool wid_exists(wid_t wid) {
  return cocoa_wid_exists(stoul(wid, nullptr, 10));
}

window_t window_from_wid(wid_t wid) {
  unsigned long ul_wid = stoul(wid, nullptr, 10);
  void *voidp_window = reinterpret_cast<void *>(cocoa_window_from_wid(ul_wid));
  return reinterpret_cast<unsigned long long>(voidp_window);
}

wid_t wid_from_window(window_t window) {
  unsigned long long ull_window = reinterpret_cast<unsigned long long>(window);
  void *voidp_window = reinterpret_cast<void *>(ull_window);
  return to_string(cocoa_wid_from_window(voidp_window));
}

process_t pid_from_wid(wid_t wid) {
  return cocoa_pid_from_wid(stoul(wid, nullptr, 10));
}

string pids_enum(bool trim_dir, bool trim_empty) {
  string pids = "PID\tPPID\t";
  pids += trim_dir ? "NAME\n" : "PATH\n";
  int cntp = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
  process_t proc_info[cntp];
  memset(&proc_info, 0, sizeof(proc_info));
  proc_listpids(PROC_ALL_PIDS, 0, proc_info, sizeof(proc_info));
  for (unsigned i = 0; i < cntp; i++) {
    string exe = trim_dir ?
      name_from_pid(proc_info[i]) :
      path_from_pid(proc_info[i]);
    if (!trim_empty || !exe.empty()) {
      if (proc_info[i] == 0) { continue; }
      pids += to_string(proc_info[i]) + "\t";
      pids += to_string(ppid_from_pid(proc_info[i])) + "\t";
      pids += exe + "\n";
    }
  }
  if (pids.back() == '\n')
    pids.pop_back();
  pids += "\0";
  return pids;
}

process_t ppid_from_pid(process_t pid) {
  process_t ppid;
  proc_bsdinfo proc_info;
  if (proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) > 0) {
    ppid = proc_info.pbi_ppid;
  }
  return ppid;
}

wid_t wid_from_top() {
  return to_string(cocoa_get_wid_or_pid(true));
}

process_t pid_from_top() {
  return cocoa_get_wid_or_pid(false);
}

void wid_to_top(wid_t wid) {
  cocoa_wid_to_top(stoul(wid, nullptr, 10));
}

void wid_set_pwid(wid_t wid, wid_t pwid) {
  // TODO: add implementation...
}

} // namespace procinfo
