/*

 MIT License
 
 Copyright © 2021 Samuel Venable
 Copyright © 2021 Lars Nilsson
 
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

#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>

#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <climits>
#include <cstdio>

#include "crossprocess.h"
#if defined(XPROCESS_WIN32EXE_INCLUDES)
#if defined(_WIN64)
#include "crossprocess32.h"
#else
#include "crossprocess64.h"
#endif
#endif

#if !defined(_WIN32)
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#if defined(_WIN32)
#include <Objbase.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <psapi.h>
#elif (defined(__APPLE__) && defined(__MACH__))
#include <sys/sysctl.h>
#include <sys/proc_info.h>
#include <libproc.h>
#elif (defined(__linux__) && !defined(__ANDROID__))
#include <proc/readproc.h>
#elif defined(__FreeBSD__)
#include <sys/socket.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <sys/queue.h>
#include <sys/user.h>
#include <libprocstat.h>
#include <libutil.h>
#elif defined(__DragonFly__)
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <libutil.h>
#include <kvm.h>
#endif

using CrossProcess::PROCID;
#if defined(XPROCESS_GUIWINDOW_IMPL)
using CrossProcess::WINDOWID;
#endif

namespace {

std::vector<std::string> StringSplitByFirstEqualsSign(std::string str) {
  std::size_t pos = 0;
  std::vector<std::string> vec;
  if ((pos = str.find_first_of("=")) != std::string::npos) {
    vec.push_back(str.substr(0, pos));
    vec.push_back(str.substr(pos + 1));
  }
  return vec;
}

#if defined(_WIN32)
enum MEMTYP {
  MEMCMD,
  MEMENV,
  MEMCWD
};

#if !defined(_MSC_VER)
#pragma pack(push, 8)
#else
#include <pshpack8.h>
#endif

/* CURDIR struct from:
 https://github.com/processhacker/phnt/ 
 CC BY 4.0 licence */

#define CURDIR struct {\
  UNICODE_STRING DosPath;\
  HANDLE Handle;\
}

/* RTL_DRIVE_LETTER_CURDIR struct from:
 https://github.com/processhacker/phnt/ 
 CC BY 4.0 licence */

#define RTL_DRIVE_LETTER_CURDIR struct {\
  USHORT Flags;\
  USHORT Length;\
  ULONG TimeStamp;\
  STRING DosPath;\
}

/* RTL_USER_PROCESS_PARAMETERS struct from:
 https://github.com/processhacker/phnt/ 
 CC BY 4.0 licence */

#define RTL_USER_PROCESS_PARAMETERS struct {\
  ULONG MaximumLength;\
  ULONG Length;\
  ULONG Flags;\
  ULONG DebugFlags;\
  HANDLE ConsoleHandle;\
  ULONG ConsoleFlags;\
  HANDLE StandardInput;\
  HANDLE StandardOutput;\
  HANDLE StandardError;\
  CURDIR CurrentDirectory;\
  UNICODE_STRING DllPath;\
  UNICODE_STRING ImagePathName;\
  UNICODE_STRING CommandLine;\
  PVOID Environment;\
  ULONG StartingX;\
  ULONG StartingY;\
  ULONG CountX;\
  ULONG CountY;\
  ULONG CountCharsX;\
  ULONG CountCharsY;\
  ULONG FillAttribute;\
  ULONG WindowFlags;\
  ULONG ShowWindowFlags;\
  UNICODE_STRING WindowTitle;\
  UNICODE_STRING DesktopInfo;\
  UNICODE_STRING ShellInfo;\
  UNICODE_STRING RuntimeData;\
  RTL_DRIVE_LETTER_CURDIR CurrentDirectories[32];\
  ULONG_PTR EnvironmentSize;\
  ULONG_PTR EnvironmentVersion;\
  PVOID PackageDependencyData;\
  ULONG ProcessGroupId;\
  ULONG LoaderThreads;\
  UNICODE_STRING RedirectionDllName;\
  UNICODE_STRING HeapPartitionName;\
  ULONG_PTR DefaultThreadpoolCpuSetMasks;\
  ULONG DefaultThreadpoolCpuSetMaskCount;\
}

#if !defined(_MSC_VER)
#pragma pack(pop)
#else
#include <poppack.h>
#endif

std::wstring widen(std::string str) {
  std::size_t wchar_count = str.size() + 1;
  std::vector<wchar_t> buf(wchar_count);
  return std::wstring { buf.data(), (std::size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
}

std::string narrow(std::wstring wstr) {
  int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
  std::vector<char> buf(nbytes);
  return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
}

HANDLE OpenProcessWithDebugPrivilege(PROCID procId) {
  HANDLE proc = nullptr; HANDLE hToken = nullptr; LUID luid = { 0 }; TOKEN_PRIVILEGES tkp = { 0 };
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
    if (LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) {
      tkp.PrivilegeCount = 1; tkp.Privileges[0].Luid = luid;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      if (AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), nullptr, nullptr)) {
        proc = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
      }
    }
    CloseHandle(hToken);
  }
  return proc;
}

BOOL IsX86Process(HANDLE proc) {
  BOOL isWow = true;
  SYSTEM_INFO systemInfo = { 0 };
  GetNativeSystemInfo(&systemInfo);
  if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
    return isWow;
  IsWow64Process(proc, &isWow);
  return isWow;
}

