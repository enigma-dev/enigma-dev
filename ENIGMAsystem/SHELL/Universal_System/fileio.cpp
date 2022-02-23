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

#include "fileio.h"

namespace filesystem = ngs::fs;

namespace enigma_user {

  std::string directory_get_current_working() {
    return filesystem::directory_get_current_working();
  }
 
  bool directory_set_current_working(std::string dname) {
    return filesystem::directory_set_current_working(dname);
  }
 
  std::string directory_get_temporary_path() {
    return filesystem::directory_get_temporary_path();
  }
 
  std::string executable_get_directory() {
    return filesystem::executable_get_directory();
  }
 
  std::string executable_get_filename() {
    return filesystem::executable_get_filename();
  }
 
  std::string executable_get_pathname() {
    return filesystem::executable_get_pathname();
  }

  bool symlink_create(std::string fname, std::string newname) {
    return filesystem::symlink_create(fname, newname);
  }

  bool symlink_copy(std::string fname, std::string newname) {
    return filesystem::symlink_copy(fname, newname);
  }

  bool symlink_exists(std::string fname) {
    return filesystem::symlink_exists(fname);
  }

  bool hardlink_create(std::string fname, std::string newname) {
    return filesystem::hardlink_create(fname, newname);
  }

  std::uintmax_t file_numblinks(std::string fname) {
    return filesystem::file_numblinks(fname);
  }

  std::uintmax_t file_bin_numblinks(int fd) {
    return filesystem::file_bin_numblinks(fd);
  }
 
  std::string file_bin_hardlinks(int fd, std::string dnames, bool recursive) {
    return filesystem::file_bin_hardlinks(fd, dnames, recursive);
  }
 
  std::string filename_absolute(std::string fname) {
    return filesystem::filename_absolute(fname);
  }
 
  std::string filename_canonical(std::string fname) {
    return filesystem::filename_canonical(fname);
  }

  bool filename_equivalent(std::string fname1, std::string fname2) {
    return filesystem::filename_equivalent(fname1, fname2);
  }
 
  bool file_exists(std::string fname) {
    return filesystem::file_exists(fname);
  }
 
  bool file_delete(std::string fname) {
    return filesystem::file_delete(fname);
  }
 
  bool file_rename(std::string oldname, std::string newname) {
    return filesystem::file_rename(oldname, newname);
  }
 
  bool file_copy(std::string fname, std::string newname) {
    return filesystem::file_copy(fname, newname);
  }
 
  std::uintmax_t file_size(std::string fname) {
    return filesystem::file_size(fname);
  }
 
  bool directory_exists(std::string dname) {
    return filesystem::directory_exists(dname);
  }
 
  bool directory_create(std::string dname) {
    return filesystem::directory_create(dname);
  }
 
  bool directory_destroy(std::string dname) {
    return filesystem::directory_destroy(dname);
  }
 
  bool directory_rename(std::string oldname, std::string newname) {
    return filesystem::directory_rename(oldname, newname);
  }
 
  bool directory_copy(std::string dname, std::string newname) {
    return filesystem::directory_copy(dname, newname);
  }
 
  std::uintmax_t directory_size(std::string dname) {
    return filesystem::directory_size(dname);
  }
 
  unsigned directory_contents_get_order() {
    return filesystem::directory_contents_get_order();
  }

  void directory_contents_set_order(unsigned order) {
    filesystem::directory_contents_set_order(order);
  }

  unsigned directory_contents_get_cntfiles() {
    return filesystem::directory_contents_get_cntfiles();
  }

  unsigned directory_contents_get_maxfiles() {
    return filesystem::directory_contents_get_maxfiles();
  }

  void directory_contents_set_maxfiles(unsigned maxfiles) {
    filesystem::directory_contents_set_maxfiles(maxfiles);
  }

  std::string directory_contents_first(std::string dname, std::string pattern, bool includedirs, bool recursive) {
    return filesystem::directory_contents_first(dname, pattern, includedirs, recursive);
  }

