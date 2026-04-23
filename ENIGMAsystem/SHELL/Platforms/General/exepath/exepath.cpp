/** Copyright (C) 2026 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "exepath.hpp"
#if defined(_WIN32)
#include <vector>
#include <cstddef>
#include <cstdlib>
#include <windef.h>
#include <stringapiset.h>
#include <libloaderapi.h>
#elif (defined(__APPLE__) && defined(__MACH__))
#include <TargetConditionals.h>
#if (defined(TARGET_OS_OSX) && TARGET_OS_OSX)
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <mach-o/dyld.h>
#include <libproc.h>
#include <unistd.h>
#endif
#elif defined(__linux__)
#include <climits>
#include <cstdlib>
#elif defined(__FreeBSD__)
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <sys/sysctl.h>
#elif defined(__DragonFly__)
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
#include <sys/sysctl.h>
#elif defined(__OpenBSD__)
#include <vector>
#include <sstream>
#include <cerrno>
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <kvm.h>
#elif defined(__sun)
#include <climits>
#include <cstdlib>
#endif

namespace exepath {

std::string get_executable_path() {
  std::string path;
  #if defined(_WIN32)
  auto narrow = [](std::wstring wstr) {
    if (wstr.empty()) return std::string("");
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
    std::vector<char> buf(nbytes);
    return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
  };
  wchar_t buffer[MAX_PATH];
  if (GetModuleFileNameW(nullptr, buffer, sizeof(buffer)) != 0) {
    wchar_t exe[MAX_PATH];
    if (_wfullpath(exe, buffer, MAX_PATH)) {
      path = narrow(exe);
    }
  }
  #elif (defined(__APPLE__) && defined(__MACH__) && defined(TARGET_OS_OSX) && TARGET_OS_OSX)
  char exe[PATH_MAX];
  std::uint32_t size = sizeof(exe);
  if (!_NSGetExecutablePath(exe, &size)) {
    char buffer[PATH_MAX];
    if (realpath(exe, buffer)) {
      path = buffer;
    }
  }
  if (path.empty()) {
    char exe[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(getpid(), exe, sizeof(exe)) > 0) {
      char buffer[PATH_MAX];
      if (realpath(exe, buffer)) {
        path = buffer;
      }
    }
  }
  #elif defined(__linux__)
  char exe[PATH_MAX];
  if (realpath("/proc/self/exe", exe)) {
    path = exe;
  }
  #elif defined(__FreeBSD__) || defined(__DragonFly__)
  int mib[4]; 
  std::size_t len = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  if (sysctl(mib, 4, nullptr, &len, nullptr, 0) == 0) {
    std::string strbuff;
    strbuff.resize(len, '\0');
    char *exe = strbuff.data();
    if (sysctl(mib, 4, exe, &len, nullptr, 0) == 0) {
      char buffer[PATH_MAX];
      if (realpath(exe, buffer)) {
        path = buffer;
      }
    }
  }
  #elif defined(__NetBSD__)
  int mib[4]; 
  std::size_t len = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC_ARGS;
  mib[2] = -1;
  mib[3] = KERN_PROC_PATHNAME;
  if (sysctl(mib, 4, nullptr, &len, nullptr, 0) == 0) {
    std::string strbuff;
    strbuff.resize(len, '\0');
    char *exe = strbuff.data();
    if (sysctl(mib, 4, exe, &len, nullptr, 0) == 0) {
      char buffer[PATH_MAX];
      if (realpath(exe, buffer)) {
        path = buffer;
      }
    }
  }
  #elif defined(__OpenBSD__)
  auto is_exe = [](std::string exe) {
    int cntp = 0;
    std::string res;
    kvm_t *kd = nullptr;
    kinfo_file *kif = nullptr;
    bool error = false;
    kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, nullptr);
    if (!kd) return res;
    if ((kif = kvm_getfiles(kd, KERN_FILE_BYPID, getpid(), sizeof(struct kinfo_file), &cntp))) {
      for (int i = 0; i < cntp && kif[i].fd_fd < 0; i++) {
        if (kif[i].fd_fd == KERN_FILE_TEXT) {
          struct stat st;
          fallback:
          char buffer[PATH_MAX];
          if (!stat(exe.c_str(), &st) && (st.st_mode & S_IXUSR) &&
            (st.st_mode & S_IFREG) && realpath(exe.c_str(), buffer) &&
            st.st_dev == (dev_t)kif[i].va_fsid && st.st_ino == (ino_t)kif[i].va_fileid) {
            res = buffer;
          }
          if (res.empty() && !error) {
            error = true;
            std::size_t last_slash_pos = exe.find_last_of("/");
            if (last_slash_pos != std::string::npos) {
              exe = exe.substr(0, last_slash_pos + 1) + kif[i].p_comm;
              goto fallback;
            }
          }
          break;
        }
      }
    }
    kvm_close(kd);
    return res;
  };
  auto cppstr_getenv = [](std::string name) {
    const char *cresult = getenv(name.c_str());
    std::string result = cresult ? cresult : "";
    return result;
  };
  int cntp = 0;
  kvm_t *kd = nullptr;
  kinfo_proc *proc_info = nullptr;
  std::vector<std::string> buffer;
  bool error = false, retried = false;
  kd = kvm_openfiles(nullptr, nullptr, nullptr, KVM_NO_FILES, nullptr);
  if (!kd) {
    path.clear();
    return path;
  }
  if ((proc_info = kvm_getprocs(kd, KERN_PROC_PID, getpid(), sizeof(struct kinfo_proc), &cntp))) {
    char **cmd = kvm_getargv(kd, proc_info, 0);
    if (cmd) {
      for (int i = 0; cmd[i]; i++) {
        buffer.push_back(cmd[i]);
      }
    }
  }
  kvm_close(kd);
  if (!buffer.empty()) {
    std::string argv0;
    if (!buffer[0].empty()) {
      fallback:
      std::size_t slash_pos = buffer[0].find('/');
      std::size_t colon_pos = buffer[0].find(':');
      if (slash_pos == 0) {
        argv0 = buffer[0];
        path = is_exe(argv0);
      } else if (slash_pos == std::string::npos || slash_pos > colon_pos) { 
        std::string penv = cppstr_getenv("PATH");
        if (!penv.empty()) {
          retry:
          std::string tmp;
          std::stringstream sstr(penv);
          while (std::getline(sstr, tmp, ':')) {
            argv0 = tmp + "/" + buffer[0];
            path = is_exe(argv0);
            if (!path.empty()) break;
            if (slash_pos > colon_pos) {
              argv0 = tmp + "/" + buffer[0].substr(0, colon_pos);
              path = is_exe(argv0);
              if (!path.empty()) break;
            }
          }
        }
        if (path.empty() && !retried) {
          retried = true;
          penv = "/usr/bin:/bin:/usr/sbin:/sbin:/usr/X11R6/bin:/usr/local/bin:/usr/local/sbin";
          std::string home = cppstr_getenv("HOME");
          if (!home.empty()) {
            penv = home + "/bin:" + penv;
          }
          goto retry;
        }
      }
      if (path.empty() && slash_pos > 0) {
        std::string pwd = cppstr_getenv("PWD");
        if (!pwd.empty()) {
          argv0 = pwd + "/" + buffer[0];
          path = is_exe(argv0);
        }
        if (path.empty()) {
          char cwd[PATH_MAX];
          if (getcwd(cwd, PATH_MAX)) {
            argv0 = std::string(cwd) + "/" + buffer[0];
            path = is_exe(argv0);
          }
        }
      }
    }
    if (path.empty() && !error) {
      error = true;
      buffer.clear();
      std::string underscore = cppstr_getenv("_");
      if (!underscore.empty()) {
        buffer.push_back(underscore);
        goto fallback;
      }
    }
  }
  if (!path.empty()) {
    errno = 0;
  }
  #elif defined(__sun)
  char exe[PATH_MAX];
  const char *execname = getexecname();
  if (execname) {
    if (realpath(execname, exe)) {
      path = exe;
      goto finish;
    }
  }
  if (realpath("/proc/self/path/a.out", exe)) {
    path = exe;
  }
  finish:
  #endif
  return path;
}

} // namespace exepath