void CwdCmdEnvFromProc(HANDLE proc, wchar_t **buffer, int type) {
  PEB peb = { 0 }; SIZE_T nRead = 0; ULONG len = 0; 
  PROCESS_BASIC_INFORMATION pbi = { 0 }; RTL_USER_PROCESS_PARAMETERS upp = { 0 };
  typedef NTSTATUS (__stdcall *NTQIP)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
  NTQIP NtQueryInformationProcess = NTQIP(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationProcess"));
  NTSTATUS status = NtQueryInformationProcess(proc, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
  if (status) return;
  ReadProcessMemory(proc, pbi.PebBaseAddress, &peb, sizeof(peb), &nRead);
  if (!nRead) return;
  ReadProcessMemory(proc, peb.ProcessParameters, &upp, sizeof(upp), &nRead);
  if (!nRead) return;
  PVOID buf = nullptr; len = 0;
  if (type == MEMCWD) {
    buf = upp.CurrentDirectory.DosPath.Buffer;
    len = upp.CurrentDirectory.DosPath.Length;
  } else if (type == MEMENV) {
    buf = upp.Environment;
    len = upp.EnvironmentSize;
  } else if (type == MEMCMD) {
    buf = upp.CommandLine.Buffer;
    len = upp.CommandLine.Length;
  }
  wchar_t *res = new wchar_t[len / 2 + 1];
  ReadProcessMemory(proc, buf, res, len, &nRead);
  if (!nRead) return; res[len / 2] = L'\0'; *buffer = res;
}
#endif

#if (defined(__APPLE__) && defined(__MACH__))
enum MEMTYP {
  MEMCMD,
  MEMENV
};

static std::vector<std::string> CmdEnvVec1;
void CmdEnvFromProcId(PROCID procId, char ***buffer, int *size, int type) {
  *buffer = nullptr; *size = 0;
  CmdEnvVec1.clear(); int i = 0;
  if (!CrossProcess::ProcIdExists(procId)) return;
  int argmax = 0, nargs = 0; std::size_t s = 0;
  char *procargs = nullptr, *sp = nullptr, *cp = nullptr; 
  int mib[3]; mib[0] = CTL_KERN; mib[1] = KERN_ARGMAX;
  s = sizeof(argmax);
  if (sysctl(mib, 2, &argmax, &s, nullptr, 0) == -1) {
    return;
  }
  procargs = (char *)malloc(argmax);
  if (procargs == nullptr) {
    return;
  }
  mib[0] = CTL_KERN; mib[1] = KERN_PROCARGS2;
  mib[2] = procId; s = argmax;
  if (sysctl(mib, 3, procargs, &s, nullptr, 0) == -1) {
    free(procargs); return;
  }
  memcpy(&nargs, procargs, sizeof(nargs));
  cp = procargs + sizeof(nargs);
  for (; cp < &procargs[s]; cp++) { 
    if (*cp == '\0') break;
  }
  if (cp == &procargs[s]) {
    free(procargs); return;
  }
  for (; cp < &procargs[s]; cp++) {
    if (*cp != '\0') break;
  }
  if (cp == &procargs[s]) {
    free(procargs); return;
  }
  sp = cp; int j = 0;
  while (*sp != '\0' && sp < &procargs[s]) {
    if (type && j >= nargs) { 
      CmdEnvVec1.push_back(sp); i++;
    } else if (!type && j < nargs) {
      CmdEnvVec1.push_back(sp); i++;
    }
    sp += strlen(sp) + 1; j++;
  }
  if (procargs) free(procargs);
  std::vector<char *> CmdEnvVec2;
  for (int j = 0; j < CmdEnvVec1.size(); j++)
    CmdEnvVec2.push_back((char *)CmdEnvVec1[j].c_str());
  char **arr = new char *[CmdEnvVec2.size()]();
  std::copy(CmdEnvVec2.begin(), CmdEnvVec2.end(), arr);
  *buffer = arr; *size = i;
}
#endif

#if defined(__DragonFly__)
kvm_t *kd = nullptr;
#endif

} // anonymous namespace

namespace CrossProcess {

void ProcIdEnumerate(PROCID **procId, int *size) {
  *procId = nullptr; *size = 0; 
  std::vector<PROCID> vec; int i = 0;
  #if defined(_WIN32)
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      vec.push_back(pe.th32ProcessID); i++;
    } while (Process32Next(hp, &pe));
  }
  CloseHandle(hp);
  #elif (defined(__APPLE__) && defined(__MACH__))
  if (ProcIdExists(0)) { vec.push_back(0); i++; }
  int cntp = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
  std::vector<PROCID> proc_info(cntp);
  std::fill(proc_info.begin(), proc_info.end(), 0);
  proc_listpids(PROC_ALL_PIDS, 0, &proc_info[0], sizeof(PROCID) * cntp);
  for (int j = cntp - 1; j >= 0; j--) {
    if (proc_info[j] == 0) { continue; }
    vec.push_back(proc_info[j]); i++;
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  if (ProcIdExists(0)) { vec.push_back(0); i++; }
  PROCTAB *proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
  while (proc_t *proc_info = readproc(proc, nullptr)) {
    vec.push_back(proc_info->tgid); i++;
    freeproc(proc_info);
  }
  closeproc(proc);
  #elif defined(__FreeBSD__)
  int cntp = 0; if (kinfo_proc *proc_info = kinfo_getallproc(&cntp)) {
    for (int j = 0; j < cntp; j++) {
      vec.push_back(proc_info[j].ki_pid); i++;
    }
    free(proc_info);
  }
  #elif defined(__DragonFly__)
  char errbuf[_POSIX2_LINE_MAX];
  kinfo_proc *proc_info = nullptr; 
  const char *nlistf, *memf; nlistf = memf = "/dev/null";
  kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
  int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntp))) {
    for (int j = 0; j < cntp; j++) {
      if (proc_info[j].kp_pid >= 0) {
        vec.push_back(proc_info[j].kp_pid); i++;
      }
    }
    free(proc_info);
  }
  #endif
  *procId = (PROCID *)malloc(sizeof(PROCID) * vec.size());
  if (procId) {
    std::copy(vec.begin(), vec.end(), *procId);
    *size = i;
  }
}

void FreeProcId(PROCID *procId) {
  if (procId) {
    free(procId);
  }
}

void ProcIdFromSelf(PROCID *procId) {
  *procId = 0;
  #if !defined(_WIN32)
  *procId = getpid();
  #elif defined(_WIN32)
  *procId = GetCurrentProcessId();
  #endif
}

PROCID ProcIdFromSelf() {
  #if !defined(_WIN32)
  return getpid();
  #elif defined(_WIN32)
  return GetCurrentProcessId();
  #endif
}

void ParentProcIdFromSelf(PROCID *parentProcId) {
  *parentProcId = 0;
  #if !defined(_WIN32)
  *parentProcId = getppid();
  #elif defined(_WIN32)
  ParentProcIdFromProcId(GetCurrentProcessId(), parentProcId);
  #endif
}

PROCID ParentProcIdFromSelf() {
  #if !defined(_WIN32)
  return getppid();
  #elif defined(_WIN32)
  PROCID parentProcId = 0;
  ParentProcIdFromProcId(GetCurrentProcessId(), &parentProcId);
  return parentProcId;
  #endif
}

bool ProcIdExists(PROCID procId) {
  #if !defined(_WIN32)
  return (kill(procId, 0) == 0);
  #elif defined(_WIN32)
  PROCID *buffer = nullptr; int size = 0;
  ProcIdEnumerate(&buffer, &size);
  if (buffer) {
    for (int i = 0; i < size; i++) {
      if (procId == buffer[i]) {
        return true;
      }
    }
    free(buffer);
  }
  return false;
  #else
  return false;
  #endif
}

bool ProcIdKill(PROCID procId) {
  #if !defined(_WIN32)
  return (kill(procId, SIGKILL) == 0);
  #elif defined(_WIN32)
  HANDLE proc = OpenProcessWithDebugPrivilege(procId);
  if (proc == nullptr) return false;
  bool result = TerminateProcess(proc, 0);
  CloseHandle(proc);
  return result;
  #else
  return false;
  #endif
}

void ParentProcIdFromProcId(PROCID procId, PROCID *parentProcId) {
  *parentProcId = 0;
  #if defined(_WIN32)
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      if (pe.th32ProcessID == procId) {
        *parentProcId = pe.th32ParentProcessID;
        break;
      }
    } while (Process32Next(hp, &pe));
  }
  CloseHandle(hp);
  #elif (defined(__APPLE__) && defined(__MACH__))
  proc_bsdinfo proc_info = { 0 };
  if (proc_pidinfo(procId, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) > 0) {
    *parentProcId = proc_info.pbi_ppid;
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  PROCTAB *proc = openproc(PROC_FILLSTATUS | PROC_PID, &procId);
  if (proc_t *proc_info = readproc(proc, nullptr)) { 
    *parentProcId = proc_info->ppid;
    freeproc(proc_info);
  }
  closeproc(proc);
  #elif defined(__FreeBSD__)
  if (kinfo_proc *proc_info = kinfo_getproc(procId)) {
    *parentProcId = proc_info->ki_ppid;
    free(proc_info);
  }
  #elif defined(__DragonFly__)
  char errbuf[_POSIX2_LINE_MAX];
  kinfo_proc *proc_info = nullptr; 
  const char *nlistf, *memf; nlistf = memf = "/dev/null";
  kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
  int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, procId, &cntp))) {
    if (proc_info->kp_ppid >= 0) {
      *parentProcId = proc_info->kp_ppid;
    }
    free(proc_info);
  }
  #endif
}

