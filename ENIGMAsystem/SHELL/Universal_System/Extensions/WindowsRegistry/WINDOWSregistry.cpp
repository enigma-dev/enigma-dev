/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
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

#include <string>
#include <algorithm>

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include "Platforms/General/PFregistry.h"
#include "Platforms/platforms_mandatory.h"
#include "Universal_System/estring.h"

using std::string;
using std::wstring;

static HKEY   key       = HKEY_CURRENT_USER;
static string keystring = "HKEY_CURRENT_USER"; 
static string path      = "SOFTWARE\\STIGMA\\";

namespace enigma_user {

  bool registry_write_string(string name, string str) {
    return registry_write_string_ext(path, name, str);
  }

  bool registry_write_real(string name, unsigned long val) {
    return registry_write_real_ext(path, name, val);
  }

  string registry_read_string(string name) {
    return registry_read_string_ext(path, name);
  }

  unsigned long registry_read_real(string name) {
    return registry_read_real_ext(path, name);
  }

  bool registry_exists(string name) {
    return registry_exists_ext(path, name);
  }

  bool registry_write_string_ext(string subpath, string name, string str) {
    static wchar_t buff[32767]; DWORD sz = sizeof(buff); HKEY subkey = nullptr;
    wstring u8subpath = widen(subpath); wstring u8name = widen(name); 
	wstring wstr = widen(str); wcsncpy_s(buff, sizeof(buff), wstr.c_str(), sizeof(buff));
    if (RegCreateKeyExW(key, u8subpath.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &subkey, nullptr) != ERROR_SUCCESS) {
      return false;
    } else if (RegSetValueExW(subkey, u8name.c_str(), 0, REG_SZ, (unsigned char *)&buff, sz) != ERROR_SUCCESS) {
      RegCloseKey(subkey);
      return false;
    }
    RegCloseKey(subkey);
    return true;
  }

  bool registry_write_real_ext(string subpath, string name, unsigned long val) {
    HKEY subkey = nullptr; wstring u8subpath = widen(subpath); wstring u8name = widen(name); DWORD sz = sizeof(DWORD);
    if (RegCreateKeyExW(key, u8subpath.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &subkey, nullptr) != ERROR_SUCCESS) {
      return false;
    } else if (RegSetValueExW(subkey, u8name.c_str(), 0, REG_DWORD, (BYTE *)&val, sz) != ERROR_SUCCESS) {
      RegCloseKey(subkey);
      return false;
    }
    RegCloseKey(subkey);
    return true;
  }

  string registry_read_string_ext(string subpath, string name) {
    static wchar_t buff[32767]; DWORD sz = sizeof(buff); wstring u8subpath = widen(subpath); wstring u8name = widen(name);
    if (RegGetValueW(key, u8subpath.c_str(), u8name.c_str(), RRF_RT_REG_SZ, nullptr, (unsigned char *)&buff, &sz) == ERROR_SUCCESS) {
      return shorten((wchar_t *)buff);
    }
    return "";
  }

  unsigned long registry_read_real_ext(string subpath, string name) {
    unsigned long val = 0; DWORD sz = sizeof(DWORD); wstring u8subpath = widen(subpath); wstring u8name = widen(name);
    if (RegGetValueW(key, u8subpath.c_str(), u8name.c_str(), RRF_RT_REG_DWORD, nullptr, &val, &sz) == ERROR_SUCCESS) {
      return val;
    }
    return 0;
  }

  bool registry_exists_ext(string subpath, string name) {
    HKEY subkey = nullptr; wstring u8subpath = widen(subpath); wstring u8name = widen(name);
    if (RegOpenKeyExW(key, u8subpath.c_str(), 0, KEY_READ, &subkey) == ERROR_SUCCESS) {
      if (RegQueryValueExW(subkey, u8name.c_str(), nullptr, nullptr, nullptr, nullptr) != ERROR_FILE_NOT_FOUND) {
        RegCloseKey(subkey);
        return true;
      }
      RegCloseKey(subkey);
    }
    return false;
  }
  
  string registry_get_path() {
    return path;
  }
 
  bool registry_set_path(string subpath) {
    HKEY subkey = nullptr; wstring u8subpath = widen(subpath);
    if (RegOpenKeyExW(key, u8subpath.c_str(), 0, KEY_READ, &subkey) == ERROR_SUCCESS) {
      path = subpath;
      RegCloseKey(subkey);
      return true;
    }
    return false;
  }
  
  string registry_get_key() {
    return keystring;
  }

  bool registry_set_key(string keystr) {
    bool success = true;
    std::transform(keystr.begin(), keystr.end(), keystr.begin(), ::toupper);
    if      (keystr == "HKEY_CLASSES_ROOT")        key = HKEY_CLASSES_ROOT;
    else if (keystr == "HKEY_CURRENT_CONFIG")      key = HKEY_CURRENT_CONFIG;
    else if (keystr == "HKEY_CURRENT_USER")        key = HKEY_CURRENT_USER;
    else if (keystr == "HKEY_LOCAL_MACHINE")       key = HKEY_LOCAL_MACHINE;
    else if (keystr == "HKEY_PERFORMANCE_DATA")    key = HKEY_PERFORMANCE_DATA;
    else if (keystr == "HKEY_PERFORMANCE_NLSTEXT") key = HKEY_PERFORMANCE_NLSTEXT;
    else if (keystr == "HKEY_PERFORMANCE_TEXT")    key = HKEY_PERFORMANCE_TEXT; 
    else if (keystr == "HKEY_USERS")               key = HKEY_USERS;
    else success = false;
    if (success) keystring = keystr;
    return success;
  }

} // namespace enigma_user
