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

#include "apifilesystem/filesystem.h"

namespace enigma_user {

  using ngs::fs::directory_get_current_working;
  using ngs::fs::directory_set_current_working;
  using ngs::fs::directory_get_temporary_path;
  using ngs::fs::executable_get_directory;
  using ngs::fs::executable_get_filename;
  using ngs::fs::executable_get_pathname;
  using ngs::fs::symlink_create;
  using ngs::fs::symlink_copy;
  using ngs::fs::symlink_exists;
  using ngs::fs::hardlink_create;
  using ngs::fs::file_numblinks;
  using ngs::fs::file_bin_numblinks;
  using ngs::fs::file_bin_hardlinks;
  using ngs::fs::filename_absolute;
  using ngs::fs::filename_canonical;
  using ngs::fs::filename_equivalent;
  using ngs::fs::file_exists;
  using ngs::fs::file_delete;
  using ngs::fs::file_rename;
  using ngs::fs::file_copy;
  using ngs::fs::file_size;
  using ngs::fs::directory_exists;
  using ngs::fs::directory_create;
  using ngs::fs::directory_destroy;
  using ngs::fs::directory_rename;
  using ngs::fs::directory_copy;
  using ngs::fs::directory_size;
  using ngs::fs::directory_contents_get_order;
  using ngs::fs::directory_contents_set_order;
  using ngs::fs::directory_contents_get_cntfiles;
  using ngs::fs::directory_contents_get_maxfiles;
  using ngs::fs::directory_contents_set_maxfiles;
  using ngs::fs::directory_contents_first;
  using ngs::fs::directory_contents_next;
  using ngs::fs::directory_contents_close;
  using ngs::fs::environment_get_variable;
  using ngs::fs::environment_get_variable_exists;
  using ngs::fs::environment_set_variable;
  using ngs::fs::environment_unset_variable;
  using ngs::fs::environment_expand_variables;
  using ngs::fs::file_datetime_accessed_year;
  using ngs::fs::file_datetime_accessed_month;
  using ngs::fs::file_datetime_accessed_day;
  using ngs::fs::file_datetime_accessed_hour;
  using ngs::fs::file_datetime_accessed_minute;
  using ngs::fs::file_datetime_accessed_second;
  using ngs::fs::file_datetime_modified_year;
  using ngs::fs::file_datetime_modified_month;
  using ngs::fs::file_datetime_modified_day;
  using ngs::fs::file_datetime_modified_hour;
  using ngs::fs::file_datetime_modified_minute;
  using ngs::fs::file_datetime_modified_second;
  using ngs::fs::file_datetime_created_year;
  using ngs::fs::file_datetime_created_month;
  using ngs::fs::file_datetime_created_day;
  using ngs::fs::file_datetime_created_hour;
  using ngs::fs::file_datetime_created_minute;
  using ngs::fs::file_datetime_created_second;
  using ngs::fs::file_bin_datetime_accessed_year;
  using ngs::fs::file_bin_datetime_accessed_month;
  using ngs::fs::file_bin_datetime_accessed_day;
  using ngs::fs::file_bin_datetime_accessed_hour;
  using ngs::fs::file_bin_datetime_accessed_minute;
  using ngs::fs::file_bin_datetime_accessed_second;
  using ngs::fs::file_bin_datetime_modified_year;
  using ngs::fs::file_bin_datetime_modified_month;
  using ngs::fs::file_bin_datetime_modified_day;
  using ngs::fs::file_bin_datetime_modified_hour;
  using ngs::fs::file_bin_datetime_modified_minute;
  using ngs::fs::file_bin_datetime_modified_second;
  using ngs::fs::file_bin_datetime_created_year;
  using ngs::fs::file_bin_datetime_created_month;
  using ngs::fs::file_bin_datetime_created_day;
  using ngs::fs::file_bin_datetime_created_hour;
  using ngs::fs::file_bin_datetime_created_minute;
  using ngs::fs::file_bin_datetime_created_second;
  using ngs::fs::file_bin_open;
  using ngs::fs::file_bin_rewrite;
  using ngs::fs::file_bin_close;
  using ngs::fs::file_bin_size;
  using ngs::fs::file_bin_position;
  using ngs::fs::file_bin_seek;
  using ngs::fs::file_bin_read_byte;
  using ngs::fs::file_bin_write_byte;
  using ngs::fs::file_text_open_read;
  using ngs::fs::file_text_open_write;
  using ngs::fs::file_text_open_append;
  using ngs::fs::file_text_write_real;
  using ngs::fs::file_text_write_string;
  using ngs::fs::file_text_writeln;
  using ngs::fs::file_text_eoln;
  using ngs::fs::file_text_eof;
  using ngs::fs::file_text_read_real;
  using ngs::fs::file_text_read_string;
  using ngs::fs::file_text_readln;
  using ngs::fs::file_text_read_all;
  using ngs::fs::file_text_open_from_string;
  using ngs::fs::file_text_close;

} // namespace enigma_fs
