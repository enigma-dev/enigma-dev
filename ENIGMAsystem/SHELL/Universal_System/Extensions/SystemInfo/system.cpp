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
#include <algorithm>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>
#include <regex>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#if (!defined(_WIN32) && defined(CREATE_OPENGL_CONTEXT))
#if !defined(__sun)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#include <sys/swap.h>
#include <unistd.h>
#endif
#endif
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <intrin.h>
#include <dxgi.h>
#else
#if (defined(__APPLE__) && defined(__MACH__))
#define GL_SILENCE_DEPRECATION
#include <OpenGL/CGLCurrent.h>
#include <OpenGL/gl.h>
#else
#include <GL/glx.h>
#include <GL/gl.h>
#endif
#if defined(__linux__)
#include <sys/sysinfo.h>
#endif
#if ((defined(__APPLE__) && defined(__MACH__)) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
#include <sys/types.h>
#if (defined(__FreeBSD__) || defined(__DragonFly__))
#include <unistd.h>
#include <fcntl.h>
#include <kvm.h>
#elif (defined(__NetBSD__) || defined(__OpenBSD__))
#include <sys/param.h>
#include <sys/swap.h>
#include <unistd.h>
#endif
#include <sys/sysctl.h>
#endif
#include <sys/utsname.h>
#endif
#if (defined(_WIN32) && defined(_MSC_VER))
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dxgi.lib")
#endif

#include "system.hpp"

namespace ngs::sys {

#if (!defined(_WIN32) && defined(CREATE_OPENGL_CONTEXT))
#if !defined(__sun)
static GLFWwindow *window = nullptr;
#else
static SDL_Window *window = nullptr;
#endif
#endif

#if (!defined(_WIN32) && defined(CREATE_OPENGL_CONTEXT))
static void create_opengl_context() {
  #if (defined(__APPLE__) && defined(__MACH__))
  if (CGLGetCurrentContext()) {
    return;
  }
  #else
  if (glXGetCurrentContext()) {
    return;
  }
  #endif
  #if !defined(__sun)
  if (!window) {
    glewExperimental = true;
    if (glfwInit()) {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
      glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
      window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
      if (window) {
        glfwMakeContextCurrent(window);
        if (window) {
          glewInit();
        }
      }
    }
  }
  #else
  if (!window) {
    if (SDL_Init(SDL_INIT_VIDEO)) return;
    window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (!window) return;
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) return;
    SDL_GL_MakeCurrent(window, context);
  }
  #endif
}
#endif

