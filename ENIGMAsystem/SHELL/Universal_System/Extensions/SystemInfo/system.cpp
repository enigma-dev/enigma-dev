/*

 MIT License
 
 Copyright © 2023 Samuel Venable
 
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

#if (defined(_WIN32) && defined(_MSC_VER))
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#if (!defined(_WIN32) && (!defined(__APPLE__) && !defined(__MACH__)))
#include <SDL.h>
#include <SDL_opengl.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#endif
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <dxgi.h>
#else
#if defined(__linux__)
#include <sys/sysinfo.h>
#endif
#if ((defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun))
#include <sys/types.h>
#if (defined(__FreeBSD__) || defined(__DragonFly__))
#include <fcntl.h>
#include <kvm.h>
#elif (defined(__NetBSD__) || defined(__OpenBSD__))
#include <sys/param.h>
#include <sys/swap.h>
#endif
#endif
#if !defined(__sun)
#if !defined(__linux__)
#include <sys/sysctl.h>
#endif
#include <sys/utsname.h>
#else
#include <sys/systeminfo.h>
#include <sys/swap.h>
#endif
#include <unistd.h>
#endif
#if (defined(_WIN32) && defined(_MSC_VER))
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dxgi.lib")
#endif

#include "pci.ids.hpp"
#include "system.hpp"

namespace ngs::sys {

/* Define CREATE_CONTEXT in your build scripts or Makefiles if
the calling process hasn't already done this on its own ... */
#if (!defined(_WIN32) && (!defined(__APPLE__) && !defined(__MACH__)))
#if defined(CREATE_CONTEXT)
static SDL_Window *window = nullptr;
static bool create_context() {
  if (!window) {
    #if (defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun))
    setenv("SDL_VIDEODRIVER", "x11", 1);
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    #endif
    if (SDL_Init(SDL_INIT_VIDEO)) return false;
    window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window) return false;
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) return false;
    int err = SDL_GL_MakeCurrent(window, context);
    if (err) return false;
  }
  return true;
}
#endif
#endif

#if defined(_WIN32)
static std::string read_output(std::wstring cmd) {
  std::string result;
  bool proceed = true;
  HANDLE stdin_read = nullptr; HANDLE stdin_write = nullptr;
  HANDLE stdout_read = nullptr; HANDLE stdout_write = nullptr;
  SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, true };
  proceed = CreatePipe(&stdin_read, &stdin_write, &sa, 0);
  if (!proceed) return "";
  SetHandleInformation(stdin_write, HANDLE_FLAG_INHERIT, 0);
  proceed = CreatePipe(&stdout_read, &stdout_write, &sa, 0);
  if (!proceed) return "";
  STARTUPINFOW si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = stdout_write;
  si.hStdOutput = stdout_write;
  si.hStdInput = stdin_read;
  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(pi));
  std::vector<wchar_t> ccmd;
  ccmd.resize(cmd.length() + 1, L'\0');
  wcsncpy_s(&ccmd[0], cmd.length() + 1, cmd.c_str(), cmd.length() + 1);
  BOOL success = CreateProcessW(nullptr, &ccmd[0], nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
  if (success) {
    DWORD nRead = 0;
    char buffer[BUFSIZ];
    CloseHandle(stdout_write);
    CloseHandle(stdin_read);
    HANDLE wait_handles[] = { pi.hProcess, stdout_read };
    while (MsgWaitForMultipleObjects(2, wait_handles, false, 5, QS_ALLEVENTS) != WAIT_OBJECT_0) {
      MSG msg;
      while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      while (ReadFile(stdout_read, buffer, BUFSIZ, &nRead, nullptr) && nRead) {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
        buffer[nRead] = '\0';
        result.append(buffer, nRead);
      }
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(stdout_read);
    CloseHandle(stdin_write);
  }
  return result;
}

static std::string windows_version(std::string *product_name) {
  auto GetOSMajorVersionNumber = []() {
    const char *result = nullptr;
    char buf[1024];
    int val = 0;
    DWORD sz = sizeof(val);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CurrentMajorVersionNumber", RRF_RT_REG_DWORD, nullptr, &val, &sz) == ERROR_SUCCESS) {
      if (sprintf(buf, "%d", val) != -1) {
        result = buf;
      }
    }
    std::string str;
    str = result ? result : "";
    return str;
  };
  auto GetOSMinorVersionNumber = []() {
    const char *result = nullptr;
    char buf[1024];
    int val = 0;
    DWORD sz = sizeof(val);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CurrentMinorVersionNumber", RRF_RT_REG_DWORD, nullptr, &val, &sz) == ERROR_SUCCESS) {
      if (sprintf(buf, "%d", val) != -1) {
        result = buf;
      }
    }
    std::string str;
    str = result ? result : "";
    return str;
  };
  auto GetOSBuildNumber = []() {
    const char *result = nullptr;
    char buf[1024];
    DWORD sz = sizeof(buf);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "CurrentBuildNumber", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
      result = buf;
    }
    std::string str;
    str = result ? result : "";
    return str;
  };
  auto GetOSRevisionNumber = []() {
    char *result = nullptr;
    char buf[1924];
    int val = 0;
    DWORD sz = sizeof(val);
    if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "UBR", RRF_RT_REG_DWORD, nullptr, &val, &sz) == ERROR_SUCCESS) {
      if (sprintf(buf, "%d", val) != -1) {
        result = buf;
      }
    }
    std::string str;
    str = strlen(result) ? result : "";
    return str;
  };
  static const char *result = nullptr;
  char buf[1024];
  if (!GetOSMajorVersionNumber().empty() && !GetOSMinorVersionNumber().empty() && !GetOSBuildNumber().empty() && !GetOSRevisionNumber().empty()) {
    if (sprintf(buf, "%s.%s.%s.%s", GetOSMajorVersionNumber().c_str(), GetOSMinorVersionNumber().c_str(), GetOSBuildNumber().c_str(), GetOSRevisionNumber().c_str()) != -1) {
      result = buf;
      char buf[1024];
      DWORD sz = sizeof(buf);
      if (RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", "ProductName", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
        if (strtoull(GetOSMajorVersionNumber().c_str(), nullptr, 10) == 10 && strtoull(GetOSBuildNumber().c_str(), nullptr, 10) >= 22000) {
          std::string tmp = strlen(buf) ? buf : "";
          if (!tmp.empty()) {
            tmp = std::regex_replace(tmp, std::regex("10"), "11");
            *product_name = tmp;
          }
        } else {
          *product_name = strlen(buf) ? buf : "";
        }
      }
    }
  }
  std::string str;
  str = result ? result : "";
  return str;
}
#endif

