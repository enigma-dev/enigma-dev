/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
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

#include "Widget_Systems/widgets_mandatory.h"

#include "Platforms/xlib/XLIBwindow.h"

#include "Universal_System/fileio.h"

using enigma_user::filename_absolute;

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
  virtual std::string widget_get_caption() = 0;
  virtual void widget_set_caption(std::string title) = 0;

}; // class CommandLineWidgetEngine

extern CommandLineWidgetEngine *zenity_widgets;
extern CommandLineWidgetEngine *kdialog_widgets;

} // namespace enigma
