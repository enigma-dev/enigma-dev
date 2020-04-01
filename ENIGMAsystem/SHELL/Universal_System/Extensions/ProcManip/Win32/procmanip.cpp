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

#include "../procmanip.h"
#include <windows.h>
#include <tlhelp32.h>
#include <string>

using std::string;

static inline void path_and_ppid_from_pid(pid_t pid, char **path, pid_t *ppid) {
  HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(h, &pe)) {
    do {
      if (pe.th32ProcessID == pid) {
        *(path) = pe.szExeFile;
        *(ppid) = pe.th32ParentProcessID;
      }
    } while (Process32Next(h, &pe));
  }
  CloseHandle(h);
}

namespace proc_manip {

string path_from_pid(pid_t pid) {
  char *path; pid_t ppid;
  path_and_ppid_from_pid(pid, &path, &ppid);
  return path;
}

pid_t ppid_from_pid(pid_t pid) {
  char *path; pid_t ppid;
  path_and_ppid_from_pid(pid, &path, &ppid);
  return ppid;
}

pid_t pid_from_wid(string wid) {
  DWORD dwProcessId;
  unsigned long long ull_wid = stoull(wid, nullptr, 10);
  void *voidp_wid = reinterpret_cast<void *>(ull_wid);
  HWND hwnd_wid = reinterpret_cast<HWND>(voidp_wid);
  GetWindowThreadProcessId(hwnd_wid, &dwProcessId);
  unsigned long ul_pid = reinterpret_cast<unsigned long>(dwProcessId);
  return reinterpret_cast<pid_t>(ul_pid);
}

} // namespace proc_manip