struct HumanReadable {
  long double size = 0;
  private: friend
  std::ostream& operator<<(std::ostream& os, HumanReadable hr) {
    int i = 0;
    long double mantissa = hr.size;
    for (; mantissa >= 1024; mantissa /= 1024, i++) { }
    mantissa = std::ceil(mantissa * 100) / 100;
    os << mantissa << " " << "BKMGTPE"[i];
    return !i ? os : os << "B";
  }
};

std::string human_readable(long double nbytes) {
  if (nbytes == -1) return "";
  std::stringstream ss;
  ss << HumanReadable{nbytes};
  return ss.str();
}

std::string utsname_sysname() {
  #if !defined(_WIN32)
  #if !defined(__sun)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.sysname;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  std::string res;
  long count = sysinfo(SI_SYSNAME, nullptr, 0);
  if (count > 0) {
    char *buf = (char *)calloc(count, sizeof(char));
    if (buf) {
      if (sysinfo(SI_SYSNAME, buf, count) > 0) {
        res = buf;
      }
      free(buf);
    }
  }
  return res;
  #endif
  #else
  const char *result = nullptr;
  char buf[1024];
  DWORD sz = sizeof(buf);
  if (RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\", "OS", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
    result = buf;
  }
  std::string str;
  str = result ? result : "";
  return str;
  #endif
}

std::string utsname_nodename() {
  #if !defined(_WIN32)
  #if !defined(__sun)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.nodename;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  std::string res;
  long count = sysinfo(SI_HOSTNAME, nullptr, 0);
  if (count > 0) {
    char *buf = (char *)calloc(count, sizeof(char));
    if (buf) {
      if (sysinfo(SI_HOSTNAME, buf, count) > 0) {
        res = buf;
      }
      free(buf);
    }
  }
  return res;
  #endif
  #else
  const char *result = nullptr;
  char buf[1024];
  WSADATA data;
  WORD wVersionRequested;
  wVersionRequested = MAKEWORD(2, 2);
  if (!WSAStartup(wVersionRequested, &data)) {
    if (!gethostname(buf, sizeof(buf))) {
      result = buf;
    }
    WSACleanup();
  }
  std::string str;
  str = result ? result : "";
  return str;
  #endif
}

std::string utsname_release() {
  #if !defined(_WIN32)
  #if !defined(__sun)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.release;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  std::string res;
  long count = sysinfo(SI_RELEASE, nullptr, 0);
  if (count > 0) {
    char *buf = (char *)calloc(count, sizeof(char));
    if (buf) {
      if (sysinfo(SI_RELEASE, buf, count) > 0) {
        res = buf;
      }
      free(buf);
    }
  }
  return res;
  #endif
  #else
  std::string product_name;
  return windows_version(&product_name);
  #endif
}

std::string utsname_version() {
  #if !defined(_WIN32)
  #if !defined(__sun)
  #if !defined(__DragonFly__)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.version;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("uname -v", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #endif
  #else
  std::string res;
  long count = sysinfo(SI_VERSION, nullptr, 0);
  if (count > 0) {
    char *buf = (char *)calloc(count, sizeof(char));
    if (buf) {
      if (sysinfo(SI_VERSION, buf, count) > 0) {
        res = buf;
      }
      free(buf);
    }
  }
  return res;
  #endif
  #else
  std::string result = read_output(L"cmd /c ver");
  if (!result.empty()) {
    result = std::regex_replace(result, std::regex("\r"), "");
    result = std::regex_replace(result, std::regex("\n"), "");
    static std::string res;
    res = result;
	return res;
  }
  return "";
  #endif
}

