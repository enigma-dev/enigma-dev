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

#include <algorithm>
#include <climits>
#include <cstddef>
#include <cwchar>
#include <thread>
#include <mutex>
#include <map>

#include "Universal_System/Extensions/VideoPlayer/insecure_bullshit.h"

#include "Universal_System/estring.h"

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <Objbase.h>
#include <tlhelp32.h>
#include <psapi.h>

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#endif

using std::string;
using std::to_string;
using std::wstring;
using std::vector;
using std::size_t;

namespace {

HANDLE OpenProcessWithDebugPrivilege(process_t pid) {
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

} // anonymous namespace

namespace enigma_insecure {

static std::map<process_t, process_t> currpid;
static std::map<process_t, process_t> prevpid;

static std::map<process_t, string> currout;
static std::map<process_t, string> prevout;

static std::mutex currout_mutex;

static inline void output_thread(process_t ind, HANDLE handle, string *output) {
  string result;
  DWORD dwRead = 0;
  char buffer[BUFSIZ];
  while (ReadFile(handle, buffer, BUFSIZ, &dwRead, NULL) && dwRead) {
    buffer[dwRead] = '\0';
    result.append(buffer, dwRead);
    *(output) = result;
  }
}

process_t process_execute(process_t ind, string command) {
  DWORD pid = 0;
  string output;
  wstring wstr_command = widen(command);
  wchar_t cwstr_command[32768];
  wcsncpy_s(cwstr_command, 32768, wstr_command.c_str(), 32768);
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
    pid = pi.dwProcessId;
    currpid.insert(std::make_pair(ind, pid));
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    MSG msg;
    HANDLE waitHandles[] = { pi.hProcess, hStdOutPipeRead };
    std::thread outthrd(output_thread, ind, hStdOutPipeRead, &output);
    while (MsgWaitForMultipleObjects(2, waitHandles, false, 5, QS_ALLEVENTS) != WAIT_OBJECT_0) {
      std::lock_guard<std::mutex> guard(currout_mutex);
      currout[ind] = output;
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    outthrd.join();
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
    // file paths need to return with no trailing newlines
    while (output.back() == '\r' || output.back() == '\n')
      output.pop_back();
    prevpid.insert(std::make_pair(ind, pid));
    prevout.insert(std::make_pair(ind, output));
  }
  return pid;
}

process_t process_current(process_t ind) {
  return currpid.find(ind)->second;
}

process_t process_previous(process_t ind) {
  return prevpid.find(ind)->second;
}

string process_output(process_t ind) {
  std::lock_guard<std::mutex> guard(currout_mutex);
  return currout[ind];
}

string process_evaluate(process_t ind) {
  return prevout.find(ind)->second;
}

void process_clear_pid(process_t ind) {
  currpid.erase(ind);
  prevpid.erase(ind);
}

void process_clear_out(process_t ind) {
  std::lock_guard<std::mutex> guard(currout_mutex);
  currout.erase(ind);
  prevout.erase(ind);
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

bool pid_kill(process_t pid) {
  HANDLE hProcess = OpenProcessWithDebugPrivilege(pid);
  if (hProcess == NULL) return false;
  bool result = TerminateProcess(hProcess, 0);
  CloseHandle(hProcess);
  return result;
}

string echo(process_t ind, string expression) {
  string result, command = "cmd /c @echo off & echo " + 
    enigma_user::string_replace_all(expression, "&", "^&");
  process_execute(ind, command);
  return process_evaluate(ind);
}

} // namespace enigma_insecure