void ProcIdFromParentProcId(PROCID parentProcId, PROCID **procId, int *size) {
  *procId = nullptr; *size = 0;
  std::vector<PROCID> vec; int i = 0;
  #if defined(_WIN32)
  HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe = { 0 };
  pe.dwSize = sizeof(PROCESSENTRY32);
  if (Process32First(hp, &pe)) {
    do {
      if (pe.th32ParentProcessID == parentProcId) {
        vec.push_back(pe.th32ProcessID); i++;
      }
    } while (Process32Next(hp, &pe));
  }
  CloseHandle(hp);
  #elif (defined(__APPLE__) && defined(__MACH__))
  int cntp = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
  std::vector<PROCID> proc_info(cntp);
  std::fill(proc_info.begin(), proc_info.end(), 0);
  proc_listpids(PROC_ALL_PIDS, 0, &proc_info[0], sizeof(PROCID) * cntp);
  for (int j = cntp - 1; j >= 0; j--) {
    if (proc_info[j] == 0) { continue; }
    PROCID ppid; ParentProcIdFromProcId(proc_info[j], &ppid);
    if (ppid == parentProcId) {
      vec.push_back(proc_info[j]); i++;
    }
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  PROCTAB *proc = openproc(PROC_FILLSTAT);
  while (proc_t *proc_info = readproc(proc, nullptr)) {
    if (proc_info->ppid == parentProcId) {
      vec.push_back(proc_info->tgid); i++;
    }
    freeproc(proc_info);
  }
  closeproc(proc);
  #elif defined(__FreeBSD__)
  int cntp = 0; if (kinfo_proc *proc_info = kinfo_getallproc(&cntp)) {
    for (int j = 0; j < cntp; j++) {
      if (proc_info[j].ki_ppid == parentProcId) {
        vec.push_back(proc_info[j].ki_pid); i++;
      }
    }
    free(proc_info);
  }
  #elif defined(__DragonFly__)
  char errbuf[_POSIX2_LINE_MAX];
  kinfo_proc *proc_info = nullptr; 
  const char *nlistf, *memf; nlistf = memf = "/dev/null";
  kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
  int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntp))) {
    for (int j = 0; j < cntp; j++) {
      if (proc_info[j].kp_pid >= 0 && proc_info[j].kp_ppid >= 0 && 
        proc_info[j].kp_ppid == parentProcId) {
        vec.push_back(proc_info[j].kp_pid); i++;
      }
    }
    free(proc_info);
  }
  #endif
  *procId = (PROCID *)malloc(sizeof(PROCID) * vec.size());
  if (procId) {
    std::copy(vec.begin(), vec.end(), *procId);
    *size = i;
  }
}

void ExeFromProcId(PROCID procId, char **buffer) {
  *buffer = nullptr;
  if (!ProcIdExists(procId)) return;
  #if defined(_WIN32)
  if (procId == ProcIdFromSelf()) {
    wchar_t exe[MAX_PATH];
    if (GetModuleFileNameW(nullptr, exe, MAX_PATH) != 0) {
      static std::string str; str = narrow(exe);
      *buffer = (char *)str.c_str();
    }
  } else {
    HANDLE proc = OpenProcessWithDebugPrivilege(procId);
    if (proc == nullptr) return;
    wchar_t exe[MAX_PATH]; DWORD size = MAX_PATH;
    if (QueryFullProcessImageNameW(proc, 0, exe, &size) != 0) {
      static std::string str; str = narrow(exe);
      *buffer = (char *)str.c_str();
    }
    CloseHandle(proc);
  }
  #elif (defined(__APPLE__) && defined(__MACH__))
  char exe[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(procId, exe, sizeof(exe)) > 0) {
    static std::string str; str = exe;
    *buffer = (char *)str.c_str();
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  char exe[PATH_MAX]; 
  std::string symLink = "/proc/" + std::to_string(procId) + "/exe";
  if (realpath(symLink.c_str(), exe)) {
    static std::string str; str = exe;
    *buffer = (char *)str.c_str();
  }
  #elif defined(__FreeBSD__)
  int mib[4]; std::size_t s = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = procId;
  if (sysctl(mib, 4, nullptr, &s, nullptr, 0) == 0) {
    std::string str1; str1.resize(s, '\0');
    char *exe = str1.data();
    if (sysctl(mib, 4, exe, &s, nullptr, 0) == 0) {
      static std::string str2; str2 = exe;
      *buffer = (char *)str2.c_str();
    }
  }
  #elif defined(__DragonFly__)
  if (procId == 0) { return; }
  int mib[4]; std::size_t s = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = procId;
  if (sysctl(mib, 4, nullptr, &s, nullptr, 0) == 0) {
    std::string str1; str1.resize(s, '\0');
    char *exe = str1.data();
    if (sysctl(mib, 4, exe, &s, nullptr, 0) == 0) {
      static std::string str2; str2 = exe;
      *buffer = (char *)str2.c_str();
    }
  }
  #endif
}

const char *ExeFromProcId(PROCID procId) {
  char *exe = nullptr; ExeFromProcId(procId, &exe);
  return exe;
}

const char *ExecutableFromSelf() {
  return ExeFromProcId(ProcIdFromSelf());
}

const char *DirectoryGetCurrentWorking() {
  static std::string str;
  #if defined(_WIN32)
  wchar_t u8dname[MAX_PATH];
  if (GetCurrentDirectoryW(MAX_PATH, u8dname) != 0) {
    str = narrow(u8dname);
  }
  #else
  char dname[PATH_MAX];
  if (getcwd(dname, sizeof(dname)) != nullptr) {
    str = dname;
  }
  #endif
  return str.c_str();
}

bool DirectorySetCurrentWorking(const char *dname) {
  #if defined(_WIN32)
  std::wstring u8dname = widen(dname);
  return SetCurrentDirectoryW(u8dname.c_str());
  #else
  return chdir(dname);
  #endif
}

static std::unordered_map<PROCESS, std::intptr_t> stdIptMap;
static std::unordered_map<PROCESS, std::string>   stdOptMap;
static std::unordered_map<PROCESS, bool>          completeMap;
static std::mutex                                 stdOptMutex;

void CwdFromProcId(PROCID procId, char **buffer) {
  *buffer = nullptr;
  if (!ProcIdExists(procId)) return;
  #if defined(_WIN32)
  HANDLE proc = OpenProcessWithDebugPrivilege(procId);
  if (proc == nullptr) return;
  #if defined(XPROCESS_WIN32EXE_INCLUDES)
  if (IsX86Process(GetCurrentProcess()) != IsX86Process(proc)) {
    std::string exe; std::string tmp;
    wchar_t tempPath[MAX_PATH + 1]; 
    if (!GetTempPathW(MAX_PATH + 1, tempPath))
    { CloseHandle(proc); return; } tmp = narrow(tempPath);
    if (IsX86Process(proc)) exe = tmp + "\\crossprocess32.exe";
    else exe = tmp + "\\crossprocess64.exe";
    std::wstring wexe = widen(exe);
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(wexe.c_str()) && 
      GetLastError() == ERROR_FILE_NOT_FOUND) {
      FILE *file = nullptr;
      std::wofstream strm(wexe.c_str());
      strm.close();
      if (_wfopen_s(&file, wexe.c_str(), L"wb") == 0) {
        if (IsX86Process(proc)) {
          #if defined(_WIN64)
          fwrite((char *)crossprocess32, sizeof(char), sizeof(crossprocess32), file);
          #endif
        } else {
          #if !defined(_WIN64)
          fwrite((char *)crossprocess64, sizeof(char), sizeof(crossprocess64), file);
          #endif
        }
        fclose(file);
      }
    }
    PROCESS ind = ProcessExecute(("\"" + exe + "\" --cwd-from-pid " + std::to_string(procId)).c_str());
    static std::string str; if (stdOptMap.find(ind) != stdOptMap.end()) str = stdOptMap.find(ind)->second;
    if (!str.empty() && std::count(str.begin(), str.end(), '\\') > 1 && str.back() == '\\') {
      static std::string substr; substr = str.substr(0, str.length() - 1);
      *buffer = (char *)substr.c_str();
    } else {
      *buffer = (char *)str.c_str();
    }
    FreeExecutedProcessStandardOutput(ind);
  } else {
  #endif
    wchar_t *cwdbuf = nullptr;
    CwdCmdEnvFromProc(proc, &cwdbuf, MEMCWD);
    if (cwdbuf) {
      static std::string str; str = narrow(cwdbuf);
      if (!str.empty() && std::count(str.begin(), str.end(), '\\') > 1 && str.back() == '\\') {
        static std::string substr; substr = str.substr(0, str.length() - 1);
        *buffer = (char *)substr.c_str();
      } else {
        *buffer = (char *)str.c_str();
      }
      delete[] cwdbuf;
    }
  #if defined(XPROCESS_WIN32EXE_INCLUDES)
  }
  #endif
  CloseHandle(proc);
  #elif (defined(__APPLE__) && defined(__MACH__))
  proc_vnodepathinfo vpi = { 0 };
  char cwd[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidinfo(procId, PROC_PIDVNODEPATHINFO, 0, &vpi, sizeof(vpi)) > 0) {
    strcpy(cwd, vpi.pvi_cdir.vip_path);
    static std::string str; str = cwd;
    *buffer = (char *)str.c_str();
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  char cwd[PATH_MAX];
  std::string symLink = "/proc/" + std::to_string(procId) + "/cwd";
  if (realpath(symLink.c_str(), cwd)) {
    static std::string str; str = cwd;
    *buffer = (char *)str.c_str();
  }
  #elif defined(__FreeBSD__)
  char cwd[PATH_MAX]; unsigned cntp;
  procstat *proc_stat = procstat_open_sysctl();
  if (proc_stat) {
    kinfo_proc *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, procId, &cntp);
    if (proc_info) {
      filestat_list *head = procstat_getfiles(proc_stat, proc_info, 0);
      if (head) {
        filestat *fst = nullptr;
        STAILQ_FOREACH(fst, head, next) {
          if (fst->fs_uflags & PS_FST_UFLAG_CDIR) {
            strcpy(cwd, fst->fs_path);
            static std::string str; str = cwd;
            *buffer = (char *)str.c_str();
          }
        }
        procstat_freefiles(proc_stat, head);
      }
      procstat_freeprocs(proc_stat, proc_info);
    }
    procstat_close(proc_stat);
  }
  #elif defined(__DragonFly__)
  int mib[4]; std::size_t s = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_CWD;
  mib[3] = procId;
  if (sysctl(mib, 4, nullptr, &s, nullptr, 0) == 0) {
    std::vector<char> str1; str1.resize(s, '\0');
    char *cwd = str1.data();
    if (sysctl(mib, 4, cwd, &s, nullptr, 0) == 0) {
      static std::string str2; str2 = cwd ? : "\0";
      *buffer = (char *)str2.c_str();
    }
  }
  #endif
}

