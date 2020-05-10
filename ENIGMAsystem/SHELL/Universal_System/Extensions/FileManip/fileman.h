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

#include <string>
#include <vector>
#include <cstddef>

namespace strings {

  std::string string_replace_all(std::string str, std::string substr, std::string nstr);

  std::vector<std::string> split_string(const std::string &str, char delimiter);

  std::string filename_path(std::string fname);

  std::string filename_name(std::string fname);

  std::string filename_ext(std::string fname);

  std::string filename_normalize(std::string fname);

  std::string filename_remove_slash(std::string dname, bool normalize = false);

  std::string filename_add_slash(std::string dname, bool normalize = false);

} // namespace slashes

namespace fileman {

  std::string get_working_directory_ns();

  bool set_working_directory_ns(std::string dname);

  std::string get_temp_directory_ns();

  std::string get_program_directory_ns();

  std::string get_program_filename_ns();

  std::string get_program_pathname_ns(bool print = true);

  std::string filename_absolute_ns(std::string fname);

  bool file_exists_ns(std::string fname);

  bool file_delete_ns(std::string fname);

  bool file_rename_ns(std::string oldname, std::string newname);

  bool file_copy_ns(std::string fname, std::string newname);

  bool directory_exists_ns(std::string dname);

  bool directory_create_ns(std::string dname);

  bool directory_destroy_ns(std::string dname);

  bool directory_rename_ns(std::string oldname, std::string newname);

  bool directory_copy_ns(std::string dname, std::string newname);

  std::string directory_contents_ns(std::string dname, std::string pattern = "*.*", bool includedirs = true);

  std::string environment_get_variable_ns(std::string name);

  bool environment_set_variable_ns(std::string name, std::string value);

} // namespace fileman

namespace enigma_user {

#ifdef OVERRIDE_SANDBOX
using ::fileman::get_working_directory_ns;
using ::fileman::set_working_directory_ns;
using ::fileman::get_temp_directory_ns;
using ::fileman::get_program_directory_ns;
using ::fileman::get_program_filename_ns;
using ::fileman::get_program_pathname_ns;
using ::fileman::filename_absolute_ns;
using ::fileman::file_exists_ns;
using ::fileman::file_delete_ns;
using ::fileman::file_rename_ns;
using ::fileman::file_copy_ns;
using ::fileman::directory_exists_ns;
using ::fileman::directory_create_ns;
using ::fileman::directory_destroy_ns;
using ::fileman::directory_rename_ns;
using ::fileman::directory_copy_ns;
using ::fileman::directory_contents_ns;
using ::fileman::directory_contents_ns;
using ::fileman::environment_get_variable_ns;
using ::fileman::environment_set_variable_ns;
#define working_directory get_working_directory_ns()
#define temp_directory get_temp_directory_ns()
#define program_directory get_program_directory_ns()
#define program_filename get_program_filename_ns()
#define program_pathname get_program_pathname_ns()
#define set_working_directory(x) set_working_directory_ns(x)
#define filename_absolute(x) filename_absolute_ns(x)
#define file_exists(x) file_exists_ns(x)
#define file_delete(x) file_delete_ns(x)
#define file_rename(x, y) file_rename_ns(x, y)
#define file_copy(x, y) file_copy_ns(x, y)
#define directory_exists(x) directory_exists_ns(x)
#define directory_create(x) directory_create_ns(x)
#define directory_destroy(x) directory_destroy_ns(x)
#define directory_rename(x, y) directory_rename_ns(x, y)
#define directory_copy(x, y) directory_copy_ns(x, y)
#define directory_contents(x) directory_contents_ns(x, "*.*", true)
#define directory_contents_ext(x, y, z) directory_contents_ns(x, y, z)
#define environment_get_variable(x) environment_get_variable_ns(x)
#define environment_set_variable(x, y) environment_set_variable_ns(x, y)
#endif

}
