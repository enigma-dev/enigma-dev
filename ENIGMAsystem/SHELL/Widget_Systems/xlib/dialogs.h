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

namespace enigma {

class CommandLineWidgetEngine {
 public:
  virtual ~CommandLineWidgetEngine() = default;
  virtual void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) = 0;
  virtual int show_message(const string &message) = 0;
  virtual int show_message_cancelable(string message) = 0;
  virtual bool show_question(string message) = 0;
  virtual int show_question_cancelable(string message) = 0;
  virtual int show_attempt(string errortext) = 0;
  virtual void show_debug_message(string errortext, MESSAGE_TYPE type) = 0;
  virtual string get_string(string message, string def) = 0;
  virtual string get_password(string message, string def) = 0;
  virtual double get_integer(string message, double def) = 0;
  virtual double get_passcode(string message, double def) = 0;
  virtual string get_open_filename(string filter, string fname) = 0;
  virtual string get_open_filenames(string filter, string fname) = 0;
  virtual string get_save_filename(string filter, string fname) = 0;
  virtual string get_open_filename_ext(string filter, string fname, string dir, string title) = 0;
  virtual string get_open_filenames_ext(string filter, string fname, string dir, string title) = 0;
  virtual string get_save_filename_ext(string filter, string fname, string dir, string title) = 0;
  virtual string get_directory(string dname) = 0;
  virtual string get_directory_alt(string capt, string root) = 0;
  virtual int get_color(int defcol) = 0;
  virtual int get_color_ext(int defcol, string title) = 0;
  virtual string message_get_caption() = 0;
  virtual void message_set_caption(string title) = 0;

}; // class CommandLineWidgetEngine

extern CommandLineWidgetEngine *zenity_widgets;
extern CommandLineWidgetEngine *kdialog_widgets;

} // namespace enigma