const char *CwdFromProcId(PROCID procId) {
  char *cwd = nullptr; CwdFromProcId(procId, &cwd);
  return cwd;
}

void FreeCmdline(char **buffer) {
  if (buffer) {
    delete[] buffer;
  }
}

static std::vector<std::string> CmdlineVec1;
void CmdlineFromProcId(PROCID procId, char ***buffer, int *size) {
  *buffer = nullptr; *size = 0;
  CmdlineVec1.clear(); int i = 0;
  if (!ProcIdExists(procId)) return;
  #if defined(_WIN32)
  HANDLE proc = OpenProcessWithDebugPrivilege(procId);
  if (proc == nullptr) return;
  #if defined(XPROCESS_WIN32EXE_INCLUDES)
  if (IsX86Process(GetCurrentProcess()) != IsX86Process(proc)) {
    std::string exe; std::string tmp;
    wchar_t tempPath[MAX_PATH + 1]; 
    if (!GetTempPathW(MAX_PATH + 1, tempPath))
    { CloseHandle(proc); return; } tmp = narrow(tempPath);
    if (IsX86Process(proc)) exe = tmp + "\\crossprocess32.exe";
    else exe = tmp + "\\crossprocess64.exe";
    std::wstring wexe = widen(exe);
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(wexe.c_str()) && 
      GetLastError() == ERROR_FILE_NOT_FOUND) {
      FILE *file = nullptr;
      std::wofstream strm(wexe.c_str());
      strm.close();
      if (_wfopen_s(&file, wexe.c_str(), L"wb") == 0) {
        if (IsX86Process(proc)) {
          #if defined(_WIN64)
          fwrite((char *)crossprocess32, sizeof(char), sizeof(crossprocess32), file);
          #endif
        } else {
          #if !defined(_WIN64)
          fwrite((char *)crossprocess64, sizeof(char), sizeof(crossprocess64), file);
          #endif
        }
        fclose(file);
      }
    }
    PROCESS ind = ProcessExecute(("\"" + exe + "\" --cmd-from-pid " + std::to_string(procId)).c_str());
    std::string str;  if (stdOptMap.find(ind) != stdOptMap.end()) str = stdOptMap.find(ind)->second;
    char *cmd = str.data();
    int j = 0; if (!str.empty()) {
      while (cmd[j] != '\0') {
        CmdlineVec1.push_back(&cmd[j]); i++;
        j += strlen(cmd + j) + 1;
      }
    }
    FreeExecutedProcessStandardOutput(ind);
  } else {
  #endif
    wchar_t *cmdbuf = nullptr; int cmdsize = 0;
    CwdCmdEnvFromProc(proc, &cmdbuf, MEMCMD);
    if (cmdbuf) {
      wchar_t **cmdline = CommandLineToArgvW(cmdbuf, &cmdsize);
      if (cmdline) {
        while (i < cmdsize) {
          CmdlineVec1.push_back(narrow(cmdline[i])); i++;
        }
        LocalFree(cmdline);
      }
      delete[] cmdbuf;
    }
  #if defined(XPROCESS_WIN32EXE_INCLUDES)
  }
  #endif
  CloseHandle(proc);
  #elif (defined(__APPLE__) && defined(__MACH__))
  char **cmdline = nullptr; int cmdsiz = 0;
  CmdEnvFromProcId(procId, &cmdline, &cmdsiz, MEMCMD);
  if (cmdline) {
    for (int j = 0; j < cmdsiz; j++) {
      CmdlineVec1.push_back(cmdline[i]); i++;
    }
    delete[] cmdline;
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  PROCTAB *proc = openproc(PROC_FILLCOM | PROC_PID, &procId);
  if (proc_t *proc_info = readproc(proc, nullptr)) {
    if (proc_info->cmdline) {
      for (int j = 0; proc_info->cmdline[j]; j++) {
        CmdlineVec1.push_back(proc_info->cmdline[j]); i++;
      }
    }
    freeproc(proc_info);
  }
  closeproc(proc);
  #elif defined(__FreeBSD__)
  procstat *proc_stat = procstat_open_sysctl(); unsigned cntp = 0;
  if (proc_stat) {
    kinfo_proc *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, procId, &cntp);
    if (proc_info) {
      char **cmdline = procstat_getargv(proc_stat, proc_info, 0);
      if (cmdline) {
        for (int j = 0; cmdline[j]; j++) {
          CmdlineVec1.push_back(cmdline[j]); i++;
        }
        procstat_freeargv(proc_stat);
      }
      procstat_freeprocs(proc_stat, proc_info);
    }
    procstat_close(proc_stat);
  }
  #elif defined(__DragonFly__)
  char errbuf[_POSIX2_LINE_MAX];
  kinfo_proc *proc_info = nullptr; 
  const char *nlistf, *memf; nlistf = memf = "/dev/null";
  kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
  int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, procId, &cntp))) {
    char **cmdline = kvm_getargv(kd, proc_info, 0);
    if (cmdline) {
      for (int j = 0; cmdline[j]; j++) {
        CmdlineVec1.push_back(cmdline[j]); i++;
      }
    }
    free(proc_info);
  }
  #endif
  std::vector<char *> CmdlineVec2;
  for (int i = 0; i < CmdlineVec1.size(); i++)
    CmdlineVec2.push_back((char *)CmdlineVec1[i].c_str());
  char **arr = new char *[CmdlineVec2.size()]();
  std::copy(CmdlineVec2.begin(), CmdlineVec2.end(), arr);
  *buffer = arr; *size = i;
}

