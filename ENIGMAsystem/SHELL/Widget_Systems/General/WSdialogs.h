/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
*** Copyright (C) 2019 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <string>
using std::string;

namespace enigma_user {

bool show_question(string message);
int show_question_ext(string message);
string get_open_filename(string filter, string fname);
string get_open_filename_ext(string filter, string fname, string title, string dir);
string get_open_filenames(string filter, string fname);
string get_open_filenames_ext(string filter, string fname, string title, string dir);
string get_save_filename(string filter, string fname);
string get_save_filename_ext(string filter, string fname, string title, string dir);
string get_directory(string dname);
string get_directory_alt(string capt, string root);
inline bool action_if_question(string message) {
  return show_question(message);
}

}