std::string utsname_codename() {
  #if defined(_WIN32)
  std::string product_name;
  windows_version(&product_name);
  return product_name;
  #elif (defined(__APPLE__) && defined(__MACH__))
  std::string result;
  FILE *fp = popen("echo $(sw_vers | grep 'ProductName:' | uniq | awk 'NR==1{$1=$1;print}' && sw_vers | grep 'ProductVersion:' | uniq | awk 'NR==1{$1=$1;print}')", "r");
  if (fp) {
    char buf[1024];
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = strlen(buf) ? buf : "";
      result = std::regex_replace(result, std::regex("ProductName: "), "");
      result = std::regex_replace(result, std::regex("ProductVersion: "), "");
      std::fstream doc;
      doc.open("/System/Library/CoreServices/Setup Assistant.app/Contents/Resources/en.lproj/OSXSoftwareLicense.rtf", std::ios::in);
      if (doc.is_open()){
        std::string tmp1;
        while(std::getline(doc, tmp1)) {
          std::string tmp2 = tmp1;
          std::transform(tmp1.begin(), tmp1.end(), tmp1.begin(), ::toupper);
          std::size_t pos1 = tmp1.find("SOFTWARE LICENSE AGREEMENT FOR MAC OS X");
          std::size_t pos2 = tmp1.find("SOFTWARE LICENSE AGREEMENT FOR MACOS");
          std::size_t len1 = strlen("SOFTWARE LICENSE AGREEMENT FOR MAC OS X");
          std::size_t len2 = strlen("SOFTWARE LICENSE AGREEMENT FOR MACOS");
          if (pos1 != std::string::npos) {
            result += tmp2.substr(pos1 + len1);
            result = result.substr(0, result.length() - 1);
            break;
          } else if (pos2 != std::string::npos) {
            result += tmp2.substr(pos2 + len2);
            result = result.substr(0, result.length() - 1);
            break;
          }
        }
        doc.close();
      }
    }
  }
  static std::string str = result;
  return str;
  #elif defined(__linux__)
  std::string result;
  FILE *fp = popen("echo $(lsb_release --id && lsb_release --release && lsb_release --codename) |  tr '\n' ' '", "r");
  if (fp) {
    char buf[1024];
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      std::string tmp = strlen(buf) ? buf : "";
      if (!tmp.empty()) {
        tmp = std::regex_replace(tmp, std::regex("Distributor ID: "), "");
        tmp = std::regex_replace(tmp, std::regex("Release: "), "");
        tmp = std::regex_replace(tmp, std::regex("Codename: "), "");
      }
      result = tmp;
    }
    pclose(fp);
  }
  static std::string str = result;
  return str;
  #elif !defined(__sun)
  return utsname_sysname() + " " + utsname_release();
  #else
  char *buf = nullptr;
  std::size_t len = 0;
  std::string buffer;
  FILE *file = popen("awk 'NR==1{print $1,$2,$3}' /etc/release", "r");
  if (file) {
    if (getline(&buf, &len, file) != -1) {
      buffer = buf;
      free(buf);
    }
    pclose(file);
  }
  if (!buffer.empty() && buffer.back() == '\n')
    buffer.pop_back();
  static std::string result;
  result = buffer;
  return result;
  #endif
  return "";
}

std::string utsname_machine() {
  #if !defined(_WIN32)
  #if !defined(__sun)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.machine;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  std::string res;
  long count = sysinfo(SI_ARCHITECTURE_NATIVE, nullptr, 0);
  if (count > 0) {
    char *buf = (char *)calloc(count, sizeof(char));
    if (buf) {
      if (sysinfo(SI_ARCHITECTURE_NATIVE, buf, count) > 0) {
        res = buf;
      }
      free(buf);
    }
  }
  return res;
  #endif
  #else
  const char *result = nullptr;
  char buf[1024];
  DWORD sz = sizeof(buf);
  if (RegGetValueA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\\", "PROCESSOR_ARCHITECTURE", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
    result = buf;
  }
  std::string str;
  str = result ? result : "";
  return str;
  #endif
}

long long memory_totalram() {
  #if defined(_WIN32)
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (long long)statex.ullTotalPhys;
  }
  return -1;
  #elif ((defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  int mib[2];
  long long physical_memory = -1;
  mib[0] = CTL_HW;
  #if (defined(__APPLE__) && defined(__MACH__))
  mib[1] = HW_MEMSIZE;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  mib[1] = HW_PHYSMEM;
  #else
  mib[1] = HW_PHYSMEM64;
  #endif
  std::size_t len = sizeof(long long);
  if (!sysctl(mib, 2, &physical_memory, &len, nullptr, 0)) {
    return physical_memory;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return (info.totalram * info.mem_unit);
  }
  return -1;
  #elif defined(__sun)
  char buf[1024];
  long long total = -1;
  const char *result = nullptr;
  FILE *fp = popen("prtconf | grep 'Memory size:' | uniq | cut -d' ' -f3- | awk '{print $1 * 1024};'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    total = strtoll(str.c_str(), nullptr, 10) * 1024;
  }
  return total;
  #else
  return -1;
  #endif
}

long long memory_availram() {
  #if defined(_WIN32)
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (long long)statex.ullAvailPhys;
  }
  return -1;
  #elif ((defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  int mib[2];
  long long user_memory = 0;
  mib[0] = CTL_HW;
  #if ((defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__DragonFly__))
  mib[1] = HW_USERMEM;
  #else
  mib[1] = HW_USERMEM64;
  #endif
  std::size_t len = sizeof(long long);
  if (!sysctl(mib, 2, &user_memory, &len, nullptr, 0)) {
    return user_memory;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return (info.freeram * info.mem_unit);
  }
  return -1;
  #elif defined(__sun)
  return (sysconf(_SC_AVPHYS_PAGES) * sysconf(_SC_PAGESIZE));
  #else
  return -1;
  #endif
}

long long memory_usedram() {
  #if defined(_WIN32)
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (long long)(statex.ullTotalPhys - statex.ullAvailPhys);
  }
  return -1;
  #elif ((defined(__APPLE__) && defined(__MACH__)) ||defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun))
  long long total = memory_totalram();
  long long avail = memory_availram();
  if (total != -1 && avail != -1) {
    return total - avail;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return ((info.totalram - info.freeram) * info.mem_unit);
  }
  return -1;
  #else
  return -1;
  #endif
}

