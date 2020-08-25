/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
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

#include <filesystem>
#include <iostream>
#include <sstream>
#include <set>

#include "filesystem.h"
#include "Platforms/General/PFmain.h"
#include "strings_util.h"
#include "estring.h"

namespace fs = std::filesystem;

using std::string;
using std::vector;
using std::size_t;

namespace {

  string filename_remove_slash(string dname, bool canonical = false) {
    if (canonical) dname = enigma_user::filename_canonical(dname);
    #ifdef _WIN32
    while (dname.back() == '\\' || dname.back() == '/') dname.pop_back();
    #else
    while (dname.back() == '/') dname.pop_back();
    #endif
    return dname;
  }

  string filename_add_slash(string dname, bool canonical = false) {
    #ifdef _WIN32
    filename_remove_slash(dname, canonical);
    dname += "\\";
    #else
    if (canonical) dname = enigma_user::filename_canonical(dname);
    if (dname.back() != '/') dname += "/";
    #endif
    return dname;
  }

} // anonymous namespace

namespace enigma_user {

  string get_working_directory() {
    std::error_code ec;
    string result = filename_add_slash(fs::current_path(ec).u8string());
    return (ec.value() == 0) ? result : "";
  }

  bool set_working_directory(string dname) {
    std::error_code ec;
    const fs::path path = fs::u8path(dname);
    fs::current_path(path, ec);
    if (ec.value() == 0) {
      working_directory = get_working_directory();
    }
    return (ec.value() == 0);
  }

  string get_temp_directory() {
    std::error_code ec;
    string result = filename_add_slash(fs::temp_directory_path(ec).u8string());
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
  
  string filename_canonical(string fname) {
    std::error_code ec;
    const fs::path path = fs::u8path(fname);
    string result = fs::weakly_canonical(path, ec).u8string();
    #ifdef _WIN32
    string result = string_replace_all(result, "/", "\\");
    #endif
    if (ec.value() == 0 && directory_exists(result)) {
      return filename_add_slash(result);
    }
    return (ec.value() == 0) ? result : "";
  }

  std::uintmax_t file_size(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return 0;
    const fs::path path = fs::u8path(fname);
    std::uintmax_t result = fs::file_size(path, ec);
    return (ec.value() == 0) ? result : 0;
  }

  bool file_exists(string fname) {
    std::error_code ec;
    const fs::path path = fs::u8path(fname);
    return (fs::exists(path, ec) && ec.value() == 0 && 
      (!fs::is_directory(path, ec)) && ec.value() == 0);
  }

  bool file_delete(string fname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    const fs::path path = fs::u8path(fname);
    return (fs::remove(path, ec) && ec.value() == 0);
  }

  bool file_rename(string oldname, string newname) {
    std::error_code ec;
    if (!file_exists(oldname)) return false;
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const fs::path path1 = fs::u8path(oldname);
    const fs::path path2 = fs::u8path(newname);
    fs::rename(path1, path2, ec);
    return (ec.value() == 0);
  }

  bool file_copy(string fname, string newname) {
    std::error_code ec;
    if (!file_exists(fname)) return false;
    if (!directory_exists(filename_path(newname)))
      directory_create(filename_path(newname));
    const fs::path path1 = fs::u8path(fname);
    const fs::path path2 = fs::u8path(newname);
    fs::copy(path1, path2, ec);
    return (ec.value() == 0);
  }

  std::uintmax_t directory_size(string dname) {
    std::uintmax_t result = 0;
    if (!directory_exists(dname)) return 0;
    const fs::path path = fs::u8path(filename_remove_slash(dname, true));
    if (fs::exists(path)) {
      fs::directory_iterator end_itr;
      for (fs::directory_iterator dir_ite(path); dir_ite != end_itr; dir_ite++) {
        fs::path file_path(filename_absolute(dir_ite->path().u8string()));
        if (!fs::is_directory(dir_ite->status())) {
          result += file_size(file_path.u8string());
        } else {
          result += directory_size(file_path.u8string());
        }
      }
    }
    return result;
  }

  bool directory_exists(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, false);
    const fs::path path = fs::u8path(dname);
    return (fs::exists(path, ec) && ec.value() == 0 && 
      fs::is_directory(path, ec) && ec.value() == 0);
  }

  bool directory_create(string dname) {
    std::error_code ec;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    return (fs::create_directories(path, ec) && ec.value() == 0);
  }

