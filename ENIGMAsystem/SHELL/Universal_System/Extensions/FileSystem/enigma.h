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

  std::string fs_directory_get_current_working();
  bool fs_directory_set_current_working(std::string dname);
  std::string fs_directory_get_temporary_path();
  std::string fs_executable_get_directory();
  std::string fs_executable_get_filename();
  std::string fs_executable_get_pathname();
  std::string fs_file_bin_pathname(int fd);
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
  unsigned fs_directory_contents_get_order();
  void fs_directory_contents_set_order(unsigned order);
  unsigned fs_directory_contents_get_cntfiles();
  unsigned fs_directory_contents_get_maxfiles();
  void fs_directory_contents_set_maxfiles(unsigned maxfiles);
  std::string fs_directory_contents_first(std::string dname, std::string pattern, bool includedirs, bool recursive);
  std::string fs_directory_contents_next();
  void fs_directory_contents_close();
  std::string fs_environment_get_variable(std::string name);
  bool fs_environment_set_variable(std::string name, std::string value);
  bool fs_environment_unset_variable(std::string name);
  std::string environment_expand_variables(std::string str);
  int fs_file_datetime_accessed_year(std::string fname);
  int fs_file_datetime_accessed_month(std::string fname);
  int fs_file_datetime_accessed_day(std::string fname);
  int fs_file_datetime_accessed_hour(std::string fname);
  int fs_file_datetime_accessed_minute(std::string fname);
  int fs_file_datetime_accessed_second(std::string fname);
  int fs_file_datetime_modified_year(std::string fname);
  int fs_file_datetime_modified_month(std::string fname);
  int fs_file_datetime_modified_day(std::string fname);
  int fs_file_datetime_modified_hour(std::string fname);
  int fs_file_datetime_modified_minute(std::string fname);
  int fs_file_datetime_modified_second(std::string fname);
  int fs_file_datetime_created_year(std::string fname);
  int fs_file_datetime_created_month(std::string fname);
  int fs_file_datetime_created_day(std::string fname);
  int fs_file_datetime_created_hour(std::string fname);
  int fs_file_datetime_created_minute(std::string fname);
  int fs_file_datetime_created_second(std::string fname);
  int fs_file_bin_datetime_accessed_year(int fd);
  int fs_file_bin_datetime_accessed_month(int fd);
  int fs_file_bin_datetime_accessed_day(int fd);
  int fs_file_bin_datetime_accessed_hour(int fd);
  int fs_file_bin_datetime_accessed_minute(int fd);
  int fs_file_bin_datetime_accessed_second(int fd);
  int fs_file_bin_datetime_modified_year(int fd);
  int fs_file_bin_datetime_modified_month(int fd);
  int fs_file_bin_datetime_modified_day(int fd);
  int fs_file_bin_datetime_modified_hour(int fd);
  int fs_file_bin_datetime_modified_minute(int fd);
  int fs_file_bin_datetime_modified_second(int fd);
  int fs_file_bin_datetime_created_year(int fd);
  int fs_file_bin_datetime_created_month(int fd);
  int fs_file_bin_datetime_created_day(int fd);
  int fs_file_bin_datetime_created_hour(int fd);
  int fs_file_bin_datetime_created_minute(int fd);
  int fs_file_bin_datetime_created_second(int fd);
  int fs_file_bin_open(std::string fname, int mode);
  int fs_file_bin_rewrite(int fd);
  int fs_file_bin_close(int fd);
  long fs_file_bin_size(int fd);
  long fs_file_bin_position(int fd);
  long fs_file_bin_seek(int fd, long pos);
  int fs_file_bin_read_byte(int fd);
  int fs_file_bin_write_byte(int fd, int byte);
  int fs_file_text_open_read(std::string fname);
  int fs_file_text_open_write(std::string fname);
  int fs_file_text_open_append(std::string fname);
  long fs_file_text_write_real(int fd, double val);
  long fs_file_text_write_string(int fd, std::string str);
  int fs_file_text_writeln(int fd);
  bool fs_file_text_eoln(int fd);
  bool fs_file_text_eof(int fd);
  double fs_file_text_read_real(int fd);
  std::string fs_file_text_read_string(int fd);
  std::string fs_file_text_readln(int fd);
  std::string fs_file_text_read_all(int fd);
  int fs_file_text_open_from_string(std::string str);
  int fs_file_text_close(int fd);

} // namespace enigma_fs

namespace enigma_user {

