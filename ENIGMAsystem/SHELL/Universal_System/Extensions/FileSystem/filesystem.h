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

#include <string>
#include <vector>

#include <ctime>
#include <cstddef>

#include "Universal_System/var4.h"

#include <sys/types.h>
#include <sys/stat.h>

namespace datetime {

  enum dt_type {
    dt_year, 
    dt_month, 
    dt_day, 
    dt_hour, 
    dt_minute, 
    dt_second
  };

  int file_get_date_accessed_modified(const char *fname, bool modified, int type);

}

namespace strings {

  std::string string_replace_all(std::string str, std::string substr, std::string nstr);
  std::vector<std::string> string_split(std::string str, char delimiter);
  std::string filename_path(std::string fname);
  std::string filename_name(std::string fname);
  std::string filename_ext(std::string fname);
  std::string filename_remove_slash(std::string dname, bool canonical = false);
  std::string filename_add_slash(std::string dname, bool canonical = false);

} // namespace slashes

namespace filesystem {

  std::string get_working_directory();
  bool set_working_directory(std::string dname);
  std::string get_temp_directory();
  std::string get_program_directory();
  std::string get_program_filename();
  std::string get_program_pathname();
  std::string filename_absolute(std::string fname);
  std::string filename_canonical(std::string fname);
  bool file_exists(std::string fname);
  bool file_delete(std::string fname);
  bool file_rename(std::string oldname, std::string newname);
  bool file_copy(std::string fname, std::string newname);
  std::uintmax_t file_size(std::string fname);
  bool directory_exists(std::string dname);
  bool directory_create(std::string dname);
  bool directory_destroy(std::string dname);
  bool directory_rename(std::string oldname, std::string newname);
  bool directory_copy(std::string dname, std::string newname);
  std::uintmax_t directory_size(std::string dname);
  std::vector<std::string> directory_contents(std::string dname, std::string pattern = "*.*", bool includedirs = true);
  std::vector<std::string> directory_contents_recursive(std::string dname, std::string pattern = "*.*", bool includedirs = true);
  std::string environment_get_variable(std::string name);
  bool environment_set_variable(std::string name, std::string value);
  bool environment_unset_variable(std::string name);
  std::string environment_expand_variables(std::string str);
  int file_get_date_accessed_year(std::string fname);
  int file_get_date_accessed_month(std::string fname);
  int file_get_date_accessed_day(std::string fname);
  int file_get_date_accessed_hour(std::string fname);
  int file_get_date_accessed_minute(std::string fname);
  int file_get_date_accessed_second(std::string fname);
  int file_get_date_modified_year(std::string fname);
  int file_get_date_modified_month(std::string fname);
  int file_get_date_modified_day(std::string fname);
  int file_get_date_modified_hour(std::string fname);
  int file_get_date_modified_minute(std::string fname);
  int file_get_date_modified_second(std::string fname);

} // namespace filesystem

namespace enigma_user {

  inline std::string fs_get_working_directory() {
    return filesystem::get_working_directory();
  }
  
  inline bool fs_set_working_directory(std::string dname) {
    return filesystem::set_working_directory(dname);
  }
  
  inline std::string fs_get_temp_directory() {
    return filesystem::get_temp_directory();
  }
  
  inline std::string fs_get_program_directory() {
    return filesystem::get_program_directory();
  }
  
  inline std::string fs_get_program_filename() {
    return filesystem::get_program_filename();
  }
  
  inline std::string fs_get_program_pathname() {
    return filesystem::get_program_pathname();
  }
  
  inline std::string fs_filename_absolute(std::string fname) {
    return filesystem::filename_absolute(fname);
  }

  inline std::string fs_filename_canonical(std::string fname) {
    return filesystem::filename_canonical(fname);
  }
  
  inline bool fs_file_exists(std::string fname) {
    return filesystem::file_exists(fname);
  }
  
  inline bool fs_file_delete(std::string fname) {
    return filesystem::file_delete(fname);
  }
  