long long memory_totalvmem() {
  #if defined(_WIN32)
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (long long)statex.ullTotalPageFile;
  }
  return -1;
  #elif (defined(__APPLE__) && defined(__MACH__))
  struct xsw_usage info;
  std::size_t len = sizeof(info);
  if (!sysctlbyname("vm.swapusage", &info, &len, nullptr, 0)) {
    return info.xsu_total;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return (info.totalswap * info.mem_unit);
  }
  return -1;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  kvm_t *kvmh = nullptr;
  long page_s = sysconf(_SC_PAGESIZE);
  kvmh = kvm_open(nullptr, "/dev/null", "/dev/null", O_RDONLY, nullptr);
  if (!kvmh) return -1;
  struct kvm_swap k_swap;
  if (kvm_getswapinfo(kvmh, &k_swap, 1, 0) != -1) {
    kvm_close(kvmh);
    return k_swap.ksw_total * page_s;
  }
  kvm_close(kvmh);
  return -1;
  #elif (defined(__NetBSD__) || defined(__OpenBSD__))
  long long total = 0;
  long block_s = 0;
  int header_len = 0;
  getbsize(&header_len, &block_s);
  int nswap = swapctl(SWAP_NSWAP, nullptr, 0);
  if (!nswap) return 0;
  if (nswap > 0) {
    struct swapent *swaps = (struct swapent *)calloc(nswap, sizeof(struct swapent));
    if (swaps) {
      if (swapctl(SWAP_STATS, swaps, nswap) > 0) {
        for (int i = 0; i < nswap; i++) {
          if (swaps[i].se_flags & SWF_ENABLE) {
            total += ((swaps[i].se_nblks / (1024 / block_s)) * 1024);
          }
        }
      }
      free(swaps);
    }
    return total;
  }
  return -1;
  #elif defined(__sun)
  int i = 0, n = 0;
  long long total = 0;
  long page_s = sysconf(_SC_PAGESIZE);
again:
  int nswap = swapctl(SC_GETNSWP, nullptr);
  if (!nswap) return 0;
  if (nswap > 0) {
    swaptbl_t *swaps = (swaptbl_t *)malloc((nswap * sizeof(swapent_t)) + sizeof(struct swaptable));
    if (swaps) {
      char *strtab = (char *)malloc((nswap + 1) * 80);
      if (strtab) {
        for (int i = 0; i < (nswap + 1); i++) {
          swaps->swt_ent[i].ste_path = strtab + (i * 80);
        }
        swaps->swt_n = nswap + 1;
        if ((n = swapctl(SC_LIST, swaps)) > 0) {
          if (n > nswap) {
            free(strtab);
            free(swaps);
            goto again;
          }
          for (int i = 0; i < n; i++) {
            total += (swaps->swt_ent[i].ste_pages * page_s);
          }
        }
        free(strtab);
      }
      free(swaps);
    }
    return total;
  }
  return -1;
  #else
  return -1;
  #endif
}

long long memory_availvmem() {
  #if defined(_WIN32)
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (long long)statex.ullAvailPageFile;
  }
  return -1;
  #elif (defined(__APPLE__) && defined(__MACH__))
  struct xsw_usage info;
  std::size_t len = sizeof(info);
  if (!sysctlbyname("vm.swapusage", &info, &len, nullptr, 0)) {
    return info.xsu_avail;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return (info.freeswap * info.mem_unit);
  }
  return -1;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  kvm_t *kvmh = nullptr;
  long page_s = sysconf(_SC_PAGESIZE);
  kvmh = kvm_open(nullptr, "/dev/null", "/dev/null", O_RDONLY, nullptr);
  if (!kvmh) return -1;
  struct kvm_swap k_swap;
  if (kvm_getswapinfo(kvmh, &k_swap, 1, 0) != -1) {
    kvm_close(kvmh);
    return (k_swap.ksw_total - k_swap.ksw_used) * page_s;
  }
  kvm_close(kvmh);
  return -1;
  #elif (defined(__NetBSD__) || defined(__OpenBSD__))
  long long avail = 0;
  long block_s = 0;
  int header_len = 0;
  getbsize(&header_len, &block_s);
  int nswap = swapctl(SWAP_NSWAP, nullptr, 0);
  if (!nswap) return 0;
  if (nswap > 0) {
    struct swapent *swaps = (struct swapent *)calloc(nswap, sizeof(struct swapent));
    if (swaps) {
      if (swapctl(SWAP_STATS, swaps, nswap) > 0) {
        for (int i = 0; i < nswap; i++) {
          if (swaps[i].se_flags & SWF_ENABLE) {
            avail += (((swaps[i].se_nblks - swaps[i].se_inuse) / (1024 / block_s)) * 1024);
          }
        }
      }
      free(swaps);
    }
    return avail;
  }
  return -1;
  #elif defined(__sun)
  int i = 0, n = 0;
  long long avail = 0;
  long page_s = sysconf(_SC_PAGESIZE);
again:
  int nswap = swapctl(SC_GETNSWP, nullptr);
  if (!nswap) return 0;
  if (nswap > 0) {
    swaptbl_t *swaps = (swaptbl_t *)malloc((nswap * sizeof(swapent_t)) + sizeof(struct swaptable));
    if (swaps) {
      char *strtab = (char *)malloc((nswap + 1) * 80);
      if (strtab) {
        for (int i = 0; i < (nswap + 1); i++) {
          swaps->swt_ent[i].ste_path = strtab + (i * 80);
        }
        swaps->swt_n = nswap + 1;
        if ((n = swapctl(SC_LIST, swaps)) > 0) {
          if (n > nswap) {
            free(strtab);
            free(swaps);
            goto again;
          }
          for (i = 0; i < n; i++) {
            avail += (swaps->swt_ent[i].ste_free * page_s);
          }
        }
        free(strtab);
      }
      free(swaps);
    }
    return avail;
  }
  return -1;
  #else
  return -1;
  #endif
}