  bool directory_destroy(string dname) {
    std::error_code ec;
    if (!directory_exists(dname)) return false;
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    return (fs::remove(path, ec) && ec.value() == 0);
  }

  bool directory_rename(string oldname, string newname) {
    std::error_code ec;
    if (!directory_exists(oldname)) return false;
    if (!directory_exists(newname)) directory_create(newname);
    oldname = filename_remove_slash(oldname, true);
    newname = filename_remove_slash(newname, true);
    bool result = false;
    const fs::path path1 = fs::u8path(oldname);
    const fs::path path2 = fs::u8path(newname);
    const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (directory_exists(oldname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        fs::rename(path1, path2, ec);
        result = (ec.value() == 0);
      }
    }
    return result;
  }
  
  static inline string directory_contents_helper(string dname, string pattern, bool includedirs) {
    std::error_code ec;
    string result = "";
    if (!directory_exists(dname)) return "";
    dname = filename_remove_slash(dname, true);
    const fs::path path = fs::u8path(dname);
    if (directory_exists(dname)) {
      fs::directory_iterator end_itr;
      for (fs::directory_iterator dir_ite(path, ec); dir_ite != end_itr; dir_ite++) {
        if (ec.value() != 0) { break; }
        fs::path file_path(filename_absolute(dir_ite->path().u8string()));
        if (!fs::is_directory(dir_ite->status(ec)) && ec.value() == 0) {
          result += file_path.u8string() + "\n";
        } else if (ec.value() == 0 && includedirs) {
          result += filename_add_slash(file_path.u8string()) + "\n";
        }
      }
    }
    if (pattern.empty()) pattern = "*.*";
    if (result.back() == '\n') result.pop_back();
    pattern = string_replace_all(pattern, " ", "");
    pattern = string_replace_all(pattern, "*", "");
    vector<string> itemVec = split_string(result, '\n');
    vector<string> extVec = split_string(pattern, ';');
    std::set<string> filteredItems;
    for (const string &item : itemVec) {
      for (const string &ext : extVec) {
        if (ext == "." || ext == filename_ext(item) || directory_exists(item)) {
          filteredItems.insert(item);
          break;
        }
      }
    }
    result = "";
    if (filteredItems.empty()) return result;
    for (const string &filteredName : filteredItems) {
      result += filteredName + "\n";
    }
    result.pop_back();
    return result;
  }

  static string retained_string = "";
  static size_t retained_length = 0;
  static std::uintmax_t szSrc   = 0;
  // this function was written to prevent infinitely copying inside itself
  static inline bool directory_copy_retained(string dname, string newname) {
    std::error_code ec;
    bool result = false;
    const fs::path path1 = fs::u8path(dname);
    const fs::path path2 = fs::u8path(newname);
    const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
    if (retained_string.empty() && retained_length == 0) {
      retained_length = path1.u8string().length();
      retained_string = path2.u8string().substr(retained_length);
    }
    if (directory_exists(dname)) {
      if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
        filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
        path1.u8string() != path3.u8string()) {
        fs::copy(path1, path2, fs::copy_options::recursive, ec);
        result = (ec.value() == 0);
      } else if (path1.u8string() == path3.u8string()) {
        vector<string> itemVec = split_string(directory_contents_helper(dname, "*.*", true), '\n');
        if (!directory_exists(newname)) {
          directory_create(newname);
          for (const string &item : itemVec) {
            if (directory_exists(filename_remove_slash(item)) && 
              filename_remove_slash(item).substr(retained_length) != retained_string) {
              directory_copy_retained(filename_remove_slash(item), filename_add_slash(path2.u8string()) + 
              filename_name(filename_remove_slash(item)));
            } else if (file_exists(item)) {
              fs::copy(item, filename_add_slash(path2.u8string()) + filename_name(item), ec);
              // ignore and skip errored copies and copy what is left.
              // uncomment the line below to break if one copy failed.
              // if (ec == 0) { result = true } else { return false; }
            }
          }
          // check size to detemine success instead of error code.
          // comment the line below out if you want stop on error.
          result = (directory_exists(newname) && szSrc == directory_size(newname));
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
    std::uintmax_t szSrc = directory_size(dname);
    return directory_copy_retained(dname, newname);
  }

  var directory_contents(string dname, string pattern, bool includedirs) {
    return string_split(directory_contents_helper(dname, pattern, includedirs), "\n", true);
  }

} // namespace enigma_user
