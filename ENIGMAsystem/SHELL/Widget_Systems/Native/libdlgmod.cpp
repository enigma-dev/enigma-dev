/** Copyright (C) 2024 Samuel Venable
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

#include "Platforms/General/PFexternals.h"

#if defined(_WIN32)
#define LIBDLGMOD "./libdlgmod.dll"
#elif (defined(__APPLE__) && defined(__MACH__))
#define LIBDLGMOD "./libdlgmod.dylib"
#else
#define LIBDLGMOD "./libdlgmod.so"
#endif

using enigma_user::external_call;
using enigma_user::external_define;
using enigma_user::ty_real;
using enigma_user::ty_string;

namespace dialog_module {

  int show_message(std::string str) {
    return (int)external_call(external_define(LIBDLGMOD, "show_message", 0, ty_real, 1, ty_string), str);
  }

  int show_message_cancelable(std::string str) {
    return (int)external_call(external_define(LIBDLGMOD, "show_message_cancelable", 0, ty_real, 1, ty_string), str);
  }

  int show_question(std::string str) {
    return (int)external_call(external_define(LIBDLGMOD, "show_question", 0, ty_real, 1, ty_string), str);
  }

  int show_question_cancelable(std::string str) {
    return (int)external_call(external_define(LIBDLGMOD, "show_question_cancelable", 0, ty_real, 1, ty_string), str);
  }

  int show_attempt(std::string str) {
    return (int)external_call(external_define(LIBDLGMOD, "show_attempt", 0, ty_real, 1, ty_string), str);
  }

  int show_error(std::string str, bool abort) {
    return (int)external_call(external_define(LIBDLGMOD, "show_error", 0, ty_real, 2, ty_string, ty_real), str, abort);
  }

  std::string get_string(std::string str, std::string def) {
    return external_call(external_define(LIBDLGMOD, "get_string", 0, ty_string, 2, ty_string, ty_string), str, def);
  }

  std::string get_password(std::string str, std::string def) {
    return external_call(external_define(LIBDLGMOD, "get_password", 0, ty_string, 2, ty_string, ty_string), str, def);
  }

  double get_integer(std::string str, double def) {
    return (double)external_call(external_define(LIBDLGMOD, "get_integer", 0, ty_real, 2, ty_string, ty_real), str, def);
  }

  double get_passcode(std::string str, double def) {
    return (double)external_call(external_define(LIBDLGMOD, "get_passcode", 0, ty_real, 2, ty_string, ty_real), str, def);
  }

  std::string get_open_filename(std::string filter, std::string fname) {
    return external_call(external_define(LIBDLGMOD, "get_open_filename", 0, ty_string, 2, ty_string, ty_string), filter, fname);
  }

  std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) {
    return external_call(external_define(LIBDLGMOD, "get_open_filename_ext", 0, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, dir, title);
  }

  std::string get_open_filenames(std::string filter, std::string fname) {
    return external_call(external_define(LIBDLGMOD, "get_open_filenames", 0, ty_string, 2, ty_string, ty_string), filter, fname);
  }

  std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title) {
    return external_call(external_define(LIBDLGMOD, "get_open_filenames_ext", 0, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, dir, title);
  }

  std::string get_save_filename(std::string filter, std::string fname) {
    return external_call(external_define(LIBDLGMOD, "get_save_filename", 0, ty_string, 2, ty_string, ty_string), filter, fname);
  }

  std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) {
    return external_call(external_define(LIBDLGMOD, "get_save_filename_ext", 0, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, dir, title);
  }

  std::string get_directory(std::string dname) {
    return external_call(external_define(LIBDLGMOD, "get_directory", 0, ty_string, 1, ty_string), dname);
  }

  std::string get_directory_alt(std::string capt, std::string root) {
    return external_call(external_define(LIBDLGMOD, "get_directory_alt", 0, ty_string, 2, ty_string, ty_string), capt, root);
  }

  int get_color(int defcol) {
    return (int)external_call(external_define(LIBDLGMOD, "get_color", 0, ty_real, 1, ty_real), defcol);
  }

  int get_color_ext(int defcol, std::string title) {
    return (int)external_call(external_define(LIBDLGMOD, "get_color_ext", 0, ty_real, 2, ty_real, ty_string), defcol, title);
  }

  std::string widget_get_caption() {
    return external_call(external_define(LIBDLGMOD, "widget_get_caption", 0, ty_string, 0));
  }

  void widget_set_caption(std::string str) {
    external_call(external_define(LIBDLGMOD, "widget_set_caption", 0, ty_real, 1, ty_string), str);
  }

  std::string widget_get_owner() {
    return external_call(external_define(LIBDLGMOD, "widget_get_owner", 0, ty_string, 0));
  }

  void widget_set_owner(std::string hwnd) {
    external_call(external_define(LIBDLGMOD, "widget_set_owner", 0, ty_real, 1, ty_string), hwnd);
  }

  std::string widget_get_icon() {
    return external_call(external_define(LIBDLGMOD, "widget_get_icon", 0, ty_string, 0));
  }

  void widget_set_icon(std::string icon) {
    external_call(external_define(LIBDLGMOD, "widget_set_icon", 0, ty_real, 1, ty_string), icon);
  }

  std::string widget_get_system() {
    return external_call(external_define(LIBDLGMOD, "widget_get_system", 0, ty_string, 0));
  }

  void widget_set_system(std::string sys) {
    external_call(external_define(LIBDLGMOD, "widget_set_system", 0, ty_real, 1, ty_string), sys);
  }

  std::string widget_get_button_name(int type) {
    return external_call(external_define(LIBDLGMOD, "widget_get_button_name", 0, ty_string, 1, ty_real), type);
  }
  
  void widget_set_button_name(int type, std::string name) {
    external_call(external_define(LIBDLGMOD, "widget_set_button_name", 0, ty_real, 2, ty_real, ty_string), type, name);
  }

  bool widget_get_canceled() {
    return external_call(external_define(LIBDLGMOD, "widget_get_canceled", 0, ty_real, 0));
  }
  
} // namespace dialog_module