long long memory_usedvmem() {
  #if defined(_WIN32)
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof(statex);
  if (GlobalMemoryStatusEx(&statex)) {
    return (long long)(statex.ullTotalPageFile - statex.ullAvailPageFile);
  }
  return -1;
  #elif (defined(__APPLE__) && defined(__MACH__))
  struct xsw_usage info;
  std::size_t len = sizeof(info);
  if (!sysctlbyname("vm.swapusage", &info, &len, nullptr, 0)) {
    return info.xsu_used;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return ((info.totalswap - info.freeswap) * info.mem_unit);
  }
  return -1;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  kvm_t *kvmh = nullptr;
  long page_s = sysconf(_SC_PAGESIZE);
  kvmh = kvm_open(nullptr, "/dev/null", "/dev/null", O_RDONLY, nullptr);
  if (!kvmh) return -1;
  struct kvm_swap k_swap;
  if (kvm_getswapinfo(kvmh, &k_swap, 1, 0) != -1) {
    kvm_close(kvmh);
    return k_swap.ksw_used * page_s;
  }
  kvm_close(kvmh);
  return -1;
  #elif (defined(__NetBSD__) || defined(__OpenBSD__))
  long long used = 0;
  long block_s = 0;
  int header_len = 0;
  getbsize(&header_len, &block_s);
  int nswap = swapctl(SWAP_NSWAP, nullptr, 0);
  if (!nswap) return 0;
  if (nswap > 0) {
    struct swapent *swaps = (struct swapent *)calloc(nswap, sizeof(struct swapent));
    if (swaps) {
      if (swapctl(SWAP_STATS, swaps, nswap) > 0) {
        for (int i = 0; i < nswap; i++) {
          if (swaps[i].se_flags & SWF_ENABLE) {
            used += ((swaps[i].se_inuse / (1024 / block_s)) * 1024);
          }
        }
      }
      free(swaps);
    }
    return used;
  }
  return -1;
  #elif defined(__sun)
  int i = 0, n = 0;
  long long used = 0;
  long page_s = sysconf(_SC_PAGESIZE);
again:
  int nswap = swapctl(SC_GETNSWP, nullptr);
  if (!nswap) return 0;
  if (nswap > 0) {
    swaptbl_t *swaps = (swaptbl_t *)malloc((nswap * sizeof(swapent_t)) + sizeof(struct swaptable));
    if (swaps) {
      char *strtab = (char *)malloc((nswap + 1) * 80);
      if (strtab) {
        for (int i = 0; i < (nswap + 1); i++) {
          swaps->swt_ent[i].ste_path = strtab + (i * 80);
        }
        swaps->swt_n = nswap + 1;
        if ((n = swapctl(SC_LIST, swaps)) > 0) {
          if (n > nswap) {
            free(strtab);
            free(swaps);
            goto again;
          }
          for (i = 0; i < n; i++) {
            used += ((swaps->swt_ent[i].ste_pages - swaps->swt_ent[i].ste_free) * page_s);
          }
        }
        free(strtab);
      }
      free(swaps);
    }
    return used;
  }
  return -1;
  #else
  return -1;
  #endif
}

std::vector<std::string> string_split(std::string str, char delimiter) {
  std::vector<std::string> vec;
  std::stringstream sstr(str);
  std::string tmp;
  while (std::getline(sstr, tmp, delimiter)) {
    #if defined(_WIN32)
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    #endif
    vec.push_back(tmp);
  }
  return vec;
}

unsigned int DeviceId = 0;
unsigned int VendorId = 0;
std::unordered_map<unsigned int, std::string> VendorNameById;
std::unordered_map<unsigned int, std::string> DeviceNameById;
std::string GetVendorOrDeviceNameById(unsigned int Id, int VendorOrDevice) {
  if (VendorNameById.find(Id) != VendorNameById.end() && !VendorOrDevice) return VendorNameById[Id];
  if (DeviceNameById.find(Id) != DeviceNameById.end() && VendorOrDevice) return DeviceNameById[Id];
  std::string str(pci_ids, pci_ids + sizeof(pci_ids) / sizeof(pci_ids[0]));
  str = std::regex_replace(str, std::regex("\r"), "");
  std::vector<std::string> vec = string_split(str, '\n');
  for (std::size_t i = 0; i < vec.size(); i++) {
    #if defined(_WIN32)
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    #endif
    if (vec[i].empty() || (!vec[i].empty() && vec[i][0] == '#')) {
      continue;
    }
    std::size_t pos1 = vec[i].find(" ");
    if (pos1 != std::string::npos && vec[i][0] != '\t') {
      std::istringstream converter1(vec[i].substr(0, pos1));
      converter1 >> std::hex >> VendorId;
      VendorNameById[VendorId] = vec[i].substr(pos1 + 2);
    }
    if (VendorNameById[VendorId] != VendorNameById[Id]) {
      continue;
    }
    std::size_t pos2 = vec[i].find("\t\t");
    if (pos2 == std::string::npos && vec[i][0] == '\t') {
      std::size_t pos3 = vec[i].find(" ");
      if (pos3 != std::string::npos) {
        std::istringstream converter2(vec[i].substr(1, 4));
        converter2 >> std::hex >> DeviceId;
        DeviceNameById[DeviceId] = &vec[i].substr(pos3)[2];
      }
    }
  }
  return VendorOrDevice ? DeviceNameById[Id] : VendorNameById[Id];
}

