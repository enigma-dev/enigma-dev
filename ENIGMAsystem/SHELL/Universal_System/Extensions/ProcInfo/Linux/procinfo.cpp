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

process_t ppid_from_pid(process_t pid) {
  process_t ppid;
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));
  PROCTAB *pt_ptr = openproc(PROC_FILLSTATUS | PROC_PID, &pid);
  if (readproc(pt_ptr, &proc_info) != 0) { 
    ppid = proc_info.ppid;
  }
  closeproc(pt_ptr);
  return ppid;
}

} // namespace procinfo