void FreeEnviron(char **buffer) {
  if (buffer) {
    delete[] buffer;
  }
}

static std::vector<std::string> EnvironVec1; 
void EnvironFromProcId(PROCID procId, char ***buffer, int *size) {
  *buffer = nullptr; *size = 0;
  EnvironVec1.clear(); int i = 0;
  if (!ProcIdExists(procId)) return;
  #if defined(_WIN32)
  HANDLE proc = OpenProcessWithDebugPrivilege(procId);
  if (proc == nullptr) return;
  #if defined(XPROCESS_WIN32EXE_INCLUDES)
  if (IsX86Process(GetCurrentProcess()) != IsX86Process(proc)) {
    std::string exe; std::string tmp;
    wchar_t tempPath[MAX_PATH + 1]; 
    if (!GetTempPathW(MAX_PATH + 1, tempPath))
    { CloseHandle(proc); return; } tmp = narrow(tempPath);
    if (IsX86Process(proc)) exe = tmp + "\\crossprocess32.exe";
    else exe = tmp + "\\crossprocess64.exe";
    std::wstring wexe = widen(exe);
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(wexe.c_str()) && 
      GetLastError() == ERROR_FILE_NOT_FOUND) {
      FILE *file = nullptr;
      std::wofstream strm(wexe.c_str());
      strm.close();
      if (_wfopen_s(&file, wexe.c_str(), L"wb") == 0) {
        if (IsX86Process(proc)) {
          #if defined(_WIN64)
          fwrite((char *)crossprocess32, sizeof(char), sizeof(crossprocess32), file);
          #endif
        } else {
          #if !defined(_WIN64)
          fwrite((char *)crossprocess64, sizeof(char), sizeof(crossprocess64), file);
          #endif
        }
        fclose(file);
      }
    }
    PROCESS ind = ProcessExecute(("\"" + exe + "\" --env-from-pid " + std::to_string(procId)).c_str());
    std::string str; if (stdOptMap.find(ind) != stdOptMap.end()) str = stdOptMap.find(ind)->second;
    char *env = str.data();
    int j = 0; if (!str.empty()) {
      while (env[j] != '\0') {
        EnvironVec1.push_back(&env[j]); i++;
        j += strlen(env + j) + 1;
      }
    }
    FreeExecutedProcessStandardOutput(ind);
  } else {
  #endif
    wchar_t *wenv = nullptr;
    CwdCmdEnvFromProc(proc, &wenv, MEMENV);
    int j = 0; if (wenv) {
      while (wenv[j] != L'\0') {
        EnvironVec1.push_back(narrow(&wenv[j])); i++;
        j += wcslen(wenv + j) + 1;
      }
      delete[] wenv;
    }
  #if defined(XPROCESS_WIN32EXE_INCLUDES)
  }
  #endif
  CloseHandle(proc);
  #elif (defined(__APPLE__) && defined(__MACH__))
  char **env = nullptr; int envsiz = 0;
  CmdEnvFromProcId(procId, &env, &envsiz, MEMENV);
  if (env) {
    for (int j = 0; j < envsiz; j++) {
      EnvironVec1.push_back(env[i]); i++;
    }
    delete[] env;
  }
  #elif (defined(__linux__) && !defined(__ANDROID__))
  PROCTAB *proc = openproc(PROC_FILLENV | PROC_PID, &procId);
  if (proc_t *proc_info = readproc(proc, nullptr)) {
    if (proc_info->environ) {
      for (int j = 0; proc_info->environ[j]; j++) {
        EnvironVec1.push_back(proc_info->environ[j]); i++;
      }
    }
    freeproc(proc_info);
  }
  closeproc(proc);
  #elif defined(__FreeBSD__)
  procstat *proc_stat = procstat_open_sysctl(); unsigned cntp = 0;
  if (proc_stat) {
    kinfo_proc *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, procId, &cntp);
    if (proc_info) {
      char **env = procstat_getenvv(proc_stat, proc_info, 0);
      if (env) {
        for (int j = 0; env[j]; j++) {
          EnvironVec1.push_back(env[j]); i++;
        }
        procstat_freeenvv(proc_stat);
      }
      procstat_freeprocs(proc_stat, proc_info);
    }
    procstat_close(proc_stat);
  }
  #elif defined(__DragonFly__)
  char errbuf[_POSIX2_LINE_MAX];
  kinfo_proc *proc_info = nullptr; 
  const char *nlistf, *memf; nlistf = memf = "/dev/null";
  kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
  int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, procId, &cntp))) {
    char **environ = kvm_getenvv(kd, proc_info, 0);
    if (environ) {
      for (int j = 0; environ[j]; j++) {
        EnvironVec1.push_back(environ[j]); i++;
      }
    }
    free(proc_info);
  }
  #endif
  std::vector<char *> EnvironVec2;
  for (int i = 0; i < EnvironVec1.size(); i++)
    EnvironVec2.push_back((char *)EnvironVec1[i].c_str());
  char **arr = new char *[EnvironVec2.size()]();
  std::copy(EnvironVec2.begin(), EnvironVec2.end(), arr);
  *buffer = arr; *size = i;
}

void EnvironFromProcIdEx(PROCID procId, const char *name, char **value) {
  char **buffer = nullptr; int size = 0; *value = (char *)"\0";
  EnvironFromProcId(procId, &buffer, &size);
  if (buffer) {
    for (int i = 0; i < size; i++) {
      std::vector<std::string> equalssplit = StringSplitByFirstEqualsSign(buffer[i]);
      for (int j = 0; j < equalssplit.size(); j++) {
        std::string str1 = name;
        std::transform(equalssplit[0].begin(), equalssplit[0].end(), equalssplit[0].begin(), ::toupper);
        std::transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
        if (j == equalssplit.size() - 1 && equalssplit[0] == str1) {
          static std::string str2; str2 = equalssplit[j];
          *value = (char *)str2.c_str();
        }
      }
    }
    FreeEnviron(buffer);
  }
}

const char *EnvironFromProcIdEx(PROCID procId, const char *name) {
  char *value = (char *)"\0";
  EnvironFromProcIdEx(procId, name, &value);
  static std::string str; str = value;
  return str.c_str();
}

const char *EnvironmentGetVariable(const char *name) {
  static std::string str;
  #if defined(_WIN32)
  char *value = (char *)"\0";
  EnvironFromProcIdEx(ProcIdFromSelf(), name, &value);
  str = value;
  #else
  char *value = getenv(name);
  str = value ? : "\0";
  #endif
  return str.c_str();
}

bool EnvironmentSetVariable(const char *name, const char *value) {
  #if defined(_WIN32)
  std::wstring u8name = widen(name);
  std::wstring u8value = widen(value);
  return (SetEnvironmentVariableW(u8name.c_str(), u8value.c_str()) != 0);
  #else
  return (setenv(name, value, 1) == 0);
  #endif
}

bool EnvironmentUnsetVariable(const char *name) {
  #if defined(_WIN32)
  std::wstring u8name = widen(name);
  return (SetEnvironmentVariableW(u8name.c_str(), nullptr) != 0);
  #else
  return (unsetenv(name) == 0);
  #endif
}

