/*

 MIT License
 
 Copyright © 2021-2022 Samuel Venable
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

#include "process.h"
#if defined(PROCESS_WIN32EXE_INCLUDES)
#if defined(_WIN64)
#include "process32.h"
#else
#include "process64.h"
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
#include <shlwapi.h>
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
#define KINFO_PROC kinfo_proc
#elif defined(__DragonFly__) || defined(__OpenBSD__)
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <kvm.h>
#define KINFO_PROC kinfo_proc
#elif defined(__NetBSD__)
#include <sys/param.h>
#include <sys/sysctl.h>
#include <kvm.h>
#define KINFO_PROC kinfo_proc2
#endif

using ngs::proc::PROCID;
#if defined(PROCESS_GUIWINDOW_IMPL)
using ngs::proc::WINDOWID;
#endif

namespace {

  #if !defined(_MSC_VER)
  #pragma pack(push, 8)
  #else
  #include <pshpack8.h>
  #endif
  typedef struct {
    char *executable_image_file_path;
    char *current_working_directory;
    PROCID parent_process_id;
    PROCID *child_process_id;
    int child_process_id_length;
    char **commandline;
    int commandline_length;
    char **environment;
    int environment_length;
    #if defined(PROCESS_GUIWINDOW_IMPL)
    WINDOWID *owned_window_id;
    int owned_window_id_length;
    #endif
  } PROCINFO_STRUCT;
  #if !defined(_MSC_VER)
  #pragma pack(pop)
  #else
  #include <poppack.h>
  #endif

  void message_pump() {
    #if defined(_WIN32) 
    MSG msg; while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    #endif
  }

  std::vector<std::string> string_split_by_first_equals_sign(std::string str) {
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
    std::size_t wchar_count = str.size() + 1; std::vector<wchar_t> buf(wchar_count);
    return std::wstring { buf.data(), (std::size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
  }

  std::string narrow(std::wstring wstr) {
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr); std::vector<char> buf(nbytes);
    return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
  }
  
  std::string string_replace_all(std::string str, std::string substr, std::string nstr) {
    std::size_t pos = 0; while ((pos = str.find(substr, pos)) != std::string::npos) {
      message_pump(); str.replace(pos, substr.length(), nstr);
      pos += nstr.length();
    }
    return str;
  }

  bool file_exists(std::string fname) {
    DWORD attr; std::wstring wfname = widen(fname);
    attr = GetFileAttributesW(wfname.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES &&
      !(attr & FILE_ATTRIBUTE_DIRECTORY));
  }

  HANDLE open_process_with_debug_privilege(PROCID proc_id) {
    HANDLE proc = nullptr; HANDLE hToken = nullptr; LUID luid = { 0 }; TOKEN_PRIVILEGES tkp = { 0 };
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
      if (LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) {
        tkp.PrivilegeCount = 1; tkp.Privileges[0].Luid = luid;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), nullptr, nullptr)) {
          proc = OpenProcess(PROCESS_ALL_ACCESS, false, proc_id);
        }
      }
      CloseHandle(hToken);
    }
    return proc;
  }

  BOOL is_x86_process(HANDLE proc) {
    BOOL isWow = true;
    SYSTEM_INFO systemInfo = { 0 };
    GetNativeSystemInfo(&systemInfo);
    if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
      return isWow;
    IsWow64Process(proc, &isWow);
    return isWow;
  }

  void cwd_cmd_env_from_proc(HANDLE proc, wchar_t **buffer, int type) {
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
      len = (ULONG)upp.EnvironmentSize;
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

  static std::vector<std::string> cmd_env_vec_1;
  void cmd_env_from_proc_id(PROCID proc_id, char ***buffer, int *size, int type) {
    *buffer = nullptr; *size = 0;
    cmd_env_vec_1.clear(); int i = 0;
    if (!ngs::proc::proc_id_exists(proc_id)) return;
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
    mib[2] = proc_id; s = argmax;
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
        cmd_env_vec_1.push_back(sp); i++;
      } else if (!type && j < nargs) {
        cmd_env_vec_1.push_back(sp); i++;
      }
      sp += strlen(sp) + 1; j++;
    }
    if (procargs) free(procargs);
    std::vector<char *> cmd_env_vec_2;
    for (int j = 0; j < cmd_env_vec_1.size(); j++)
      cmd_env_vec_2.push_back((char *)cmd_env_vec_1[j].c_str());
    char **arr = new char *[cmd_env_vec_2.size()]();
    std::copy(cmd_env_vec_2.begin(), cmd_env_vec_2.end(), arr);
    *buffer = arr; *size = i;
  }
  #endif

  #if defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
  kvm_t *kd = nullptr;
  #endif

} // anonymous namespace

namespace ngs::proc {

  void proc_id_enumerate(PROCID **proc_id, int *size) {
    *proc_id = nullptr; *size = 0;
    std::vector<PROCID> vec; int i = 0;
    #if defined(_WIN32)
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hp, &pe)) {
      do {
        message_pump();
        vec.push_back(pe.th32ProcessID); i++;
      } while (Process32Next(hp, &pe));
    }
    CloseHandle(hp);
    #elif (defined(__APPLE__) && defined(__MACH__))
    if (proc_id_exists(0)) { vec.push_back(0); i++; }
    int cntp = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
    std::vector<PROCID> proc_info(cntp);
    std::fill(proc_info.begin(), proc_info.end(), 0);
    proc_listpids(PROC_ALL_PIDS, 0, &proc_info[0], sizeof(PROCID) * cntp);
    for (int j = cntp - 1; j >= 0; j--) {
      if (proc_info[j] == 0) { continue; }
      vec.push_back(proc_info[j]); i++;
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    if (proc_id_exists(0)) { vec.push_back(0); i++; }
    PROCTAB *proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
    while (proc_t *proc_info = readproc(proc, nullptr)) {
      vec.push_back(proc_info->tgid); i++;
      freeproc(proc_info);
    }
    closeproc(proc);
    #elif defined(__FreeBSD__)
    int cntp = 0; if (KINFO_PROC *proc_info = kinfo_getallproc(&cntp)) {
      for (int j = 0; j < cntp; j++) {
        vec.push_back(proc_info[j].ki_pid); i++;
      }
      free(proc_info);
    }
    #elif defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
    char errbuf[_POSIX2_LINE_MAX];
    KINFO_PROC *proc_info = nullptr; int cntp = 0;
    #if defined(__DragonFly__)
    const char *nlistf, *memf; nlistf = memf = "/dev/null";
    kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntp))) {
      for (int j = 0; j < cntp; j++) {
        if (proc_info[j].kp_pid >= 0) {
          vec.push_back(proc_info[j].kp_pid); i++;
        }
      }
    #elif defined(__NetBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getproc2(kd, KERN_PROC_ALL, 0, sizeof(struct KINFO_PROC), &cntp))) {
      for (int j = cntp - 1; j >= 0; j--) {
        vec.push_back(proc_info[j].p_pid); i++;
      }
    #elif defined(__OpenBSD__)
    if (proc_id_exists(0)) { vec.push_back(0); i++; }
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, sizeof(struct KINFO_PROC), &cntp))) {
      for (int j = cntp - 1; j >= 0; j--) {
        vec.push_back(proc_info[j].p_pid); i++;
      }
    #endif
    }
    kvm_close(kd);
    #endif
    *proc_id = (PROCID *)malloc(sizeof(PROCID) * vec.size());
    if (proc_id) {
      std::copy(vec.begin(), vec.end(), *proc_id);
      *size = i;
    }
  }

  void free_proc_id(PROCID *proc_id) {
    if (proc_id) {
      free(proc_id);
    }
  }

  void proc_id_from_self(PROCID *proc_id) {
    *proc_id = 0;
    #if !defined(_WIN32)
    *proc_id = getpid();
    #else
    *proc_id = GetCurrentProcessId();
    #endif
  }

  PROCID proc_id_from_self() {
    #if !defined(_WIN32)
    return getpid();
    #else
    return GetCurrentProcessId();
    #endif
  }

  void parent_proc_id_from_self(PROCID *parent_proc_id) {
    *parent_proc_id = 0;
    #if !defined(_WIN32)
    *parent_proc_id = getppid();
    #else
    parent_proc_id_from_proc_id(proc_id_from_self(), parent_proc_id);
    #endif
  }

  PROCID parent_proc_id_from_self() {
    #if !defined(_WIN32)
    return getppid();
    #else
    PROCID parent_proc_id = 0;
    parent_proc_id_from_proc_id(proc_id_from_self(), &parent_proc_id);
    return parent_proc_id;
    #endif
  }

  bool proc_id_exists(PROCID proc_id) {
    #if !defined(_WIN32)
    return (kill(proc_id, 0) != -1);
    #else
    PROCID *buffer = nullptr; int size = 0;
    proc_id_enumerate(&buffer, &size);
    if (buffer) {
      for (int i = 0; i < size; i++) {
        message_pump();
        if (proc_id == buffer[i]) {
          return true;
        }
      }
      free_proc_id(buffer);
    }
    return false;
    #endif
  }

  bool proc_id_suspend(PROCID proc_id) {
    #if !defined(_WIN32)
    return (kill(proc_id, SIGSTOP) != -1);
    #else
    typedef NTSTATUS (__stdcall *NtSuspendProcess)(IN HANDLE ProcessHandle);
    HANDLE proc = open_process_with_debug_privilege(proc_id);
    if (proc == nullptr) return false;
    NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtSuspendProcess");
    NTSTATUS status = pfnNtSuspendProcess(proc);
    CloseHandle(proc);
    return (!status);
    #endif
  }

  bool proc_id_resume(PROCID proc_id) {
    #if !defined(_WIN32)
    return (kill(proc_id, SIGCONT) != -1);
    #else
    typedef NTSTATUS (__stdcall *NtResumeProcess)(IN HANDLE ProcessHandle);
    HANDLE proc = open_process_with_debug_privilege(proc_id);
    if (proc == nullptr) return false;
    NtResumeProcess pfnNtResumeProcess = (NtResumeProcess)GetProcAddress(GetModuleHandle("ntdll"), "NtResumeProcess");
    NTSTATUS status = pfnNtResumeProcess(proc);
    CloseHandle(proc);
    return (!status);
    #endif
  }

  bool proc_id_kill(PROCID proc_id) {
    #if !defined(_WIN32)
    return (kill(proc_id, SIGKILL) != -1);
    #else
    HANDLE proc = open_process_with_debug_privilege(proc_id);
    if (proc == nullptr) return false;
    bool result = TerminateProcess(proc, 0);
    CloseHandle(proc);
    return result;
    #endif
  }

  void parent_proc_id_from_proc_id(PROCID proc_id, PROCID *parent_proc_id) {
    *parent_proc_id = 0;
    #if defined(_WIN32)
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hp, &pe)) {
      do {
        if (pe.th32ProcessID == proc_id) {
          message_pump();
          *parent_proc_id = pe.th32ParentProcessID;
          break;
        }
      } while (Process32Next(hp, &pe));
    }
    CloseHandle(hp);
    #elif (defined(__APPLE__) && defined(__MACH__))
    proc_bsdinfo proc_info = { 0 };
    if (proc_pidinfo(proc_id, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) > 0) {
      *parent_proc_id = proc_info.pbi_ppid;
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    PROCTAB *proc = openproc(PROC_FILLSTATUS | PROC_PID, &proc_id);
    if (proc_t *proc_info = readproc(proc, nullptr)) {
      *parent_proc_id = proc_info->ppid;
      freeproc(proc_info);
    }
    closeproc(proc);
    #elif defined(__FreeBSD__)
    if (KINFO_PROC *proc_info = kinfo_getproc(proc_id)) {
      *parent_proc_id = proc_info->ki_ppid;
      free(proc_info);
    }
    #elif defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
    char errbuf[_POSIX2_LINE_MAX];
    KINFO_PROC *proc_info = nullptr; int cntp = 0; 
    #if defined(__DragonFly__)
    const char *nlistf, *memf; nlistf = memf = "/dev/null";
    kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, proc_id, &cntp))) {
      if (proc_info->kp_ppid >= 0) {
        *parent_proc_id = proc_info->kp_ppid;
      }
    #elif defined(__NetBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getproc2(kd, KERN_PROC_PID, proc_id, sizeof(struct KINFO_PROC), &cntp))) {
      *parent_proc_id = proc_info->p_ppid;
    #elif defined(__OpenBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, proc_id, sizeof(struct KINFO_PROC), &cntp))) {
      *parent_proc_id = proc_info->p_ppid;
    #endif
    }
    kvm_close(kd);
    #endif
  }

  void proc_id_from_parent_proc_id(PROCID parent_proc_id, PROCID **proc_id, int *size) {
    *proc_id = nullptr; *size = 0;
    std::vector<PROCID> vec; int i = 0;
    #if defined(_WIN32)
    HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hp, &pe)) {
      do {
        message_pump();
        if (pe.th32ParentProcessID == parent_proc_id) {
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
      PROCID ppid; parent_proc_id_from_proc_id(proc_info[j], &ppid);
      if (proc_info[j] == 1 && ppid == 0 && parent_proc_id == 0) {
        vec.push_back(0); i++;
      }
      if (ppid == parent_proc_id) {
        vec.push_back(proc_info[j]); i++;
      }
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    PROCTAB *proc = openproc(PROC_FILLSTAT);
    while (proc_t *proc_info = readproc(proc, nullptr)) {
      if (proc_info->tgid == 1 && proc_info->ppid == 0 && parent_proc_id == 0) {
        vec.push_back(0); i++;
      }
      if (proc_info->ppid == parent_proc_id) {
        vec.push_back(proc_info->tgid); i++;
      }
      freeproc(proc_info);
    }
    closeproc(proc);
    #elif defined(__FreeBSD__)
    int cntp = 0; if (KINFO_PROC *proc_info = kinfo_getallproc(&cntp)) {
      for (int j = 0; j < cntp; j++) {
        if (proc_info[j].ki_ppid == parent_proc_id) {
          vec.push_back(proc_info[j].ki_pid); i++;
        }
      }
      free(proc_info);
    }
    #elif defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
    char errbuf[_POSIX2_LINE_MAX];
    KINFO_PROC *proc_info = nullptr; int cntp = 0;
    #if defined(__DragonFly__)
    const char *nlistf, *memf; nlistf = memf = "/dev/null";
    kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntp))) {
      for (int j = 0; j < cntp; j++) {
        if (proc_info[j].kp_pid == 1 && proc_info[j].kp_ppid == 0 && parent_proc_id == 0) {
          vec.push_back(0); i++;
        }
        if (proc_info[j].kp_pid >= 0 && proc_info[j].kp_ppid >= 0 && proc_info[j].kp_ppid == parent_proc_id) {
          vec.push_back(proc_info[j].kp_pid); i++;
        }
      }
    #elif defined(__NetBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getproc2(kd, KERN_PROC_ALL, 0, sizeof(struct KINFO_PROC), &cntp))) {
      for (int j = cntp - 1; j >= 0; j--) {
        if (proc_info[j].p_ppid == parent_proc_id) {
          vec.push_back(proc_info[j].p_pid); i++;
        }
      }
    #elif defined(__OpenBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, sizeof(struct KINFO_PROC), &cntp))) {
      for (int j = cntp - 1; j >= 0; j--) {
        if (proc_info[j].p_pid == 1 && proc_info[j].p_ppid == 0 && parent_proc_id == 0) {
          vec.push_back(0); i++;
        }
        if (proc_info[j].p_ppid == parent_proc_id) {
          vec.push_back(proc_info[j].p_pid); i++;
        }
      }
    #endif
    }
    kvm_close(kd);
    #endif
    *proc_id = (PROCID *)malloc(sizeof(PROCID) * vec.size());
    if (proc_id) {
      std::copy(vec.begin(), vec.end(), *proc_id);
      *size = i;
    }
  }

  void exe_from_proc_id(PROCID proc_id, char **buffer) {
    *buffer = nullptr;
    if (!proc_id_exists(proc_id)) return;
    #if defined(_WIN32)
    if (proc_id == proc_id_from_self()) {
      wchar_t exe[MAX_PATH];
      if (GetModuleFileNameW(nullptr, exe, MAX_PATH) != 0) {
        static std::string str; str = narrow(exe);
        *buffer = (char *)str.c_str();
      }
    } else {
      HANDLE proc = open_process_with_debug_privilege(proc_id);
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
    if (proc_pidpath(proc_id, exe, sizeof(exe)) > 0) {
      static std::string str; str = exe;
      *buffer = (char *)str.c_str();
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    char exe[PATH_MAX];
    std::string symLink = "/proc/" + std::to_string(proc_id) + "/exe";
    if (realpath(symLink.c_str(), exe)) {
      static std::string str; str = exe;
      *buffer = (char *)str.c_str();
    }
    #elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__)
    #if defined(__DragonFly__)
    if (proc_id == 0) { return; }
    #endif
    int mib[4]; std::size_t s = 0;
    mib[0] = CTL_KERN;
    #if defined(__NetBSD__)
    mib[1] = KERN_PROC_ARGS;
    mib[2] = proc_id;
    mib[3] = KERN_PROC_PATHNAME;
    #else
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = proc_id;
    #endif
    if (sysctl(mib, 4, nullptr, &s, nullptr, 0) == 0) {
      std::string str1; str1.resize(s, '\0');
      char *exe = str1.data();
      if (sysctl(mib, 4, exe, &s, nullptr, 0) == 0) {
        static std::string str2; str2 = exe;
        *buffer = (char *)str2.c_str();
      }
    }
    #elif defined(__OpenBSD__)
    char **cmdbuf = nullptr; int cmdsize = 0;
    cmdline_from_proc_id(proc_id, &cmdbuf, &cmdsize);
    if (cmdsize) {
      static std::string cmd;
      cmd = cmdbuf[0]; 
      *buffer = (char *)cmd.c_str();
      free_cmdline(cmdbuf);
    }
    #endif
  }

  const char *exe_from_proc_id(PROCID proc_id) {
    char *exe = nullptr; exe_from_proc_id(proc_id, &exe);
    return exe;
  }

  const char *executable_from_self() {
    return exe_from_proc_id(proc_id_from_self());
  }

  const char *directory_get_current_working() {
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

  bool directory_set_current_working(const char *dname) {
    #if defined(_WIN32)
    std::wstring u8dname = widen(dname);
    return SetCurrentDirectoryW(u8dname.c_str());
    #else
    return chdir(dname);
    #endif
  }

  static std::unordered_map<LOCALPROCID, std::intptr_t> stdipt_map;
  static std::unordered_map<LOCALPROCID, std::string> stdopt_map;
  static std::unordered_map<LOCALPROCID, bool> complete_map;
  static std::mutex stdopt_mutex;

  void cwd_from_proc_id(PROCID proc_id, char **buffer) {
    *buffer = nullptr;
    if (!proc_id_exists(proc_id)) return;
    #if defined(_WIN32)
    HANDLE proc = open_process_with_debug_privilege(proc_id);
    if (proc == nullptr) return;
    #if defined(PROCESS_WIN32EXE_INCLUDES)
    if (is_x86_process(GetCurrentProcess()) != is_x86_process(proc)) {
      std::string exe = directory_get_temporary_path();
      if (is_x86_process(proc)) exe = exe + "\\process32.exe";
      else exe = exe + "\\process64.exe";
      if (!file_exists(exe.c_str())) {
        FILE *file = nullptr; std::wstring wexe = widen(exe);
        if (_wfopen_s(&file, wexe.c_str(), L"wb") == 0) {
          if (is_x86_process(proc)) {
            #if defined(_WIN64)
            fwrite((char *)process32, sizeof(char), sizeof(process32), file);
            #endif
          } else {
            #if !defined(_WIN64)
            fwrite((char *)process64, sizeof(char), sizeof(process64), file);
            #endif
          }
          fclose(file);
        }
      }
      LOCALPROCID ind = process_execute(("\"" + exe + "\" --cwd-from-pid " + std::to_string(proc_id)).c_str());
      static std::string str; if (stdopt_map.find(ind) != stdopt_map.end()) str = stdopt_map.find(ind)->second;
      if (!str.empty() && std::count(str.begin(), str.end(), '\\') > 1 && str.back() == '\\') {
        static std::string substr; substr = str.substr(0, str.length() - 1);
        *buffer = (char *)substr.c_str();
      } else {
        *buffer = (char *)str.c_str();
      }
      free_executed_process_standard_output(ind);
    } else {
    #endif
      wchar_t *cwdbuf = nullptr;
      cwd_cmd_env_from_proc(proc, &cwdbuf, MEMCWD);
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
    #if defined(PROCESS_WIN32EXE_INCLUDES)
    }
    #endif
    CloseHandle(proc);
    #elif (defined(__APPLE__) && defined(__MACH__))
    proc_vnodepathinfo vpi = { 0 };
    char cwd[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidinfo(proc_id, PROC_PIDVNODEPATHINFO, 0, &vpi, sizeof(vpi)) > 0) {
      strcpy(cwd, vpi.pvi_cdir.vip_path);
      static std::string str; str = cwd;
      *buffer = (char *)str.c_str();
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    char cwd[PATH_MAX];
    std::string symLink = "/proc/" + std::to_string(proc_id) + "/cwd";
    if (realpath(symLink.c_str(), cwd)) {
      static std::string str; str = cwd;
      *buffer = (char *)str.c_str();
    }
    #elif defined(__FreeBSD__)
    char cwd[PATH_MAX]; unsigned cntp;
    procstat *proc_stat = procstat_open_sysctl();
    if (proc_stat) {
      KINFO_PROC *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, proc_id, &cntp);
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
    #elif defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
    #if defined(__OpenBSD__)
    int mib[3]; std::size_t s = 0;
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC_CWD;
    mib[2] = proc_id;
    if (sysctl(mib, 3, nullptr, &s, nullptr, 0) == 0) {
      std::vector<char> str1; str1.resize(s, '\0');
      char *cwd = str1.data();
      if (sysctl(mib, 3, cwd, &s, nullptr, 0) == 0) {
        static std::string str2; str2 = cwd ? cwd : "\0";
        *buffer = (char *)str2.c_str();
      }
    }
    #else
    int mib[4]; std::size_t s = 0;
    mib[0] = CTL_KERN;
    #if defined(__NetBSD__)
    mib[1] = KERN_PROC_ARGS;
    mib[2] = proc_id;
    mib[3] = KERN_PROC_CWD;
    #else
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_CWD;
    mib[3] = proc_id;
    #endif
    if (sysctl(mib, 4, nullptr, &s, nullptr, 0) == 0) {
      std::vector<char> str1; str1.resize(s, '\0');
      char *cwd = str1.data();
      if (sysctl(mib, 4, cwd, &s, nullptr, 0) == 0) {
        static std::string str2; str2 = cwd ? cwd : "\0";
        *buffer = (char *)str2.c_str();
      }
    }
    #endif
    #endif
  }

  const char *cwd_from_proc_id(PROCID proc_id) {
    char *cwd = nullptr; cwd_from_proc_id(proc_id, &cwd);
    return cwd;
  }

  void free_cmdline(char **buffer) {
    if (buffer) {
      delete[] buffer;
    }
  }

  static std::vector<std::string> cmdline_vec_1;
  void cmdline_from_proc_id(PROCID proc_id, char ***buffer, int *size) {
    *buffer = nullptr; *size = 0;
    cmdline_vec_1.clear(); int i = 0;
    if (!proc_id_exists(proc_id)) return;
    #if defined(_WIN32)
    HANDLE proc = open_process_with_debug_privilege(proc_id);
    if (proc == nullptr) return;
    #if defined(PROCESS_WIN32EXE_INCLUDES)
    if (is_x86_process(GetCurrentProcess()) != is_x86_process(proc)) {
      std::string exe = directory_get_temporary_path();
      if (is_x86_process(proc)) exe = exe + "\\process32.exe";
      else exe = exe + "\\process64.exe";
      if (!file_exists(exe.c_str())) {
        FILE *file = nullptr; std::wstring wexe = widen(exe);
        if (_wfopen_s(&file, wexe.c_str(), L"wb") == 0) {
          if (is_x86_process(proc)) {
            #if defined(_WIN64)
            fwrite((char *)process32, sizeof(char), sizeof(process32), file);
            #endif
          } else {
            #if !defined(_WIN64)
            fwrite((char *)process64, sizeof(char), sizeof(process64), file);
            #endif
          }
          fclose(file);
        }
      }
      LOCALPROCID ind = process_execute(("\"" + exe + "\" --cmd-from-pid " + std::to_string(proc_id)).c_str());
      std::string str;  if (stdopt_map.find(ind) != stdopt_map.end()) str = stdopt_map.find(ind)->second;
      char *cmd = str.data();
      int j = 0; if (!str.empty()) {
        while (cmd[j] != '\0') {
          message_pump();
          cmdline_vec_1.push_back(&cmd[j]); i++;
          j += (int)(strlen(cmd + j) + 1);
        }
      }
      free_executed_process_standard_output(ind);
    } else {
    #endif
      wchar_t *cmdbuf = nullptr; int cmdsize = 0;
      cwd_cmd_env_from_proc(proc, &cmdbuf, MEMCMD);
      if (cmdbuf) {
        wchar_t **cmdline = CommandLineToArgvW(cmdbuf, &cmdsize);
        if (cmdline) {
          while (i < cmdsize) {
            message_pump();
            cmdline_vec_1.push_back(narrow(cmdline[i])); i++;
          }
          LocalFree(cmdline);
        }
        delete[] cmdbuf;
      }
    #if defined(PROCESS_WIN32EXE_INCLUDES)
    }
    #endif
    CloseHandle(proc);
    #elif (defined(__APPLE__) && defined(__MACH__))
    char **cmdline = nullptr; int cmdsiz = 0;
    cmd_env_from_proc_id(proc_id, &cmdline, &cmdsiz, MEMCMD);
    if (cmdline) {
      for (int j = 0; j < cmdsiz; j++) {
        cmdline_vec_1.push_back(cmdline[i]); i++;
      }
      delete[] cmdline;
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    PROCTAB *proc = openproc(PROC_FILLCOM | PROC_PID, &proc_id);
    if (proc_t *proc_info = readproc(proc, nullptr)) {
      if (proc_info->cmdline) {
        for (int j = 0; proc_info->cmdline[j]; j++) {
          cmdline_vec_1.push_back(proc_info->cmdline[j]); i++;
        }
      }
      freeproc(proc_info);
    }
    closeproc(proc);
    #elif defined(__FreeBSD__)
    procstat *proc_stat = procstat_open_sysctl(); unsigned cntp = 0;
    if (proc_stat) {
      KINFO_PROC *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, proc_id, &cntp);
      if (proc_info) {
        char **cmdline = procstat_getargv(proc_stat, proc_info, 0);
        if (cmdline) {
          for (int j = 0; cmdline[j]; j++) {
            cmdline_vec_1.push_back(cmdline[j]); i++;
          }
          procstat_freeargv(proc_stat);
        }
        procstat_freeprocs(proc_stat, proc_info);
      }
      procstat_close(proc_stat);
    }
    #elif defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
    char errbuf[_POSIX2_LINE_MAX];
    KINFO_PROC *proc_info = nullptr; int cntp = 0;
    #if defined(__DragonFly__)
    const char *nlistf, *memf; nlistf = memf = "/dev/null";
    kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, proc_id, &cntp))) {
      char **cmdline = kvm_getargv(kd, proc_info, 0);
    #elif defined(__NetBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getproc2(kd, KERN_PROC_PID, proc_id, sizeof(struct KINFO_PROC), &cntp))) {
      char **cmdline = kvm_getargv2(kd, proc_info, 0);
    #elif defined(__OpenBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, proc_id, sizeof(struct KINFO_PROC), &cntp))) {
      char **cmdline = kvm_getargv(kd, proc_info, 0);
    #endif
      if (cmdline) {
        for (int j = 0; cmdline[j]; j++) {
          cmdline_vec_1.push_back(cmdline[j]); i++;
        }
      }
    }
    kvm_close(kd);
    #endif
    std::vector<char *> cmdline_vec_2;
    for (int i = 0; i < (int)cmdline_vec_1.size(); i++) {
      message_pump();
      cmdline_vec_2.push_back((char *)cmdline_vec_1[i].c_str());
    }
    char **arr = new char *[cmdline_vec_2.size()]();
    std::copy(cmdline_vec_2.begin(), cmdline_vec_2.end(), arr);
    *buffer = arr; *size = i;
  }

  void free_environ(char **buffer) {
    if (buffer) {
      delete[] buffer;
    }
  }

  static std::vector<std::string> environ_vec_1;
  void environ_from_proc_id(PROCID proc_id, char ***buffer, int *size) {
    *buffer = nullptr; *size = 0;
    environ_vec_1.clear(); int i = 0;
    if (!proc_id_exists(proc_id)) return;
    #if defined(_WIN32)
    HANDLE proc = open_process_with_debug_privilege(proc_id);
    if (proc == nullptr) return;
    #if defined(PROCESS_WIN32EXE_INCLUDES)
    if (is_x86_process(GetCurrentProcess()) != is_x86_process(proc)) {
      std::string exe = directory_get_temporary_path();
      if (is_x86_process(proc)) exe = exe + "\\process32.exe";
      else exe = exe + "\\process64.exe";
      if (!file_exists(exe.c_str())) {
        FILE *file = nullptr; std::wstring wexe = widen(exe);
        if (_wfopen_s(&file, wexe.c_str(), L"wb") == 0) {
          if (is_x86_process(proc)) {
            #if defined(_WIN64)
            fwrite((char *)process32, sizeof(char), sizeof(process32), file);
            #endif
          } else {
            #if !defined(_WIN64)
            fwrite((char *)process64, sizeof(char), sizeof(process64), file);
            #endif
          }
          fclose(file);
        }
      }
      LOCALPROCID ind = process_execute(("\"" + exe + "\" --env-from-pid " + std::to_string(proc_id)).c_str());
      std::string str; if (stdopt_map.find(ind) != stdopt_map.end()) str = stdopt_map.find(ind)->second;
      char *env = str.data();
      int j = 0; if (!str.empty()) {
        while (env[j] != '\0') {
          message_pump();
          environ_vec_1.push_back(&env[j]); i++;
          j += (int)(strlen(env + j) + 1);
        }
      }
      free_executed_process_standard_output(ind);
    } else {
    #endif
      wchar_t *wenv = nullptr;
      cwd_cmd_env_from_proc(proc, &wenv, MEMENV);
      int j = 0; if (wenv) {
        while (wenv[j] != L'\0') {
          message_pump();
          environ_vec_1.push_back(narrow(&wenv[j])); i++;
          j += (int)(wcslen(wenv + j) + 1);
        }
        delete[] wenv;
      }
    #if defined(PROCESS_WIN32EXE_INCLUDES)
    }
    #endif
    CloseHandle(proc);
    #elif (defined(__APPLE__) && defined(__MACH__))
    char **env = nullptr; int envsiz = 0;
    cmd_env_from_proc_id(proc_id, &env, &envsiz, MEMENV);
    if (env) {
      for (int j = 0; j < envsiz; j++) {
        environ_vec_1.push_back(env[i]); i++;
      }
      delete[] env;
    }
    #elif (defined(__linux__) && !defined(__ANDROID__))
    PROCTAB *proc = openproc(PROC_FILLENV | PROC_PID, &proc_id);
    if (proc_t *proc_info = readproc(proc, nullptr)) {
      if (proc_info->environ) {
        for (int j = 0; proc_info->environ[j]; j++) {
          environ_vec_1.push_back(proc_info->environ[j]); i++;
        }
      }
      freeproc(proc_info);
    }
    closeproc(proc);
    #elif defined(__FreeBSD__)
    procstat *proc_stat = procstat_open_sysctl(); unsigned cntp = 0;
    if (proc_stat) {
      KINFO_PROC *proc_info = procstat_getprocs(proc_stat, KERN_PROC_PID, proc_id, &cntp);
      if (proc_info) {
        char **env = procstat_getenvv(proc_stat, proc_info, 0);
        if (env) {
          for (int j = 0; env[j]; j++) {
            environ_vec_1.push_back(env[j]); i++;
          }
          procstat_freeenvv(proc_stat);
        }
        procstat_freeprocs(proc_stat, proc_info);
      }
      procstat_close(proc_stat);
    }
    #elif defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)
    char errbuf[_POSIX2_LINE_MAX];
    KINFO_PROC *proc_info = nullptr; int cntp = 0;
    #if defined(__DragonFly__)
    const char *nlistf, *memf; nlistf = memf = "/dev/null";
    kd = kvm_openfiles(nlistf, memf, nullptr, O_RDONLY, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, proc_id, &cntp))) {
      char **environ = kvm_getenvv(kd, proc_info, 0);
    #elif defined(__NetBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getproc2(kd, KERN_PROC_PID, proc_id, sizeof(struct KINFO_PROC), &cntp))) {
      char **environ = kvm_getenvv2(kd, proc_info, 0);
    #elif defined(__OpenBSD__)
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, errbuf); if (!kd) return;
    if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, proc_id, sizeof(struct KINFO_PROC), &cntp))) {
      char **environ = kvm_getenvv(kd, proc_info, 0);
    #endif
      if (environ) {
        for (int j = 0; environ[j]; j++) {
          environ_vec_1.push_back(environ[j]); i++;
        }
      }
    }
    kvm_close(kd);
    #endif
    std::vector<char *> environ_vec_2;
    for (int i = 0; i < (int)environ_vec_1.size(); i++) {
      message_pump();
      environ_vec_2.push_back((char *)environ_vec_1[i].c_str());
    }
    char **arr = new char *[environ_vec_2.size()]();
    std::copy(environ_vec_2.begin(), environ_vec_2.end(), arr);
    *buffer = arr; *size = i;
  }

  void environ_from_proc_id_ex(PROCID proc_id, const char *name, char **value) {
    char **buffer = nullptr; int size = 0; *value = (char *)"\0";
    environ_from_proc_id(proc_id, &buffer, &size);
    if (buffer) {
      for (int i = 0; i < size; i++) {
        std::vector<std::string> equalssplit = string_split_by_first_equals_sign(buffer[i]);
        for (int j = 0; j < (int)equalssplit.size(); j++) {
          std::string str1 = name;
          std::transform(equalssplit[0].begin(), equalssplit[0].end(), equalssplit[0].begin(), ::toupper);
          std::transform(str1.begin(), str1.end(), str1.begin(), ::toupper);
          if (j == equalssplit.size() - 1 && equalssplit[0] == str1) {
            if (str1.empty()) { *value = (char *)"\0"; } else {
              static std::string str2; str2 = equalssplit[j];
              *value = (char *)str2.c_str();
            }
          }
        }
      }
      free_environ(buffer);
    }
  }

  const char *environ_from_proc_id_ex(PROCID proc_id, const char *name) {
    char *value = (char *)"\0";
    environ_from_proc_id_ex(proc_id, name, &value);
    static std::string str; str = value;
    return str.c_str();
  }

  bool environ_from_proc_id_ex_exists(PROCID proc_id, const char *name) {
    char **buffer = nullptr; int size = 0; bool result = false;
    environ_from_proc_id(proc_id, &buffer, &size);
    if (buffer) {
      for (int i = 0; i < size; i++) {
        message_pump();
        std::vector<std::string> equalssplit = string_split_by_first_equals_sign(buffer[i]);
        if (!equalssplit.empty()) {
          std::string str = name;
          std::transform(equalssplit[0].begin(), equalssplit[0].end(), equalssplit[0].begin(), ::toupper);
          std::transform(str.begin(), str.end(), str.begin(), ::toupper);
          if (equalssplit[0] == str) {
            result = true;
          }
        }
      }
      free_environ(buffer);
    }
    return result;
  }

  const char *environment_get_variable(const char *name) {
    static std::string str;
    #if defined(_WIN32)
    char *value = (char *)"\0";
    environ_from_proc_id_ex(proc_id_from_self(), name, &value);
    str = value;
    #else
    char *value = getenv(name);
    str = value ? value : "\0";
    #endif
    return str.c_str();
  }

  bool environment_get_variable_exists(const char *name) {
    #if defined(_WIN32)
    return environ_from_proc_id_ex_exists(proc_id_from_self(), name);
    #else
    char *value = getenv(name);
    return (value != nullptr);
    #endif		
  }

  bool environment_set_variable(const char *name, const char *value) {
    #if defined(_WIN32)
    std::wstring u8name = widen(name);
    std::wstring u8value = widen(value);
    return (SetEnvironmentVariableW(u8name.c_str(), u8value.c_str()) != 0);
    #else
    return (setenv(name, value, 1) == 0);
    #endif
  }

  bool environment_unset_variable(const char *name) {
    #if defined(_WIN32)
    std::wstring u8name = widen(name);
    return (SetEnvironmentVariableW(u8name.c_str(), nullptr) != 0);
    #else
    return (unsetenv(name) == 0);
    #endif
  }

  static std::string tempdir;
  const char *directory_get_temporary_path() {
    #if defined(_WIN32)
    wchar_t tmp[MAX_PATH + 1];
    if (GetTempPathW(MAX_PATH + 1, tmp)) {
      tempdir = narrow(tmp);
      tempdir = string_replace_all(tempdir, "/", "\\");
      while (!tempdir.empty() && tempdir.back() == '\\') {
        tempdir.pop_back();
      }
      if (tempdir.find("\\") == std::string::npos) {
        tempdir += "\\";
      }
      return tempdir.c_str();
    }
    tempdir = directory_get_current_working();
    #else
    tempdir = environment_get_variable("TMPDIR"); 
    if (tempdir.empty()) tempdir = environment_get_variable("TMP");
    if (tempdir.empty()) tempdir = environment_get_variable("TEMP");
    if (tempdir.empty()) tempdir = environment_get_variable("TEMPDIR");
    if (tempdir.empty()) tempdir = "/tmp";
    struct stat sb = { 0 };
    if (stat(tempdir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
      while (!tempdir.empty() && tempdir.back() == '/') {
        tempdir.pop_back();
      }
      if (tempdir.find("/") == std::string::npos) {
        tempdir += "/";
      }
      return tempdir.c_str();
    }
    tempdir = directory_get_current_working();
    #endif
    return tempdir.c_str();
  }

  #if defined(PROCESS_GUIWINDOW_IMPL)
  #if (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(PROCESS_XQUARTZ_IMPL)
  static inline int x_error_handler_impl(Display *display, XErrorEvent *event) {
    return 0;
  }

  static inline int x_io_error_handler_impl(Display *display) {
    return 0;
  }

  static inline void set_error_handlers() {
    XSetErrorHandler(x_error_handler_impl);
    XSetIOErrorHandler(x_io_error_handler_impl);
  }
  #endif

  WINDOWID window_id_from_native_window(WINDOW window) {
    static std::string res;
    #if (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
    res = std::to_string((unsigned long long)[(NSWindow *)window windowNumber]);
    #else
    res = std::to_string((unsigned long long)window);
    #endif
    return (WINDOWID)res.c_str();
  }

  WINDOW native_window_from_window_id(WINDOWID win_id) {
    #if (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
    return (WINDOW)[NSApp windowWithWindowNumber:(CGWindowID)strtoul(win_id, nullptr, 10)];
    #else
    return (WINDOW)strtoull(win_id, nullptr, 10);
    #endif
  }

  static std::vector<std::string> wid_vec_1;
  void window_id_from_proc_id(PROCID proc_id, WINDOWID **win_id, int *size) {
    *win_id = nullptr; *size = 0;
    wid_vec_1.clear(); int i = 0;
    if (!proc_id_exists(proc_id)) return;
    #if defined(_WIN32)
    HWND hWnd = GetTopWindow(GetDesktopWindow());
    PROCID pid = 0; proc_id_from_window_id(window_id_from_native_window(hWnd), &pid);
    if (proc_id == pid) {
      wid_vec_1.push_back(window_id_from_native_window(hWnd)); i++;
    }
    while (hWnd = GetWindow(hWnd, GW_HWNDNEXT)) {
      message_pump();
      PROCID pid = 0; proc_id_from_window_id(window_id_from_native_window(hWnd), &pid);
      if (proc_id == pid) {
        wid_vec_1.push_back(window_id_from_native_window(hWnd)); i++;
      }
    }
    #elif (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
    CFArrayRef window_array = CGWindowListCopyWindowInfo(
    kCGWindowListOptionAll, kCGNullWindowID);
    CFIndex windowCount = 0;
    if ((windowCount = CFArrayGetCount(window_array))) {
      for (CFIndex j = 0; j < windowCount; j++) {
        CFDictionaryRef windowInfoDictionary =
        (CFDictionaryRef)CFArrayGetValueAtIndex(window_array, j);
        CFNumberRef ownerPID = (CFNumberRef)CFDictionaryGetValue(
        windowInfoDictionary, kCGWindowOwnerPID); PROCID pid = 0;
        CFNumberGetValue(ownerPID, kCFNumberIntType, &pid);
        if (proc_id == pid) {
          CFNumberRef windowID = (CFNumberRef)CFDictionaryGetValue(
          windowInfoDictionary, kCGWindowNumber);
          CGWindowID wid; CFNumberGetValue(windowID, kCGWindowIDCFNumberType, &wid);
          wid_vec_1.push_back(std::to_string((unsigned int)wid)); i++;
        }
      }
    }
    CFRelease(window_array);
    #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(PROCESS_XQUARTZ_IMPL)
    set_error_handlers();
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
          PROCID pid; proc_id_from_window_id(window_id_from_native_window(array[j]), &pid);
          if (proc_id == pid) {
            wid_vec_1.push_back(window_id_from_native_window(array[j])); i++;
          }
        }
      }
      XFree(prop);
    }
    XCloseDisplay(display);
    #endif
    std::vector<WINDOWID> wid_vec_2;
    for (int i = 0; i < (int)wid_vec_1.size(); i++) {
      message_pump();
      wid_vec_2.push_back((WINDOWID)wid_vec_1[i].c_str());
    }
    WINDOWID *arr = new WINDOWID[wid_vec_2.size()]();
    std::copy(wid_vec_2.begin(), wid_vec_2.end(), arr);
    *win_id = arr; *size = i;
  }

  void free_window_id(WINDOWID *win_id) {
    if (win_id) {
      delete[] win_id;
    }
  }

  static std::vector<std::string> wid_vec_3;
  void window_id_enumerate(WINDOWID **win_id, int *size) {
    *win_id = nullptr; *size = 0;
    wid_vec_3.clear(); int i = 0;
    PROCID *pid = nullptr; int pidsize = 0;
    proc_id_enumerate(&pid, &pidsize);
    if (pid) {
      for (int j = 0; j < pidsize; j++) {
        message_pump();
        WINDOWID *wid = nullptr; int widsize = 0;
        window_id_from_proc_id(pid[j], &wid, &widsize);
        if (wid) {
          for (int ii = 0; ii < widsize; ii++) {
            wid_vec_3.push_back(wid[ii]); i++;
          }
          free_window_id(wid);
        }
      }
    }
    std::vector<WINDOWID> widVec4;
    for (int i = 0; i < (int)wid_vec_3.size(); i++) {
      message_pump();
      widVec4.push_back((WINDOWID)wid_vec_3[i].c_str());
    }
    WINDOWID *arr = new WINDOWID[widVec4.size()]();
    std::copy(widVec4.begin(), widVec4.end(), arr);
    *win_id = arr; *size = i;
  }

  void proc_id_from_window_id(WINDOWID win_id, PROCID *proc_id) {
    *proc_id = 0;
    #if defined(_WIN32)
    DWORD pid = 0; GetWindowThreadProcessId(native_window_from_window_id(win_id), &pid);
    *proc_id = (PROCID)pid;
    #elif (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
    CFArrayRef window_array = CGWindowListCopyWindowInfo(
    kCGWindowListOptionIncludingWindow, (CGWindowID)strtol(win_id, nullptr, 10));
    CFIndex windowCount = 0;
    if ((windowCount = CFArrayGetCount(window_array))) {
      for (CFIndex i = 0; i < windowCount; i++) {
        CFDictionaryRef windowInfoDictionary =
        (CFDictionaryRef)CFArrayGetValueAtIndex(window_array, i);
        CFNumberRef ownerPID = (CFNumberRef)CFDictionaryGetValue(
        windowInfoDictionary, kCGWindowOwnerPID); PROCID pid = 0;
        CFNumberGetValue(ownerPID, kCFNumberIntType, &pid);
        WINDOWID *wid = nullptr; int size = 0;
        window_id_from_proc_id(pid, &wid, &size);
        if (wid) {
          for (int j = 0; j < size; j++) {
            if (strtoul(win_id, nullptr, 10) == strtoul(wid[j], nullptr, 10)) {
              *proc_id = pid;
              break;
            }
          }
          free_window_id(wid);
        }
      }
    }
    CFRelease(window_array);
    #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(PROCESS_XQUARTZ_IMPL)
    set_error_handlers();
    Display *display = XOpenDisplay(nullptr);
    unsigned long property = 0;
    unsigned char *prop = nullptr;
    Atom actual_type = 0, filter_atom = 0;
    int actual_format = 0, status = 0;
    unsigned long nitems = 0, bytes_after = 0;
    filter_atom = XInternAtom(display, "_NET_WM_PID", true);
    status = XGetWindowProperty(display, native_window_from_window_id(win_id), filter_atom, 0, 1000, false,
    AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    if (status == Success && prop != nullptr) {
      property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
      XFree(prop);
    }
    *proc_id = (PROCID)property;
    XCloseDisplay(display);
    #endif
    if (!proc_id_exists(*proc_id)) {
      *proc_id = 0;
    }
  }

  bool window_id_exists(WINDOWID win_id) {
    PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return proc_id_exists(proc_id);
    }
    return false;
  }

  bool window_id_suspend(WINDOWID win_id) {
    PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return proc_id_suspend(proc_id);
    }
    return false;
  }

  bool window_id_resume(WINDOWID win_id) {
    PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return proc_id_resume(proc_id);
    }
    return false;
  }

  bool window_id_kill(WINDOWID win_id) {
    PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return proc_id_kill(proc_id);
    }
    return false;
  }
  #endif

  static int procInfoIndex = -1;
  static int procListIndex = -1;
  static std::unordered_map<PROCINFO, PROCINFO_STRUCT *> proc_info_map;
  static std::vector<std::vector<PROCID>> proc_list_vec;

  PROCINFO proc_info_from_proc_id(PROCID proc_id) {
    KINFOFLAGS kinfo_flags = KINFO_EXEP | KINFO_CWDP | KINFO_PPID | KINFO_CPID | KINFO_ARGV | KINFO_ENVV;
    #if defined(PROCESS_GUIWINDOW_IMPL)
    kinfo_flags |= KINFO_OWID;
    #endif
    return proc_info_from_proc_id_ex(proc_id, kinfo_flags);
  }

  PROCINFO proc_info_from_proc_id_ex(PROCID proc_id, KINFOFLAGS kinfo_flags) {
    char *exe = nullptr; if (kinfo_flags & KINFO_EXEP) exe_from_proc_id(proc_id, &exe);
    char *cwd = nullptr; if (kinfo_flags & KINFO_CWDP) cwd_from_proc_id(proc_id, &cwd);
    PROCID ppid = 0;     if (kinfo_flags & KINFO_PPID) parent_proc_id_from_proc_id(proc_id, &ppid);
    PROCID *pid = nullptr; int pidsize = 0;
    if (kinfo_flags & KINFO_CPID) proc_id_from_parent_proc_id(proc_id, &pid, &pidsize);
    char **cmd = nullptr; int cmdsize = 0;
    if (kinfo_flags & KINFO_ARGV) cmdline_from_proc_id(proc_id, &cmd, &cmdsize);
    char **env = nullptr; int envsize = 0;
    if (kinfo_flags & KINFO_ENVV) environ_from_proc_id(proc_id, &env, &envsize);
    #if defined(PROCESS_GUIWINDOW_IMPL)
    WINDOWID *wid = nullptr; int widsize = 0;
    if (kinfo_flags & KINFO_OWID) window_id_from_proc_id(proc_id, &wid, &widsize);
    #endif
    PROCINFO_STRUCT *proc_info = new PROCINFO_STRUCT();
    proc_info->executable_image_file_path = exe;
    proc_info->current_working_directory  = cwd;
    proc_info->parent_process_id          = ppid;
    proc_info->child_process_id           = pid;
    proc_info->child_process_id_length    = pidsize;
    proc_info->commandline                = cmd;
    proc_info->commandline_length         = cmdsize;
    proc_info->environment                = env;
    proc_info->environment_length         = envsize;
    #if defined(PROCESS_GUIWINDOW_IMPL)
    proc_info->owned_window_id            = wid;
    proc_info->owned_window_id_length     = widsize;
    #endif
    procInfoIndex++; proc_info_map[procInfoIndex] = proc_info;
    return procInfoIndex;
  }

  char *executable_image_file_path(PROCINFO proc_info) { 
    return proc_info_map[proc_info]->executable_image_file_path ? proc_info_map[proc_info]->executable_image_file_path : (char *)"\0"; }
  char *current_working_directory(PROCINFO proc_info) { 
    return proc_info_map[proc_info]->current_working_directory ? proc_info_map[proc_info]->current_working_directory : (char *)"\0"; }
  PROCID parent_process_id(PROCINFO proc_info) { return proc_info_map[proc_info]->parent_process_id; }
  PROCID *child_process_id(PROCINFO proc_info) { return proc_info_map[proc_info]->child_process_id; }
  PROCID child_process_id(PROCINFO proc_info, int i) { return proc_info_map[proc_info]->child_process_id[i]; }
  int child_process_id_length(PROCINFO proc_info) { return proc_info_map[proc_info]->child_process_id_length; }
  char **commandline(PROCINFO proc_info) { return proc_info_map[proc_info]->commandline; }
  char *commandline(PROCINFO proc_info, int i) { return proc_info_map[proc_info]->commandline[i]; }
  int commandline_length(PROCINFO proc_info) { return proc_info_map[proc_info]->commandline_length; }
  char **environment(PROCINFO proc_info) { return proc_info_map[proc_info]->environment; }
  char *environment(PROCINFO proc_info, int i) { return proc_info_map[proc_info]->environment[i]; }
  int environment_length(PROCINFO proc_info) { return proc_info_map[proc_info]->environment_length; }
  #if defined(PROCESS_GUIWINDOW_IMPL)
  WINDOWID *owned_window_id(PROCINFO proc_info) { return proc_info_map[proc_info]->owned_window_id; }
  WINDOWID owned_window_id(PROCINFO proc_info, int i) { return proc_info_map[proc_info]->owned_window_id[i]; }
  int owned_window_id_length(PROCINFO proc_info) { return proc_info_map[proc_info]->owned_window_id_length; }
  #endif

  void free_proc_info(PROCINFO proc_info) {
    if (proc_info_map.find(proc_info) == proc_info_map.end()) return;
    free_proc_id(proc_info_map[proc_info]->child_process_id);
    free_cmdline(proc_info_map[proc_info]->commandline);
    free_environ(proc_info_map[proc_info]->environment);
    #if defined(PROCESS_GUIWINDOW_IMPL)
    free_window_id(proc_info_map[proc_info]->owned_window_id);
    #endif
    delete proc_info_map[proc_info];
    proc_info_map.erase(proc_info);
  }

  PROCLIST proc_list_create() {
    PROCID *proc_id = nullptr; int size = 0;
    proc_id_enumerate(&proc_id, &size);
    if (proc_id) {
      std::vector<PROCID> res;
      for (int i = 0; i < size; i++) {
        message_pump();
        res.push_back(proc_id[i]);
      }
      proc_list_vec.push_back(res);
      free_proc_id(proc_id);
    }
    procListIndex++;
    return procListIndex;
  }

  PROCID process_id(PROCLIST proc_list, int i) {
    std::vector<PROCID> proc_id = proc_list_vec[proc_list];
    return proc_id[i];
  }

  int process_id_length(PROCLIST proc_list) {
    return (int)proc_list_vec[proc_list].size();
  }

  void free_proc_list(PROCLIST proc_list) {
    proc_list_vec[proc_list].clear();
  }

  #if !defined(_WIN32)
  static inline PROCID process_execute_helper(const char *command, int *infp, int *outfp) {
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
      dup2(p_stdout[1], 2);
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

  static inline void output_thread(std::intptr_t file, LOCALPROCID proc_index) {
    #if !defined(_WIN32)
    ssize_t nRead = 0; char buffer[BUFSIZ];
    while ((nRead = read((int)file, buffer, BUFSIZ)) > 0) {
      buffer[nRead] = '\0';
    #else
    DWORD nRead = 0; char buffer[BUFSIZ];
    while (ReadFile((HANDLE)(void *)file, buffer, BUFSIZ, &nRead, nullptr) && nRead) {
      message_pump();
      buffer[nRead] = '\0';
    #endif
      std::lock_guard<std::mutex> guard(stdopt_mutex);
      stdopt_map[proc_index].append(buffer, nRead);
    }
  }

  static int index = -1;
  static std::unordered_map<int, PROCID> child_proc_id;
  static std::unordered_map<int, bool> proc_did_execute;
  static std::string standard_input;

  #if !defined(_WIN32)
  static inline PROCID proc_id_from_fork_proc_id(PROCID proc_id) {
    PROCID *pid = nullptr; int pidsize = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    proc_id_from_parent_proc_id(proc_id, &pid, &pidsize);
    if (pid) { if (pidsize) { proc_id = pid[pidsize - 1]; }
    free_proc_id(pid); }
    return proc_id;
  }
  #endif

  LOCALPROCID process_execute(const char *command) {
    index++;
    #if !defined(_WIN32)
    int infd = 0, outfd = 0;
    PROCID proc_id = 0, fork_proc_id = 0, wait_proc_id = 0;
    fork_proc_id = process_execute_helper(command, &infd, &outfd);
    proc_id = fork_proc_id; wait_proc_id = proc_id;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    if (fork_proc_id != -1) {
      while ((proc_id = proc_id_from_fork_proc_id(proc_id)) == wait_proc_id) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        int status; wait_proc_id = waitpid(fork_proc_id, &status, WNOHANG);
        char **cmd = nullptr; int cmdsize; cmdline_from_proc_id(fork_proc_id, &cmd, &cmdsize);
        if (cmd) { if (cmdsize && strcmp(cmd[0], "/bin/sh") == 0) {
        if (wait_proc_id > 0) proc_id = wait_proc_id; } free_cmdline(cmd); }
      }
    } else { proc_id = 0; }
    child_proc_id[index] = proc_id; std::this_thread::sleep_for(std::chrono::milliseconds(5));
    proc_did_execute[index] = true; LOCALPROCID proc_index = (LOCALPROCID)proc_id;
    stdipt_map.insert(std::make_pair(proc_index, (std::intptr_t)infd));
    std::thread opt_thread(output_thread, (std::intptr_t)outfd, proc_index);
    opt_thread.join();
    #else
    wchar_t cwstr_command[32767];
    std::wstring wstr_command = widen(command); bool proceed = true;
    wcsncpy_s(cwstr_command, 32767, wstr_command.c_str(), 32767);
    HANDLE stdin_read = nullptr; HANDLE stdin_write = nullptr;
    HANDLE stdout_read = nullptr; HANDLE stdout_write = nullptr;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, true };
    proceed = CreatePipe(&stdin_read, &stdin_write, &sa, 0);
    if (proceed == false) return 0;
    SetHandleInformation(stdin_write, HANDLE_FLAG_INHERIT, 0);
    proceed = CreatePipe(&stdout_read, &stdout_write, &sa, 0);
    if (proceed == false) return 0;
    STARTUPINFOW si = { 0 };
    si.cb = sizeof(STARTUPINFOW);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdError = stdout_write;
    si.hStdOutput = stdout_write;
    si.hStdInput = stdin_read;
    PROCESS_INFORMATION pi = { 0 }; LOCALPROCID proc_index = 0;
    if (CreateProcessW(nullptr, cwstr_command, nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
      CloseHandle(stdout_write);
      CloseHandle(stdin_read);
      PROCID proc_id = pi.dwProcessId; child_proc_id[index] = proc_id; proc_index = (LOCALPROCID)proc_id;
      std::this_thread::sleep_for(std::chrono::milliseconds(5)); proc_did_execute[index] = true;
      stdipt_map.insert(std::make_pair(proc_index, (std::intptr_t)(void *)stdin_write));
      HANDLE wait_handles[] = { pi.hProcess, stdout_read };
      std::thread opt_thread(output_thread, (std::intptr_t)(void *)stdout_read, proc_index);
      while (MsgWaitForMultipleObjects(2, wait_handles, false, 5, QS_ALLEVENTS) != WAIT_OBJECT_0) {
        message_pump();
      }
      opt_thread.join();
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
      CloseHandle(stdout_read);
      CloseHandle(stdin_write);
    } else {
      proc_did_execute[index] = true;
      child_proc_id[index] = 0;
    }
    #endif
    free_executed_process_standard_input(proc_index);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    complete_map[proc_index] = true;
    return proc_index;
  }

  LOCALPROCID process_execute_async(const char *command) {
    int prevIndex = index;
    std::thread proc_thread(process_execute, command);
    while (prevIndex == index) {
      message_pump();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    while (proc_did_execute.find(index) == proc_did_execute.end() || !proc_did_execute[index]) {
      message_pump();
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    LOCALPROCID proc_index = (LOCALPROCID)child_proc_id[index];
    complete_map[proc_index] = false;
    proc_thread.detach();
    return proc_index;
  }

  void executed_process_write_to_standard_input(LOCALPROCID proc_index, const char *input) {
    if (stdipt_map.find(proc_index) == stdipt_map.end()) return;
    std::string str = input;
    #if !defined(_WIN32)
    write((int)stdipt_map[proc_index], str.data(), str.length() + 1);
    #else
    DWORD dwwritten; WriteFile((HANDLE)(void *)stdipt_map[proc_index], str.data(),
    (DWORD)(str.length() + 1), &dwwritten, nullptr);
    #endif
  }

  const char *executed_process_read_from_standard_output(LOCALPROCID proc_index) {
    if (stdopt_map.find(proc_index) == stdopt_map.end()) return "\0";
    std::lock_guard<std::mutex> guard(stdopt_mutex);
    return stdopt_map.find(proc_index)->second.c_str();
  }

  void free_executed_process_standard_input(LOCALPROCID proc_index) {
    if (stdipt_map.find(proc_index) == stdipt_map.end()) return;
    stdipt_map.erase(proc_index);
  }

  void free_executed_process_standard_output(LOCALPROCID proc_index) {
    if (stdopt_map.find(proc_index) == stdopt_map.end()) return;
    stdopt_map.erase(proc_index);
  }

  bool completion_status_from_executed_process(LOCALPROCID proc_index) {
    if (complete_map.find(proc_index) == complete_map.end()) return false;
    return complete_map.find(proc_index)->second;
  }

  const char *current_process_read_from_standard_input() {
    standard_input = "";
    #if defined(_WIN32)
    DWORD bytes_avail = 0;
    HANDLE hpipe = GetStdHandle(STD_INPUT_HANDLE);
    if (PeekNamedPipe(hpipe, nullptr, 0, nullptr, &bytes_avail, nullptr)) {
      DWORD bytes_read = 0;    
      std::string buffer; buffer.resize(bytes_avail, '\0');
      if (PeekNamedPipe(hpipe, &buffer[0], bytes_avail, &bytes_read, nullptr, nullptr)) {
        standard_input = buffer;
      }
    }
    #else
    char buffer[BUFSIZ]; ssize_t nread = 0;
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0); if (-1 == flags) return "";
    while ((nread = read(fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK), buffer, BUFSIZ)) > 0) {
      buffer[nread] = '\0';
      standard_input.append(buffer, nread);
    }
    #endif
    return standard_input.c_str();
  }

} // namespace ngs::proc

#if defined(_WIN32)
#if !defined(PROCESS_WIN32EXE_INCLUDES)
int main(int argc, char **argv) {
  if (argc >= 3) {
    using namespace std::string_literals;
    if (strcmp(argv[1], "--cwd-from-pid") == 0) {
      char *buffer = nullptr;
      ngs::proc::cwd_from_proc_id(strtoul(argv[2], nullptr, 10), &buffer);
      if (buffer) {
        printf("%s", buffer);
      }
    } else if (strcmp(argv[1], "--cmd-from-pid") == 0) {
      char **buffer = nullptr; int size = 0;
      ngs::proc::cmdline_from_proc_id(strtoul(argv[2], nullptr, 10), &buffer, &size);
      if (buffer) {
        for (int i = 0; i < size; i++) {
          message_pump();
          std::cout << buffer[i] << "\0"s;
        }
        std::cout << "\0"s;
        ngs::proc::free_cmdline(buffer);
      }
    } else if (strcmp(argv[1], "--env-from-pid") == 0) {
      char **buffer = nullptr; int size = 0;
      ngs::proc::environ_from_proc_id(strtoul(argv[2], nullptr, 10), &buffer, &size);
      if (buffer) {
        for (int i = 0; i < size; i++) {
          message_pump();
          std::cout << buffer[i] << "\0"s;
        }
        std::cout << "\0"s;
        ngs::proc::free_environ(buffer);
      }
    }
  }
  return 0;
}
#endif
#endif
