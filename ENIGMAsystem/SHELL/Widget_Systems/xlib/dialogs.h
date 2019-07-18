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

#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/xlib/XLIBwindow.h"
#include <string>
using std::string;

namespace zenity {

  void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption);
  int show_message(const string &message);
  int show_message_cancelable(string message);
  int show_message_ext(string message, string but1, string but2, string but3);
  bool show_question(string message);
  int show_question_cancelable(string message);
  int show_attempt(string errortext);
  void show_debug_message(string errortext, MESSAGE_TYPE type);
  string get_string(string message, string def);
  string get_password(string message, string def);
  double get_integer(string message, double def);
  double get_passcode(string message, double def);
  string get_open_filename(string filter, string fname);
  string get_open_filenames(string filter, string fname);
  string get_save_filename(string filter, string fname);
  string get_open_filename_ext(string filter, string fname, string dir, string title);
  string get_open_filenames_ext(string filter, string fname, string dir, string title);
  string get_save_filename_ext(string filter, string fname, string dir, string title);
  string get_directory(string dname);
  string get_directory_alt(string capt, string root);
  int get_color(int defcol);
  int get_color_ext(int defcol, string title);
  string message_get_caption();
  void message_set_caption(string title);

}

namespace kdialog {

  void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption);
  int show_message(const string &message);
  int show_message_cancelable(string message);
  int show_message_ext(string message, string but1, string but2, string but3);
  bool show_question(string message);
  int show_question_cancelable(string message);
  int show_attempt(string errortext);
  void show_debug_message(string errortext, MESSAGE_TYPE type);
  string get_string(string message, string def);
  string get_password(string message, string def);
  double get_integer(string message, double def);
  double get_passcode(string message, double def);
  string get_open_filename(string filter, string fname);
  string get_open_filenames(string filter, string fname);
  string get_save_filename(string filter, string fname);
  string get_open_filename_ext(string filter, string fname, string dir, string title);
  string get_open_filenames_ext(string filter, string fname, string dir, string title);
  string get_save_filename_ext(string filter, string fname, string dir, string title);
  string get_directory(string dname);
  string get_directory_alt(string capt, string root);
  int get_color(int defcol);
  int get_color_ext(int defcol, string title);
  string message_get_caption();
  void message_set_caption(string title);

}
