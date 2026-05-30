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

#include "internal.h"
#include <string>
#if (defined(_WIN32) || defined(_WIN64))
#include <vector>
#include <cwchar>
#include <cstddef>
#include <cstdlib>
#include <windef.h>
#include <fileapi.h>
#include <stringapiset.h>
#include <libloaderapi.h>
#include <handleapi.h>
#include <winbase.h>
#elif (defined(__APPLE__) && defined(__MACH__))
#include <cstdint>
#include <climits>
#include <cstdlib>
#include <mach-o/dyld.h>
#elif (defined(__linux__) || defined(__ANDROID__))
#include <climits>
#include <cstdlib>
#elif ((defined(__FreeBSD__) || defined(__FreeBSD_kernel__)) || defined(__DragonFly__))
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

const char *__getexecname(void) {
  std::string path;
  #if (defined(_WIN32) || defined(_WIN64))
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
  wchar_t buffer[MAX_PATH];
  if (GetModuleFileNameW(nullptr, buffer, sizeof(buffer))) {
    std::wstring exe = resolve_symbolic_links(buffer);
    path = narrow(exe);
  }
  #elif (defined(__APPLE__) && defined(__MACH__))
  char buffer[PATH_MAX];
  uint32_t size = sizeof(buffer);
  if (!_NSGetExecutablePath(buffer, &size)) {
    char exe[PATH_MAX];
    if (realpath(buffer, exe)) {
      path = exe;
    }
  }
  #elif (defined(__linux__) || defined(__ANDROID__))
  char exe[PATH_MAX];
  if (realpath("/proc/self/exe", exe)) {
    path = exe;
  }
  #elif ((defined(__FreeBSD__) || defined(__FreeBSD_kernel__)) || defined(__DragonFly__))
  int mib[4]; 
  size_t len = 0;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
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
  #endif
  static std::string result; result = path;
  return ((!result.empty()) ? result.c_str() : nullptr);
}
