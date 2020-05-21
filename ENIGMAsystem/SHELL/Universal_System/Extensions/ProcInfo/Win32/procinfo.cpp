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
#include <windows.h>
#include <tlhelp32.h>
#include <process.h>
#include <psapi.h>
#include <cstddef>
#include <vector>
#include <cwchar>

using std::string;
using std::to_string;
using std::wstring;
using std::vector;
using std::size_t;

static inline wstring widen(string str) {
  size_t wchar_count = str.size() + 1;
  vector<wchar_t> buf(wchar_count);
  return wstring{ buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
}

static inline string narrow(wstring wstr) {
  int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
  vector<char> buf(nbytes);
  return string{ buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, NULL, NULL) };
}

static inline HANDLE OpenProcessWithDebugPrivilege(process_t pid) {
  HANDLE hToken;
  LUID luid;
  TOKEN_PRIVILEGES tkp;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
  LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
  tkp.PrivilegeCount = 1;
  tkp.Privileges[0].Luid = luid;
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
  AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);
  CloseHandle(hToken);
  return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
}

namespace procinfo {

static process_t prevpid;
static string prevout;

process_t process_execute(string command) {
  process_t pid;
  string output;
  wstring wstr_command = widen(command);
  wchar_t cwstr_command[32768];
  wcsncpy(cwstr_command, wstr_command.c_str(), 32768);
  BOOL proceed = TRUE;
  HANDLE hStdInPipeRead = NULL;
  HANDLE hStdInPipeWrite = NULL;
  HANDLE hStdOutPipeRead = NULL;
  HANDLE hStdOutPipeWrite = NULL;
  SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
  proceed = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
  if (proceed == FALSE) return pid;
  proceed = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);
  if (proceed == FALSE) return pid;
  STARTUPINFOW si = { 0 };
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = hStdOutPipeWrite;
  si.hStdOutput = hStdOutPipeWrite;
  si.hStdInput = hStdInPipeRead;
  PROCESS_INFORMATION pi = { 0 };
  if (CreateProcessW(NULL, cwstr_command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
    while (WaitForSingleObject(pi.hProcess, 5) == WAIT_TIMEOUT) {
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    char buffer[4096];
    DWORD dwRead = 0;
    proceed = ReadFile(hStdOutPipeRead, buffer, 4096, &dwRead, NULL);
    while (proceed == TRUE) {
      buffer[dwRead] = 0;
      proceed = ReadFile(hStdOutPipeRead, buffer, 4096, &dwRead, NULL);
    }
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
    pid = pi.dwProcessId;
    prevpid = pid;
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    output = buffer;
    while (output.back() == '\r' || output.back() == '\n')
      output.pop_back();
    prevout = output;
  }
  return pid;
}

process_t process_previous() {
  return prevpid;
}

string process_evaluate() {
  return prevout;
}

void process_clear_pid() {
  prevpid = 0;
}

void process_clear_out() {
  prevout = "";
}

process_t pid_from_self() {
  return _getpid();
}

process_t ppid_from_self() {
  return ppid_from_pid(pid_from_self());
}

string path_from_pid(process_t pid) {
  string path;
  HANDLE hProcess = OpenProcessWithDebugPrivilege(pid);
  wchar_t szFilename[MAX_PATH]; DWORD dwPathSize = MAX_PATH;
  if (QueryFullProcessImageNameW(hProcess, 0, szFilename, &dwPathSize) != 0) {
    path = narrow(szFilename);
  }
  CloseHandle(hProcess);
  return path;
}

bool pid_exists(process_t pid) {
  // Slower than OpenProcess + GetExitCodeProcess approach,
  // but doesn't return true with processes of a wrong pid.
  // OpenProcess will succeed with a specific number within
  // 3 of any existing pid, which returns true incorrectly.
  bool result = false;
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      if (pe.th32ProcessID == pid) {
        result = true;
        break;
      }
    } while (Process32Next(hp, &pe));
  }
  CloseHandle(hp);
  return result;
}

bool wid_exists(wid_t wid) {
  void *voidp_wid = reinterpret_cast<void *>(stoull(wid, nullptr, 10));
  HWND hwnd_wid = reinterpret_cast<HWND>(voidp_wid);
  return (IsWindow(hwnd_wid) && hwnd_wid == GetAncestor(hwnd_wid, GA_ROOT));
}

bool pid_kill(process_t pid) {
  HANDLE hProcess = OpenProcessWithDebugPrivilege(pid);
  if (hProcess == NULL) return false;
  bool result = TerminateProcess(hProcess, 0);
  CloseHandle(hProcess);
  return result;
}

window_t window_from_wid(wid_t wid) {
  return stoull(wid, nullptr, 10);
}

wid_t wid_from_window(window_t window) {
  return to_string(reinterpret_cast<unsigned long long>(window));
}

process_t pid_from_wid(wid_t wid) {
  DWORD dw_pid;
  void *voidp_wid = reinterpret_cast<void *>(stoull(wid, nullptr, 10));
  HWND hwnd_wid = reinterpret_cast<HWND>(voidp_wid);
  GetWindowThreadProcessId(hwnd_wid, &dw_pid);
  return dw_pid;
}


string pids_enum(bool trim_dir, bool trim_empty) {
  string pids = "PID\tPPID\t";
  pids += trim_dir ? "NAME\n" : "PATH\n";
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      string exe = trim_dir ? 
        name_from_pid(pe.th32ProcessID) :
        path_from_pid(pe.th32ProcessID);
      if (!trim_empty || !exe.empty()) {
        pids += to_string(pe.th32ProcessID) + "\t";
        pids += to_string(pe.th32ParentProcessID) + "\t";
        pids += exe + "\n";
      }
    } while (Process32Next(hp, &pe));
  }
  if (pids.back() == '\n')
    pids.pop_back();
  pids += "\0";
  CloseHandle(hp);
  return pids;
}

process_t ppid_from_pid(process_t pid) {
  process_t ppid;
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      if (pe.th32ProcessID == pid) {
        ppid = pe.th32ParentProcessID;
        break;
      }
    } while (Process32Next(hp, &pe));
  }
  CloseHandle(hp);
  return ppid;
}

wid_t wid_from_top() {
  void *voidp_wid = reinterpret_cast<void *>(GetForegroundWindow());
  return to_string(reinterpret_cast<unsigned long long>(voidp_wid));
}

process_t pid_from_top() {
  return pid_from_wid(wid_from_top());
}

void wid_to_top(wid_t wid) {
  DWORD dw_pid; void *voidp_wid = reinterpret_cast<void *>(stoull(wid, nullptr, 10));
  HWND hwnd_wid = reinterpret_cast<HWND>(voidp_wid);
  GetWindowThreadProcessId(hwnd_wid, &dw_pid);
  AllowSetForegroundWindow(dw_pid);
  SetForegroundWindow(hwnd_wid);
}

void wid_set_pwid(wid_t wid, wid_t pwid) {
  void *voidp_wid = reinterpret_cast<void *>(stoull(wid, nullptr, 10));
  void *voidp_pwid = reinterpret_cast<void *>(stoull(pwid, nullptr, 10));
  HWND hwnd_wid = reinterpret_cast<HWND>(voidp_wid);
  LONG_PTR longp_pwid = reinterpret_cast<LONG_PTR>(voidp_pwid);
  SetWindowLongPtr(hwnd_wid, GWLP_HWNDPARENT, longp_pwid);
}

} // namespace procinfo