#if defined(XPROCESS_GUIWINDOW_IMPL)
#if (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__)) || defined(XPROCESS_XQUARTZ_IMPL)
static inline int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
  return 0;
}

static inline int XIOErrorHandlerImpl(Display *display) {
  return 0;
}

static inline void SetErrorHandlers() {
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);
}
#endif

WINDOWID WindowIdFromNativeWindow(WINDOW window) {
  static std::string res; 
  #if (defined(__APPLE__) && defined(__MACH__))
  res = std::to_string([(NSWindow *)window windowNumber]);
  #else
  res = std::to_string((unsigned long long)window);
  #endif
  return (WINDOWID)res.c_str();
}

WINDOW NativeWindowFromWindowId(WINDOWID winId) {
  #if (defined(__APPLE__) && defined(__MACH__))
  return (WINDOW)[NSApp windowWithWindowNumber:strtoul(window, nullptr, 10)]);
  #else
  return (WINDOW)strtoull(winId, nullptr, 10);
  #endif
}

static std::vector<std::string> widVec1;
void WindowIdFromProcId(PROCID procId, WINDOWID **winId, int *size) {
  *winId = nullptr; *size = 0;
  widVec1.clear(); int i = 0;
  if (!ProcIdExists(procId)) return;
  #if defined(_WIN32)
  HWND hWnd = GetTopWindow(GetDesktopWindow());
  PROCID pid = 0; ProcIdFromWindowId(WindowIdFromNativeWindow(hWnd), &pid);
  if (procId == pid) {  
    widVec1.push_back(WindowIdFromNativeWindow(hWnd)); i++; 
  }
  while (hWnd = GetWindow(hWnd, GW_HWNDNEXT)) {
    PROCID pid = 0; ProcIdFromWindowId(WindowIdFromNativeWindow(hWnd), &pid);
    if (procId == pid) {  
      widVec1.push_back(WindowIdFromNativeWindow(hWnd)); i++; 
    }
  }
  #elif (defined(__APPLE__) && defined(__MACH__)) && !defined(XPROCESS_XQUARTZ_IMPL)
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(
  kCGWindowListOptionAll, kCGNullWindowID);
  CFIndex windowCount = 0;
  if ((windowCount = CFArrayGetCount(windowArray))) {
    for (CFIndex j = 0; j < windowCount; j++) {
      CFDictionaryRef windowInfoDictionary = 
      (CFDictionaryRef)CFArrayGetValueAtIndex(windowArray, j);
      CFNumberRef ownerPID = (CFNumberRef)CFDictionaryGetValue(
      windowInfoDictionary, kCGWindowOwnerPID); PROCID pid = 0;
      CFNumberGetValue(ownerPID, kCFNumberIntType, &pid);
      if (procId == pid) {
        CFNumberRef windowID = (CFNumberRef)CFDictionaryGetValue(
        windowInfoDictionary, kCGWindowNumber);
        CGWindowID wid; CFNumberGetValue(windowID,
        kCGWindowIDCFNumberType, &wid);
        widVec1.push_back(WindowIdFromNativeWindow(wid)); i++;
      }
    }
  }
  CFRelease(windowArray);
  #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__)) || defined(XPROCESS_XQUARTZ_IMPL)
  SetErrorHandlers();
  Display *display = XOpenDisplay(nullptr);
  Window window = XDefaultRootWindow(display);
  unsigned char *prop = nullptr;
  Atom actual_type = 0, filter_atom = 0;
  int actual_format = 0, status = 0;
  unsigned long nitems = 0, bytes_after = 0;
  filter_atom = XInternAtom(display, "_NET_CLIENT_LIST_STACKING", true);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1024, false,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != nullptr && nitems) {
    if (actual_format == 32) {
      unsigned long *array = (unsigned long *)prop;
      for (int j = nitems - 1; j >= 0; j--) {
        PROCID pid; ProcIdFromWindowId(WindowIdFromNativeWindow(array[j]), &pid);
        if (procId == pid) {
          widVec1.push_back(WindowIdFromNativeWindow(array[j])); i++;
        }
      }
    }
    XFree(prop);
  }
  XCloseDisplay(display);
  #endif
  std::vector<WINDOWID> widVec2;
  for (int i = 0; i < widVec1.size(); i++)
    widVec2.push_back((WINDOWID)widVec1[i].c_str());
  WINDOWID *arr = new WINDOWID[widVec2.size()]();
  std::copy(widVec2.begin(), widVec2.end(), arr);
  *winId = arr; *size = i;
}

void FreeWindowId(WINDOWID *winId) {
  if (winId) {
    delete[] winId;
  }
}

static std::vector<std::string> widVec3;
void WindowIdEnumerate(WINDOWID **winId, int *size) {
  *winId = nullptr; *size = 0;
  widVec3.clear(); int i = 0;
  PROCID *pid = nullptr; int pidsize = 0; 
  ProcIdEnumerate(&pid, &pidsize);
  if (pid) {
    for (int j = 0; j < pidsize; j++) {
      WINDOWID *wid = nullptr; int widsize = 0;
      WindowIdFromProcId(pid[j], &wid, &widsize);
      if (wid) {
        for (int ii = 0; ii < widsize; ii++) {
          widVec3.push_back(wid[ii]); i++;
        }
        FreeWindowId(wid);
      }
    }
  }
  std::vector<WINDOWID> widVec4;
  for (int i = 0; i < widVec3.size(); i++)
    widVec4.push_back((WINDOWID)widVec3[i].c_str());
  WINDOWID *arr = new WINDOWID[widVec4.size()]();
  std::copy(widVec4.begin(), widVec4.end(), arr);
  *winId = arr; *size = i;
}

void ProcIdFromWindowId(WINDOWID winId, PROCID *procId) {
  *procId = 0;
  #if defined(_WIN32)
  DWORD pid = 0; GetWindowThreadProcessId(NativeWindowFromWindowId(winId), &pid);
  *procId = (PROCID)pid;
  #elif (defined(__APPLE__) && defined(__MACH__)) && !defined(XPROCESS_XQUARTZ_IMPL)
  CFArrayRef windowArray = CGWindowListCopyWindowInfo(
  kCGWindowListOptionIncludingWindow, NativeWindowFromWindowId(winId));
  CFIndex windowCount = 0;
  if ((windowCount = CFArrayGetCount(windowArray))) {
    for (CFIndex i = 0; i < windowCount; i++) {
      CFDictionaryRef windowInfoDictionary = 
      (CFDictionaryRef)CFArrayGetValueAtIndex(windowArray, i);
      CFNumberRef ownerPID = (CFNumberRef)CFDictionaryGetValue(
      windowInfoDictionary, kCGWindowOwnerPID); PROCID pid = 0;
      CFNumberGetValue(ownerPID, kCFNumberIntType, &pid);
      WINDOWID *wid = nullptr; int size = 0; 
      WindowIdFromProcId(pid, &wid, &size);
      if (wid) {
        for (int j = 0; j < size; j++) {
          if (strtoul(winId, nullptr, 10) == strtoul(wid[j], nullptr, 10)) {
            *procId = pid;
            break;
          }
        }
        FreeWindowId(wid);
      }      
    }
  }
  CFRelease(windowArray);
  #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__)) || defined(XPROCESS_XQUARTZ_IMPL)
  SetErrorHandlers();
  Display *display = XOpenDisplay(nullptr);
  unsigned long property = 0;
  unsigned char *prop = nullptr;
  Atom actual_type = 0, filter_atom = 0;
  int actual_format = 0, status = 0;
  unsigned long nitems = 0, bytes_after = 0;
  filter_atom = XInternAtom(display, "_NET_WM_PID", true);
  status = XGetWindowProperty(display, NativeWindowFromWindowId(winId), filter_atom, 0, 1000, false,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != nullptr) {
    property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);
  }
  *procId = (PROCID)property;
  XCloseDisplay(display);
  #endif
  if (!ProcIdExists(*procId)) {
    *procId = 0;
  }
}