  inline bool fs_file_rename(std::string oldname, std::string newname) {
    return filesystem::file_rename(oldname, newname);
  }
  
  inline bool fs_file_copy(std::string fname, std::string newname) {
    return filesystem::file_copy(fname, newname);
  }
  
  inline std::uintmax_t fs_file_size(std::string fname) {
    return filesystem::file_size(fname);
  }
  
  inline bool fs_directory_exists(std::string dname) {
    return filesystem::directory_exists(dname);
  }
  
  inline bool fs_directory_create(std::string dname) {
    return filesystem::directory_create(dname);
  }
  
  inline bool fs_directory_destroy(std::string dname) {
    return filesystem::directory_destroy(dname);
  }
  
  inline bool fs_directory_rename(std::string oldname, std::string newname) {
    return filesystem::directory_rename(oldname, newname);
  }
  
  inline bool fs_directory_copy(std::string dname, std::string newname) {
    return filesystem::directory_copy(dname, newname);
  }
  
  inline std::uintmax_t fs_directory_size(std::string dname) {
    return filesystem::directory_size(dname);
  }
  
  inline var fs_directory_contents(std::string dname, std::string pattern = "*.*", bool includedirs = true) {
    var resVar; std::vector<std::string> resVec = filesystem::directory_contents(dname, pattern, includedirs);
    for (unsigned i = 0; i < resVec.size(); i++) resVar[i] = resVec[i];
    return resVar;
  }

  inline var fs_directory_contents_recursive(std::string dname, std::string pattern = "*.*", bool includedirs = true) {
    var resVar; std::vector<std::string> resVec = filesystem::directory_contents_recursive(dname, pattern, includedirs);
    for (unsigned i = 0; i < resVec.size(); i++) resVar[i] = resVec[i];
	return resVar;
  }

  inline std::string fs_environment_get_variable(std::string name) {
    return filesystem::environment_get_variable(name);
  }
  
  inline bool fs_environment_set_variable(std::string name, std::string value) {
    return filesystem::environment_set_variable(name, value);
  }

  inline bool fs_environment_unset_variable(std::string name) {
    return filesystem::environment_unset_variable(name);
  }
  
  inline std::string fs_environment_expand_variables(std::string str) {
    return filesystem::environment_expand_variables(str);
  }
  
  inline int fs_file_get_date_accessed_year(std::string fname) {
    return filesystem::file_get_date_accessed_year(fname);
  }
  
  inline int fs_file_get_date_accessed_month(std::string fname) {
    return filesystem::file_get_date_accessed_month(fname);
  }
  
  inline int fs_file_get_date_accessed_day(std::string fname) {
    return filesystem::file_get_date_accessed_day(fname);
  }
  
  inline int fs_file_get_date_accessed_hour(std::string fname) {
    return filesystem::file_get_date_accessed_hour(fname);
  }
  
  inline int fs_file_get_date_accessed_minute(std::string fname) {
    return filesystem::file_get_date_accessed_minute(fname);
  }
  
  inline int fs_file_get_date_accessed_second(std::string fname) {
    return filesystem::file_get_date_accessed_second(fname);
  }
  
  inline int fs_file_get_date_modified_year(std::string fname) {
    return filesystem::file_get_date_modified_year(fname);
  }
  
  inline int fs_file_get_date_modified_month(std::string fname) {
    return filesystem::file_get_date_modified_month(fname);
  }
  
  inline int fs_file_get_date_modified_day(std::string fname) {
    return filesystem::file_get_date_modified_day(fname);
  }
  
  inline int fs_file_get_date_modified_hour(std::string fname) {
    return filesystem::file_get_date_modified_hour(fname);
  }
  
  inline int fs_file_get_date_modified_minute(std::string fname) {
    return filesystem::file_get_date_modified_minute(fname);
  }
  
  inline int fs_file_get_date_modified_second(std::string fname) {
    return filesystem::file_get_date_modified_second(fname);
  }

} // namespace enigma_user