static std::string gpuvendor;
std::string gpu_vendor() {
  if (!gpuvendor.empty()) return gpuvendor;
  std::string result;
  #if defined(_WIN32)
  IDXGIFactory *pFactory = nullptr;
  if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory) == S_OK) {
    IDXGIAdapter *pAdapter = nullptr;
    if (pFactory->EnumAdapters(0, &pAdapter) == S_OK) {
      DXGI_ADAPTER_DESC adapterDesc;
      if (pAdapter->GetDesc(&adapterDesc) == S_OK) {
        result = GetVendorOrDeviceNameById(adapterDesc.VendorId, 0);
      }
      pAdapter->Release();
    }
    pFactory->Release();
  }
  #elif (!defined(__APPLE__) && !defined(__MACH__))
  #if defined(CREATE_CONTEXT)
  if (!create_context()) return "";
  #endif
  #if defined(__sun)
  char buf[1024];
  FILE *fp = popen("prtconf |  awk '/display/{p=3} p > 0 {print $0; p--}'| awk -F'pci' 'NR==3{print $0}' | sed 's/.*pci//g' | awk -F' ' '{print $1}' | awk -F',' '{print $1}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (strlen(buf)) {
        unsigned int Id = 0;
        std::istringstream converter(buf);
        converter >> std::hex >> Id;
        result = strlen(buf) ? GetVendorOrDeviceNameById(Id, 0) : "";
      }
    }
    pclose(fp);
  }
  if (!result.empty()) {
    return result;
  }
  #endif
  unsigned int v = 0;
  PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC queryInteger;
  queryInteger = (PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC)glXGetProcAddressARB((const GLubyte *)"glXQueryCurrentRendererIntegerMESA");
  queryInteger(GLX_RENDERER_VENDOR_ID_MESA, &v);
  result = v ? GetVendorOrDeviceNameById(v, 0) : "";
  if (result.empty()) {
    PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC queryString;
    queryString = (PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC)glXGetProcAddressARB((const GLubyte *)"glXQueryCurrentRendererStringMESA");
    result = queryString(GLX_RENDERER_VENDOR_ID_MESA);
  }
  #else
  char buf[1024];
  FILE *fp = popen("ioreg -bls | grep -n2 '    | |   | |   \"model\" = <\"' | awk -F',\"pci' 'NR==5{print $2}' | rev | cut -c 2- | rev | awk -F',' '{print $1}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (strlen(buf)) {
        unsigned int Id = 0;
        std::istringstream converter(buf);
        converter >> std::hex >> Id;
        result = strlen(buf) ? GetVendorOrDeviceNameById(Id, 0) : "";
      }
    }
    pclose(fp);
  }
  if (result.empty()) {
    char buf[1024];
    FILE *fp = popen("system_profiler SPDisplaysDataType | grep -i 'Vendor: ' | uniq | awk -F 'Vendor: ' 'NR==1{$1=$1;print}' | awk 'NR==1{$1=$1;print}'", "r");
    if (fp) {
      if (fgets(buf, sizeof(buf), fp)) {
        buf[strlen(buf) - 1] = '\0';
        result = buf;
      }
      pclose(fp);
    }
  }
  #endif
  gpuvendor = result;
  return result;
}

static std::string gpurenderer;
std::string gpu_renderer() {
  if (!gpurenderer.empty()) return gpurenderer;
  std::string result;
  #if defined(_WIN32)
  auto narrow = [](std::wstring wstr) {
    if (wstr.empty()) return std::string("");
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
    std::vector<char> buf(nbytes);
    return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
  };
  IDXGIFactory *pFactory = nullptr;
  if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory) == S_OK) {
    IDXGIAdapter *pAdapter = nullptr;
    if (pFactory->EnumAdapters(0, &pAdapter) == S_OK) {
      DXGI_ADAPTER_DESC adapterDesc;
      if (pAdapter->GetDesc(&adapterDesc) == S_OK) {
        result = GetVendorOrDeviceNameById(adapterDesc.DeviceId, 1);
        if (result.empty()) {
          result = narrow(adapterDesc.Description);
        }
      }
      pAdapter->Release();
    }
    pFactory->Release();
  }
  #elif (!defined(__APPLE__) && !defined(__MACH__))
  #if defined(CREATE_CONTEXT)
  if (!create_context()) return "";
  #endif
  #if defined(__sun)
  char buf[1024];
  FILE *fp = popen("prtconf | awk '/display/{p=3} p > 0 {print $0; p--}' | awk -F'pci' 'NR==3{print $0}' | sed 's/.*pci//g' | awk -F' ' '{print $1}' | awk -F',' '{print $2}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (strlen(buf)) {
        unsigned int Id = 0;
        std::istringstream converter(buf);
        converter >> std::hex >> Id;
        result = strlen(buf) ? GetVendorOrDeviceNameById(Id, 1) : "";
      }
    }
    pclose(fp);
  }
  if (!result.empty()) {
    return result;
  }
  #endif
  unsigned int v = 0;
  PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC queryInteger;
  queryInteger = (PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC)glXGetProcAddressARB((const GLubyte *)"glXQueryCurrentRendererIntegerMESA");
  queryInteger(GLX_RENDERER_DEVICE_ID_MESA, &v);
  result = v ? GetVendorOrDeviceNameById(v, 1) : "";
  if (result.empty()) {
    PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC queryString;
    queryString = (PFNGLXQUERYCURRENTRENDERERSTRINGMESAPROC)glXGetProcAddressARB((const GLubyte *)"glXQueryCurrentRendererStringMESA");
    result = queryString(GLX_RENDERER_DEVICE_ID_MESA);
  }
  #else
  char buf[1024];
  FILE *fp = popen("ioreg -bls | grep -n2 '    | |   | |   \"model\" = <\"' | awk -F',\"pci' 'NR==5{print $2}' | rev | cut -c 2- | rev | awk -F',' '{print $2}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (strlen(buf)) {
        unsigned int Id = 0;
        std::istringstream converter(buf);
        converter >> std::hex >> Id;
        result = strlen(buf) ? GetVendorOrDeviceNameById(Id, 1) : "";
      }
    }
    pclose(fp);
  }
  if (result.empty()) {
    char buf[1024];
    FILE *fp = popen("system_profiler SPDisplaysDataType | grep -i 'Chipset Model: ' | uniq | awk -F 'Chipset Model: ' 'NR==1{$1=$1;print}' | awk 'NR==1{$1=$1;print}'", "r");
    if (fp) {
      if (fgets(buf, sizeof(buf), fp)) {
        buf[strlen(buf) - 1] = '\0';
        result = buf;
      }
      pclose(fp);
    }
  }
  #endif
  gpurenderer = result;
  return result;
}

