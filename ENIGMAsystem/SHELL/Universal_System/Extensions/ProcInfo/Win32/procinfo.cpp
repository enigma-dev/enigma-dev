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

#define _WIN32_DCOM

#include <cstddef>
#include <cwchar>

#include "../procinfo.h"

#include <windows.h>
#include <Objbase.h>
#include <wbemidl.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <process.h>
#include <psapi.h>

#ifdef _MSC_VER
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "wbemuuid.lib")
#endif

using std::string;
using std::to_string;
using std::wstring;
using std::vector;
using std::size_t;

static inline wstring widen(string str) {
  size_t wchar_count = str.size() + 1;
  vector<wchar_t> buf(wchar_count);
  return wstring { buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
}

static inline string narrow(wstring wstr) {
  int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), NULL, 0, NULL, NULL);
  vector<char> buf(nbytes);
  return string { buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, NULL, NULL) };
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

static inline bool HasReadAccess(HANDLE hProcess, void *pAddress, SIZE_T &nSize) {
  MEMORY_BASIC_INFORMATION memInfo;
  try {
    VirtualQueryEx(hProcess, pAddress, &memInfo, sizeof(memInfo));
    if (PAGE_NOACCESS == memInfo.Protect || PAGE_EXECUTE == memInfo.Protect) {
      nSize = 0;
      return false;
    }
    nSize = memInfo.RegionSize;
    return true;
  } catch(...) { }
  return false;
}

static inline wchar_t *GetEnvironmentStringsW(HANDLE hproc) {
  HMODULE p_ntdll = GetModuleHandleW(L"ntdll.dll");
  typedef NTSTATUS (__stdcall *tfn_qip )(HANDLE, PROCESSINFOCLASS, void *, ULONG, PULONG);
  tfn_qip pfn_qip = tfn_qip(GetProcAddress(p_ntdll, "NtQueryInformationProcess"));
  PROCESS_BASIC_INFORMATION pbi;
  ULONG res_len = 0;
  NTSTATUS status = pfn_qip(hproc, ProcessBasicInformation, &pbi, sizeof(pbi), &res_len);
  size_t ppeb = size_t(pbi.PebBaseAddress);
  char peb[sizeof(PEB)];
  SIZE_T read;
  ReadProcessMemory(hproc, pbi.PebBaseAddress, peb, sizeof(peb), &read); 
  enum { OFFSET_PEB = 0x10, OFFSET_X = 0x48 };
  void *ptr = (void *) *(INT_PTR *)(peb + OFFSET_PEB);
  char buffer[OFFSET_X + sizeof(void *)];
  ReadProcessMemory(hproc, ptr, buffer, sizeof(buffer), &read); 
  void *penv = (void *) *(INT_PTR *)(buffer + OFFSET_X);
  SIZE_T nSiZe;
  if (!HasReadAccess(hproc, penv, nSiZe)) { return (wchar_t *)""; }
  wchar_t *env = new wchar_t[nSiZe];
  ReadProcessMemory(hproc, penv, env, nSiZe, &read); 
  return env;
}

static string proc_wids;
static process_t proc_pid;
static inline BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lparam) {
  DWORD dw_pid;
  GetWindowThreadProcessId(hWnd, &dw_pid);
  if (dw_pid == proc_pid) {
    void *voidp_wid = reinterpret_cast<void *>(hWnd);
    proc_wids += to_string(reinterpret_cast<unsigned long long>(voidp_wid));
    proc_wids += "|";
  }
  return TRUE;
}