struct HumanReadable {
  long double size{};
  private: friend
  std::ostream& operator<<(std::ostream& os, HumanReadable hr) {
    int i{};
    long double mantissa = hr.size;
    for (; mantissa >= 1024; mantissa /= 1024, i++) { }
    mantissa = std::ceil(mantissa * 100) / 100;
    os << mantissa << " " << "BKMGTPE"[i];
    return i == 0 ? os : os << "B";
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
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.sysname;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  const char *result = nullptr;
  char buf[255]; 
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
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.nodename;
  std::string str;
  str = result ? result : "";
  return str;
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

#if defined(_WIN32)
std::string windows_version(std::string *product_name) {
  auto GetOSMajorVersionNumber = []() {
    const char *result = nullptr;
    char buf[10];
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
    char buf[10];
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
    char buf[255]; 
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
    char buf[10];
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
      char buf[255];
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

std::string utsname_release() {
  #if !defined(_WIN32)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.release;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  std::string product_name;
  return windows_version(&product_name);    
  #endif
}

std::string utsname_version() {
  #if !defined(_WIN32)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.version;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  char buf[2048];
  std::string str;
  if (sprintf(buf, "%s%s%s", "Microsoft Windows [Version ", utsname_release().c_str(), "]") != -1) {
    str = strlen(buf) ? buf : "";
  }
  return str;
  #endif
}

std::string utsname_codename() {
  #if defined(_WIN32)
  std::string product_name;
  windows_version(&product_name);
  return product_name;
  #elif (defined(__APPLE__) && defined(__MACH__))
  std::string result;
  FILE *fp = popen("echo $(sw_vers | grep 'ProductName:' | uniq | awk '{$1=$1};1' && sw_vers | grep 'ProductVersion:' | uniq | awk '{$1=$1};1')", "r");
  if (fp) {
    char buf[255];
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
    char buf[255];
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
  #else
  return utsname_sysname() + " " + utsname_release();
  #endif
}

std::string utsname_machine() {
  #if !defined(_WIN32)
  const char *result = nullptr;
  struct utsname name;
  if (!uname(&name))
    result = name.machine;
  std::string str;
  str = result ? result : "";
  return str;
  #else
  SYSTEM_INFO sysinfo;
  GetNativeSystemInfo(&sysinfo);
  if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
    return "AMD64";  
  } else if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM) {
    return "ARM";
  } else if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64) {
    return "ARM64"; 
  } else if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
    return "IA64"; 
  } else if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
    return "x86"; 
  }
  return "";
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
  long long physical_memory = 0;
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
    return info.totalram;
  }
  return -1;
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
    return info.freeram;
  }
  return -1;
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
  #elif ((defined(__APPLE__) && defined(__MACH__)) ||defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  long long total = memory_totalram();
  long long avail = memory_availram();
  if (total != -1 && avail != -1) {
    return total - avail;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.totalram - info.freeram;
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
    return info.totalswap;
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
          total += ((swaps[i].se_nblks / (1024 / block_s)) * 1024);
        }
      }
      free(swaps);
    }
    return total;
  }
  return -1;
  #elif defined(__sun)
  long long total = 0;
  long page_s = sysconf(_SC_PAGESIZE);
  int nswap = swapctl(SC_GETNSWP, nullptr);
  if (!nswap) return 0;
  if (nswap > 0) {
    swaptbl_t *swaps = (swaptbl_t *)malloc(nswap * sizeof(swapent_t));
    if (swaps) {
      char *strtab = (char *)malloc((nswap + 1) * 80);
      if (strtab) {
        for (int i = 0; i < (nswap + 1); i++) {
          swaps->swt_ent[i].ste_path = strtab + (i * 80);
        }
        if (swapctl(SC_LIST, swaps) > 0) {
          for (int i = 0; i < nswap; i++) {
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
    return info.freeswap;
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
          avail += (((swaps[i].se_nblks - swaps[i].se_inuse) / (1024 / block_s)) * 1024);
        }
      }
      free(swaps);
    }
    return avail;
  }
  return -1;
  #elif defined(__sun)
  long long avail = 0;
  long page_s = sysconf(_SC_PAGESIZE);
  int nswap = swapctl(SC_GETNSWP, nullptr);
  if (!nswap) return 0;
  if (nswap > 0) {
    swaptbl_t *swaps = (swaptbl_t *)malloc(nswap * sizeof(swapent_t));
    if (swaps) {
      char *strtab = (char *)malloc((nswap + 1) * 80);
      if (strtab) {
        for (int i = 0; i < (nswap + 1); i++) {
          swaps->swt_ent[i].ste_path = strtab + (i * 80);
        }
        if (swapctl(SC_LIST, swaps) > 0) {
          for (int i = 0; i < nswap; i++) {
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
    return info.totalswap - info.freeswap;
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
          used += ((swaps[i].se_inuse / (1024 / block_s)) * 1024);
        }
      }
      free(swaps);
    }
    return used;
  }
  return -1;
  #elif defined(__sun)
  long long used = 0;
  long page_s = sysconf(_SC_PAGESIZE);
  int nswap = swapctl(SC_GETNSWP, nullptr);
  if (!nswap) return 0;
  if (nswap > 0) {
    swaptbl_t *swaps = (swaptbl_t *)malloc(nswap * sizeof(swapent_t));
    if (swaps) {
      char *strtab = (char *)malloc((nswap + 1) * 80);
      if (strtab) {
        for (int i = 0; i < (nswap + 1); i++) {
          swaps->swt_ent[i].ste_path = strtab + (i * 80);
        }
        if (swapctl(SC_LIST, swaps) > 0) {
          for (int i = 0; i < nswap; i++) {
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

std::string gpu_vendor() {
  #if !defined(_WIN32)
  #if defined(CREATE_OPENGL_CONTEXT)
  create_opengl_context();
  #endif
  const char *result = (char *)glGetString(GL_VENDOR);
  std::string str;
  str = result ? result : "";
  return str;
  #else
  std::string result;
  IDXGIFactory *pFactory = nullptr;
  if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory) == S_OK) {
    IDXGIAdapter *pAdapter = nullptr;
    if (pFactory->EnumAdapters(0, &pAdapter) == S_OK) {
      DXGI_ADAPTER_DESC adapterDesc;
      if (pAdapter->GetDesc(&adapterDesc) == S_OK) {
        std::string str = gpu_renderer();
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        if (str.find("INTEL") != std::string::npos || adapterDesc.VendorId == 0x163C || 
          adapterDesc.VendorId == 0x8086 || adapterDesc.VendorId == 0x8087) {
          result = "Intel";
        } else if (str.find("AMD") != std::string::npos || adapterDesc.VendorId == 0x1002 || 
          adapterDesc.VendorId == 0x1022) {
          result = "AMD";
        } else if (str.find("NVIDIA") != std::string::npos || adapterDesc.VendorId == 0x10DE) {
          result = "NVIDIA Corporation";
        } else if (str.find("MICROSOFT") != std::string::npos) {
          result = "Microsoft Corporation";
        } else if (str.find("APPLE") != std::string::npos) {
          result = "Apple";
        }
      }
      pAdapter->Release();
    }
    pFactory->Release();
  }
  return result;
  #endif
}

std::string gpu_renderer() {
  #if !defined(_WIN32)
  #if defined(CREATE_OPENGL_CONTEXT)
  create_opengl_context();
  #endif
  const char *result = (char *)glGetString(GL_RENDERER);
  std::string str;
  str = result ? result : "";
  return str;
  #else
  auto narrow = [](std::wstring wstr) {
    if (wstr.empty()) return std::string("");
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr);
    std::vector<char> buf(nbytes);
    return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
  };
  std::string result;
  IDXGIFactory *pFactory = nullptr;
  if (CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory) == S_OK) {
    IDXGIAdapter *pAdapter = nullptr;
    if (pFactory->EnumAdapters(0, &pAdapter) == S_OK) {
      DXGI_ADAPTER_DESC adapterDesc;
      if (pAdapter->GetDesc(&adapterDesc) == S_OK) {
        result = narrow(adapterDesc.Description);
      }
      pAdapter->Release();
    }
    pFactory->Release();
  }
  return result;
  #endif
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
  char buf[1024];
  /* needs glxinfo installed via mesa-utils (Ubuntu), glx-utils (FreeBSD), or equivalent distro package */
  FILE *fp = popen("glxinfo 2> /dev/null | grep 'Video memory: ' | uniq | awk -F ': ' '{print $2}'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      if (strlen(buf)) {
        result = strtoll(buf, nullptr, 10) * 1024 * 1024;
      }
    }
    pclose(fp);
  }
  #endif
  videomemory = result;
  return result;
}

std::string cpu_vendor() {
  #if (defined(_WIN32) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  std::string str = cpu_brand();
  if (str.empty()) return "";
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  if (str.find("INTEL") != std::string::npos) {
    return "GenuineIntel";
  } else if (str.find("AMD") != std::string::npos) {
    return "AuthenticAMD";
  } else if (str.find("APPLE") != std::string::npos) {
    return "Apple";
  }
  #if !defined(__DragonFly__)
  std::string arch = utsname_machine();
  std::transform(arch.begin(), arch.end(), arch.begin(), ::toupper);
  if (!arch.empty()) {
    if (arch.find("ARM") != std::string::npos || arch.find("AARCH64") != std::string::npos) {
      return "ARM";
    }
  }
  #endif
  return "";
  #elif (defined(__APPLE__) && defined(__MACH__))
  char buf[1024];
  const char *result = nullptr;
  std::size_t len = sizeof(buf);
  if (!sysctlbyname("machdep.cpu.vendor", &buf, &len, nullptr, 0)) {
    result = buf;
  }
  std::string str;
  str = result ? result : "";
  if (str.empty()) {
    std::string brand = cpu_brand();
    std::transform(brand.begin(), brand.end(), brand.begin(), ::toupper);
    if (!brand.empty()) {
      if (brand.find("APPLE") != std::string::npos) {
        return "Apple";
      }
    }
  }
  return str;
  #elif defined(__linux__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("lscpu | grep 'Vendor ID:' | uniq | cut -d' ' -f3- | awk '{$1=$1};1'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = result ? result : "";
    if (str.empty()) {
      std::string brand = cpu_brand();
      std::transform(brand.begin(), brand.end(), brand.begin(), ::toupper);
      if (!brand.empty()) {
        if (brand.find("APPLE") != std::string::npos) {
          return "Apple";
        }
      }
    }
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
  char buf[255]; 
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
  FILE *fp = popen("lscpu | grep 'Model name:' | uniq | cut -d' ' -f3- | awk '{$1=$1};1'", "r");
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
  #else
  return "";
  #endif
}

static int numcores = -1;
int cpu_numcores() {
  if (numcores != -1) { 
    return numcores;
  }
  #if defined(_WIN32)
  std::string result;
  bool proceed = true;
  HANDLE stdin_read = nullptr; HANDLE stdin_write = nullptr;
  HANDLE stdout_read = nullptr; HANDLE stdout_write = nullptr;
  SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), nullptr, true };
  proceed = CreatePipe(&stdin_read, &stdin_write, &sa, 0);
  if (!proceed) return -1;
  SetHandleInformation(stdin_write, HANDLE_FLAG_INHERIT, 0);
  proceed = CreatePipe(&stdout_read, &stdout_write, &sa, 0);
  if (!proceed) return -1;
  STARTUPINFOW si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(STARTUPINFOW);
  si.dwFlags = STARTF_USESTDHANDLES;
  si.hStdError = stdout_write;
  si.hStdOutput = stdout_write;
  si.hStdInput = stdin_read;
  PROCESS_INFORMATION pi; 
  ZeroMemory(&pi, sizeof(pi));
  std::size_t len = wcslen(L"cmd /c wmic cpu get NumberOfCores 2>nul");
  wchar_t *cwstr_command = new wchar_t[len]();
  wcsncpy_s(cwstr_command, len, L"wmic cpu get NumberOfCores", len);
  BOOL success = CreateProcessW(nullptr, cwstr_command, nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
  delete[] cwstr_command;
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
  if (!sysctlbyname("hw.physicalcpu", &physical_cpus, &len, nullptr, 0)) {
    numcores = physical_cpus;
  }
  return numcores;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
  int mib[2];
  int physical_cpus = -1;
  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  std::size_t len = sizeof(int);
  if (!sysctl(mib, 2, &physical_cpus, &len, nullptr, 0)) {
    numcores = physical_cpus;
  }
  return numcores;
  #elif defined(__linux__)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("lscpu | grep 'Core(s) per socket:' | uniq | cut -d' ' -f4- | awk '{$1=$1};1'", "r");
  if (fp) {
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = buf;
    }
    pclose(fp);
    static std::string str;
    str = (result && strlen(result)) ? result : "-1";
    if (str == "-1") {
      FILE *fp = popen("lscpu | grep 'Core(s) per cluster:' | uniq | cut -d' ' -f4- | awk '{$1=$1};1'", "r");
      if (fp) {
        if (fgets(buf, sizeof(buf), fp)) {
          buf[strlen(buf) - 1] = '\0';
          result = buf;
        }
        pclose(fp);
        static std::string str2;
        str = (result && strlen(result)) ? result : "-1";
        numcores = (int)strtol(str.c_str(), nullptr, 10);
      }
    }
    numcores = (int)strtol(str.c_str(), nullptr, 10);
  }
  return numcores;
  #elif defined(__sun)
  char buf[1024];
  const char *result = nullptr;
  FILE *fp = popen("psrinfo -p", "r");
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

static int numcpus = -1;
int cpu_numcpus() {
  if (numcpus != -1) return numcpus;
  auto result = std::thread::hardware_concurrency();
  numcpus = (int)(result ? result : -1);
  return numcpus;
}

} // namespace ngs::sys