static long long videomemory = -1;
long long gpu_videomemory() {
  if (videomemory != -1) return videomemory;
  long long result = -1;
  #if defined(_WIN32)
  IDXGIFactory *pFactory = nullptr;
  if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory) == S_OK) {
    IDXGIAdapter *pAdapter = nullptr;
    if (pFactory->EnumAdapters(0, &pAdapter) == S_OK) {
      DXGI_ADAPTER_DESC adapterDesc;
      if (pAdapter->GetDesc(&adapterDesc) == S_OK) {
        result = (long long)adapterDesc.DedicatedVideoMemory;
        if (!result) {
          result = -1;
        }
      }
      pAdapter->Release();
    }
    pFactory->Release();
  }
  #elif (defined(__APPLE__) && defined(__MACH__))
  char buf[1024];
  FILE *fp = popen("ioreg -r -d 1 -w 0 -c \"IOAccelerator\" | grep '\"VRAM,totalMB\"' | uniq | awk -F '= ' '{print $2}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (strlen(buf)) {
        result = strtoll(buf, nullptr, 10) * 1024 * 1024;
      }
    }
    pclose(fp);
  }
  #else
  #if defined(CREATE_CONTEXT)
  if (!create_context()) return "";
  #endif
  unsigned int v = 0;
  PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC queryInteger;
  queryInteger = (PFNGLXQUERYCURRENTRENDERERINTEGERMESAPROC)glXGetProcAddressARB((const GLubyte *)"glXQueryCurrentRendererIntegerMESA");
  queryInteger(GLX_RENDERER_VIDEO_MEMORY_MESA, &v);
  v = v * 1024 * 1024;
  if (!v) return -1;
  result = v;
  #endif
  videomemory = result;
  return result;
}

std::string cpu_vendor() {
  #if defined(_WIN32)
  const char *result = nullptr;
  char buf[1024];
  DWORD sz = sizeof(buf);
  if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\\", "VendorIdentifier", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
    result = buf;
  }
  return result ? result : "";
  #elif (defined(__APPLE__) && defined(__MACH__))
  const char *result = nullptr;
  char buf[1024];
  std::size_t len = sizeof(buf);
  if (!sysctlbyname("machdep.cpu.vendor", &buf, &len, nullptr, 0)) {
    result = buf;
  }
  return result ? result : "";
  #elif defined(__linux__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("lscpu | grep 'Vendor ID:' | uniq | cut -d' ' -f3- | awk 'NR==1{$1=$1;print}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #elif defined(__FreeBSD__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("dmesg | awk '/CPU: /{p++;if(p==0){next}}p' | awk -F'Origin=\"' 'NR==2{print $0}' | sed 's/.*Origin=\"//g' | awk -F' ' '{print $1}' | awk -F',' '{print $1}' | awk '{print substr($0, 1, length($0) - 1)}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #elif defined(__DragonFly__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("dmesg | awk '/CPU: /{p++;if(p==0){next}}p' | awk -F'Origin = \"' 'NR==2{print $0}' | sed 's/.*Origin = \"//g' | awk -F' ' '{print $1}' | awk -F',' '{print $1}' | awk '{print substr($0, 1, length($0) - 1)}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #elif defined(__NetBSD__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("cat /proc/cpuinfo | grep 'vendor_id' | awk 'NR==1{print $3}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #elif defined(__OpenBSD__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("sysctl -n machdep.cpuvendor", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #elif defined(__sun)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("psrinfo -v -p | awk 'NR==2{print substr($2, 2)}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #else
  return "";
  #endif
}

std::string cpu_brand() {
  #if defined(_WIN32)
  const char *result = nullptr;
  char buf[1024];
  DWORD sz = sizeof(buf);
  if (RegGetValueA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\\", "ProcessorNameString", RRF_RT_REG_SZ, nullptr, &buf, &sz) == ERROR_SUCCESS) {
    result = buf;
  }
  return result ? result : "";
  #elif (defined(__APPLE__) && defined(__MACH__))
  const char *result = nullptr;
  char buf[1024];
  std::size_t len = sizeof(buf);
  if (!sysctlbyname("machdep.cpu.brand_string", &buf, &len, nullptr, 0)) {
    result = buf;
  }
  return result ? result : "";
  #elif defined(__linux__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("lscpu | grep 'Model name:' | uniq | cut -d' ' -f3- | awk 'NR==1{$1=$1;print}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    return str;
  }
  return "";
  #elif (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  int mib[2];
  char buf[1024];
  mib[0] = CTL_HW;
  mib[1] = HW_MODEL;
  std::size_t len = sizeof(buf);
  if (!sysctl(mib, 2, buf, &len, nullptr, 0)) {
    return strlen(buf) ? buf : "";
  }
  return "";
  #elif defined(__sun)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("psrinfo -v -p | awk 'NR==3{print}' | awk 'NR==1{$1=$1;print}''", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
  }
  static std::string str;
  str = result ? result : "";
  return str;
  #else
  return "";
  #endif
}

