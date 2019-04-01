/** Copyright (C) 2019 Samuel Venable
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

//  void show_error(std::string errortext, const bool fatal);

namespace enigma_user {
  
//  int show_message(const std::string &str);
int show_message_cancelable(std::string str);
bool show_question(std::string str);
int show_question_cancelable(std::string str);
int show_attempt(std::string str);
std::string get_string(std::string str, std::string def);
std::string get_password(std::string str, std::string def);
double get_integer(std::string str, double def);
double get_passcode(std::string str, double def);
std::string get_open_filename(std::string filter, std::string fname);
std::string get_open_filenames(std::string filter, std::string fname);
std::string get_save_filename(std::string filter, std::string fname);
std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title);
std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
std::string get_directory(std::string dname);
std::string get_directory_alt(std::string capt, std::string root);
int get_color(int defcol);
int get_color_ext(int defcol, std::string title);
std::string message_get_caption();
void message_set_caption(std::string str);
  
} // namespave enigma_user
