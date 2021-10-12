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

#include "filesystem.h"
#include "Universal_System/var4.h"

namespace enigma_fs {

  std::string fs_get_working_directory();
  bool fs_set_working_directory(std::string dname);
  std::string fs_get_temp_directory();
  std::string fs_get_program_directory();
  std::string fs_get_program_filename();
  std::string fs_get_program_pathname();
  std::string fs_filename_absolute(std::string fname);
  std::string fs_filename_canonical(std::string fname);
  bool fs_file_exists(std::string fname);
  bool fs_file_delete(std::string fname);
  bool fs_file_rename(std::string oldname, std::string newname);
  bool fs_file_copy(std::string fname, std::string newname);
  std::uintmax_t fs_file_size(std::string fname);
  bool fs_directory_exists(std::string dname);
  bool fs_directory_create(std::string dname);
  bool fs_directory_destroy(std::string dname);
  bool fs_directory_rename(std::string oldname, std::string newname);
  bool fs_directory_copy(std::string dname, std::string newname);
  std::uintmax_t fs_directory_size(std::string dname);
  var fs_directory_contents(std::string dname, std::string pattern = "*.*", bool includedirs = true);
  var fs_directory_contents_recursive(std::string dname, std::string pattern = "*.*", bool includedirs = true);
  std::string fs_environment_get_variable(std::string name);
  bool fs_environment_set_variable(std::string name, std::string value);
  bool fs_environment_unset_variable(std::string name);
  std::string fs_environment_expand_variables(std::string str);
  int fs_file_get_date_accessed_year(std::string fname);
  int fs_file_get_date_accessed_month(std::string fname);
  int fs_file_get_date_accessed_day(std::string fname);
  int fs_file_get_date_accessed_hour(std::string fname);
  int fs_file_get_date_accessed_minute(std::string fname);
  int fs_file_get_date_accessed_second(std::string fname);
  int fs_file_get_date_modified_year(std::string fname);
  int fs_file_get_date_modified_month(std::string fname);
  int fs_file_get_date_modified_day(std::string fname);
  int fs_file_get_date_modified_hour(std::string fname);
  int fs_file_get_date_modified_minute(std::string fname);
  int fs_file_get_date_modified_second(std::string fname);

} // namespace enigma_fs

namespace enigma_user {

  using namespace enigma_fs;
  #define get_working_directory fs_get_working_directory()
  #define set_working_directory(x) fs_set_working_directory(x)
  #define get_temp_directory fs_get_temp_directory()
  #define get_program_directory fs_get_program_directory()
  #define get_program_filename fs_get_program_filename()
  #define get_program_pathname fs_get_program_pathname()
  #define filename_absolute(x) fs_filename_absolute(x)
  #define filename_canonical(x) fs_filename_canonical(x)
  #define file_exists(x) fs_file_exists(x)
  #define file_delete(x) fs_file_delete(x)
  #define file_rename(x, y) fs_file_rename(x, y)
  #define file_copy(x, y) fs_file_copy(x, y)
  #define file_size(x) fs_file_size(x)
  #define directory_exists(x) fs_directory_exists(x)
  #define directory_create(x) fs_directory_create(x)
  #define directory_destroy(x) fs_directory_destroy(x)
  #define directory_rename(x, y) fs_directory_rename(x, y)
  #define directory_copy(x, y) fs_directory_copy(x, y)
  #define directory_size(x) fs_directory_size(x)
  #define directory_contents(x) fs_directory_contents(x)
  #define directory_contents_ext(x, y, z) fs_directory_contents_ext(x, y, z)
  #define directory_contents_recursive(x) fs_directory_contents_recursive(x)
  #define directory_contents_recursive_ext(x, y, z) fs_directory_contents_recursive_ext(x, y, z)
  #define environment_get_variable(x) fs_environment_get_variable(x)
  #define environment_set_variable(x, y) fs_environment_set_variable(x, y)
  #define environment_unset_variable(x) fs_environment_unset_variable(x)
  #define environment_expand_variables(str) fs_environment_expand_variables(str)
  #define file_get_date_accessed_year(x) fs_file_get_date_accessed_year(x)
  #define file_get_date_accessed_month(x) fs_file_get_date_accessed_month(x)
  #define file_get_date_accessed_day(x) fs_file_get_date_accessed_day(x)
  #define file_get_date_accessed_hour(x) fs_file_get_date_accessed_hour(x)
  #define file_get_date_accessed_minute(x) fs_file_get_date_accessed_minute(x)
  #define file_get_date_accessed_second(x) fs_file_get_date_accessed_second(x)
  #define file_get_date_modified_year(x) fs_file_get_date_modified_year(x)
  #define file_get_date_modified_month(x) fs_file_get_date_modified_month(x)
  #define file_get_date_modified_day(x) fs_file_get_date_modified_day(x)
  #define file_get_date_modified_hour(x) fs_file_get_date_modified_hour(x)
  #define file_get_date_modified_minute(x) fs_file_get_date_modified_minute(x)
  #define file_get_date_modified_second(x) fs_file_get_date_modified_second(x)

} // namespace enigma_user
