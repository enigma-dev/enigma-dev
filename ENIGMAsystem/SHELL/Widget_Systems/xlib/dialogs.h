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

#include "Widget_Systems/widgets_mandatory.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Platforms/xlib/XLIBwindow.h"

inline static int color_get_red  (int c) { return COL_GET_R(c); }
inline static int color_get_green(int c) { return COL_GET_G(c); }
inline static int color_get_blue (int c) { return COL_GET_B(c); }
inline static int make_color_rgb(unsigned char r, unsigned char g, unsigned char b) {
  return r | (g << 8) | (b << 16);
}

namespace enigma {

std::string create_shell_dialog(std::string command);

class CommandLineWidgetEngine {
 public:
  virtual ~CommandLineWidgetEngine() = default;
  virtual void show_info(std::string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, std::string caption) = 0;
  virtual int show_message(const std::string &message) = 0;
  virtual int show_message_cancelable(std::string message) = 0;
  virtual bool show_question(std::string message) = 0;
  virtual int show_question_cancelable(std::string message) = 0;
  virtual int show_attempt(std::string errortext) = 0;
  virtual void show_debug_message(std::string errortext, MESSAGE_TYPE type) = 0;
  virtual std::string get_string(std::string message, std::string def) = 0;
  virtual std::string get_password(std::string message, std::string def) = 0;
  virtual double get_integer(std::string message, double def) = 0;
  virtual double get_passcode(std::string message, double def) = 0;
  virtual std::string get_open_filename(std::string filter, std::string fname) = 0;
  virtual std::string get_open_filenames(std::string filter, std::string fname) = 0;
  virtual std::string get_save_filename(std::string filter, std::string fname) = 0;
  virtual std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) = 0;
  virtual std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title) = 0;
  virtual std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) = 0;
  virtual std::string get_directory(std::string dname) = 0;
  virtual std::string get_directory_alt(std::string capt, std::string root) = 0;
  virtual int get_color(int defcol) = 0;
  virtual int get_color_ext(int defcol, std::string title) = 0;
  virtual std::string message_get_caption() = 0;
  virtual void message_set_caption(std::string title) = 0;

}; // class CommandLineWidgetEngine

extern CommandLineWidgetEngine *zenity_widgets;
extern CommandLineWidgetEngine *kdialog_widgets;

} // namespace enigma
