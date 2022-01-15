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

namespace filesystem = ngs::fs;

namespace enigma_fs {

  std::string fs_directory_get_current_working() {
    return filesystem::directory_get_current_working();
  }
 
  bool fs_directory_set_current_working(std::string dname) {
    return filesystem::directory_set_current_working(dname);
  }
 
  std::string fs_directory_get_temporary_path() {
    return filesystem::directory_get_temporary_path();
  }
 
  std::string fs_executable_get_directory() {
    return filesystem::executable_get_directory();
  }
 
  std::string fs_executable_get_filename() {
    return filesystem::executable_get_filename();
  }
 
  std::string fs_executable_get_pathname() {
    return filesystem::executable_get_pathname();
  }
 
  std::string fs_file_bin_pathname(int fd) {
    return filesystem::file_bin_pathname(fd);
  }
 
  std::string fs_filename_absolute(std::string fname) {
    return filesystem::filename_absolute(fname);
  }
 
  std::string fs_filename_canonical(std::string fname) {
    return filesystem::filename_canonical(fname);
  }
 
  bool fs_file_exists(std::string fname) {
    return filesystem::file_exists(fname);
  }
 
  bool fs_file_delete(std::string fname) {
    return filesystem::file_delete(fname);
  }
 
  bool fs_file_rename(std::string oldname, std::string newname) {
    return filesystem::file_rename(oldname, newname);
  }
 
  bool fs_file_copy(std::string fname, std::string newname) {
    return filesystem::file_copy(fname, newname);
  }
 
  std::uintmax_t fs_file_size(std::string fname) {
    return filesystem::file_size(fname);
  }
 
  bool fs_directory_exists(std::string dname) {
    return filesystem::directory_exists(dname);
  }
 
  bool fs_directory_create(std::string dname) {
    return filesystem::directory_create(dname);
  }
 
  bool fs_directory_destroy(std::string dname) {
    return filesystem::directory_destroy(dname);
  }
 
  bool fs_directory_rename(std::string oldname, std::string newname) {
    return filesystem::directory_rename(oldname, newname);
  }
 
  bool fs_directory_copy(std::string dname, std::string newname) {
    return filesystem::directory_copy(dname, newname);
  }
 
  std::uintmax_t fs_directory_size(std::string dname) {
    return filesystem::directory_size(dname);
  }
 
  var fs_directory_contents(std::string dname, std::string pattern = "*.*", bool includedirs = true) {
    var resVar; std::vector<std::string> resVec = filesystem::directory_contents(dname, pattern, includedirs);
    for (unsigned i = 0; i < resVec.size(); i++) resVar[i] = resVec[i];
    return resVar;
  }

  var fs_directory_contents_recursive(std::string dname, std::string pattern = "*.*", bool includedirs = true) {
    var resVar; std::vector<std::string> resVec = filesystem::directory_contents_recursive(dname, pattern, includedirs);
    for (unsigned i = 0; i < resVec.size(); i++) resVar[i] = resVec[i];
    return resVar;
  }
 
  std::string fs_environment_get_variable(std::string name) {
    return filesystem::environment_get_variable(name);
  }
 
  bool fs_environment_set_variable(std::string name, std::string value) {
    return filesystem::environment_set_variable(name, value);
  }
 
  bool fs_environment_unset_variable(std::string name) {
    return filesystem::environment_unset_variable(name);
  }
 
  std::string environment_expand_variables(std::string str) {
    return filesystem::environment_expand_variables(str);
  }
 
  int fs_file_datetime_accessed_year(std::string fname) {
    return filesystem::file_datetime_accessed_year(fname);
  }
 
  int fs_file_datetime_accessed_month(std::string fname) {
    return filesystem::file_datetime_accessed_month(fname);
  }
 
  int fs_file_datetime_accessed_day(std::string fname) {
    return filesystem::file_datetime_accessed_day(fname);
  }
 
  int fs_file_datetime_accessed_hour(std::string fname) {
    return filesystem::file_datetime_accessed_hour(fname);
  }
 
