/*

 MIT License
 
 Copyright © 2021-2022 Samuel Venable
 
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

#include <climits>

#if defined(PROCESS_GUIWINDOW_IMPL)
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#if ((defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL))
#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <AppKit/AppKit.h>
#elif ((defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(__sun) || defined(PROCESS_XQUARTZ_IMPL))
#include <X11/Xlib.h>
#endif
#endif
#endif

#include "xprocess.hpp"

using namespace ngs::ps;

namespace {

  #if !defined(_MSC_VER)
  #pragma pack(push, 8)
  #else
  #include <pshpack8.h>
  #endif

  typedef struct {
    char *executable_image_file_path;
    char *current_working_directory;
    NGS_PROCID parent_process_id;
    NGS_PROCID *child_process_id;
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

  #if defined(_WIN32) 
  std::string string_replace_all(std::string str, std::string substr, std::string nstr) {
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
      message_pump();
      str.replace(pos, substr.length(), nstr);
      pos += nstr.length();
    }
    return str;
  }

  std::wstring widen(std::string str) {
    if (str.empty()) return L"";
    std::size_t wchar_count = str.size() + 1; 
    std::vector<wchar_t> buf(wchar_count);
    return std::wstring { buf.data(), (std::size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
  }

  std::string narrow(std::wstring wstr) {
    if (wstr.empty()) return "";
    int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr); 
    std::vector<char> buf(nbytes);
    return std::string { buf.data(), (std::size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
  }
  #endif

} // anonymous namespace

namespace xprocess {

  void proc_id_enumerate(NGS_PROCID **proc_id, int *size) {
    *proc_id = nullptr; *size = 0;
    std::vector<NGS_PROCID> vec;
    vec = ::proc_id_enum();
    *proc_id = (NGS_PROCID *)malloc(sizeof(NGS_PROCID) * vec.size());
    if (proc_id) {
      std::copy(vec.begin(), vec.end(), *proc_id);
      *size = (int)vec.size();
    }
  }

  void free_proc_id(NGS_PROCID *proc_id) {
    if (proc_id) {
      free(proc_id);
    }
  }

  void proc_id_from_self(NGS_PROCID *proc_id) {
    *proc_id = ::proc_id_from_self();
  }

  NGS_PROCID proc_id_from_self() {
    return ::proc_id_from_self();
  }

  void parent_proc_id_from_self(NGS_PROCID *parent_proc_id) {
    *parent_proc_id = 0;
    std::vector<NGS_PROCID> vec = ::parent_proc_id_from_proc_id(::proc_id_from_self());
    if (!vec.empty()) *parent_proc_id = vec[0];
  }

  NGS_PROCID parent_proc_id_from_self() {
    NGS_PROCID parent_proc_id = 0;
    std::vector<NGS_PROCID> vec = ::parent_proc_id_from_proc_id(::proc_id_from_self());
    if (!vec.empty()) parent_proc_id = vec[0];
    return parent_proc_id;
  }

  bool proc_id_exists(NGS_PROCID proc_id) {
    return ::proc_id_exists(proc_id);
  }

  bool proc_id_suspend(NGS_PROCID proc_id) {
    return ::proc_id_suspend(proc_id);
  }

  bool proc_id_resume(NGS_PROCID proc_id) {
    return ::proc_id_resume(proc_id);
  }

  bool proc_id_kill(NGS_PROCID proc_id) {
    return ::proc_id_kill(proc_id);
  }

  void parent_proc_id_from_proc_id(NGS_PROCID proc_id, NGS_PROCID *parent_proc_id) {
    *parent_proc_id = 0;
    std::vector<NGS_PROCID> vec = ::parent_proc_id_from_proc_id(proc_id);
    if (!vec.empty()) *parent_proc_id = vec[0];
  }

  NGS_PROCID parent_proc_id_from_proc_id(NGS_PROCID proc_id) {
    NGS_PROCID parent_proc_id = 0;
    std::vector<NGS_PROCID> vec = ::parent_proc_id_from_proc_id(proc_id);
    if (!vec.empty()) parent_proc_id = vec[0];
    return parent_proc_id;
  }

  void proc_id_from_parent_proc_id(NGS_PROCID parent_proc_id, NGS_PROCID **proc_id, int *size) {
    *proc_id = nullptr; *size = 0;
    std::vector<NGS_PROCID> vec;
    vec = ::proc_id_from_parent_proc_id(parent_proc_id);
    *proc_id = (NGS_PROCID *)malloc(sizeof(NGS_PROCID) * vec.size());
    if (proc_id) {
      std::copy(vec.begin(), vec.end(), *proc_id);
      *size = (int)vec.size();
    }
  }

  void exe_from_proc_id(NGS_PROCID proc_id, char **buffer) {
    *buffer = (char *)"";
    static std::string str;
    str = ::exe_from_proc_id(proc_id);
    if (!str.empty()) *buffer = (char *)str.c_str();
  }

  const char *exe_from_proc_id(NGS_PROCID proc_id) {
    static std::string exe;
    exe = ::exe_from_proc_id(proc_id);
    return exe.c_str();
  }

  const char *executable_from_self() {
    static std::string exe;
    exe = ::exe_from_proc_id(::proc_id_from_self());
    return exe.c_str();
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

  void cwd_from_proc_id(NGS_PROCID proc_id, char **buffer) {
    *buffer = (char *)"";
    static std::string str;
    str = ::cwd_from_proc_id(proc_id);
    if (!str.empty()) *buffer = (char *)str.c_str();
  }

  const char *cwd_from_proc_id(NGS_PROCID proc_id) {
    static std::string cwd;
    cwd = ::cwd_from_proc_id(proc_id);
    return cwd.c_str();
  }

  void free_cmdline(char **buffer) {
    if (buffer) {
      delete[] buffer;
    }
  }

  void cmdline_from_proc_id(NGS_PROCID proc_id, char ***buffer, int *size) {
    *buffer = nullptr; *size = 0;
    static std::vector<std::string> cmdline_vec_1;
    cmdline_vec_1 = ::cmdline_from_proc_id(proc_id);
    std::vector<char *> cmdline_vec_2;
    for (std::size_t i = 0; i < cmdline_vec_1.size(); i++) {
      message_pump();
      cmdline_vec_2.push_back((char *)cmdline_vec_1[i].c_str());
    }
    char **arr = new char *[cmdline_vec_2.size()]();
    std::copy(cmdline_vec_2.begin(), cmdline_vec_2.end(), arr);
    *buffer = arr; *size = (int)cmdline_vec_2.size();
  }

  void free_environ(char **buffer) {
    if (buffer) {
      delete[] buffer;
    }
  }

  void environ_from_proc_id(NGS_PROCID proc_id, char ***buffer, int *size) {
    *buffer = nullptr; *size = 0;
    static std::vector<std::string> environ_vec_1;
    environ_vec_1 = ::environ_from_proc_id(proc_id);
    std::vector<char *> environ_vec_2;
    for (std::size_t i = 0; i < environ_vec_1.size(); i++) {
      message_pump();
      environ_vec_2.push_back((char *)environ_vec_1[i].c_str());
    }
    char **arr = new char *[environ_vec_2.size()]();
    std::copy(environ_vec_2.begin(), environ_vec_2.end(), arr);
    *buffer = arr; *size = (int)environ_vec_2.size();
  }

  void environ_from_proc_id_ex(NGS_PROCID proc_id, const char *name, char **value) {
    static std::string str;
    str = ::envvar_value_from_proc_id(proc_id, name);
    *value = (char *)str.c_str();
  }

  const char *environ_from_proc_id_ex(NGS_PROCID proc_id, const char *name) {
    char *value = (char *)"";
    environ_from_proc_id_ex(proc_id, name, &value);
    static std::string str; str = value;
    return str.c_str();
  }

  bool environ_from_proc_id_ex_exists(NGS_PROCID proc_id, const char *name) {
    return ::envvar_exists_from_proc_id(proc_id, name);
  }

  const char *environment_get_variable(const char *name) {
    static std::string str;
    #if defined(_WIN32)
    DWORD length = 0; 
    std::wstring u8name = widen(name);
    if ((length = GetEnvironmentVariableW(u8name.c_str(), nullptr, 0)) != 0) {
      wchar_t *buffer = new wchar_t[length]();
      if (GetEnvironmentVariableW(u8name.c_str(), buffer, length) != 0) {
        str = narrow(buffer);
      }
      delete[] buffer;
    }
    #else
    char *value = getenv(name);
    str = value ? value : "\0";
    #endif
    return str.c_str();
  }

  bool environment_get_variable_exists(const char *name) {
    #if defined(_WIN32)
    std::wstring u8name = widen(name);
    return (!(GetEnvironmentVariableW(u8name.c_str(), nullptr, 0) == 0 && 
      GetLastError() == ERROR_ENVVAR_NOT_FOUND));
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

  const char *directory_get_temporary_path() {
    static std::string tempdir;
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
    struct stat sb;
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
  #if ((defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(__sun) || (defined(__APPLE__) && defined(__MACH__) && defined(PROCESS_XQUARTZ_IMPL)))
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
    #if ((defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL))
    res = std::to_string((unsigned long long)[(NSWindow *)window windowNumber]);
    #else
    res = std::to_string((unsigned long long)window);
    #endif
    return (WINDOWID)res.c_str();
  }

  WINDOW native_window_from_window_id(WINDOWID win_id) {
    #if ((defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL))
    return (WINDOW)[NSApp windowWithWindowNumber:(CGWindowID)strtoul(win_id, nullptr, 10)];
    #else
    return (WINDOW)strtoull(win_id, nullptr, 10);
    #endif
  }

  void window_id_from_proc_id(NGS_PROCID proc_id, WINDOWID **win_id, int *size) {
    static std::vector<std::string> wid_vec_1;
    *win_id = nullptr; *size = 0;
    wid_vec_1.clear();
    if (!proc_id_exists(proc_id)) return;
    #if defined(_WIN32)
    HWND hWnd = GetTopWindow(GetDesktopWindow());
    NGS_PROCID pid = 0; proc_id_from_window_id(window_id_from_native_window((WINDOW)hWnd), &pid);
    if (proc_id == pid) {
      wid_vec_1.push_back(window_id_from_native_window((WINDOW)hWnd));
    }
    while (hWnd = GetWindow(hWnd, GW_HWNDNEXT)) {
      message_pump();
      NGS_PROCID pid = 0; proc_id_from_window_id(window_id_from_native_window((WINDOW)hWnd), &pid);
      if (proc_id == pid) {
        wid_vec_1.push_back(window_id_from_native_window((WINDOW)hWnd));
      }
    }
    #elif (defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL)
    CFArrayRef window_array = CGWindowListCopyWindowInfo(
    kCGWindowListOptionAll, kCGNullWindowID);
    CFIndex windowCount = 0;
    if ((windowCount = CFArrayGetCount(window_array))) {
      for (CFIndex i = 0; i < windowCount; i++) {
        CFDictionaryRef windowInfoDictionary =
        (CFDictionaryRef)CFArrayGetValueAtIndex(window_array, i);
        CFNumberRef ownerPID = (CFNumberRef)CFDictionaryGetValue(
        windowInfoDictionary, kCGWindowOwnerPID); NGS_PROCID pid = 0;
        CFNumberGetValue(ownerPID, kCFNumberIntType, &pid);
        if (proc_id == pid) {
          CFNumberRef windowID = (CFNumberRef)CFDictionaryGetValue(
          windowInfoDictionary, kCGWindowNumber);
          CGWindowID wid; CFNumberGetValue(windowID, kCGWindowIDCFNumberType, &wid);
          wid_vec_1.push_back(std::to_string((unsigned long)wid));
        }
      }
    }
    CFRelease(window_array);
    #elif (defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(__sun) || defined(PROCESS_XQUARTZ_IMPL)
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
        for (int i = nitems - 1; i >= 0; i--) {
          NGS_PROCID pid; proc_id_from_window_id(window_id_from_native_window((WINDOW)array[i]), &pid);
          if (proc_id == pid) {
            wid_vec_1.push_back(window_id_from_native_window((WINDOW)array[i]));
          }
        }
      }
      XFree(prop);
    }
    XCloseDisplay(display);
    #endif
    std::vector<WINDOWID> wid_vec_2;
    for (std::size_t i = 0; i < wid_vec_1.size(); i++) {
      message_pump();
      wid_vec_2.push_back((WINDOWID)wid_vec_1[i].c_str());
    }
    WINDOWID *arr = new WINDOWID[wid_vec_2.size()]();
    std::copy(wid_vec_2.begin(), wid_vec_2.end(), arr);
    *win_id = arr; *size = (int)wid_vec_2.size();
  }

  void free_window_id(WINDOWID *win_id) {
    if (win_id) {
      delete[] win_id;
    }
  }

  void window_id_enumerate(WINDOWID **win_id, int *size) {
    static std::vector<std::string> wid_vec_3;
    *win_id = nullptr; *size = 0;
    wid_vec_3.clear(); int i = 0;
    NGS_PROCID *pid = nullptr; int pidsize = 0;
    proc_id_enumerate(&pid, &pidsize);
    if (pid) {
      for (int i = 0; i < pidsize; i++) {
        message_pump();
        WINDOWID *wid = nullptr; int widsize = 0;
        window_id_from_proc_id(pid[i], &wid, &widsize);
        if (wid) {
          for (int ii = 0; ii < widsize; ii++) {
            wid_vec_3.push_back(wid[ii]);
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

  void proc_id_from_window_id(WINDOWID win_id, NGS_PROCID *proc_id) {
    *proc_id = 0;
    #if defined(_WIN32)
    DWORD pid = 0; GetWindowThreadProcessId((HWND)native_window_from_window_id(win_id), &pid);
    *proc_id = (NGS_PROCID)pid;
    #elif ((defined(__APPLE__) && defined(__MACH__)) && !defined(PROCESS_XQUARTZ_IMPL))
    CFArrayRef window_array = CGWindowListCopyWindowInfo(
    kCGWindowListOptionIncludingWindow, (CGWindowID)strtol(win_id, nullptr, 10));
    CFIndex windowCount = 0;
    if ((windowCount = CFArrayGetCount(window_array))) {
      for (CFIndex i = 0; i < windowCount; i++) {
        CFDictionaryRef windowInfoDictionary =
        (CFDictionaryRef)CFArrayGetValueAtIndex(window_array, i);
        CFNumberRef ownerPID = (CFNumberRef)CFDictionaryGetValue(
        windowInfoDictionary, kCGWindowOwnerPID); NGS_PROCID pid = 0;
        CFNumberGetValue(ownerPID, kCFNumberIntType, &pid);
        WINDOWID *wid = nullptr; int size = 0;
        window_id_from_proc_id(pid, &wid, &size);
        if (wid) {
          for (int i = 0; i < size; i++) {
            if (strtoul(win_id, nullptr, 10) == strtoul(wid[i], nullptr, 10)) {
              *proc_id = pid;
              break;
            }
          }
          free_window_id(wid);
        }
      }
    }
    CFRelease(window_array);
    #elif ((defined(__linux__) && !defined(__ANDROID__)) || (defined(__FreeBSD__) || defined(__DragonFly__) || defined(__NetBSD__) || defined(__OpenBSD__)) || defined(__sun) || defined(PROCESS_XQUARTZ_IMPL))
    set_error_handlers();
    Display *display = XOpenDisplay(nullptr);
    unsigned long property = 0;
    unsigned char *prop = nullptr;
    Atom actual_type = 0, filter_atom = 0;
    int actual_format = 0, status = 0;
    unsigned long nitems = 0, bytes_after = 0;
    filter_atom = XInternAtom(display, "_NET_WM_PID", true);
    status = XGetWindowProperty(display, (Window)native_window_from_window_id(win_id), filter_atom, 0, 1000, false,
    AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    if (status == Success && prop != nullptr) {
      property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
      XFree(prop);
    }
    *proc_id = (NGS_PROCID)property;
    XCloseDisplay(display);
    #endif
    if (!proc_id_exists(*proc_id)) {
      *proc_id = 0;
    }
  }

  bool window_id_exists(WINDOWID win_id) {
    NGS_PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return ::proc_id_exists(proc_id);
    }
    return false;
  }

  bool window_id_suspend(WINDOWID win_id) {
    NGS_PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return ::proc_id_suspend(proc_id);
    }
    return false;
  }

  bool window_id_resume(WINDOWID win_id) {
    NGS_PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return ::proc_id_resume(proc_id);
    }
    return false;
  }

  bool window_id_kill(WINDOWID win_id) {
    NGS_PROCID proc_id = 0;
    proc_id_from_window_id(win_id, &proc_id);
    if (proc_id) {
      return ::proc_id_kill(proc_id);
    }
    return false;
  }
  #endif

  static int procInfoIndex = -1;
  static int procListIndex = -1;
  static std::unordered_map<PROCINFO, PROCINFO_STRUCT *> proc_info_map;
  static std::vector<std::vector<NGS_PROCID>> proc_list_vec;

  PROCINFO proc_info_from_proc_id(NGS_PROCID proc_id) {
    KINFOFLAGS kinfo_flags = KINFO_EXEP | KINFO_CWDP | KINFO_PPID | KINFO_CPID | KINFO_ARGV | KINFO_ENVV;
    #if defined(PROCESS_GUIWINDOW_IMPL)
    kinfo_flags |= KINFO_OWID;
    #endif
    return proc_info_from_proc_id_ex(proc_id, kinfo_flags);
  }

  PROCINFO proc_info_from_proc_id_ex(NGS_PROCID proc_id, KINFOFLAGS kinfo_flags) {
    char *exe = nullptr; if (kinfo_flags & KINFO_EXEP) exe_from_proc_id(proc_id, &exe);
    char *cwd = nullptr; if (kinfo_flags & KINFO_CWDP) cwd_from_proc_id(proc_id, &cwd);
    NGS_PROCID ppid = 0;     if (kinfo_flags & KINFO_PPID) parent_proc_id_from_proc_id(proc_id, &ppid);
    NGS_PROCID *pid = nullptr; int pidsize = 0;
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
    return proc_info_map[proc_info]->executable_image_file_path ? proc_info_map[proc_info]->executable_image_file_path : (char *)""; }
  char *current_working_directory(PROCINFO proc_info) { 
    return proc_info_map[proc_info]->current_working_directory ? proc_info_map[proc_info]->current_working_directory : (char *)""; }
  NGS_PROCID parent_process_id(PROCINFO proc_info) { return proc_info_map[proc_info]->parent_process_id; }
  NGS_PROCID *child_process_id(PROCINFO proc_info) { return proc_info_map[proc_info]->child_process_id; }
  NGS_PROCID child_process_id(PROCINFO proc_info, int i) { return proc_info_map[proc_info]->child_process_id[i]; }
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
    NGS_PROCID *proc_id = nullptr; int size = 0;
    proc_id_enumerate(&proc_id, &size);
    if (proc_id) {
      std::vector<NGS_PROCID> res;
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

  NGS_PROCID process_id(PROCLIST proc_list, int i) {
    std::vector<NGS_PROCID> proc_id = proc_list_vec[proc_list];
    return proc_id[i];
  }

  int process_id_length(PROCLIST proc_list) {
    return (int)proc_list_vec[proc_list].size();
  }

  void free_proc_list(PROCLIST proc_list) {
    proc_list_vec[proc_list].clear();
  }

  NGS_PROCID process_execute(const char *command) {
    return ::spawn_child_proc_id(command, true);
  }

  NGS_PROCID process_execute_async(const char *command) {
    return ::spawn_child_proc_id(command, false);
  }

  long long executed_process_write_to_standard_input(NGS_PROCID proc_index, const char *input) {
    return ::write_to_stdin_for_child_proc_id(proc_index, input);
  }

  const char *executed_process_read_from_standard_output(NGS_PROCID proc_index) {
    static std::string result;
    result = ::read_from_stdout_for_child_proc_id(proc_index);
    return result.c_str();
  }

  void set_buffer_limit_for_standard_output(long long limit) {
    ::stdout_set_buffer_limit(limit);
  }

  bool free_executed_process_standard_input(NGS_PROCID proc_index) {
    return ::free_stdin_for_child_proc_id(proc_index);
  }

  bool free_executed_process_standard_output(NGS_PROCID proc_index) {
    return ::free_stdout_for_child_proc_id(proc_index);
  }

  bool completion_status_from_executed_process(NGS_PROCID proc_index) {
    return ::child_proc_id_is_complete(proc_index);
  }

  const char *current_process_read_from_standard_input() {
    static std::string result;
    result = ::read_from_stdin_for_self();
    return result.c_str();
  }

} // namespace xprocess
