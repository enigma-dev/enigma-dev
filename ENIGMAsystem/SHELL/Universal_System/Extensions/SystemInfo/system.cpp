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
#include <cstring>
#include <cstdio>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <intrin.h>
#include <GL/gl.h>
#include <d3d11.h>
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
#endif
#include <sys/sysctl.h>
#endif
#include <sys/utsname.h>
#endif
#if defined(_MSC_VER)
#if defined(_WIN32) && !defined(_WIN64)
#pragma comment(lib, __FILE__"\\..\\lib\\x86\\glew32.lib")
#pragma comment(lib, __FILE__"\\..\\lib\\x86\\glfw3.lib")
#elif defined(_WIN32) && defined(_WIN64)
#pragma comment(lib, __FILE__"\\..\\lib\\x64\\glew32.lib")
#pragma comment(lib, __FILE__"\\..\\lib\\x64\\glfw3.lib")
#endif
#if defined(_WIN32)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#endif
#endif

#include "system.hpp"

namespace ngs::sys {

static GLFWwindow *window = nullptr;
static void create_opengl_context() {
  #if defined(_WIN32)
  if (wglGetCurrentContext()) {
    return;
  }
  #elif (defined(__APPLE__) && defined(__MACH__))
  if (CGLGetCurrentContext()) {
    return;
  }
  #else
  if (glXGetCurrentContext()) {
    return;
  }
  #endif
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
    #if defined(_WIN32)
    if (!window) {
      GLuint PixelFormat;
      static PIXELFORMATDESCRIPTOR pfd;
      HDC hDC = GetDC(GetDesktopWindow());
      PixelFormat = ChoosePixelFormat(hDC, &pfd);
      SetPixelFormat(hDC, PixelFormat, &pfd);
      HGLRC hRC = wglCreateContext(hDC);
      wglMakeCurrent(hDC, hRC);
      // just doing this so window != nullptr...
      window = (GLFWwindow *)GetDesktopWindow();
      ReleaseDC(GetDesktopWindow(), hDC);
    }
    #endif
  }
}

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
  char buf[2048];
  std::string product_name;
  if (sprintf(buf, "%s%s%s", "Microsoft Windows [Version ", windows_version(&product_name).c_str(), "]") != -1) {
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
  FILE *fp = popen("echo $(sw_vers --productName && sw_vers --productVersion) |  tr '\n' ' '", "r");
  if (fp) {
    char buf[255];
    if (fgets(buf, sizeof(buf), fp)) {
      buf[strlen(buf) - 1] = '\0';
      result = strlen(buf) ? buf : "";
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
  int page_s = getpagesize();
  kvmh = kvm_open(nullptr, "/dev/null", "/dev/null", O_RDONLY, nullptr);
  if (!kvmh) return -1;
  struct kvm_swap k_swap;
  if (kvm_getswapinfo(kvmh, &k_swap, 1, 0) != -1) {
    kvm_close(kvmh);
    return k_swap.ksw_total * getpagesize();
  }
  kvm_close(kvmh);
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
  int page_s = getpagesize();
  kvmh = kvm_open(nullptr, "/dev/null", "/dev/null", O_RDONLY, nullptr);
  if (!kvmh) return -1;
  struct kvm_swap k_swap;
  if (kvm_getswapinfo(kvmh, &k_swap, 1, 0) != -1) {
    kvm_close(kvmh);
    return (k_swap.ksw_total - k_swap.ksw_used) * getpagesize();
  }
  kvm_close(kvmh);
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
  int page_s = getpagesize();
  kvmh = kvm_open(nullptr, "/dev/null", "/dev/null", O_RDONLY, nullptr);
  if (!kvmh) return -1;
  struct kvm_swap k_swap;
  if (kvm_getswapinfo(kvmh, &k_swap, 1, 0) != -1) {
    kvm_close(kvmh);
    return k_swap.ksw_used * getpagesize();
  }
  kvm_close(kvmh);
  return -1;
  #else
  return -1;
  #endif
}

std::string gpu_vendor() {
  create_opengl_context();
  const char *result = (char *)glGetString(GL_VENDOR);
  std::string str;
  str = result ? result : "";
  return str;
}

std::string gpu_renderer() {
  create_opengl_context();
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
  #if (defined(_WIN32) || defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__))
  std::string str = cpu_brand();
  if (str.empty()) return "";
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  if (str.find("INTEL") != std::string::npos) {
    return "GenuineIntel";
  } else if (str.find("AMD") != std::string::npos) {
    return "AuthenticAMD";
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
    return str.c_str();
  }
  return "";
  #elif defined(__OpenBSD__)
  int mib[2];
  char buf[1024];
  mib[0] = CTL_HW;
  mib[1] = HW_VENDOR;
  std::size_t len = sizeof(buf);
  if (!sysctl(mib, 2, buf, &len, nullptr, 0)) {
    return strlen(buf) ? buf : "";
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
    return str.c_str();
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

int cpu_numcpus() {
  auto result = std::thread::hardware_concurrency();
  return (int)(result ? result : -1);
}

} // namespace ngs::sys