static int numcpus = -1;
static int numcores = -1;
int cpu_numcores() {
  if (numcores != -1) {
    return numcores;
  }
  #if defined(_WIN32)
  std::string result = read_output(L"wmic cpu get NumberOfCores");
  if (!result.empty()) {
    result = std::regex_replace(result, std::regex("NumberOfCores"), "");
    result = std::regex_replace(result, std::regex(" "), "");
    result = std::regex_replace(result, std::regex("\r"), "");
    result = std::regex_replace(result, std::regex("\n"), "");
    static std::string res;
    res = result;
    numcores = (int)strtol(res.c_str(), nullptr, 10);
  }
  return numcores;
  #elif (defined(__APPLE__) && defined(__MACH__))
  int physical_cpus = -1;
  std::size_t len = sizeof(int);
  if (!sysctlbyname("machdep.cpu.core_count", &physical_cpus, &len, nullptr, 0)) {
    numcores = physical_cpus;
  }
  return numcores;
  #elif defined(__linux__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("lscpu | grep 'Thread(s) per core:' | uniq | cut -d' ' -f4- | awk 'NR==1{$1=$1;print}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    numcores = (int)(cpu_numcpus() / strtol(str.c_str(), nullptr, 10));
  }
  return numcores;
  #elif defined(__FreeBSD__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("sysctl -n kern.sched.topology_spec | grep -c 'THREAD group'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    numcores = (int)strtol(str.c_str(), nullptr, 10);
    numcores = ((!numcores) ? cpu_numcpus() : numcores);
  }
  return numcores;
  #elif defined(__DragonFly__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("dmesg | grep 'threads_per_core: ' | awk '{print substr($6, 0, length($6) - 1)}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    numcores = (int)(cpu_numcpus() / strtol(str.c_str(), nullptr, 10));
  }
  return numcores;
  #elif (defined(__NetBSD__) || defined(__OpenBSD__))
  class CPUID {
    unsigned regs[4];
    public:
    explicit CPUID(unsigned funcId, unsigned subFuncId) {
      asm volatile ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3]) : "a" (funcId), "c" (subFuncId));
    }
    const unsigned &EAX() const { return regs[0]; }
    const unsigned &EBX() const { return regs[1]; }
    const unsigned &ECX() const { return regs[2]; }
    const unsigned &EDX() const { return regs[3]; }
  };
  static const unsigned AVX_POS = 0x10000000;
  static const unsigned LVL_CORES = 0x0000FFFF;
  CPUID cpuID0(0, 0);
  unsigned HFS = cpuID0.EAX();
  CPUID cpuID1(1, 0);
  int mNumSMT = 0;
  int mNumCores = 0;
  bool mIsHTT = cpuID1.EDX() & AVX_POS;
  std::string cpuvendor = cpu_vendor();
  numcpus = cpu_numcpus();
  if (cpuvendor == "GenuineIntel") {
    if(HFS >= 11) {
      CPUID cpuID4(0x0B, 0);
      mNumSMT = LVL_CORES & cpuID4.EBX();
      mNumCores = numcpus / mNumSMT;
    } else {
      if (HFS >= 1) {
        if (HFS >= 4) {
          mNumCores = 1 + (CPUID(4, 0).EAX() >> 26) & 0x3F;
        }
      }
      if (mIsHTT) {
        if (mNumCores < 1) {
          mNumCores = 1;
        }
      } else {
        mNumCores = 1;
      }
    }
  } else if (cpuvendor == "AuthenticAMD" || cpuvendor == "AMDisbetter!") {
    mNumSMT = 1 + ((CPUID(0x8000001e, 0).EBX() >> 8) & 0xff);
    if (numcpus > 0 && mNumSMT > 0) {
      mNumCores = numcpus / mNumSMT;
    } else {
      if (HFS >= 1) {
        if (CPUID(0x80000000, 0).EAX() >= 8) {
          mNumCores = 1 + (CPUID(0x80000008, 0).ECX() & 0xFF);
        }
      }
      if (mIsHTT) {
        if (mNumCores < 1) {
          mNumCores = 1;
        }
      } else {
        mNumCores = 1;
      }
    }
  }
  if (mNumCores > 0) {
    numcores = mNumCores;
  }
  return numcores;
  #elif defined(__sun)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("echo `expr $(kstat cpu_info | grep 'pkg_core_id' | uniq | wc -l | awk '{print $1}') / $(psrinfo -p)`", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    numcores = (int)strtol(str.c_str(), nullptr, 10);
  }
  return numcores;
  #else
  return -1;
  #endif
}

int cpu_numcpus() {
  if (numcpus != -1) {
    return numcpus;
  }
  #if defined(_WIN32)
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  numcpus = sysinfo.dwNumberOfProcessors;
  return numcpus;
  #elif (defined(__APPLE__) && defined(__MACH__))
  int logical_cpus = -1;
  std::size_t len = sizeof(int);
  if (!sysctlbyname("machdep.cpu.thread_count", &logical_cpus, &len, nullptr, 0)) {
    numcpus = logical_cpus;
  }
  return numcpus;
  #elif defined(__linux__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("lscpu | grep 'CPU(s):' | uniq | cut -d' ' -f4- | awk 'NR==1{$1=$1;print}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    numcpus = (int)strtol(str.c_str(), nullptr, 10);
  }
  return numcpus;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  int mib[2];
  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  int logical_cpus = -1;
  std::size_t len = sizeof(int);
  if (!sysctl(mib, 2, &logical_cpus, &len, nullptr, 0)) {
    numcpus = logical_cpus;
  }
  return numcpus;
  #elif defined(__sun)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("psrinfo -v -p | grep 'The physical processor has ' | awk '{print $5}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    numcpus = (int)strtol(str.c_str(), nullptr, 10);
  }
  return numcpus;
  #else
  return -1;
  #endif
}

} // namespace ngs::sys
