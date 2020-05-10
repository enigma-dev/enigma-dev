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

#include "fileman.h"

using std::string;

namespace enigma_user {

using ::fileman::get_working_directory_ns;

  bool set_working_directory_ns(string dname) {
    return fileman::set_working_directory_ns(dname);
  }

  string get_temp_directory_ns() {
    return fileman::get_temp_directory_ns();
  }

  string get_program_directory_ns() {
    return fileman::get_program_directory_ns();
  }

  string get_program_filename_ns() {
    return fileman::get_program_filename_ns();
  }

  string get_program_pathname_ns() {
    return fileman::get_program_pathname_ns();
  }

  string filename_absolute_ns(string fname) {
    return fileman::filename_absolute_ns(fname);
  }

  bool file_exists_ns(string fname) {
    return fileman::file_exists_ns(fname);
  }

  bool file_delete_ns(string fname) {
    return fileman::file_delete_ns(fname);
  }

  bool file_rename_ns(string oldname, string newname) {
    return fileman::file_rename_ns(oldname, newname);
  }

  bool file_copy_ns(string fname, string newname) {
    return fileman::file_copy_ns(fname, newname);
  }

  bool directory_exists_ns(string dname) {
    return fileman::directory_exists_ns(dname);
  }

  bool directory_create_ns(string dname) {
    return fileman::directory_create_ns(dname);
  }

  bool directory_destroy_ns(string dname) {
    return fileman::directory_destroy_ns(dname);
  }

  bool directory_rename_ns(string oldname, string newname) {
    return fileman::directory_rename_ns(oldname, newname);
  }

  bool directory_copy_ns(string dname, string newname) {
    return fileman::directory_copy_ns(dname, newname);
  }

  string directory_contents_ns(string dname) {
    return fileman::directory_contents_ns(dname);
  }

  string directory_contents_ext_ns(string dname, string pattern, bool includedirs) {
    return fileman::directory_contents_ns(dname, pattern, includedirs);
  }

  string environment_get_variable_ns(string name) {
    return fileman::environment_get_variable_ns(name);
  }

  bool environment_set_variable_ns(string name, string value) {
    return fileman::environment_set_variable_ns(name, value);
  }

} // namespace enigma_user
