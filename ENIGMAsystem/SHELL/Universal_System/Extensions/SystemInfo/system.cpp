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
#include <string>
#include <thread>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cmath>
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <intrin.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <d3d11.h>
#include <dxgi.h>
#else
#if (defined(__APPLE__) && defined(__MACH__))
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#if (defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__))
#include <sys/sysinfo.h>
#endif
#if (defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun))
#include <cpuid.h>
#if (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__))
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#endif
#include <sys/utsname.h>
#endif
#endif
#if (defined(_WIN32) && defined(_MSC_VER))
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#endif

#include "system.hpp"

namespace ngs::sys {

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
  return "Windows_NT";
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
    }
  }
  std::string str;
  str = result ? result : "";
  return str;
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
  const char *result = nullptr;
  char buf[2048];
  if (sprintf(buf, "%s%s%s", "Microsoft Windows [Version ", utsname_release().c_str(), "]") != -1) {
    result = buf;
  }
  std::string str;
  str = strlen(result) ? result : "";
  return str;
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
  GetSystemInfo(&sysinfo);
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
  #elif (defined(__APPLE__) && defined(__MACH__))
  int mib[2];
  long long physical_memory;
  mib[0] = CTL_HW;
  mib[1] = HW_MEMSIZE;
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
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.totalram * 1024;
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
  #elif (defined(__APPLE__) && defined(__MACH__))
  vm_size_t page_size;
  mach_port_t mach_port;
  mach_msg_type_number_t count;
  vm_statistics64_data_t vm_stats;
  mach_port = mach_host_self();
  count = sizeof(vm_stats) / sizeof(natural_t);
  if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
    KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
    (host_info64_t)&vm_stats, &count)) {
    long long free_memory = (long long)vm_stats.free_count * (long long)page_size;
    long long used_memory = ((long long)vm_stats.active_count +
    (long long)vm_stats.inactive_count +
    (long long)vm_stats.wire_count) *  (long long)page_size;
    return free_memory;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.freeram;
  }
  return -1;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.freeram * 1024;
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
  #elif (defined(__APPLE__) && defined(__MACH__))
  vm_size_t page_size;
  mach_port_t mach_port;
  mach_msg_type_number_t count;
  vm_statistics64_data_t vm_stats;
  mach_port = mach_host_self();
  count = sizeof(vm_stats) / sizeof(natural_t);
  if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
    KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
    (host_info64_t)&vm_stats, &count)) {
    long long free_memory = (long long)vm_stats.free_count * (long long)page_size;
    long long used_memory = ((long long)vm_stats.active_count +
    (long long)vm_stats.inactive_count +
    (long long)vm_stats.wire_count) *  (long long)page_size;
    return used_memory;
  }
  return -1;
  #elif defined(__linux__)
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.totalram - info.freeram;
  }
  return -1;
  #elif (defined(__FreeBSD__) || defined(__DragonFly__))
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return (info.totalram - info.freeram) * 1024;
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
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.totalswap * 1024;
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
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return info.freeswap * 1024;
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
  struct sysinfo info;
  if (!sysinfo(&info)) {
    return (info.totalswap - info.freeswap) * 1024;
  }
  return -1;
  #else
  return -1;
  #endif
}

std::string gpu_vendor() {
  const char *result = (char *)glGetString(GL_VENDOR);
  std::string str;
  str = result ? result : "";
  return str;
}

std::string gpu_renderer() {
  const char *result = (char *)glGetString(GL_RENDERER);
  std::string str;
  str = result ? result : "";
  return str;
}

