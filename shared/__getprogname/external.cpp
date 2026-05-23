/*

MIT License

Copyright © 2026 Samuel Venable

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

#include "external.h"
#include <string>
#if (defined(_WIN32) || defined(_WIN64))
#include <vector>
#include <cwchar>
#include <cstddef>
#include <cstdlib>
#include <windef.h>
#include <fileapi.h>
#include <stringapiset.h>
#include <processthreadsapi.h>
#include <securitybaseapi.h>
#include <libloaderapi.h>
#include <handleapi.h>
#include <winbase.h>
#include <intsafe.h>
#include <winnt.h>
#include <ntdef.h>
#elif (defined(__APPLE__) && defined(__MACH__))
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <mach-o/dyld.h>
#include <TargetConditionals.h>
#if (defined(TARGET_OS_OSX) && TARGET_OS_OSX)
#include <libproc.h>
#endif
#elif (defined(__linux__) || defined(__ANDROID__))
#include <climits>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#elif ((defined(__FreeBSD__) || defined(__FreeBSD_kernel__)) || defined(__DragonFly__))
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(__NetBSD__)
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#elif defined(__OpenBSD__)
#include <vector>
#include <sstream>
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <kvm.h>
#elif (defined(__sun) && defined(__SVR4))
#include <climits>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <libproc.h>
#elif (defined(__QNX__) || defined(__QNXNTO__))
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <process.h>
#elif defined(__HAIKU__)
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <image.h>
#include <OS.h>
#endif

const char *__getprogname(long long pid) {
  std::string path;
  if (pid < -1) {
    return nullptr;
  }
  #if (defined(_WIN32) || defined(_WIN64))
  DWORD processid = (DWORD)pid;
  auto resolve_symbolic_links = [](std::wstring wstr) {
    std::wstring result;
    wchar_t path[MAX_PATH];
    HANDLE hFile = CreateFileW(wstr.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) {
      DWORD len = GetFinalPathNameByHandleW(hFile, path, MAX_PATH, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
      if (len) {
        result = path;
        if (!result.substr(0, 8).compare(L"\\\\?\\UNC\\")) {
          result = L"\\" + result.substr(7);
        } else if (!result.substr(0, 4).compare(L"\\\\?\\")) {
          result = result.substr(4);
        }
      }
      CloseHandle(hFile);
    }
    return result;
  };
  auto narrow = [](std::wstring wstr) {
    if (wstr.empty()) return std::string("");
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
    if (!nbytes) return std::string("");
    std::vector<char> buf((size_t)nbytes);
    nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr);
    if (!nbytes) return std::string("");
    return std::string { buf.data(), (size_t)nbytes };
  };
  auto open_process_with_debug_privilege = [](DWORD processid) {
    HANDLE process = nullptr;
    HANDLE hToken = nullptr;
    LUID luid;
    TOKEN_PRIVILEGES tkp;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
      if (LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) {
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Luid = luid;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), nullptr, nullptr)) {
          process = OpenProcess(PROCESS_ALL_ACCESS, false, processid);
        }
      }
      CloseHandle(hToken);
    }
    if (!process) {
      process = OpenProcess(PROCESS_ALL_ACCESS, false, processid);
    }
    return process;
  };
  if (pid == -1 || processid == GetCurrentProcessId()) {
    wchar_t buffer[MAX_PATH];
    if (GetModuleFileNameW(nullptr, buffer, sizeof(buffer))) {
      std::wstring exe = resolve_symbolic_links(buffer);
      path = narrow(exe);
    }
  } else {
    HANDLE process = open_process_with_debug_privilege(processid);
    if (!process) { 
      return nullptr;
    }
    wchar_t buffer[MAX_PATH];
    DWORD size = sizeof(buffer);
    if (QueryFullProcessImageNameW(process, 0, buffer, &size)) {
      std::wstring exe = resolve_symbolic_links(buffer);
      path = narrow(exe);
    }
    CloseHandle(process);
  }
  #elif (defined(__APPLE__) && defined(__MACH__))
  pid_t processid = (pid_t)pid;
  if (processid == -1 || processid == getpid()) {
    char buffer[PATH_MAX];
    uint32_t size = sizeof(buffer);
    if (!_NSGetExecutablePath(buffer, &size)) {
      char exe[PATH_MAX];
      if (realpath(buffer, exe)) {
        path = exe;
      }
    }
  #if (defined(TARGET_OS_OSX) && TARGET_OS_OSX)
  } else {
    char buffer[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(processid, buffer, sizeof(buffer)) > 0) {
      char exe[PATH_MAX];
      if (realpath(buffer, exe)) {
        path = exe;
      }
    }
  #endif
  }
  #elif (defined(__linux__) || defined(__ANDROID__))
  pid_t processid = (pid_t)pid;
  char exe[PATH_MAX];
  if (processid == -1 || processid == getpid()) {
    if (realpath("/proc/self/exe", exe)) {
      path = exe;
    }
  } else {
    if (realpath((std::string("/proc/") + std::to_string(processid) + 
      std::string("/exe")).c_str(), exe)) {
      path = exe;
    }
  }
  #elif ((defined(__FreeBSD__) || defined(__FreeBSD_kernel__)) || defined(__DragonFly__))
  pid_t processid = (pid_t)pid;
  int mib[4]; 
  size_t len = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = processid;
  if (!sysctl(mib, 4, nullptr, &len, nullptr, 0)) {
    std::string strbuff;
    strbuff.resize(len, '\0');
    char *buffer = strbuff.data();
    if (!sysctl(mib, 4, buffer, &len, nullptr, 0)) {
      char exe[PATH_MAX];
      if (realpath(buffer, exe)) {
        path = exe;
      }
    }
  }
  #elif defined(__NetBSD__)
  pid_t processid = (pid_t)pid;
  int mib[4]; 
  size_t len = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC_ARGS;
  mib[2] = processid;
  mib[3] = KERN_PROC_PATHNAME;
  if (!sysctl(mib, 4, nullptr, &len, nullptr, 0)) {
    std::string strbuff;
    strbuff.resize(len, '\0');
    char *buffer = strbuff.data();
    if (!sysctl(mib, 4, buffer, &len, nullptr, 0)) {
      char exe[PATH_MAX];
      if (realpath(buffer, exe)) {
        path = exe;
      }
    }
  }
  #elif defined(__OpenBSD__)
  pid_t processid = (pid_t)pid;
  auto verifyexeex = [](std::string exe, pid_t processid) {
    int cntp = 0;
    std::string res;
    kvm_t *kd = nullptr;
    kinfo_file *kif = nullptr;
    bool error1 = false, error2 = false;
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, nullptr);
    if (kd) {
      if ((kif = kvm_getfiles(kd, KERN_FILE_BYPID, (processid == -1) ? getpid() : processid, sizeof(struct kinfo_file), &cntp))) {
        for (int i = 0; i < cntp && kif[i].fd_fd < 0; i++) {
          if (kif[i].fd_fd == KERN_FILE_TEXT) {
            fallback:
            struct stat st;
            char buffer[PATH_MAX];
            if (!stat(exe.c_str(), &st) && (st.st_mode & S_IXUSR) &&
              S_ISREG(st.st_mode) && realpath(exe.c_str(), buffer) &&
              st.st_dev == (dev_t)kif[i].va_fsid && st.st_ino == (ino_t)kif[i].va_fileid) {
              res = buffer;
            }
            if (res.empty() && !error1) {
              error1 = true;
              size_t last_slash_pos = exe.find_last_of("/");
              if (last_slash_pos != std::string::npos) {
                exe = exe.substr(0, last_slash_pos + 1) + kif[i].p_comm;
                goto fallback;
              }
            }
            if (res.empty() && !error2 && (processid == -1 || processid == getpid())) {
              error2 = true;
              size_t last_slash_pos = exe.find_last_of("/");
              if (last_slash_pos != std::string::npos) {
                const char *progname = getprogname();
                if (progname) {
                  exe = exe.substr(0, last_slash_pos + 1) + progname;
                  goto fallback;
                }
              }
            }
            break;
          }
        }
      }
      kvm_close(kd);
    }
    return res;
  };
  auto cppgetenvex = [](std::string name, pid_t processid) {
    if (processid == -1 || processid == getpid()) {
      const char *cvalue = getenv(name.c_str());
      return std::string(cvalue ? cvalue : "");
    }
    auto cppenvironex = [](pid_t processid) {
      std::vector<std::string> vec;
      int cntp = 0;
      kvm_t *kd = nullptr;
      kinfo_proc *process_info = nullptr;
      kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, nullptr);
      if (kd) {
        if ((process_info = kvm_getprocs(kd, KERN_PROC_PID, processid, sizeof(struct kinfo_proc), &cntp))) {
          char **env = kvm_getenvv(kd, process_info, 0);
          if (env) {
            for (int i = 0; env[i]; i++) {
              vec.push_back(env[i]);
            }
          }
        }
        kvm_close(kd);
      }
      return vec;
    };
    auto string_split_by_first_equals_sign = [](std::string str) {
      size_t pos = 0;
      std::vector<std::string> vec;
      if ((pos = str.find_first_of("=")) != std::string::npos) {
        vec.push_back(str.substr(0, pos));
        vec.push_back(str.substr(pos + 1));
      }
      return vec;
    };
    std::string value;
    if (name.empty()) {
      return value;
    }
    std::vector<std::string> vec = cppenvironex(processid);
    if (!vec.empty()) {
      for (size_t i = 0; i < vec.size(); i++) {
        std::vector<std::string> equalssplit = string_split_by_first_equals_sign(vec[i]);
        if (equalssplit.size() == 2) {
          if (equalssplit[0] == name) {
            value = equalssplit[1];
            break;
          }
        }
      }
    }
    return value;
  };
  int cntp = 0;
  std::string buffer;
  kvm_t *kd = nullptr;
  kinfo_proc *process_info = nullptr;
  bool error = false, retried = false, leading_dash_removed = false;
  kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, nullptr);
  if (kd) {
    if ((process_info = kvm_getprocs(kd, KERN_PROC_PID, (processid == -1) ? getpid() : processid, sizeof(struct kinfo_proc), &cntp))) {
      char **cmd = kvm_getargv(kd, process_info, 0);
      if (cmd && cmd[0]) {
        buffer = cmd[0];
      }
    }
    kvm_close(kd);
  }
  std::string argv0;
  bool argv0_does_not_exist = false;
  size_t slash_pos = std::string::npos;
  size_t colon_pos = std::string::npos;
  if (buffer.empty()) {
    argv0_does_not_exist = true;
    goto path_lookup;
  } else {
    fallback:
    slash_pos = buffer.find('/');
    colon_pos = buffer.find(':');
    if (slash_pos == 0) {
      argv0 = buffer;
      path = verifyexeex(argv0, processid);
    } else if (slash_pos == std::string::npos || (colon_pos != std::string::npos && colon_pos > 0 && slash_pos > colon_pos)) {
      path_lookup:
      retry_without_leading_dash:
      std::string penv = cppgetenvex("PATH", processid);
      if (!penv.empty()) {
        retry:
        std::string tmp;
        std::stringstream sstr(penv);
        while (std::getline(sstr, tmp, ':')) {
          argv0 = tmp + "/" + buffer;
          path = verifyexeex(argv0, processid);
          if (!path.empty()) break;
          if (!argv0_does_not_exist && colon_pos != std::string::npos && colon_pos > 0 && slash_pos > colon_pos) {
            argv0 = tmp + "/" + buffer.substr(0, colon_pos);
            path = verifyexeex(argv0, processid);
            if (!path.empty()) break;
          }
        }
      }
      if (path.empty() && !retried) {
        retried = true;
        penv = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11R6/bin:/usr/local/bin:/usr/local/sbin";
        std::string home = cppgetenvex("HOME", processid);
        if (!home.empty()) {
          penv = home + "/bin:" + penv;
        }
        goto retry;
      }
      if (path.empty() && !argv0_does_not_exist && !leading_dash_removed && slash_pos == std::string::npos && buffer.length() > 1 && buffer[0] == '-') {
        buffer = buffer.substr(1);
        retried = false;
        leading_dash_removed = true;
        goto retry_without_leading_dash;
      }
    }
    if (path.empty() && (argv0_does_not_exist || (slash_pos != std::string::npos && slash_pos > 0))) {
      std::string pwd = cppgetenvex("PWD", processid);
      if (!pwd.empty()) {
        argv0 = pwd + "/" + buffer;
        path = verifyexeex(argv0, processid);
      }
      if (path.empty()) {
        if (processid == -1 || processid == getpid()) {
          char cwd[PATH_MAX];
          if (getcwd(cwd, PATH_MAX)) {
            argv0 = std::string(cwd) + "/" + buffer;
            path = verifyexeex(argv0, processid);
          }
        } else {
          int mib[3];
          size_t len = 0;
          mib[0] = CTL_KERN;
          mib[1] = KERN_PROC_CWD;
          mib[2] = processid;
          if (!sysctl(mib, 3, nullptr, &len, nullptr, 0)) {
            std::vector<char> vecbuff;
            vecbuff.resize(len);
            char *cwd = &vecbuff[0];
            if (!sysctl(mib, 3, cwd, &len, nullptr, 0)) {
              argv0 = std::string(cwd) + "/" + buffer;
              path = verifyexeex(argv0, processid);
            }
          }
        }
      }
    }
    if (path.empty() && !error) {
      error = true;
      buffer.clear();
      std::string underscore = cppgetenvex("_", processid);
      if (!underscore.empty()) {
        buffer = underscore;
        leading_dash_removed = false;
        retried = false;
        goto fallback;
      }
    }
  }
  if (path.empty() && !argv0_does_not_exist) {
    argv0_does_not_exist = true;
    retried = false;
    buffer.clear();
    goto path_lookup;
  }
  #elif (defined(__sun) && defined(__SVR4))
  pid_t processid = (pid_t)pid;
  if (processid == -1 || processid == getpid()) {
    const char *execname = getexecname();
    if (execname) {
      char exe[PATH_MAX];
      if (realpath(execname, exe)) {
        path = exe;
      }
    }
  } else {
    int err = 0;
    char buffer[PATH_MAX];
    struct ps_prochandle *P = nullptr;
    P = Pgrab(processid, PGRAB_RDONLY, &err);
    if (P) {
      if (!err) {
        if (Pexecname(P, buffer, sizeof(buffer))) {
          char exe[PATH_MAX];
          if (realpath(buffer, exe)) {
            path = exe;
          }
        }
      }
      Pfree(P);
    }
  }
  if (path.empty()) {
    char exe[PATH_MAX];
    if (processid == -1 || processid == getpid()) {
      if (realpath("/proc/self/path/a.out", exe)) {
        path = exe;
      }
    } else {
      if (realpath((std::string("/proc/") + std::to_string(processid) + 
        std::string("/path/a.out")).c_str(), exe)) {
        path = exe;
      }
    }
  }
  #elif (defined(__QNX__) || defined(__QNXNTO__))
  pid_t processid = (pid_t)pid;
  if (processid == -1 || processid == getpid()) {
    char buffer[PATH_MAX];
    if(_cmdname(buffer)) {
      char exe[PATH_MAX];
      if (realpath(buffer, exe)) {
        path = exe;
      }
    }
    if (path.empty()) {
      FILE *fp = fopen("/proc/self/exefile", "r");
      if (fp) {
        char buffer[PATH_MAX];
        if (fgets(buffer, sizeof(buffer), fp)) {
          char exe[PATH_MAX];
          if (realpath(buffer, exe)) {
            path = exe;
          }
        }
        fclose(fp);
      }
    }
  } else {
    FILE *fp = fopen((std::string("/proc/") + std::to_string(processid) + 
      std::string("/exefile")).c_str(), "r");
    if (fp) {
      char buffer[PATH_MAX];
      if (fgets(buffer, sizeof(buffer), fp)) {
        char exe[PATH_MAX];
        if (realpath(buffer, exe)) {
          path = exe;
        }
      }
      fclose(fp);
    }
  }
  #elif defined(__HAIKU__)
  team_id processid = (team_id)pid;
  image_info info;
  int32_t cookie = 0;
  while (get_next_image_info((processid == -1) ? B_CURRENT_TEAM : processid, &cookie, &info) == B_OK) {
    if (info.type == B_APP_IMAGE) {
      char exe[PATH_MAX];
      if (realpath(info.name, exe)) {
        path = exe;
      }
      break;
    }
  }
  #endif
  if (path.empty()) return nullptr;
  size_t pos = path.find_last_of("/\\");
  static std::string result; 
  result = path.substr(pos + 1);
  return result.c_str();
}
