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

#include "../fileman.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <set>

namespace fs = std::filesystem;

using namespace strings;

using std::string;
using std::vector;
using std::size_t;
using std::cout;
using std::endl;

namespace strings {

  string string_replace_all(string str, string substr, string nstr) {
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != string::npos) {
      str.replace(pos, substr.length(), nstr);
      pos += nstr.length();
    }
    return str;
  }

  vector<string> string_split(string str, char delimiter) {
    vector<string> vec;
    std::stringstream sstr(str);
    string tmp;
    while (std::getline(sstr, tmp, delimiter))
      vec.push_back(tmp);
    return vec;
  }

  string filename_path(string fname) {
    size_t fp = fname.find_last_of("/\\");
    return fname.substr(0,fp + 1);
  }

  string filename_name(string fname) {
    size_t fp = fname.find_last_of("/\\");
    return fname.substr(fp + 1);
  }

  string filename_ext(string fname) {
    fname = filename_name(fname);
    size_t fp = fname.find_last_of(".");
    if (fp == string::npos)
      return "";
    return fname.substr(fp);
  }

} // namespace strings

namespace fileman {

  string get_working_directory_ns() {
    string result = "";
    try {
      result = filename_add_slash(fs::current_path().u8string());
      if (!result.empty()) cout << "working_directory = \"" << result << "\"" << endl;
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  string get_temp_directory_ns() {
    string result = "";
    try {
      result = filename_add_slash(fs::temp_directory_path().u8string());
      if (!result.empty()) cout << "temp_directory = \"" << result << "\"" << endl;
    } catch (const fs::filesystem_error& e) {
      result = environment_get_variable_ns("TMP");
      if (result.empty()) result = environment_get_variable_ns("TEMP");
      if (result.empty()) result = environment_get_variable_ns("USERPROFILE");
      if (result.empty()) result = environment_get_variable_ns("WINDIR");
      if (!result.empty()) {
        result = filename_add_slash(result);
        cout << "temp_directory = \"" << result << "\"" << endl;
      } else {
        cout << e.what() << endl;
      }
    }
    return result;
  }

  string get_program_directory_ns() {
    string result = filename_path(get_program_pathname_ns(false));
    if (!result.empty()) cout << "program_directory = \"" << result << "\"" << endl;
    return result; 
  }

  string get_program_filename_ns() {
    string result = filename_name(get_program_pathname_ns(false));
    if (!result.empty()) cout << "program_filename = \"" << result << "\"" << endl;
    return result;
  }

  string filename_absolute_ns(string fname) {
    string result = "";
    try {
      if (directory_exists_ns(fname)) {
        result = filename_add_slash(fname, true);
      } else if (file_exists_ns(fname)) {
        result = filename_normalize(fname);
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  static inline std::uintmax_t file_size(string fname) {
    fname = filename_normalize(fname);
    std::uintmax_t result = 0;
    try {
      const fs::path path = fs::u8path(fname);
      result = fs::file_size(path);
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool file_exists_ns(string fname) {
    fname = filename_normalize(fname);
    bool result = false;
    try {
      const fs::path path = fs::u8path(fname);
      result = (fs::exists(path) && (!fs::is_directory(path)));
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool file_delete_ns(string fname) {
    fname = filename_normalize(fname);
    cout << "try: file_delete(\"" << fname << "\")" << endl;
    bool result = false;
    try {
      const fs::path path = fs::u8path(fname);
      if (file_exists_ns(fname)) {
        result = fs::remove(path);
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool file_rename_ns(string oldname, string newname) {
    if (!directory_exists_ns(filename_path(newname)))
      directory_create_ns(filename_path(newname));
    oldname = filename_normalize(oldname);
    newname = filename_normalize(newname);
    cout << "try: file_rename(\"" << oldname << "\", \"" << newname << "\")" << endl;
    bool result = false;
    try {
      const fs::path path1 = fs::u8path(oldname);
      const fs::path path2 = fs::u8path(newname);
      if (file_exists_ns(oldname)) {
        std::uintmax_t oldsize = file_size(oldname);
        fs::rename(path1, path2);
        result = (file_exists_ns(newname) && oldsize == file_size(newname));
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool file_copy_ns(string fname, string newname) {
    if (!directory_exists_ns(filename_path(newname)))
      directory_create_ns(filename_path(newname));
    fname = filename_normalize(fname);
    newname = filename_normalize(newname);
    cout << "try: file_copy(\"" << fname << "\", \"" << newname << "\")" << endl;
    bool result = false;
    try {
      const fs::path path1 = fs::u8path(fname);
      const fs::path path2 = fs::u8path(newname);
      if (file_exists_ns(fname)) {
        fs::copy(path1, path2);
        result = (file_exists_ns(newname) && file_size(fname) == file_size(newname));
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  static inline std::uintmax_t directory_size(string dname) {
    std::uintmax_t result = 0;
    try {                     
      const fs::path path = fs::u8path(filename_remove_slash(dname, true));
      if (fs::exists(path)) {
        fs::directory_iterator end_itr;
        for (fs::directory_iterator dir_ite(path); dir_ite != end_itr; dir_ite++) {
          fs::path file_path(filename_absolute_ns(dir_ite->path().u8string()));
          if (!fs::is_directory(dir_ite->status())) {
            result += file_size(file_path.u8string());
          } else {
            result += directory_size(file_path.u8string());
          }
        }
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool directory_exists_ns(string dname) {
    dname = filename_remove_slash(dname, true);
    bool result = false;
    try {
      const fs::path path = fs::u8path(dname);
      result = (fs::exists(path) && fs::is_directory(path));
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool directory_create_ns(string dname) {
    dname = filename_remove_slash(dname, true);
    bool result = false;
    try {
      const fs::path path = fs::u8path(dname);
      result = ((!fs::exists(path)) && fs::create_directory(path));
      while (!directory_exists_ns(dname) && directory_create_ns(dname));
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool directory_destroy_ns(string dname) {
    dname = filename_remove_slash(dname, true);
    cout << "try: directory_destroy(\"" << dname << "\")" << endl;
    bool result = false;
    try {
      const fs::path path = fs::u8path(dname);
      if (directory_exists_ns(dname)) {
        result = fs::remove_all(path);
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool directory_rename_ns(string oldname, string newname) {
    if (!directory_exists_ns(newname)) directory_create_ns(newname);
    oldname = filename_remove_slash(oldname, true);
    newname = filename_remove_slash(newname, true);
    cout << "try: directory_rename(\"" << oldname << "\", \"" << newname << "\")" << endl;
    bool result = false;
    try {
      const fs::path path1 = fs::u8path(oldname);
      const fs::path path2 = fs::u8path(newname);
      const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
      if (directory_exists_ns(oldname)) {
        if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
          filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
          path1.u8string() != path3.u8string()) {
          std::uintmax_t oldsize = directory_size(oldname);
          fs::rename(path1, path2);
          result = (directory_exists_ns(newname) && oldsize == directory_size(newname));
        }
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  static string retained_string = "";
  static size_t retained_length = 0;
  // this function was written to prevent infinitely copying inside itself
  static inline bool directory_copy_ns_retained(string dname, string newname) {
    bool result = false;
    try {
      const fs::path path1 = fs::u8path(dname);
      const fs::path path2 = fs::u8path(newname);
      const fs::path path3 = fs::u8path(path2.u8string().substr(0, path1.u8string().length()));
      if (retained_string.empty() && retained_length == 0) {
        retained_length = path1.u8string().length();
        retained_string = path2.u8string().substr(retained_length);
      }
      if (directory_exists_ns(dname)) {
        if ((filename_name(path1.u8string()) != filename_name(path2.u8string()) &&
          filename_path(path1.u8string()) == filename_path(path2.u8string())) ||
          path1.u8string() != path3.u8string()) {
          fs::copy(path1, path2, fs::copy_options::recursive);
          result = (directory_exists_ns(newname) && directory_size(dname) == directory_size(newname));
        } else if (path1.u8string() == path3.u8string()) {
          vector<string> itemVec = string_split(directory_contents_ns(dname), '\n');
          if (!directory_exists_ns(newname)) {
            directory_create_ns(newname);
            for (const string &item : itemVec) {
              if (directory_exists_ns(filename_remove_slash(item)) && 
                filename_remove_slash(item).substr(retained_length) != retained_string) {
                directory_copy_ns_retained(filename_remove_slash(item), filename_add_slash(path2.u8string()) + 
                  filename_name(filename_remove_slash(item)));
              } else if (file_exists_ns(item)) {
                fs::copy(item, filename_add_slash(path2.u8string()) + filename_name(item));
              }
            }
          }
          return (directory_exists_ns(newname) && (directory_size(dname) * 2) >= directory_size(newname));
        }
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    return result;
  }

  bool directory_copy_ns(string dname, string newname) {
    if (!directory_exists_ns(newname)) directory_create_ns(newname);
    dname = filename_remove_slash(dname, true);
    newname = filename_remove_slash(newname, true);
    cout << "try: directory_copy(\"" << dname << "\", \"" << newname << "\")" << endl;
    retained_string = "";
    retained_length = 0;
    return directory_copy_ns_retained(dname, newname);
  }

  string directory_contents_ns(string dname, string pattern, bool includedirs) {
    string result = "";
    try {
      const fs::path path = fs::u8path(filename_remove_slash(dname, true));
      if (fs::exists(path)) {
        fs::directory_iterator end_itr;
        for (fs::directory_iterator dir_ite(path); dir_ite != end_itr; dir_ite++) {
          fs::path file_path(filename_absolute_ns(dir_ite->path().u8string()));
          if (!fs::is_directory(dir_ite->status())) {
            result += file_path.u8string() + "\n";
          } else if (includedirs) {
            result += filename_add_slash(file_path.u8string()) + "\n";
          }
        }
      }
    } catch (const fs::filesystem_error& e) {
      cout << e.what() << endl;
    }
    if (pattern.empty()) pattern = "*.*";
    if (result.back() == '\n') result.pop_back();
    pattern = string_replace_all(pattern, " ", "");
    pattern = string_replace_all(pattern, "*", "");
    vector<string> itemVec = string_split(result, '\n');
    vector<string> extVec = string_split(pattern, ';');
    std::set<string> filteredItems;
    for (const string &item : itemVec) {
      for (const string &ext : extVec) {
        if (ext == "." || ext == filename_ext(item) || directory_exists_ns(item)) {
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

} // namespace fileman
