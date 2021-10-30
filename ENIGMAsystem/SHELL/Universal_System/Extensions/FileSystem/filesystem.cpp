/*

 MIT License
 
 Copyright © 2020-2021 Samuel Venable
 
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
#include <filesystem>

#if defined(__linux__)
#include <cstdlib>
#endif

#include "filesystem.h"

#if defined(_WIN32) 
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#else
#if defined(__APPLE__) && defined(__MACH__)
#include <libproc.h>
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#include <sys/sysctl.h>
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

    int file_get_date_accessed_modified(const char *fname, bool modified, int type) {
      int result = -1; // returns -1 on failure
      #if defined(_WIN32)
      std::wstring wfname = widen(fname);
      struct _stat info = { 0 }; 
      result = _wstat(wfname.c_str(), &info);
      #else
      struct stat info = { 0 }; 
      result = stat(fname, &info);
      #endif
      time_t time = modified ? info.st_mtime : info.st_atime;
      if (result == -1) return result; // failure: stat errored
      struct tm *timeinfo = std::localtime(&time);
      switch (type) {
        case  0: return timeinfo->tm_year + 1900;
        case  1: return timeinfo->tm_mon  + 1;
        case  2: return timeinfo->tm_mday;
        case  3: return timeinfo->tm_hour;
        case  4: return timeinfo->tm_min;
        case  5: return timeinfo->tm_sec;
        default: return result;
      }
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
      while (dname.back() == '/') {
        message_pump(); dname.pop_back();
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

  } // anonymous namespace

  string get_working_directory() {
    std::error_code ec;
    string result = filename_add_slash(std::filesystem::current_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  bool set_working_directory(string dname) {
    std::error_code ec;
    const std::filesystem::path path = std::filesystem::u8path(dname);
    std::filesystem::current_path(path, ec);
    return (ec.value() == 0);
  }

  string get_temp_directory() {
    std::error_code ec;
    string result = filename_add_slash(std::filesystem::temp_directory_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  string get_program_pathname() {
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
    char *buffer = realpath("/proc/self/exe", nullptr);
    path = buffer ? : "";
    free(buffer);
    #elif defined(__FreeBSD__) || defined(__DragonFly__)
    size_t length;
    // CTL_KERN::KERN_PROC::KERN_PROC_PATHNAME(-1)
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    if (sysctl(mib, 4, nullptr, &length, nullptr, 0) == 0) {
      path.resize(length, '\0');
      char *buffer = path.data();
      if (sysctl(mib, 4, buffer, &length, nullptr, 0) == 0) {
        path = string(buffer) + "\0";
      }
    }
    #endif
    return path;
  }

  string get_program_directory() {
    return filename_path(get_program_pathname());
  }

  string get_program_filename() {
    return filename_name(get_program_pathname());
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
    const std::filesystem::path path = std::filesystem::u8path(fname);
    return (std::filesystem::exists(path, ec) && ec.value() == 0 && 
      (!std::filesystem::is_directory(path, ec)) && ec.value() == 0);
  }

  bool directory_exists(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, false);
    dname = environment_expand_variables(dname);
    const std::filesystem::path path = std::filesystem::u8path(dname);
    return (std::filesystem::exists(path, ec) && ec.value() == 0 && 
      std::filesystem::is_directory(path, ec) && ec.value() == 0);
  }

  string filename_canonical(string fname) {
    std::error_code ec;
    fname = environment_expand_variables(fname);
    const std::filesystem::path path = std::filesystem::u8path(fname);
    string result = std::filesystem::weakly_canonical(path, ec).u8string();
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
    const std::filesystem::path path = std::filesystem::u8path(fname);
    std::uintmax_t result = std::filesystem::file_size(path, ec);
    return (ec.value() == 0) ? result : 0;
  }

  bool file_delete(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    fname = environment_expand_variables(fname);
    const std::filesystem::path path = std::filesystem::u8path(fname);
    return (std::filesystem::remove(path, ec) && ec.value() == 0);
  }

  bool directory_create(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, true);
    const std::filesystem::path path = std::filesystem::u8path(dname);
    return (std::filesystem::create_directories(path, ec) && ec.value() == 0);
  }

  bool file_rename(string oldname, string newname) {
    std::error_code ec;
    if (!file_exists(oldname)) return false;
    oldname = environment_expand_variables(oldname);
    newname = environment_expand_variables(newname);
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const std::filesystem::path path1 = std::filesystem::u8path(oldname);
    const std::filesystem::path path2 = std::filesystem::u8path(newname);
    std::filesystem::rename(path1, path2, ec);
    return (ec.value() == 0);
  }

  bool file_copy(string fname, string newname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    fname = environment_expand_variables(fname);
    newname = environment_expand_variables(newname);
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const std::filesystem::path path1 = std::filesystem::u8path(fname);
    const std::filesystem::path path2 = std::filesystem::u8path(newname);
    std::filesystem::copy(path1, path2, ec);
    return (ec.value() == 0);
  }

  std::uintmax_t directory_size(string dname) {
    std::uintmax_t result = 0;
    if (!directory_exists(dname)) return 0;
    const std::filesystem::path path = std::filesystem::u8path(filename_remove_slash(dname, true));
    if (std::filesystem::exists(path)) {
      std::filesystem::directory_iterator end_itr;
      for (std::filesystem::directory_iterator dir_ite(path); dir_ite != end_itr; dir_ite++) {
        message_pump();
        std::filesystem::path file_path = std::filesystem::u8path(filename_absolute(dir_ite->path().u8string()));
        if (!std::filesystem::is_directory(dir_ite->status())) {
          result += file_size(file_path.u8string());
        } else {
          result += directory_size(file_path.u8string());
        }
      }
    }
    return result;
  }

  bool directory_destroy(string dname) {
    std::error_code ec;
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    const std::filesystem::path path = std::filesystem::u8path(dname);
    return (std::filesystem::remove_all(path, ec) && ec.value() == 0);
  }

  bool directory_rename(string oldname, string newname) {
    std::error_code ec;
    if (!directory_exists(oldname)) return false;
    oldname = filename_remove_slash(oldname, true);
    newname = filename_remove_slash(newname, true);
    if (!directory_exists(newname)) directory_create(filename_path(newname));
    bool result = false;
    const std::filesystem::path path1 = std::filesystem::u8path(oldname);
    const std::filesystem::path path2 = std::filesystem::u8path(newname);
    const std::filesystem::path path3 = std::filesystem::u8path(
    filename_add_slash(path2.u8string(), true).substr(0, 
    filename_add_slash(path1.u8string(), true).length()));
    if (directory_exists(oldname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        std::filesystem::rename(path1, path2, ec);
        result = (ec.value() == 0);
      }
    }
    return result;
  }

  vector<string> directory_contents(string dname, string pattern, bool includedirs) {
    std::error_code ec; vector<string> result_unfiltered;
    if (!directory_exists(dname)) return result_unfiltered;
    dname = filename_remove_slash(dname, true);
    const std::filesystem::path path = std::filesystem::u8path(dname);
    if (directory_exists(dname)) {
      std::filesystem::directory_iterator end_itr;
      for (std::filesystem::directory_iterator dir_ite(path, ec); dir_ite != end_itr; dir_ite++) {
        message_pump();
        if (ec.value() != 0) { break; }
        std::filesystem::path file_path = std::filesystem::u8path(filename_absolute(dir_ite->path().u8string()));
        if (!std::filesystem::is_directory(dir_ite->status(ec)) && ec.value() == 0) {
          result_unfiltered.push_back(file_path.u8string());
        } else if (ec.value() == 0 && includedirs) {
          result_unfiltered.push_back(filename_add_slash(file_path.u8string()));
        }
      }
    }
    if (pattern.empty()) pattern = "*.*";
    pattern = string_replace_all(pattern, " ", "");
    pattern = string_replace_all(pattern, "*", "");
    vector<string> extVec = string_split(pattern, ';');
    std::set<string> filteredItems;
    for (const string &item : result_unfiltered) {
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

  static inline vector<string> directory_contents_recursive_helper(string dname, string pattern) {
    vector<string> result = directory_contents(dname, pattern, true);
    for (unsigned i = 0; i < result.size(); i++) {
      message_pump();
      if (directory_exists(result[i])) {
        vector<string> recursive_result = directory_contents_recursive_helper(result[i], pattern);
        if (recursive_result.size() > 0) {
          result.insert(result.end(), recursive_result.begin(), recursive_result.end());
        }
      }
    }
    return result;
  }

  vector<string> directory_contents_recursive(string dname, string pattern, bool includedirs) {
    vector<string> result_unfiltered = directory_contents_recursive_helper(dname, pattern);
    vector<string> result_filtered;
    for (unsigned i = 0; i < result_unfiltered.size(); i++) {
      message_pump();
      if (!directory_exists(result_unfiltered[i])) {
        result_filtered.push_back(result_unfiltered[i]);
      } else if (includedirs) {
        result_filtered.push_back(result_unfiltered[i]);
      }
    }
    std::set<string> removed_duplicates(result_filtered.begin(), result_filtered.end());
    result_filtered.clear(); result_filtered.assign(removed_duplicates.begin(), removed_duplicates.end());
    return result_filtered;
  }

  static string retained_string = "";
  static size_t retained_length = 0;
  static std::uintmax_t szsrc   = 0;
  // this function was written to prevent infinitely copying inside itself
  static inline bool directory_copy_retained(string dname, string newname) {
    std::error_code ec;
    bool result = false;
    const std::filesystem::path path1 = std::filesystem::u8path(dname);
    const std::filesystem::path path2 = std::filesystem::u8path(newname);
    const std::filesystem::path path3 = std::filesystem::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (retained_string.empty() && retained_length == 0) {
      retained_length = path1.u8string().length();
      retained_string = path2.u8string().substr(retained_length);
    }
    if (directory_exists(dname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        std::filesystem::copy(path1, path2, std::filesystem::copy_options::recursive, ec);
        result = (ec.value() == 0);
      } else if (path1.u8string() == path3.u8string()) {
        vector<string> itemVec = directory_contents(dname, "*.*", true);
        if (!directory_exists(newname)) {
          directory_create(newname);
          for (const string &item : itemVec) {
            message_pump();
            if (directory_exists(filename_remove_slash(item)) && 
              filename_remove_slash(item).substr(retained_length) != retained_string) {
              directory_copy_retained(filename_remove_slash(item), filename_add_slash(path2.u8string()) + 
              filename_name(filename_remove_slash(item)));
            } else if (file_exists(item)) {
              std::filesystem::copy(item, filename_add_slash(path2.u8string()) + filename_name(item), ec);
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

  int file_get_date_accessed_year(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), false, 0);
  }

  int file_get_date_accessed_month(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), false, 1);
  }

  int file_get_date_accessed_day(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), false, 2);
  }

  int file_get_date_accessed_hour(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), false, 3);
  }

  int file_get_date_accessed_minute(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), false, 4);
  }

  int file_get_date_accessed_second(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), false, 5);
  }

  int file_get_date_modified_year(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), true, 0);
  }

  int file_get_date_modified_month(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), true, 1);
  }

  int file_get_date_modified_day(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), true, 2);
  }

  int file_get_date_modified_hour(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), true, 3);
  }

  int file_get_date_modified_minute(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), true, 4);
  }

  int file_get_date_modified_second(string fname) {
    fname = environment_expand_variables(fname);
    return file_get_date_accessed_modified(fname.c_str(), true, 5);
  }

  int file_bin_open(string fname, int mode) {
    fname = environment_expand_variables(fname);
    #if defined(_WIN32)
    wstring wfname = widen(fname);
    FILE *fp = nullptr;
    switch (mode) {
      case  0: { fp = _wfopen(wfname.c_str(), L"rb, ccs=UTF-8" ); break; }
      case  1: { fp = _wfopen(wfname.c_str(), L"wb, ccs=UTF-8" ); break; }
      case  2: { fp = _wfopen(wfname.c_str(), L"w+b, ccs=UTF-8"); break; }
      case  3: { fp = _wfopen(wfname.c_str(), L"ab, ccs=UTF-8" ); break; }
      case  4: { fp = _wfopen(wfname.c_str(), L"a+b, ccs=UTF-8"); break; }
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
    return close(fd);
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
    int byte = -1;
    #if defined(_WIN32)
    int num = (int)_read(fd, &byte, 1);
    #else
    int num = (int)read(fd, &byte, 1);
    #endif
    if (num == -1) return -1;
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
    for (unsigned i = 0; i < str.length(); i++) {
      message_pump();
      if (file_bin_write_byte(fd, str[i]) == -1) {
        return -1;
      }
    }
    return (long)str.length();
  }

  long file_text_write_real(int fd, double val) {
    string str = std::to_string(val);
    return file_text_write_string(fd, str);
  }

  int file_text_writeln(int fd) {
    return file_bin_write_byte(fd, '\n');
  }

  bool file_text_eof(int fd) {
    return (file_bin_position(fd) >= file_bin_size(fd));
  }

  bool file_text_eoln(int fd) {
    file_bin_seek(fd, -1);
    bool res = ((char)file_bin_read_byte(fd) == '\n');
    return (file_text_eof(fd) || res);
  }

  double file_text_read_real(int fd) {
    bool dot = false, sign = false;
    string str; char byte = (char)file_bin_read_byte(fd);
    if (byte == '\n') byte = (char)file_bin_read_byte(fd);
    if (byte == '.' && !dot) {
      dot = true;
    } else if (!is_digit(byte) && byte != '+' && 
      byte != '-' && byte != '.') {
      return 0;
    } else if (byte == '+' || byte == '-') {
      sign = true;
    }
    if (byte == -1) goto finish;
    str.resize(str.length() + 1, ' ');
    str[str.length() - 1] = byte;
    if (sign) {
      byte = (char)file_bin_read_byte(fd);
      if (byte == '.' && !dot) {
        dot = true;
      } else if (!is_digit(byte) && byte != '.') {
        return strtod(str.c_str(), nullptr);
      }
      if (byte == -1) goto finish;
      str.resize(str.length() + 1, ' ');
      str[str.length() - 1] = byte;
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
      if (byte == -1) goto finish;
      str.resize(str.length() + 1, ' ');
      str[str.length() - 1] = byte;
    }
    finish:
    return strtod(str.c_str(), nullptr);
  }

  string file_text_read_string(int fd) {
    int byte = -1; string str;
    while ((char)byte != '\n' && !file_text_eof(fd)) {
      message_pump();
      byte = file_bin_read_byte(fd);
      str.resize(str.length() + 1, ' ');
      str[str.length() - 1] = byte;
    }
    if (str[str.length() - 2] != '\r' && str[str.length() - 1] == '\n') {
      file_bin_seek(fd, -1);
    }
    if (str[str.length() - 2] == '\r' && str[str.length() - 1] == '\n') {
      file_bin_seek(fd, -2);
    }
    return str;
  }

  string file_text_readln(int fd) {
    int byte = -1; string str;
    while ((char)byte != '\n' && !file_text_eof(fd)) {
      message_pump();
      byte = file_bin_read_byte(fd);
      str.resize(str.length() + 1, ' ');
      str[str.length() - 1] = ((byte == -1) ? 0 : byte);
      if (byte == -1) break;
    }
    return str;
  }

  string file_text_read_all(int fd) {
    string str;
    while (!file_text_eof(fd)) {
      message_pump();
      char byte = (char)file_bin_read_byte(fd);
      str.resize(str.length() + 1, ' ');
      str[str.length() - 1] = ((byte == -1) ? 0 : byte);
      if (byte == -1) break;
    }
    return str;
  }

  int file_text_open_from_string(string str) {
    int fd[2];
    #if defined(_WIN32)
    if (_pipe(fd, str.length(), O_BINARY) < 0) return -1;
    #else
    if (pipe(fd) < 0) return -1;
    #endif
    file_text_write_string(fd[1], str);
    file_bin_close(fd[1]);
    return fd[0];
  }
  
  int file_text_close(int fd) {
    return file_bin_close(fd);
  }

} // namespace ngs::fs