  int fs_file_datetime_accessed_minute(std::string fname) {
    return filesystem::file_datetime_accessed_minute(fname);
  }
 
  int fs_file_datetime_accessed_second(std::string fname) {
    return filesystem::file_datetime_accessed_second(fname);
  }
 
  int fs_file_datetime_modified_year(std::string fname) {

  }
 
  int fs_file_datetime_modified_month(std::string fname) {

  }
 
  int fs_file_datetime_modified_day(std::string fname) {

  }
 
  int fs_file_datetime_modified_hour(std::string fname) {

  }
 
  int fs_file_datetime_modified_minute(std::string fname) {

  }
 
  int fs_file_datetime_modified_second(std::string fname) {

  }
 
  int fs_file_datetime_created_year(std::string fname) {

  }
 
  int fs_file_datetime_created_month(std::string fname) {

  }
 
  int fs_file_datetime_created_day(std::string fname) {

  }
 
  int fs_file_datetime_created_hour(std::string fname) {

  }
 
  int fs_file_datetime_created_minute(std::string fname) {

  }
 
  int fs_file_datetime_created_second(std::string fname) {

  }
 
  int fs_file_bin_datetime_accessed_year(int fd) {

  }
 
  int fs_file_bin_datetime_accessed_month(int fd) {

  }
 
  int fs_file_bin_datetime_accessed_day(int fd) {

  }
 
  int fs_file_bin_datetime_accessed_hour(int fd) {

  }
 
  int fs_file_bin_datetime_accessed_minute(int fd) {

  }
 
  int fs_file_bin_datetime_accessed_second(int fd) {

  }
 
  int fs_file_bin_datetime_modified_year(int fd) {

  }
 
  int fs_file_bin_datetime_modified_month(int fd) {

  }
 
  int fs_file_bin_datetime_modified_day(int fd) {

  }
 
  int fs_file_bin_datetime_modified_hour(int fd) {

  }
 
  int fs_file_bin_datetime_modified_minute(int fd) {

  }
 
  int fs_file_bin_datetime_modified_second(int fd) {

  }
 
  int fs_file_bin_datetime_created_year(int fd) {

  }
 
  int fs_file_bin_datetime_created_month(int fd) {

  }
 
  int fs_file_bin_datetime_created_day(int fd) {

  }
 
  int fs_file_bin_datetime_created_hour(int fd) {

  }
 
  int fs_file_bin_datetime_created_minute(int fd) {

  }
 
  int fs_file_bin_datetime_created_second(int fd) {

  }
 
  int fs_file_bin_open(std::string fname, int mode) {

  }
 
  int fs_file_bin_rewrite(int fd) {

  }
 
  int fs_file_bin_close(int fd) {

  }
 
  long fs_file_bin_size(int fd) {

  }
 
  long fs_file_bin_position(int fd) {

  }
 
  long fs_file_bin_seek(int fd, long pos) {

  }
 
  int fs_file_bin_read_byte(int fd) {

  }
 
  int fs_file_bin_write_byte(int fd, int byte) {

  }
 
  int fs_file_text_open_read(std::string fname) {

  }
 
  int fs_file_text_open_write(std::string fname) {

  }
 
  int fs_file_text_open_append(std::string fname) {

  }
 
  long fs_file_text_write_real(int fd, double val) {

  }
 
  long fs_file_text_write_string(int fd, std::string str) {

  }
 
  int fs_file_text_writeln(int fd) {

  }
 
  bool fs_file_text_eoln(int fd) {

  }
 
  bool fs_file_text_eof(int fd) {

  }

  double fs_file_text_read_real(int fd) {

  }

  std::string fs_file_text_read_string(int fd) {

  }

  std::string fs_file_text_readln(int fd) {

  }
 
  std::string fs_file_text_read_all(int fd) {

  }
 
  int fs_file_text_open_from_string(std::string str) {

  }
 
  int fs_file_text_close(int fd) {

  }

} // namespace enigma_fs
