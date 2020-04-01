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