long long gpu_videomemory() {
  long long result = -1;
  #if defined(_WIN32)
  ID3D11Device *g_pd3dDevice = nullptr;
  D3D_FEATURE_LEVEL featlvl[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
  if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featlvl, 
    ARRAYSIZE(featlvl), D3D11_SDK_VERSION, &g_pd3dDevice, nullptr, nullptr) == S_OK) {
    IDXGIDevice *pDXGIDevice = nullptr;
    if (g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice) == S_OK) {
      IDXGIAdapter *pDXGIAdapter = nullptr;
      if (pDXGIDevice->GetAdapter(&pDXGIAdapter) == S_OK) {
        DXGI_ADAPTER_DESC adapterDesc;
        if (pDXGIAdapter->GetDesc(&adapterDesc) == S_OK) {
          result = (long long)adapterDesc.DedicatedVideoMemory;
        }
        pDXGIAdapter->Release();
      }
      pDXGIDevice->Release();
    }
    g_pd3dDevice->Release();
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
  /* needs glxinfo installed via mesa-utils (linux) / glx-utils (bsd) package */
  FILE *fp = popen("glxinfo | grep 'Video memory: ' | uniq | awk -F ': ' '{print $2}'", "r");
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
  return result;
}

std::string cpu_vendor() {
  #if defined(_WIN32)
  const char *result = nullptr;
  int regs[4];
  char vendor[13];
  __cpuid(regs, 0);
  memcpy(vendor, &regs[1], 4);
  memcpy(vendor + 4, &regs[3], 4);
  memcpy(vendor + 8, &regs[2], 4);
  vendor[12] = '\0';
  std::string untrimmed;
  untrimmed = vendor ? vendor : "";
  std::size_t pos = untrimmed.find_first_not_of(" ");
  if (pos != std::string::npos) {
    std::string str;
    str = untrimmed.substr(pos);
    return str;
  }
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
  return str;
  #elif (defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun))
  int a[4] { 0, 0, 0, 0 };
  __asm__("mov $0x0, %eax\n\t");
  __asm__("cpuid\n\t");
  __asm__("mov %%ebx, %0\n\t":"=r" (a[0]));
  __asm__("mov %%edx, %0\n\t":"=r" (a[1]));
  __asm__("mov %%ecx, %0\n\t":"=r" (a[2]));
  return std::string((const char *)&a);
  #endif
}

std::string cpu_brand() {
  #if defined(_WIN32)
  const char *result = nullptr;
  int CPUInfo[4];
  unsigned nExIds, i = 0;
  char CPUBrandString[0x40];
  __cpuid(CPUInfo, 0x80000000);
  nExIds = CPUInfo[0];
  for (i = 0x80000000; i <= nExIds; i++) {
    __cpuid(CPUInfo, i);
    if  (i == 0x80000002) {
      memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
    } else if  (i == 0x80000003) {
      memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
    } else if  (i == 0x80000004) {
      memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
  }
  std::string untrimmed;
  result = CPUBrandString;
  untrimmed = result ? result : "";
  std::size_t pos = untrimmed.find_first_not_of(" ");
  if (pos != std::string::npos) {
    std::string str;
    str = untrimmed.substr(pos);
    return str;
  }
  return "";
  #elif (defined(__APPLE__) && defined(__MACH__))
  const char *result = nullptr;
  char buf[1024];
  std::size_t len = sizeof(buf);
  if (!sysctlbyname("machdep.cpu.brand_string", &buf, &len, nullptr, 0)) {
    result = buf;
  }
  std::string str;
  str = result ? result : "";
  return str;
  #elif (defined(__linux__) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__sun))
  const char *result = nullptr;
  char CPUBrandString[0x40];
  unsigned CPUInfo[4] = { 0, 0, 0, 0 };
  __cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  unsigned nExIds = CPUInfo[0];
  memset(CPUBrandString, 0, sizeof(CPUBrandString));
  for (unsigned i = 0x80000000; i <= nExIds; i++) {
    __cpuid(i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
    if (i == 0x80000002) {
      memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
    } else if (i == 0x80000003) {
      memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
    } else if (i == 0x80000004) {
      memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    }
  }
  std::string untrimmed;
  result = CPUBrandString;
  untrimmed = result ? result : "";
  std::size_t pos = untrimmed.find_first_not_of(" ");
  if (pos != std::string::npos) {
    std::string str;
    str = untrimmed.substr(pos);
    return str;
  }
  return "";
  #endif
}

int cpu_numcpus() {
  auto result = std::thread::hardware_concurrency();
  return (int)(result ? result : -1);
}

} // namespace ngs::sys