  void directory_contents_first_async(std::string dname, std::string pattern, bool includedirs, bool recursive) {
    filesystem::directory_contents_first_async(dname, pattern, includedirs, recursive);
  }
   
  unsigned directory_contents_get_length() {
    return filesystem::directory_contents_get_length();
  }
 
  bool directory_contents_get_completion_status() {
    return filesystem::directory_contents_get_completion_status();
  }
 
  void directory_contents_set_completion_status(bool complete) {
    filesystem::directory_contents_set_completion_status(complete);
  }
 
  std::string directory_contents_next() {
    return filesystem::directory_contents_next();
  }
 
  void directory_contents_close() {
    return filesystem::directory_contents_close();
  }
 
  std::string environment_get_variable(std::string name) {
    return filesystem::environment_get_variable(name);
  }

  bool environment_get_variable_exists(std::string name) {
    return filesystem::environment_get_variable_exists(name);
  }
 
  bool environment_set_variable(std::string name, std::string value) {
    return filesystem::environment_set_variable(name, value);
  }
 
  bool environment_unset_variable(std::string name) {
    return filesystem::environment_unset_variable(name);
  }
 
  std::string environment_expand_variables(std::string str) {
    return filesystem::environment_expand_variables(str);
  }
 
  int file_datetime_accessed_year(std::string fname) {
    return filesystem::file_datetime_accessed_year(fname);
  }
 
  int file_datetime_accessed_month(std::string fname) {
    return filesystem::file_datetime_accessed_month(fname);
  }
 
  int file_datetime_accessed_day(std::string fname) {
    return filesystem::file_datetime_accessed_day(fname);
  }
 
  int file_datetime_accessed_hour(std::string fname) {
    return filesystem::file_datetime_accessed_hour(fname);
  }
 
  int file_datetime_accessed_minute(std::string fname) {
    return filesystem::file_datetime_accessed_minute(fname);
  }
 
  int file_datetime_accessed_second(std::string fname) {
    return filesystem::file_datetime_accessed_second(fname);
  }
 
  int file_datetime_modified_year(std::string fname) {
    return filesystem::file_datetime_modified_year(fname);
  }
 
  int file_datetime_modified_month(std::string fname) {
    return filesystem::file_datetime_modified_month(fname);
  }
 
  int file_datetime_modified_day(std::string fname) {
    return filesystem::file_datetime_modified_day(fname);
  }
 
  int file_datetime_modified_hour(std::string fname) {
    return filesystem::file_datetime_modified_hour(fname);
  }
 
  int file_datetime_modified_minute(std::string fname) {
    return filesystem::file_datetime_modified_minute(fname);
  }
 
  int file_datetime_modified_second(std::string fname) {
    return filesystem::file_datetime_modified_second(fname);
  }
 
  int file_datetime_created_year(std::string fname) {
    return filesystem::file_datetime_created_year(fname);
  }
 
  int file_datetime_created_month(std::string fname) {
    return filesystem::file_datetime_created_month(fname);
  }
 
  int file_datetime_created_day(std::string fname) {
    return filesystem::file_datetime_created_day(fname);
  }
 
  int file_datetime_created_hour(std::string fname) {
    return filesystem::file_datetime_created_hour(fname);
  }
 
  int file_datetime_created_minute(std::string fname) {
    return filesystem::file_datetime_created_minute(fname);
  }
 
  int file_datetime_created_second(std::string fname) {
    return filesystem::file_datetime_created_second(fname);
  }
 
  int file_bin_datetime_accessed_year(int fd) {
    return filesystem::file_bin_datetime_accessed_year(fd);
  }
 
  int file_bin_datetime_accessed_month(int fd) {
    return filesystem::file_bin_datetime_accessed_month(fd);
  }
 
  int file_bin_datetime_accessed_day(int fd) {
    return filesystem::file_bin_datetime_accessed_day(fd);
  }
 
  int file_bin_datetime_accessed_hour(int fd) {
    return filesystem::file_bin_datetime_accessed_hour(fd);
  }
 
  int file_bin_datetime_accessed_minute(int fd) {
    return filesystem::file_bin_datetime_accessed_minute(fd);
  }
 
