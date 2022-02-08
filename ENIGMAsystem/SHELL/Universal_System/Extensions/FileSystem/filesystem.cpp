/*

 MIT License
 
 Copyright © 2020-2022 Samuel Venable
 
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

#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <random>

#include <climits>
#include <cstdlib>
#include <cstring>
#if defined(_WIN32)
#include <cwchar>
#endif

#include "filesystem.h"
#include "filesystem.hpp"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#if defined(_WIN32) 
#include <windows.h>
#include <share.h>
#include <io.h>
#else
#if defined(__APPLE__) && defined(__MACH__)
#include <libproc.h>
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__OpenBSD__)
#include <sys/sysctl.h>
#include <sys/user.h>
#endif
#include <unistd.h>
#endif

#if defined(_WIN32)
using std::wstring;
#endif

using std::string;
using std::vector;
using std::size_t;

namespace ngs::fs {

  namespace {

    void message_pump() {
      #if defined(_WIN32) 
      MSG msg; while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      #endif
    }

    #if defined(_WIN32) 
    wstring widen(string str) {
      size_t wchar_count = str.size() + 1; vector<wchar_t> buf(wchar_count);
      return wstring{ buf.data(), (size_t)MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buf.data(), (int)wchar_count) };
    }

    string narrow(wstring wstr) {
      int nbytes = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), nullptr, 0, nullptr, nullptr); vector<char> buf(nbytes);
      return string{ buf.data(), (size_t)WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.length(), buf.data(), nbytes, nullptr, nullptr) };
    }
    #endif

    bool is_digit(char byte) {
      return (byte == '0' || byte == '1' || byte == '2' || byte == '3' || byte == '4' || 
        byte == '5' || byte == '6' || byte == '7' || byte == '8' || byte == '9');
    }

    vector<string> directory_contents;
    unsigned directory_contents_index = 0;
    unsigned directory_contents_order = DC_ATOZ;
    unsigned directory_contents_cntfiles = 1;
    unsigned directory_contents_maxfiles = 0;

    time_t file_datetime_helper(string fname, int timestamp) {
      int result = -1;
      #if defined(_WIN32)
      wstring wfname = widen(fname);
      struct _stat info = { 0 }; 
      result = _wstat(wfname.c_str(), &info);
      #else
      struct stat info = { 0 }; 
      result = stat(fname.c_str(), &info);
      #endif
      if (result == -1) return 0;
      time_t time = 0;
      if (timestamp == 0) time = info.st_atime;
      if (timestamp == 1) time = info.st_mtime;
      if (timestamp == 2) time = info.st_ctime;
      return time;
    }

    int file_datetime(string fname, int timestamp, int measurement) {
      int result = -1;
      time_t time = file_datetime_helper(fname, timestamp);
      #if defined(_WIN32)
      struct tm timeinfo = { 0 };
      if (localtime_s(&timeinfo, &time)) return -1;
      switch (measurement) {
        case  0: return timeinfo.tm_year + 1900;
        case  1: return timeinfo.tm_mon  + 1;
        case  2: return timeinfo.tm_mday;
        case  3: return timeinfo.tm_hour;
        case  4: return timeinfo.tm_min;
        case  5: return timeinfo.tm_sec;
        default: return result;
      }
      #else
      struct tm *timeinfo = std::localtime(&time);
      switch (measurement) {
        case  0: return timeinfo->tm_year + 1900;
        case  1: return timeinfo->tm_mon  + 1;
        case  2: return timeinfo->tm_mday;
        case  3: return timeinfo->tm_hour;
        case  4: return timeinfo->tm_min;
        case  5: return timeinfo->tm_sec;
        default: return result;
      }
      #endif
      return result;
    }

    int file_bin_datetime(int fd, int timestamp, int measurement) {
      int result = -1;
      #if defined(_WIN32)
      struct _stat info = { 0 }; 
      result = _fstat(fd, &info);
      #else
      struct stat info = { 0 }; 
      result = fstat(fd, &info);
      #endif
      time_t time = 0; 
      if (timestamp == 0) time = info.st_atime;
      if (timestamp == 1) time = info.st_mtime;
      if (timestamp == 2) time = info.st_ctime;
      if (result == -1) return result;
      #if defined(_WIN32)
      struct tm timeinfo = { 0 };
      if (localtime_s(&timeinfo, &time)) return -1;
      switch (measurement) {
        case  0: return timeinfo.tm_year + 1900;
        case  1: return timeinfo.tm_mon  + 1;
        case  2: return timeinfo.tm_mday;
        case  3: return timeinfo.tm_hour;
        case  4: return timeinfo.tm_min;
        case  5: return timeinfo.tm_sec;
        default: return result;
      }
      #else
      struct tm *timeinfo = std::localtime(&time);
      switch (measurement) {
        case  0: return timeinfo->tm_year + 1900;
        case  1: return timeinfo->tm_mon  + 1;
        case  2: return timeinfo->tm_mday;
        case  3: return timeinfo->tm_hour;
        case  4: return timeinfo->tm_min;
        case  5: return timeinfo->tm_sec;
        default: return result;
      }
      #endif
      return result;
    }
    
    string string_replace_all(string str, string substr, string nstr) {
      size_t pos = 0;
      while ((pos = str.find(substr, pos)) != string::npos) {
        message_pump();
        str.replace(pos, substr.length(), nstr);
        pos += nstr.length();
      }
      return str;
    }

    vector<string> string_split(string str, char delimiter) {
      vector<string> vec;
      std::stringstream sstr(str);
      string tmp;
      while (std::getline(sstr, tmp, delimiter)) {
        message_pump();
        vec.push_back(tmp);
      }
      return vec;
    }

    string filename_path(string fname) {
      #if defined(_WIN32)
      size_t fp = fname.find_last_of("\\/");
      #else
      size_t fp = fname.find_last_of("/");
      #endif
      if (fp == string::npos) return fname;
      return fname.substr(0, fp + 1);
    }

    string filename_name(string fname) {
      #if defined(_WIN32)
      size_t fp = fname.find_last_of("\\/");
      #else
      size_t fp = fname.find_last_of("/");
      #endif
      if (fp == string::npos) return fname;
      return fname.substr(fp + 1);
    }

    string filename_ext(string fname) {
      fname = filename_name(fname);
      size_t fp = fname.find_last_of(".");
      if (fp == string::npos) return "";
      return fname.substr(fp);
    }

    string filename_remove_slash(string dname, bool canonical = false) {
      if (canonical) dname = ngs::fs::filename_canonical(dname);
      #if defined(_WIN32)
      while (dname.back() == '\\' || dname.back() == '/') {
        message_pump(); dname.pop_back();
      }
      #else
      while (dname.back() == '/' && (!dname.empty() && dname[0] != '/' && dname.length() != 1)) {
        dname.pop_back();
      }
      #endif
      return dname;
    }

    string filename_add_slash(string dname, bool canonical = false) {
      dname = filename_remove_slash(dname, canonical);
      #if defined(_WIN32)
      if (dname.back() != '\\') dname += "\\";
      #else
      if (dname.back() != '/') dname += "/";
      #endif
      return dname;
    }

    struct dir_ite_struct {
      vector<string> vec;
      unsigned index;
      unsigned nlink;
      #if defined(_WIN32)
      _ino_t ino;
      _dev_t dev;
      #else
      ino_t ino;
      dev_t dev;
      #endif
    };

    vector<string> thread_result;
    void *directory_iterator_thread(void *p) {
      struct dir_ite_struct *s = (struct dir_ite_struct *)p; pthread_t thr;
      if (thread_result.size() >= s->nlink) return nullptr; std::error_code ec;
      if (!directory_exists(s->vec[s->index])) return nullptr;
      s->vec[s->index] = filename_remove_slash(s->vec[s->index], true);
      const ghc::filesystem::path path = ghc::filesystem::path(s->vec[s->index]);
      if (directory_exists(s->vec[s->index])) {
        ghc::filesystem::directory_iterator end_itr;
        for (ghc::filesystem::directory_iterator dir_ite(path, ec); dir_ite != end_itr; dir_ite.increment(ec)) {
          message_pump(); if (ec.value() != 0) { break; }
          ghc::filesystem::path file_path = ghc::filesystem::path(filename_absolute(dir_ite->path().string()));
          #if defined(_WIN32)
          struct _stat info = { 0 }; 
          if (ghc::filesystem::exists(file_path, ec) && ec.value() == 0 && !_stat(file_path.string().c_str(), &info)) {
          #else
          struct stat info = { 0 }; 
          if (ghc::filesystem::exists(file_path, ec) && ec.value() == 0 && !stat(file_path.string().c_str(), &info)) {
          #endif
            if (info.st_ino == s->ino && info.st_dev == s->dev) {
              thread_result.push_back(file_path.string());
              if (thread_result.size() >= info.st_nlink) {
                s->nlink = info.st_nlink; s->vec.clear();
                return nullptr;
              }
            }
            if (directory_exists(file_path.string())) {
              s->vec.push_back(file_path.string()); sort(s->vec.begin(), s->vec.end() );
              s->vec.erase(unique(s->vec.begin(), s->vec.end() ), s->vec.end());
              s->index++; pthread_create(&thr, nullptr, directory_iterator_thread, (void *)s);
              message_pump(); pthread_join(thr, nullptr);
            }
          }
        }
      }
      return nullptr;
    }

  } // anonymous namespace

  string directory_get_current_working() {
    std::error_code ec;
    string result = filename_add_slash(ghc::filesystem::current_path(ec).string());
    return (ec.value() == 0) ? result : "";
  }

  bool directory_set_current_working(string dname) {
    std::error_code ec;
    const ghc::filesystem::path path = ghc::filesystem::path(dname);
    ghc::filesystem::current_path(path, ec);
    return (ec.value() == 0);
  }

  string directory_get_temporary_path() {
    std::error_code ec;
    string result = filename_add_slash(ghc::filesystem::temp_directory_path(ec).string());
    return (ec.value() == 0) ? result : "";
  }

  string executable_get_pathname() {
    string path;
    #if defined(_WIN32) 
    wchar_t buffer[MAX_PATH];
    if (GetModuleFileNameW(nullptr, buffer, MAX_PATH) != 0) {
      path = narrow(buffer);
    }
    #elif defined(__APPLE__) && defined(__MACH__)
    char buffer[PROC_PIDPATHINFO_MAXSIZE];
    if (proc_pidpath(getpid(), buffer, sizeof(buffer)) > 0) {
      path = string(buffer) + "\0";
    }
    #elif defined(__linux__)
    char *buffer = nullptr;
    if ((buffer = realpath("/proc/self/exe", nullptr))) {
      path = buffer;
      free(buffer);
    }
    #elif defined(__FreeBSD__) || defined(__DragonFly__)
    size_t length = 0;
    // CTL_KERN::KERN_PROC::KERN_PROC_PATHNAME(-1)
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    if (sysctl(mib, 4, nullptr, &length, nullptr, 0) == 0) {
      path.resize(length, '\0');
      char *buffer = path.data();
      if (sysctl(mib, 4, buffer, &length, nullptr, 0) == 0) {
        path = string(buffer) + "\0";
      }
    }
    #elif defined(__OpenBSD__)
    char **buffer = nullptr; size_t length = 0; 
    int mib[4] = { CTL_KERN, KERN_PROC_ARGS, getpid(), KERN_PROC_ARGV };
    if (sysctl(mib, 4, nullptr, &length, nullptr, 0) == 0) {
      if ((buffer = (char **)malloc(length))) {
        if (sysctl(mib, 4, buffer, &length, nullptr, 0) == 0) {
          path = string(buffer[0]) + "\0";
        }
        free(buffer);
      }
    }
    #endif
    return path;
  }

  string file_bin_pathname(int fd) {
    string path;
    #if defined(_WIN32)
    struct _stat info = { 0 };
    if (!_fstat(fd, &info) && 0 < info.st_nlink) {
    #else
    struct stat info = { 0 };
    if (!fstat(fd, &info) && 0 < info.st_nlink) {
    #endif
      pthread_t t; 
      vector<string> in; 
      in.push_back("/");
      in.push_back(directory_get_temporary_path());
      #if defined(_WIN32)
      in.push_back(environment_expand_variables("${HOMEDRIVE}${HOMEPATH}"));
      vector<string> epath = string_split(environment_get_variable("PATH"), ';');
      #else
      in.push_back(environment_get_variable("HOME"));
      vector<string> epath = string_split(environment_get_variable("PATH"), ':');
      #endif
      in.insert(in.end(), epath.begin(), epath.end()); thread_result.clear();
      struct dir_ite_struct new_struct; 
      new_struct.vec = in; 
      new_struct.index = 0;
      new_struct.ino = info.st_ino; 
      new_struct.dev = info.st_dev;
      pthread_create(&t, nullptr, directory_iterator_thread, (void *)&new_struct);
      message_pump(); pthread_join(t, nullptr); 
      for (unsigned i = 0; i < thread_result.size(); i++) {
        message_pump(); path += thread_result[i] + "\n";
      }
      if (!path.empty()) {
        path.pop_back();
      }
    }
    return path;
  }

  string executable_get_directory() {
    return filename_path(executable_get_pathname());
  }

  string executable_get_filename() {
    return filename_name(executable_get_pathname());
  }

  string environment_get_variable(string name) {
    #if defined(_WIN32)
    string value;
    wchar_t buffer[32767];
    wstring u8name = widen(name);
    if (GetEnvironmentVariableW(u8name.c_str(), buffer, 32767) != 0) {
      value = narrow(buffer);
    }
    return value;
    #else
    char *value = getenv(name.c_str());
    return value ? value : "";
    #endif
  }

  bool environment_set_variable(string name, string value) {
    #if defined(_WIN32)
    wstring u8name = widen(name);
    wstring u8value = widen(value);
    return (SetEnvironmentVariableW(u8name.c_str(), u8value.c_str()) != 0);
    #else
    return (setenv(name.c_str(), value.c_str(), 1) == 0);
    #endif
  }

  bool environment_unset_variable(string name) {
    #if defined(_WIN32)
    wstring u8name = widen(name);
    return (SetEnvironmentVariableW(u8name.c_str(), nullptr) != 0);
    #else
    return (unsetenv(name.c_str()) == 0);
    #endif
  }

  string environment_expand_variables(string str) {
    if (str.find("${") == string::npos) return str;
    string pre = str.substr(0, str.find("${"));
    string post = str.substr(str.find("${") + 2);
    if (post.find('}') == string::npos) return str;
    string variable = post.substr(0, post.find('}'));
    post = post.substr(post.find('}') + 1);
    string value = environment_get_variable(variable);
    return environment_expand_variables(pre + value + post);
  }

  bool file_exists(string fname) {
    std::error_code ec;
    fname = environment_expand_variables(fname);
    const ghc::filesystem::path path = ghc::filesystem::path(fname);
    return (ghc::filesystem::exists(path, ec) && ec.value() == 0 && 
      (!ghc::filesystem::is_directory(path, ec)) && ec.value() == 0);
  }

  bool directory_exists(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, false);
    dname = environment_expand_variables(dname);
    const ghc::filesystem::path path = ghc::filesystem::path(dname);
    return (ghc::filesystem::exists(path, ec) && ec.value() == 0 && 
      ghc::filesystem::is_directory(path, ec) && ec.value() == 0);
  }

  string filename_canonical(string fname) {
    std::error_code ec;
    fname = environment_expand_variables(fname);
    const ghc::filesystem::path path = ghc::filesystem::path(fname);
    string result = ghc::filesystem::weakly_canonical(path, ec).string();
    if (ec.value() == 0 && directory_exists(result)) {
      return filename_add_slash(result);
    }
    return (ec.value() == 0) ? result : "";
  }

  string filename_absolute(string fname) {
    string result = "";
    if (directory_exists(fname)) {
      result = filename_add_slash(fname, true);
    } else if (file_exists(fname)) {
      result = filename_canonical(fname);
    }
    return result;
  }

  std::uintmax_t file_size(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return 0;
    fname = environment_expand_variables(fname);
    const ghc::filesystem::path path = ghc::filesystem::path(fname);
    std::uintmax_t result = ghc::filesystem::file_size(path, ec);
    return (ec.value() == 0) ? result : 0;
  }

  bool file_delete(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    fname = environment_expand_variables(fname);
    const ghc::filesystem::path path = ghc::filesystem::path(fname);
    return (ghc::filesystem::remove(path, ec) && ec.value() == 0);
  }

  bool directory_create(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, true);
    const ghc::filesystem::path path = ghc::filesystem::path(dname);
    return (ghc::filesystem::create_directories(path, ec) && ec.value() == 0);
  }

  bool file_rename(string oldname, string newname) {
    std::error_code ec;
    if (!file_exists(oldname)) return false;
    oldname = environment_expand_variables(oldname);
    newname = environment_expand_variables(newname);
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const ghc::filesystem::path path1 = ghc::filesystem::path(oldname);
    const ghc::filesystem::path path2 = ghc::filesystem::path(newname);
    ghc::filesystem::rename(path1, path2, ec);
    return (ec.value() == 0);
  }

  bool file_copy(string fname, string newname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    fname = environment_expand_variables(fname);
    newname = environment_expand_variables(newname);
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const ghc::filesystem::path path1 = ghc::filesystem::path(fname);
    const ghc::filesystem::path path2 = ghc::filesystem::path(newname);
    ghc::filesystem::copy(path1, path2, ec);
    return (ec.value() == 0);
  }

  std::uintmax_t directory_size(string dname) {
    std::error_code ec;
    std::uintmax_t result = 0;
    if (!directory_exists(dname)) return 0;
    const ghc::filesystem::path path = ghc::filesystem::path(filename_remove_slash(dname, true));
    if (ghc::filesystem::exists(path, ec)) {
      ghc::filesystem::directory_iterator end_itr;
      for (ghc::filesystem::directory_iterator dir_ite(path, ec); dir_ite != end_itr; dir_ite.increment(ec)) {
        message_pump(); if (ec.value() != 0) { break; }
        ghc::filesystem::path file_path = ghc::filesystem::path(filename_absolute(dir_ite->path().string()));
        if (!directory_exists(file_path.string())) {
          result += file_size(file_path.string());
        } else {
          result += directory_size(file_path.string());
        }
      }
    }
    return result;
  }

  bool directory_destroy(string dname) {
    std::error_code ec;
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    const ghc::filesystem::path path = ghc::filesystem::path(dname);
    return (ghc::filesystem::remove_all(path, ec) && ec.value() == 0);
  }

  bool directory_rename(string oldname, string newname) {
    std::error_code ec;
    if (!directory_exists(oldname)) return false;
    oldname = filename_remove_slash(oldname, true);
    newname = filename_remove_slash(newname, true);
    if (!directory_exists(newname)) directory_create(filename_path(newname));
    bool result = false;
    const ghc::filesystem::path path1 = ghc::filesystem::path(oldname);
    const ghc::filesystem::path path2 = ghc::filesystem::path(newname);
    const ghc::filesystem::path path3 = ghc::filesystem::path(
    filename_add_slash(path2.string(), true).substr(0, 
    filename_add_slash(path1.string(), true).length()));
    if (directory_exists(oldname)) {
      if ((filename_name(path1.string()) != filename_name(path2.string()) &&
        filename_path(path1.string()) == filename_path(path2.string())) ||
        path1.string() != path3.string()) {
        ghc::filesystem::rename(path1, path2, ec);
        result = (ec.value() == 0);
      }
    }
    return result;
  }

  unsigned directory_contents_get_cntfiles() {
    return directory_contents_cntfiles;
  }

  unsigned directory_contents_get_maxfiles() {
    return directory_contents_maxfiles;
  }

  void directory_contents_set_maxfiles(unsigned maxfiles) {
    directory_contents_maxfiles = maxfiles;
  }

  static inline vector<string> directory_contents_helper(string dname, string pattern, bool includedirs) {
    std::error_code ec; vector<string> result;
    if (!directory_exists(dname)) return result;
    dname = filename_remove_slash(dname, true);
    const ghc::filesystem::path path = ghc::filesystem::path(dname);
    if (directory_exists(dname)) {
      ghc::filesystem::directory_iterator end_itr;
      for (ghc::filesystem::directory_iterator dir_ite(path, ec); dir_ite != end_itr && 
        (directory_contents_maxfiles == 0 || directory_contents_cntfiles < directory_contents_maxfiles); dir_ite.increment(ec)) {
        message_pump(); if (ec.value() != 0) { break; }
        ghc::filesystem::path file_path = ghc::filesystem::path(filename_absolute(dir_ite->path().string()));
        if (!directory_exists(file_path.string())) {
          result.push_back(file_path.string());
        } else if (ec.value() == 0 && includedirs) {
          result.push_back(filename_add_slash(file_path.string()));
        }
        directory_contents_cntfiles++;
      }
    }
    if (pattern.empty()) pattern = "*.*";
    pattern = string_replace_all(pattern, " ", "");
    pattern = string_replace_all(pattern, "*", "");
    vector<string> extVec = string_split(pattern, ';');
    std::set<string> filteredItems;
    for (const string &item : result) {
      message_pump();
      for (const string &ext : extVec) {
        message_pump();
        if (ext == "." || ext == filename_ext(item) || directory_exists(item)) {
          filteredItems.insert(item);
          break;
        }
      }
    }
    vector<string> result_filtered;
    if (filteredItems.empty()) return result_filtered;
    for (const string &filteredName : filteredItems) {
      message_pump();
      result_filtered.push_back(filteredName);
    }
    return result_filtered;
  }

  static inline vector<string> directory_contents_recursive_helper(string dname, string pattern, bool includedirs) {
    vector<string> result = directory_contents_helper(dname, pattern, true);
    for (unsigned i = 0; i < result.size(); i++) {
      message_pump();
      if (directory_exists(result[i])) {
        vector<string> recursive_result = directory_contents_recursive_helper(result[i], pattern, includedirs);
        for (unsigned j = 0; j < recursive_result.size(); j++) {
          message_pump();
          result.insert(result.end(), recursive_result[j]);
        }
      }
    }
    vector<string> result_filtered;
    for (unsigned i = 0; i < result.size(); i++) {
      message_pump();
      if (!directory_exists(result[i])) {
        result_filtered.push_back(result[i]);
      } else if (includedirs) {
        result_filtered.push_back(result[i]);
      }
    }
    std::set<string> removed_duplicates(result_filtered.begin(), result_filtered.end());
    result_filtered.clear(); result_filtered.assign(removed_duplicates.begin(), removed_duplicates.end());
    return result_filtered;
  }

  void directory_contents_close() {
    directory_contents.clear(); 
    directory_contents_index = 0;
    directory_contents_cntfiles = 1;
  }

  unsigned directory_contents_get_order() {
    return directory_contents_order;
  }

  void directory_contents_set_order(unsigned order) {
    directory_contents_order = order;
  }

  string directory_contents_first(string dname, string pattern, bool includedirs, bool recursive) {
    directory_contents_close();
    if (!recursive) directory_contents = directory_contents_helper(dname, pattern, includedirs);
    else directory_contents = directory_contents_recursive_helper(dname, pattern, includedirs);
    if (directory_contents_index < directory_contents.size()) {
      if (directory_contents_order == DC_ZTOA) {
        std::reverse(directory_contents.begin(), directory_contents.end());
      } else if (directory_contents_order == DC_AOTON) {
        std::sort(directory_contents.begin(), directory_contents.end(),
        [](const std::string &l, const std::string &r) {
        return (file_datetime_helper(l, 0) < file_datetime_helper(r, 0));
        });
      } else if (directory_contents_order == DC_ANTOO) {
        std::sort(directory_contents.begin(), directory_contents.end(),
        [](const std::string &l, const std::string &r) {
        return (file_datetime_helper(l, 0) > file_datetime_helper(r, 0));
        });
      } else if (directory_contents_order == DC_MOTON) {
        std::sort(directory_contents.begin(), directory_contents.end(),
        [](const std::string &l, const std::string &r) {
        return (file_datetime_helper(l, 1) < file_datetime_helper(r, 1));
        });
      } else if (directory_contents_order == DC_MNTOO) {
        std::sort(directory_contents.begin(), directory_contents.end(),
        [](const std::string &l, const std::string &r) {
        return (file_datetime_helper(l, 1) > file_datetime_helper(r, 1));
        });
      } else if (directory_contents_order == DC_COTON) {
        std::sort(directory_contents.begin(), directory_contents.end(),
        [](const std::string &l, const std::string &r) {
        return (file_datetime_helper(l, 2) < file_datetime_helper(r, 2));
        });
      } else if (directory_contents_order == DC_CNTOO) {
        std::sort(directory_contents.begin(), directory_contents.end(),
        [](const std::string &l, const std::string &r) {
        return (file_datetime_helper(l, 2) > file_datetime_helper(r, 2));
        });
      } else if (directory_contents_order == DC_RAND) {
        std::random_device rd; std::mt19937 g(rd());
        std::shuffle(directory_contents.begin(), directory_contents.end(), g);
      }
      return directory_contents[directory_contents_index];
    } 
    return "";
  }

  string directory_contents_next() {
    directory_contents_index++;
    if (directory_contents_index < directory_contents.size())
      return directory_contents[directory_contents_index];
    return "";
  }

  static string retained_string = "";
  static size_t retained_length = 0;
  static std::uintmax_t szsrc   = 0;
  // this function was written to prevent infinitely copying inside itself
  static inline bool directory_copy_retained(string dname, string newname) {
    std::error_code ec;
    bool result = false;
    const ghc::filesystem::path path1 = ghc::filesystem::path(dname);
    const ghc::filesystem::path path2 = ghc::filesystem::path(newname);
    const ghc::filesystem::path path3 = ghc::filesystem::path(path2.string().substr(0, path1.string().length()));
    if (retained_string.empty() && retained_length == 0) {
      retained_length = path1.string().length();
      retained_string = path2.string().substr(retained_length);
    }
    if (directory_exists(dname)) {
      if ((filename_name(path1.string()) != filename_name(path2.string()) &&
        filename_path(path1.string()) == filename_path(path2.string())) ||
        path1.string() != path3.string()) {
        ghc::filesystem::copy(path1, path2, ghc::filesystem::copy_options::recursive, ec);
        result = (ec.value() == 0);
      } else if (path1.string() == path3.string()) {
        unsigned directory_contents_maxprev = directory_contents_get_maxfiles();
        directory_contents_set_maxfiles(0);
        vector<string> itemVec = directory_contents_helper(dname, "*.*", true);
        directory_contents_set_maxfiles(directory_contents_maxprev);
        if (!directory_exists(newname)) {
          directory_create(newname);
          for (const string &item : itemVec) {
            message_pump();
            if (directory_exists(filename_remove_slash(item)) && 
              filename_remove_slash(item).substr(retained_length) != retained_string) {
              directory_copy_retained(filename_remove_slash(item), filename_add_slash(path2.string()) + 
              filename_name(filename_remove_slash(item)));
            } else if (file_exists(item)) {
              ghc::filesystem::copy(item, filename_add_slash(path2.string()) + filename_name(item), ec);
              // ignore and skip errored copies and copy what is left.
              // uncomment the line below to break if one copy failed.
              // if (ec.value() == 0) { result = true; } else { return false; }
            }
          }
          // check size to determine success instead of error code.
          // comment the line below out if you want break on error.
          result = (directory_exists(newname) && szsrc == directory_size(newname));
        }
      }
    }
    return result;
  }

  bool directory_copy(string dname, string newname) {
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    newname = filename_remove_slash(newname, true);
    retained_string = "";
    retained_length = 0;
    // check size to determine success instead of error code.
    // comment the line below out if you want break on error.
    szsrc = directory_size(dname);
    return directory_copy_retained(dname, newname);
  }

  int file_datetime_accessed_year(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 0, 0);
  }

  int file_datetime_accessed_month(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 0, 1);
  }

  int file_datetime_accessed_day(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 0, 2);
  }

  int file_datetime_accessed_hour(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 0, 3);
  }

  int file_datetime_accessed_minute(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 0, 4);
  }

  int file_datetime_accessed_second(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 0, 5);
  }

  int file_datetime_modified_year(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 1, 0);
  }

  int file_datetime_modified_month(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 1, 1);
  }

  int file_datetime_modified_day(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 1, 2);
  }

  int file_datetime_modified_hour(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 1, 3);
  }

  int file_datetime_modified_minute(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 1, 4);
  }

  int file_datetime_modified_second(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 1, 5);
  }

  int file_datetime_created_year(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 2, 0);
  }

  int file_datetime_created_month(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 2, 1);
  }

  int file_datetime_created_day(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 2, 2);
  }

  int file_datetime_created_hour(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 2, 3);
  }

  int file_datetime_created_minute(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 2, 4);
  }

  int file_datetime_created_second(string fname) {
    fname = environment_expand_variables(fname);
    return file_datetime(fname.c_str(), 2, 5);
  }

  int file_bin_datetime_accessed_year(int fd) {
    return file_bin_datetime(fd, 0, 0);
  }

  int file_bin_datetime_accessed_month(int fd) {

    return file_bin_datetime(fd, 0, 1);
  }

  int file_bin_datetime_accessed_day(int fd) {
    return file_bin_datetime(fd, 0, 2);
  }

  int file_bin_datetime_accessed_hour(int fd) {
    return file_bin_datetime(fd, 0, 3);
  }

  int file_bin_datetime_accessed_minute(int fd) {
    return file_bin_datetime(fd, 0, 4);
  }

  int file_bin_datetime_accessed_second(int fd) {
    return file_bin_datetime(fd, 0, 5);
  }

  int file_bin_datetime_modified_year(int fd) {
    return file_bin_datetime(fd, 1, 0);
  }

  int file_bin_datetime_modified_month(int fd) {
    return file_bin_datetime(fd, 1, 1);
  }

  int file_bin_datetime_modified_day(int fd) {

    return file_bin_datetime(fd, 1, 2);
  }

  int file_bin_datetime_modified_hour(int fd) {
    return file_bin_datetime(fd, 1, 3);
  }

  int file_bin_datetime_modified_minute(int fd) {
    return file_bin_datetime(fd, 1, 4);
  }

  int file_bin_datetime_modified_second(int fd) {
    return file_bin_datetime(fd, 1, 5);
  }

  int file_bin_datetime_created_year(int fd) {
    return file_bin_datetime(fd, 2, 0);
  }

  int file_bin_datetime_created_month(int fd) {
    return file_bin_datetime(fd, 2, 1);
  }

  int file_bin_datetime_created_day(int fd) {
    return file_bin_datetime(fd, 2, 2);
  }

  int file_bin_datetime_created_hour(int fd) {
    return file_bin_datetime(fd, 2, 3);
  }

  int file_bin_datetime_created_minute(int fd) {
    return file_bin_datetime(fd, 2, 4);
  }

  int file_bin_datetime_created_second(int fd) {
    return file_bin_datetime(fd, 2, 5);
  }

  int file_bin_open(string fname, int mode) {
    fname = environment_expand_variables(fname);
    #if defined(_WIN32)
    wstring wfname = widen(fname);
    FILE *fp = nullptr;
    switch (mode) {
      case  0: { if (!_wfopen_s(&fp, wfname.c_str(), L"rb, ccs=UTF-8" )) break; return -1; }
      case  1: { if (!_wfopen_s(&fp, wfname.c_str(), L"wb, ccs=UTF-8" )) break; return -1; }
      case  2: { if (!_wfopen_s(&fp, wfname.c_str(), L"w+b, ccs=UTF-8")) break; return -1; }
      case  3: { if (!_wfopen_s(&fp, wfname.c_str(), L"ab, ccs=UTF-8" )) break; return -1; }
      case  4: { if (!_wfopen_s(&fp, wfname.c_str(), L"a+b, ccs=UTF-8")) break; return -1; }
      default: return -1;
    }
    if (fp) { int fd = _dup(_fileno(fp));
    fclose(fp); return fd; }
    #else
    FILE *fp = nullptr;
    switch (mode) {
      case  0: { fp = fopen(fname.c_str(), "rb" ); break; }
      case  1: { fp = fopen(fname.c_str(), "wb" ); break; }
      case  2: { fp = fopen(fname.c_str(), "w+b"); break; }
      case  3: { fp = fopen(fname.c_str(), "ab" ); break; }
      case  4: { fp = fopen(fname.c_str(), "a+b"); break; }
      default: return -1;
    }
    if (fp) { int fd = dup(fileno(fp));
    fclose(fp); return fd; }
    #endif
    return -1;
  }

  int file_bin_rewrite(int fd) {
    #if defined(_WIN32)
    _lseek(fd, 0, SEEK_SET);
    return _chsize(fd, 0);
    #else
    lseek(fd, 0, SEEK_SET);
    return ftruncate(fd, 0);
    #endif
  }
  
  int file_bin_close(int fd) {
    #if defined(_WIN32)
    return _close(fd);
    #else
    return close(fd);
    #endif
  }
  
  long file_bin_size(int fd) {
    #if defined(_WIN32)
    struct _stat info = { 0 }; 
    int result = _fstat(fd, &info);
    #else
    struct stat info = { 0 }; 
    int result = fstat(fd, &info);
    #endif
    if (result != -1) {
      return info.st_size;
    }
    return 0;
  }

  long file_bin_position(int fd) {
    #if defined(_WIN32)
    return _lseek(fd, 0, SEEK_CUR);
    #else
    return lseek(fd, 0, SEEK_CUR);
    #endif
  }
  
  long file_bin_seek(int fd, long pos) {
    #if defined(_WIN32)
    return _lseek(fd, pos, SEEK_CUR);
    #else
    return lseek(fd, pos, SEEK_CUR);
    #endif
  }

  int file_bin_read_byte(int fd) {
    int byte = 0;
    #if defined(_WIN32)
    int num = (int)_read(fd, &byte, 1);
    #else
    int num = (int)read(fd, &byte, 1);
    #endif
    if (num == -1) return 0;
    return byte;
  }

  int file_bin_write_byte(int fd, int byte) {
    #if defined(_WIN32)
    return (int)_write(fd, &byte, 1);
    #else
    return (int)write(fd, &byte, 1);
    #endif
  }

  int file_text_open_read(string fname) {
    return file_bin_open(fname, 0);
  }

  int file_text_open_write(string fname) {
    return file_bin_open(fname, 1);
  }

  int file_text_open_append(string fname) {
    return file_bin_open(fname, 3);
  }

  long file_text_write_string(int fd, string str) {
    char *buffer = str.data();
    #if defined(_WIN32)
    long result = _write(fd, buffer, (unsigned)str.length());
    #else
    long result = write(fd, buffer, (unsigned)str.length());
    #endif
    return result;
  }

  long file_text_write_real(int fd, double val) {
    string str = std::to_string(val);
    return file_text_write_string(fd, str);
  }

  int file_text_writeln(int fd) {
    return file_bin_write_byte(fd, '\n');
  }

  static unsigned cnt = 0;
  bool file_text_eof(int fd) {
    bool res1 = ((char)file_bin_read_byte(fd) == '\0');
    bool res2 = (file_bin_position(fd) > file_bin_size(fd));
    while (res2 && cnt < 2) { message_pump(); 
    file_bin_seek(fd, -1); cnt++; }
    if (!res2) file_bin_seek(fd, -1);
    cnt = 0; return (res1 || res2);
  }

  bool file_text_eoln(int fd) {
    bool res1 = ((char)file_bin_read_byte(fd) == '\n');
    bool res2 = file_text_eof(fd);
    while (res2 && cnt < 2) { message_pump(); 
    file_bin_seek(fd, -1); cnt++; }
    if (!res2) file_bin_seek(fd, -1);
    cnt = 0; return (res1 || res2);
  }

  string file_text_read_string(int fd) {
    int byte = file_bin_read_byte(fd); string str;
    str.push_back((char)byte);
    while ((char)byte != '\n') {
      message_pump();
      byte = file_bin_read_byte(fd);
      str.push_back((char)byte);
      if (byte == 0) break;
    }
    if (str.length() >= 2) {
      if (str[str.length() - 2] != '\r' && str[str.length() - 1] == '\n') {
        file_bin_seek(fd, -1);
        str = str.substr(0, str.length() - 1);
      }
      if (str[str.length() - 2] == '\r' && str[str.length() - 1] == '\n') {
        file_bin_seek(fd, -2);
        str = str.substr(0, str.length() - 2);
      }
    } else if (str.length() == 1) {
      if (str[str.length() - 1] == '\n') {
        file_bin_seek(fd, -1);
        str = str.substr(0, str.length() - 1);
      }
    }
    return str;
  }

  double file_text_read_real(int fd) {
    bool dot = false, sign = false;
    string str; char byte = (char)file_bin_read_byte(fd);
    while (byte == '\r' || byte == '\n') { 
      message_pump();
      byte = (char)file_bin_read_byte(fd);
    }
    if (byte == '.' && !dot) {
      dot = true;
    } else if (!is_digit(byte) && byte != '+' && 
      byte != '-' && byte != '.') {
      return 0;
    } else if (byte == '+' || byte == '-') {
      sign = true;
    }
    if (byte == 0) goto finish;
    str.push_back(byte);
    if (sign) {
      byte = (char)file_bin_read_byte(fd);
      if (byte == '.' && !dot) {
        dot = true;
      } else if (!is_digit(byte) && byte != '.') {
        return strtod(str.c_str(), nullptr);
      }
      if (byte == 0) goto finish;
      str.push_back(byte);
    }
    while (byte != '\n' && !(file_bin_position(fd) > file_bin_size(fd))) {
      message_pump();
      byte = (char)file_bin_read_byte(fd);
      if (byte == '.' && !dot) {
        dot = true;
      } else if (byte == '.' && dot) {
        break;
      } else if (!is_digit(byte) && byte != '.') {
        break;
      } else if (byte == '\n' || file_bin_position(fd) > file_bin_size(fd)) {
        break;
      }
      if (byte == 0) goto finish;
      str.push_back(byte);
    }
    finish:
    return strtod(str.c_str(), nullptr);
  }

  string file_text_readln(int fd) {
    int byte = file_bin_read_byte(fd); string str;
    str.push_back((char)byte);
    while ((char)byte != '\n') {
      message_pump();
      byte = file_bin_read_byte(fd);
      str.push_back((char)byte);
      if (byte == 0) break;
    }
    return str;
  }

  string file_text_read_all(int fd) {
    string str;
    long sz = file_bin_size(fd);
    char *buffer = new char[sz];
    #if defined(_WIN32)
    long result = _read(fd, buffer, sz);
    #else
    long result = read(fd, buffer, sz);
    #endif
    if (result == -1) {
      delete[] buffer;
      return "";
    }
    str = buffer ? buffer : "";
    delete[] buffer;
    return str;
  }

  int file_text_open_from_string(string str) {
    string fname = directory_get_temporary_path() + "temp.XXXXXX";
    #if defined(_WIN32)
    int fd = -1; wstring wfname = widen(fname); 
    wchar_t *buffer = wfname.data(); if (_wmktemp_s(buffer, wfname.length() + 1)) return -1;
    if (_wsopen_s(&fd, buffer, _O_CREAT | _O_RDWR | _O_WTEXT, _SH_DENYNO, _S_IREAD | _S_IWRITE)) {
      return -1;
    }
    #else
    char *buffer = fname.data();
    int fd = mkstemp(buffer);
    #endif
    if (fd == -1) return -1;
    file_text_write_string(fd, str);
    #if defined(_WIN32)
    _lseek(fd, 0, SEEK_SET);
    #else
    lseek(fd, 0, SEEK_SET);
    #endif
    return fd;
  }
  
  int file_text_close(int fd) {
    return file_bin_close(fd);
  }

} // namespace ngs::fs