bool WindowIdExists(WINDOWID winId) {
  PROCID procId;
  ProcIdFromWindowId(winId, &procId);
  if (procId) {
    return ProcIdExists(procId);
  }
  return false;
}

bool WindowIdKill(WINDOWID winId) {
  PROCID procId;
  ProcIdFromWindowId(winId, &procId);
  if (procId) {
    return ProcIdKill(procId);
  }
  return false;
}
#endif

static int procInfoIndex = -1;
static int procListIndex = -1;
static std::unordered_map<PROCINFO, _PROCINFO *> procInfoMap;
static std::vector<std::vector<PROCID>>          procListVec;

PROCINFO ProcInfoFromProcId(PROCID procId) {
  char *exe    = nullptr; ExeFromProcId(procId, &exe);
  char *cwd    = nullptr; CwdFromProcId(procId, &cwd);
  PROCID ppid  = 0; ParentProcIdFromProcId(procId, &ppid);
  PROCID *pid  = nullptr; int pidsize = 0; 
  ProcIdFromParentProcId(procId, &pid, &pidsize);
  char **cmd   = nullptr; int cmdsize = 0; 
  CmdlineFromProcId(procId, &cmd, &cmdsize);
  char **env   = nullptr; int envsize = 0; 
  EnvironFromProcId(procId, &env, &envsize);
  #if defined(XPROCESS_GUIWINDOW_IMPL)
  WINDOWID *wid = nullptr; int widsize = 0;
  WindowIdFromProcId(procId, &wid, &widsize);
  #endif
  _PROCINFO *procInfo = new _PROCINFO();
  procInfo->ExecutableImageFilePath = exe;
  procInfo->CurrentWorkingDirectory = cwd;
  procInfo->ParentProcessId         = ppid;
  procInfo->ChildProcessId          = pid;
  procInfo->ChildProcessIdLength    = pidsize;
  procInfo->CommandLine             = cmd;
  procInfo->CommandLineLength       = cmdsize;
  procInfo->Environment             = env;
  procInfo->EnvironmentLength       = envsize;
  #if defined(XPROCESS_GUIWINDOW_IMPL)
  procInfo->OwnedWindowId           = wid;
  procInfo->OwnedWindowIdLength     = widsize;
  #endif
  procInfoIndex++; procInfoMap[procInfoIndex] = procInfo;
  return procInfoIndex;
}

char *ExecutableImageFilePath(PROCINFO procInfo) { return procInfoMap[procInfo]->ExecutableImageFilePath ? : (char *)"\0"; }
char *CurrentWorkingDirectory(PROCINFO procInfo) { return procInfoMap[procInfo]->CurrentWorkingDirectory ? : (char *)"\0"; }
PROCID ParentProcessId(PROCINFO procInfo) { return procInfoMap[procInfo]->ParentProcessId; }
PROCID *ChildProcessId(PROCINFO procInfo) { return procInfoMap[procInfo]->ChildProcessId; }
PROCID ChildProcessId(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->ChildProcessId[i]; }
int ChildProcessIdLength(PROCINFO procInfo) { return procInfoMap[procInfo]->ChildProcessIdLength; }
char **CommandLine(PROCINFO procInfo) { return procInfoMap[procInfo]->CommandLine; }
char *CommandLine(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->CommandLine[i]; }
int CommandLineLength(PROCINFO procInfo) { return procInfoMap[procInfo]->CommandLineLength; }
char **Environment(PROCINFO procInfo) { return procInfoMap[procInfo]->Environment; }
char *Environment(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->Environment[i]; }
int EnvironmentLength(PROCINFO procInfo) { return procInfoMap[procInfo]->EnvironmentLength; }
#if defined(XPROCESS_GUIWINDOW_IMPL)
WINDOWID *OwnedWindowId(PROCINFO procInfo) { return procInfoMap[procInfo]->OwnedWindowId; }
WINDOWID OwnedWindowId(PROCINFO procInfo, int i) { return procInfoMap[procInfo]->OwnedWindowId[i]; }
int OwnedWindowIdLength(PROCINFO procInfo) { return procInfoMap[procInfo]->OwnedWindowIdLength; }
#endif

void FreeProcInfo(PROCINFO procInfo) {
  if (procInfoMap.find(procInfo) == procInfoMap.end()) return;
  FreeProcId(procInfoMap[procInfo]->ChildProcessId);
  FreeCmdline(procInfoMap[procInfo]->CommandLine);
  FreeEnviron(procInfoMap[procInfo]->Environment);
  #if defined(XPROCESS_GUIWINDOW_IMPL)
  FreeWindowId(procInfoMap[procInfo]->OwnedWindowId);
  #endif
  delete procInfoMap[procInfo];
  procInfoMap.erase(procInfo);
}

PROCLIST ProcListCreate() { 
  PROCID *procId = nullptr; int size = 0; 
  ProcIdEnumerate(&procId, &size);
  if (procId) {
    std::vector<PROCID> res;
    for (int i = 0; i < size; i++)
      res.push_back(procId[i]); 
    procListVec.push_back(res);
    FreeProcId(procId);
  }
  procListIndex++;
  return procListIndex;
}

PROCID ProcessId(PROCLIST procList, int i) {
  std::vector<PROCID> procId = procListVec[procList];
  return procId[i];
}

int ProcessIdLength(PROCLIST procList) {
  return procListVec[procList].size();
}

void FreeProcList(PROCLIST procList) {
  procListVec[procList].clear();
}

#if !defined(_WIN32)
static inline PROCID ProcessExecuteHelper(const char *command, int *infp, int *outfp) {
  int p_stdin[2];
  int p_stdout[2];
  PROCID pid = -1;
  if (pipe(p_stdin) == -1)
    return -1;
  if (pipe(p_stdout) == -1) {
    close(p_stdin[0]);
    close(p_stdin[1]);
    return -1;
  }
  pid = fork();
  if (pid < 0) {
    close(p_stdin[0]);
    close(p_stdin[1]);
    close(p_stdout[0]);
    close(p_stdout[1]);
    return pid;
  } else if (pid == 0) {
    close(p_stdin[1]);
    dup2(p_stdin[0], 0);
    close(p_stdout[0]);
    dup2(p_stdout[1], 1);
    dup2(open("/dev/null", O_WRONLY), 2);
    for (int i = 3; i < 4096; i++)
      close(i);
    setsid();
    execl("/bin/sh", "/bin/sh", "-c", command, nullptr);
    _exit(-1);
  }
  close(p_stdin[0]);
  close(p_stdout[1]);
  if (infp == nullptr) {
    close(p_stdin[1]);
  } else {
    *infp = p_stdin[1];
  }
  if (outfp == nullptr) {
    close(p_stdout[0]);
  } else {
    *outfp = p_stdout[0];
  }
  return pid;
}
#endif

