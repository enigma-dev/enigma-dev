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

#include <libproc.h>
#include <string>

using std::string;

extern "C" unsigned long cocoa_pid_from_wid(unsigned long wid);

namespace proc_manip {

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

} // namespace proc_manip
