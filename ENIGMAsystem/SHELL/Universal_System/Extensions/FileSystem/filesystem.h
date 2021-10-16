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

#include <sys/types.h>
#include <sys/stat.h>

namespace datetime {

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
  int file_bin_open(std::string fname, int mode);
  int file_bin_rewrite(int fd);
  int file_bin_close(int fd);
  long file_bin_size(int fd);
  long file_bin_position(int fd);
  long file_bin_seek(int fd, long pos);
  int file_bin_read_byte(int fd);
  int file_bin_write_byte(int fd, int byte); 

} // namespace filesystem