namespace procinfo {

static process_t prevpid;
static string prevout;

process_t process_execute(string command) {
  process_t pid = 0;
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
  DWORD dwRead = 0;
  DWORD dwAvail = 0;
  if (CreateProcessW(NULL, cwstr_command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
    while (WaitForSingleObject(pi.hProcess, 5) == WAIT_TIMEOUT) {
      PeekNamedPipe(hStdOutPipeRead, NULL, 0, NULL, &dwAvail, NULL);
      if (dwAvail) {
        vector<char> buffer(dwAvail);
        BOOL success = ReadFile(hStdOutPipeRead, &buffer[0], dwAvail, &dwRead, NULL);
        if (success || dwRead) {
          buffer[dwAvail] = 0;
          output.append(buffer.data(), dwAvail);
        }
      }
      MSG msg;
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    PeekNamedPipe(hStdOutPipeRead, NULL, 0, NULL, &dwAvail, NULL);
    if (dwAvail) {
      vector<char> buffer(dwAvail);
      BOOL success = ReadFile(hStdOutPipeRead, &buffer[0], dwAvail, &dwRead, NULL);
      if (success || dwRead) {
        buffer[dwAvail] = 0;
        output.append(buffer.data(), dwAvail);
      }
    }
    pid = pi.dwProcessId;
    prevpid = pid;
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
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

string cmd_from_pid(process_t pid) {
  string cmd;
  HRESULT hr = 0;
  IWbemLocator *WbemLocator  = NULL;
  IWbemServices *WbemServices = NULL;
  IEnumWbemClassObject *EnumWbem  = NULL;
  hr = CoInitializeEx(0, COINIT_MULTITHREADED);
  hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
  hr = CoCreateInstance((REFCLSID)CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&WbemLocator);
  hr = WbemLocator->ConnectServer((BSTR)L"ROOT\\CIMV2", NULL, NULL, NULL, 0, NULL, NULL, &WbemServices);   
  hr = WbemServices->ExecQuery((BSTR)L"WQL", (BSTR)L"SELECT ProcessId,CommandLine FROM Win32_Process", WBEM_FLAG_FORWARD_ONLY, NULL, &EnumWbem);
  if (EnumWbem != NULL) {
    IWbemClassObject *result = NULL;
    ULONG returnedCount = 0;
    while((hr = EnumWbem->Next(WBEM_INFINITE, 1, &result, &returnedCount)) == S_OK) {
      VARIANT ProcessId;
      VARIANT CommandLine;
      hr = result->Get(L"ProcessId", 0, &ProcessId, 0, 0);
      hr = result->Get(L"CommandLine", 0, &CommandLine, 0, 0);            
      if (ProcessId.uintVal == pid && !(CommandLine.vt == VT_NULL))
      cmd = narrow((wchar_t *)CommandLine.bstrVal);
      result->Release();
    }
  }
  EnumWbem->Release();
  WbemServices->Release();
  WbemLocator->Release();
  CoUninitialize();
  return cmd;
}

string env_from_pid(process_t pid) {
  string envs;
  if (pid == 0 || !pid_exists(pid)) { return ""; }
  HANDLE proc = OpenProcessWithDebugPrivilege(pid);
  try { 
    wchar_t *wenvs = GetEnvironmentStringsW(proc);
    size_t i = 1, j = 0;
    string arg = narrow(wenvs);
    do {
      if (string_has_whitespace(arg)) {
        vector<string> envVec = string_split(arg, '=');
        for (const string &env : envVec) {
          if (j == 0) { envs += env; }
          else { envs += "=\"" + string_replace_all(env, "\"", "\\\"") + "\"\n"; }
          j++;
        }
        j = 0;
      } else {
        envs += string(arg) + "\n";
      }
      wenvs += wcslen(wenvs) + 1;
      arg = narrow(wenvs);
      i++;
    } while (*wenvs);
    if (envs.back() == '\n')
      envs.pop_back();
    delete[] wenvs;
  } catch (...) { }
  CloseHandle(proc);
  return envs;
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
  return to_string(window);
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

string pids_from_ppid(process_t ppid) {
  string pids;
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      if (pe.th32ParentProcessID == ppid) {
        pids += to_string(pe.th32ProcessID) + "|";
      }
    } while (Process32Next(hp, &pe));
  }
  if (pids.back() == '|')
    pids.pop_back();
  pids += "\0";
  CloseHandle(hp);
  return pids;
}

string wids_from_pid(process_t pid) {
  proc_wids = "";
  proc_pid = pid;
  EnumWindows(EnumWindowsProc, 0);
  if (proc_wids.back() == '|')
    proc_wids.pop_back();
  return proc_wids;
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

string echo(string expression) {
  string result, command = "cmd /c @echo off & echo " + 
    string_replace_all(expression, "&", "^&");
  process_execute(command);
  return process_evaluate();
}

} // namespace procinfo