  using namespace enigma_fs;
  #define directory_get_current_working fs_directory_get_current_working()
  #define directory_set_current_working(x) fs_directory_set_current_working(x)
  #define directory_get_temporary_path fs_directory_get_temporary_path()
  #define executable_get_directory fs_executable_get_directory()
  #define executable_get_filename fs_executable_get_filename()
  #define executable_get_pathname fs_executable_get_pathname()
  #define file_bin_pathname(x) fs_file_bin_pathname(x)
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
  #define directory_contents_get_order fs_directory_contents_get_order();
  #define directory_contents_set_order(x) fs_directory_contents_set_order(x);
  #define directory_contents_get_cntfiles fs_directory_contents_get_cntfiles();
  #define directory_contents_get_maxfiles fs_directory_contents_get_maxfiles();
  #define directory_contents_set_maxfiles(x) fs_directory_contents_set_maxfiles(x);
  #define directory_contents_first(w, x, y, z) fs_directory_contents_first(w, x, y, z);
  #define directory_contents_next fs_directory_contents_next();
  #define directory_contents_close fs_directory_contents_close(); 
  #define environment_get_variable(x) fs_environment_get_variable(x)
  #define environment_set_variable(x, y) fs_environment_set_variable(x, y)
  #define environment_unset_variable(x) fs_environment_unset_variable(x)
  #define environment_expand_variables(str) fs_environment_expand_variables(str)
  #define file_bin_datetime_accessed_year(x) fs_file_bin_datetime_accessed_year(x)
  #define file_bin_datetime_accessed_month(x) fs_file_bin_datetime_accessed_month(x)
  #define file_bin_datetime_accessed_day(x) fs_file_bin_datetime_accessed_day(x)
  #define file_bin_datetime_accessed_hour(x) fs_file_bin_datetime_accessed_hour(x)
  #define file_bin_datetime_accessed_minute(x) fs_file_bin_datetime_accessed_minute(x)
  #define file_bin_datetime_accessed_second(x) fs_file_bin_datetime_accessed_second(x)
  #define file_bin_datetime_modified_year(x) fs_file_bin_datetime_modified_year(x)
  #define file_bin_datetime_modified_month(x) fs_file_bin_datetime_modified_month(x)
  #define file_bin_datetime_modified_day(x) fs_file_bin_datetime_modified_day(x)
  #define file_bin_datetime_modified_hour(x) fs_file_bin_datetime_modified_hour(x)
  #define file_bin_datetime_modified_minute(x) fs_file_bin_datetime_modified_minute(x)
  #define file_bin_datetime_modified_second(x) fs_file_bin_datetime_modified_second(x)
  #define file_bin_datetime_created_year(x) fs_file_bin_datetime_created_year(x)
  #define file_bin_datetime_created_month(x) fs_file_bin_datetime_created_month(x)
  #define file_bin_datetime_created_day(x) fs_file_bin_datetime_created_day(x)
  #define file_bin_datetime_created_hour(x) fs_file_bin_datetime_created_hour(x)
  #define file_bin_datetime_created_minute(x) fs_file_bin_datetime_created_minute(x)
  #define file_bin_datetime_created_second(x) fs_file_bin_datetime_created_second(x)
  #define file_bin_open(x, y) fs_file_bin_open(x, y)
  #define file_bin_rewrite(x) fs_file_bin_rewrite(x)
  #define file_bin_close(x) fs_file_bin_close(x)
  #define file_bin_size(x) fs_file_bin_size(x)
  #define file_bin_position(x) fs_file_bin_position(x)
  #define file_bin_seek(x, y) fs_file_bin_seek(x, y)
  #define file_bin_read_byte(x) fs_file_bin_read_byte(x)
  #define file_bin_write_byte(x, y) fs_file_bin_write_byte(x, y)
  #define file_text_open_read(x) fs_file_text_open_read(x)
  #define file_text_open_write(x) fs_file_text_open_write(x)
  #define file_text_open_append(x) fs_file_text_open_append(x)
  #define file_text_write_real(x, y) fs_file_text_write_real(x, y)
  #define file_text_write_string(x, y) fs_file_text_write_string(x, y)
  #define file_text_writeln(x) fs_file_text_writeln(x)
  #define file_text_eoln(x) fs_file_text_eoln(x)
  #define file_text_eof(x) fs_file_text_eof(x)
  #define file_text_read_real(x) fs_file_text_read_real(x)
  #define file_text_read_string(x) fs_file_text_read_string(x)
  #define file_text_readln(x) fs_file_text_readln(x)
  #define file_text_read_all(x) fs_file_text_read_all(x)
  #define file_text_open_from_string(x) fs_file_text_open_from_string(x)
  #define file_text_close(x) fs_file_text_close(x)

} // namespace enigma_user
