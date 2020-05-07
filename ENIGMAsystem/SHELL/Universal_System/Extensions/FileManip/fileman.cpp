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

  string get_working_directory() {
    return fileman::get_working_directory();
  }

  bool set_working_directory(string dname) {
    return fileman::set_working_directory(dname);
  }

  string get_temp_directory() {
    return fileman::get_temp_directory();
  }

  string get_program_directory() {
    return fileman::get_program_directory();
  }

  string get_program_filename() {
    return fileman::get_program_filename();
  }

  string get_program_pathname() {
    return fileman::get_program_pathname();
  }

  string filename_absolute(string fname) {
    return fileman::filename_absolute(fname);
  }

  bool file_exists(string fname) {
    return fileman::file_exists(fname);
  }

  bool file_delete(string fname) {
    return fileman::file_delete(fname);
  }

  bool file_rename(string oldname, string newname) {
    return fileman::file_rename(oldname, newname);
  }

  bool file_copy(string fname, string newname) {
    return fileman::file_copy(fname, newname);
  }

  std::uintmax_t file_size(string fname) {
    return fileman::file_size(fname);
  }

  bool directory_exists(string dname) {
    return fileman::directory_exists(dname);
  }

  bool directory_create(string dname) {
    return fileman::directory_create(dname);
  }

  bool directory_destroy(string dname) {
    return fileman::directory_destroy(dname);
  }

  bool directory_rename(string oldname, string newname) {
    return fileman::directory_rename(oldname, newname);
  }

  bool directory_copy(string dname, string newname) {
    return fileman::directory_copy(dname, newname);
  }

  std::uintmax_t directory_size(string dname) {
    return fileman::directory_size(dname);
  }

  string directory_contents(string dname) {
    return fileman::directory_contents(dname);
  }

  string directory_contents_ext(string dname, string pattern, bool includedirs) {
    return fileman::directory_contents(dname, pattern, includedirs);
  }

  string environment_get_variable(string name) {
    return fileman::environment_get_variable(name);
  }

  bool environment_set_variable(string name, string value) {
    return fileman::environment_set_variable(name, value);
  }

} // namespace enigma_user
