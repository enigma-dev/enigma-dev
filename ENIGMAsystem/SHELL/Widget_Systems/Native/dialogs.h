/*

 MIT License

 Copyright © 2021-2022 Samuel Venable

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

#ifndef USE_SDL_POLLEVENT
#define USE_SDL_POLLEVENT
#endif

namespace enigma_user {

  int show_message_cancelable(std::string str);
  bool show_question(std::string str);
  int show_question_cancelable(std::string str);
  int show_attempt(std::string str);
  std::string get_string(std::string str, std::string def);
  std::string get_password(std::string str, std::string def);
  double get_number(std::string str, double def);
  double get_integer(std::string str, double def);
  double get_passcode(std::string str, double def);
  std::string get_open_filename(std::string filter, std::string fname);
  std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
  std::string get_open_filenames(std::string filter, std::string fname);
  std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title);
  std::string get_save_filename(std::string filter, std::string fname);
  std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
  std::string get_directory(std::string dname);
  std::string get_directory_alt(std::string capt, std::string root);
  int get_color(int defcol);
  int get_color_ext(int defcol, std::string title);
  std::string widget_get_caption();
  void widget_set_caption(std::string str);
  std::string widget_get_owner();
  void widget_set_owner(std::string hwnd);
  std::string widget_get_icon();
  void widget_set_icon(std::string icon);
  std::string widget_get_system();
  void widget_set_system(std::string sys);
  void widget_set_button_name(int type, std::string name);
  std::string widget_get_button_name(int type);
  bool widget_get_canceled();
  inline bool action_if_question(std::string message) {
    return show_question(message);
  }

} // namespace enigma_user