  int file_bin_datetime_accessed_second(int fd) {
    return filesystem::file_bin_datetime_accessed_second(fd);
  }
 
  int file_bin_datetime_modified_year(int fd) {
    return filesystem::file_bin_datetime_modified_year(fd);
  }
 
  int file_bin_datetime_modified_month(int fd) {
    return filesystem::file_bin_datetime_modified_month(fd);
  }
 
  int file_bin_datetime_modified_day(int fd) {
    return filesystem::file_bin_datetime_modified_day(fd);
  }
 
  int file_bin_datetime_modified_hour(int fd) {
    return filesystem::file_bin_datetime_modified_hour(fd);
  }
 
  int file_bin_datetime_modified_minute(int fd) {
    return filesystem::file_bin_datetime_modified_minute(fd);
  }
 
  int file_bin_datetime_modified_second(int fd) {
    return filesystem::file_bin_datetime_modified_second(fd);
  }
 
  int file_bin_datetime_created_year(int fd) {
    return filesystem::file_bin_datetime_created_year(fd);
  }
 
  int file_bin_datetime_created_month(int fd) {
    return filesystem::file_bin_datetime_created_month(fd);
  }
 
  int file_bin_datetime_created_day(int fd) {
    return filesystem::file_bin_datetime_created_day(fd);
  }
 
  int file_bin_datetime_created_hour(int fd) {
    return filesystem::file_bin_datetime_created_hour(fd);
  }
 
  int file_bin_datetime_created_minute(int fd) {
    return filesystem::file_bin_datetime_created_minute(fd);
  }
 
  int file_bin_datetime_created_second(int fd) {
    return filesystem::file_bin_datetime_created_second(fd);
  }
 
  int file_bin_open(std::string fname, int mode) {
    return filesystem::file_bin_open(fname, mode);
  }
 
  int file_bin_rewrite(int fd) {
    return filesystem::file_bin_rewrite(fd);
  }
 
  int file_bin_close(int fd) {
    return filesystem::file_bin_close(fd);
  }
 
  long file_bin_size(int fd) {
    return filesystem::file_bin_size(fd);
  }
 
  long file_bin_position(int fd) {
    return filesystem::file_bin_position(fd);
  }
 
  long file_bin_seek(int fd, long pos) {
    return filesystem::file_bin_seek(fd, pos);
  }
 
  int file_bin_read_byte(int fd) {
    return filesystem::file_bin_read_byte(fd);
  }
 
  int file_bin_write_byte(int fd, int byte) {
    return filesystem::file_bin_write_byte(fd, byte);
  }
 
  int file_text_open_read(std::string fname) {
    return filesystem::file_text_open_read(fname);
  }
 
  int file_text_open_write(std::string fname) {
    return filesystem::file_text_open_write(fname);
  }
 
  int file_text_open_append(std::string fname) {
    return filesystem::file_text_open_append(fname);
  }
 
  long file_text_write_real(int fd, double val) {
    return filesystem::file_text_write_real(fd, val);
  }
 
  long file_text_write_string(int fd, std::string str) {
    return filesystem::file_text_write_string(fd, str);
  }
 
  int file_text_writeln(int fd) {
    return filesystem::file_text_writeln(fd);
  }
 
  bool file_text_eoln(int fd) {
    return filesystem::file_text_eoln(fd);
  }
 
  bool file_text_eof(int fd) {
    return filesystem::file_text_eof(fd);
  }

  double file_text_read_real(int fd) {
    return filesystem::file_text_read_real(fd);
  }

  std::string file_text_read_string(int fd) {
    return filesystem::file_text_read_string(fd);
  }

  std::string file_text_readln(int fd) {
    return filesystem::file_text_readln(fd);
  }
 
  std::string file_text_read_all(int fd) {
    return filesystem::file_text_read_all(fd);
  }
 
  int file_text_open_from_string(std::string str) {
    return filesystem::file_text_open_from_string(str);
  }
 
  int file_text_close(int fd) {
    return filesystem::file_text_close(fd);
  }

} // namespace enigma_user