static inline void OutputThread(std::intptr_t file, PROCESS procIndex) {
  #if !defined(_WIN32)
  ssize_t nRead = 0; char buffer[BUFSIZ];
  while ((nRead = read((int)file, buffer, BUFSIZ)) > 0) {
    buffer[nRead] = '\0';
  #else
  DWORD nRead = 0; char buffer[BUFSIZ];
  while (ReadFile((HANDLE)(void *)file, buffer, BUFSIZ, &nRead, nullptr) && nRead) {
    buffer[nRead] = '\0';
  #endif
    std::lock_guard<std::mutex> guard(stdOptMutex);
    stdOptMap[procIndex].append(buffer, nRead);
  }
}

static int index = -1;
static std::unordered_map<int, PROCID>  childProcId;
static std::unordered_map<int, bool> procDidExecute;

#if !defined(_WIN32)
static inline PROCID ProcIdFromForkProcId(PROCID procId) {
  PROCID *pid = nullptr; int pidsize = 0;
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ProcIdFromParentProcId(procId, &pid, &pidsize);
  if (pid) { if (pidsize) { procId = pid[pidsize - 1]; }
  FreeProcId(pid); } 
  return procId;
}
#endif

PROCESS ProcessExecute(const char *command) {
  index++;
  #if !defined(_WIN32)
  int infd = 0, outfd = 0; 
  PROCID procId = 0, forkProcId = 0, waitProcId = 0;
  forkProcId = ProcessExecuteHelper(command, &infd, &outfd);
  procId = forkProcId; waitProcId = procId; 
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  if (forkProcId != -1) {
    while ((procId = ProcIdFromForkProcId(procId)) == waitProcId) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      int status; waitProcId = waitpid(forkProcId, &status, WNOHANG);
      char **cmd = nullptr; int cmdsize; CmdlineFromProcId(forkProcId, &cmd, &cmdsize);
      if (cmd) { if (cmdsize && strcmp(cmd[0], "/bin/sh") == 0) {
      if (waitProcId > 0) procId = waitProcId; } FreeCmdline(cmd); }
    }
  } else { procId = 0; }
  childProcId[index] = procId; std::this_thread::sleep_for(std::chrono::milliseconds(5));
  procDidExecute[index] = true; PROCESS procIndex = (PROCESS)procId;
  stdIptMap.insert(std::make_pair(procIndex, (std::intptr_t)infd));
  std::thread optThread(OutputThread, (std::intptr_t)outfd, procIndex);
  optThread.join();
  #else
  wchar_t cwstr_command[32768];
  std::wstring wstr_command = widen(command); bool proceed = true;
  wcsncpy_s(cwstr_command, 32768, wstr_command.c_str(), 32768);
  HANDLE hStdInPipeRead = nullptr; HANDLE hStdInPipeWrite = nullptr;
  HANDLE hStdOutPipeRead = nullptr; HANDLE hStdOutPipeWrite = nullptr;
  SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, true };
  proceed = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
  if (proceed == false) return 0;
  SetHandleInformation(hStdInPipeWrite, HANDLE_FLAG_INHERIT, 0);
  proceed = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);
  if (proceed == false) return 0;
  STARTUPINFOW si = { 0 };
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = hStdOutPipeWrite;
  si.hStdOutput = hStdOutPipeWrite;
  si.hStdInput = hStdInPipeRead;
  PROCESS_INFORMATION pi = { 0 }; PROCESS procIndex = 0;
  if (CreateProcessW(nullptr, cwstr_command, nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
    CloseHandle(hStdOutPipeWrite);
    CloseHandle(hStdInPipeRead);
    PROCID procId = pi.dwProcessId; childProcId[index] = procId; procIndex = (PROCESS)procId;
    std::this_thread::sleep_for(std::chrono::milliseconds(5)); procDidExecute[index] = true;
    stdIptMap.insert(std::make_pair(procIndex, (std::intptr_t)(void *)hStdInPipeWrite));
    MSG msg; HANDLE waitHandles[] = { pi.hProcess, hStdOutPipeRead };
    std::thread optThread(OutputThread, (std::intptr_t)(void *)hStdOutPipeRead, procIndex);
    while (MsgWaitForMultipleObjects(2, waitHandles, false, 5, QS_ALLEVENTS) != WAIT_OBJECT_0) {
      while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
    optThread.join();
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutPipeRead);
    CloseHandle(hStdInPipeWrite);
  } else { 
    procDidExecute[index] = true;
    childProcId[index] = 0;
  }
  #endif
  FreeExecutedProcessStandardInput(procIndex);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  completeMap[procIndex] = true;
  return procIndex;
}

PROCESS ProcessExecuteAsync(const char *command) {
  int prevIndex = index;
  std::thread procThread(ProcessExecute, command);
  while (prevIndex == index)
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  while (procDidExecute.find(index) == procDidExecute.end() || !procDidExecute[index])
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  PROCESS procIndex = (PROCESS)childProcId[index];
  completeMap[procIndex] = false;
  procThread.detach();
  return procIndex;
}

void ExecutedProcessWriteToStandardInput(PROCESS procIndex, const char *input) {
  if (stdIptMap.find(procIndex) == stdIptMap.end()) return;
  std::string str = input; char *buffer = new char[str.length() + 1]();
  #if !defined(_WIN32)
  strcpy(buffer, str.c_str());
  write((int)stdIptMap[procIndex], buffer, str.length() + 1);
  #else
  strncpy_s(buffer, str.length() + 1, str.c_str(), str.length() + 1);
  DWORD dwWritten; WriteFile((HANDLE)(void *)stdIptMap[procIndex], buffer, 
  str.length() + 1, &dwWritten, nullptr);
  #endif
  delete[] buffer;
}

const char *ExecutedProcessReadFromStandardOutput(PROCESS procIndex) {
  if (stdOptMap.find(procIndex) == stdOptMap.end()) return "\0";
  std::lock_guard<std::mutex> guard(stdOptMutex);
  return stdOptMap.find(procIndex)->second.c_str();
}

void FreeExecutedProcessStandardInput(PROCESS procIndex) {
  if (stdIptMap.find(procIndex) == stdIptMap.end()) return;
  stdIptMap.erase(procIndex);
}

void FreeExecutedProcessStandardOutput(PROCESS procIndex) {
  if (stdOptMap.find(procIndex) == stdOptMap.end()) return;
  stdOptMap.erase(procIndex);
}

bool CompletionStatusFromExecutedProcess(PROCESS procIndex) {
  if (completeMap.find(procIndex) == completeMap.end()) return false;
  return completeMap.find(procIndex)->second;
}

} // namespace CrossProcess

#if defined(_WIN32)
#if !defined(XPROCESS_WIN32EXE_INCLUDES)
static std::string StringReplaceAll(std::string str, std::string substr, std::string nstr) {
  std::size_t pos = 0;
  while ((pos = str.find(substr, pos)) != std::string::npos) {
    str.replace(pos, substr.length(), nstr);
    pos += nstr.length();
  }
  return str;
}

int main(int argc, char **argv) {
  if (argc >= 3) {
    using namespace std::string_literals;
    if (strcmp(argv[1], "--cwd-from-pid") == 0) {
      char *buffer = nullptr;
      CrossProcess::CwdFromProcId(strtoul(argv[2], nullptr, 10), &buffer);
      if (buffer) {
        printf("%s", buffer);
      }
    } else if (strcmp(argv[1], "--cmd-from-pid") == 0) {
      char **buffer = nullptr; int size = 0;
      CrossProcess::CmdlineFromProcId(strtoul(argv[2], nullptr, 10), &buffer, &size);
      if (buffer) {
        for (int i = 0; i < size; i++)
          std::cout << buffer[i] << "\0"s;
        std::cout << "\0"s;
        CrossProcess::FreeCmdline(buffer);
      }
    } else if (strcmp(argv[1], "--env-from-pid") == 0) {
      char **buffer = nullptr; int size = 0;
      CrossProcess::EnvironFromProcId(strtoul(argv[2], nullptr, 10), &buffer, &size);
      if (buffer) {
        for (int i = 0; i < size; i++)
          std::cout << buffer[i] << "\0"s;
        std::cout << "\0"s;
        CrossProcess::FreeEnviron(buffer);
      }
    }
  }
  return 0;
}
#endif
#endif
