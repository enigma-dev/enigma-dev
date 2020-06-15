/** Copyright (C) 2020 Samuel Venable
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

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/PFexternals.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "Universal_System/estring.h"
#include <stdlib.h>
#include <cstdio>
#include <string>

#ifdef DEBUG_MODE
#include "Universal_System/Resources/resource_data.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/debugscope.h"
#endif

using std::string;

static string dll = string("DialogModule.") + 
#ifdef _WIN32
"dll";
#elif defined(__APPLE__) && defined(__MACH__)
"dylib";
#else
"so";
#endif

namespace enigma {

bool widget_system_initialize() {
  dll = enigma_user::working_directory + dll;
  string png = enigma_user::working_directory + 
  enigma_user::filename_name(enigma_user::parameter_string(0)) + ".png";
  enigma_user::widget_set_owner(reinterpret_cast<void *>(enigma_user::window_handle()));
  if (enigma_user::file_exists(png)) enigma_user::widget_set_icon(png);
  return enigma_user::file_exists(dll);
}

} // namespave enigma

namespace enigma_user {

  void show_info(string text, int bgcolor, int left, int top, int width, int height,
    bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
    bool pauseGame, string caption) {

  }

  void show_debug_message(string errortext, MESSAGE_TYPE type) {
    const bool fatal = (type == M_FATAL_ERROR || type == M_FATAL_USER_ERROR);
    #ifdef DEBUG_MODE
    errortext += "\n\n" + enigma::debug_scope::GetErrors();
    #endif
    if (type != M_INFO && type != M_WARNING) {
      external_call(external_define(dll, "show_error", dll_cdecl, ty_real, 2, ty_string, ty_real), errortext, fatal);
    } else {
      #ifndef DEBUG_MODE
      errortext += "\n";
      fputs(errortext.c_str(), stderr);
      #endif
      if (fatal) { abort(); }
    }
  }

  int show_message(const string &str) {
    return (int)external_call(external_define(dll, "show_message", dll_cdecl, ty_real, 1, ty_string), str);
  }

  int show_message_cancelable(string str) {
    return (int)external_call(external_define(dll, "show_message_cancelable", dll_cdecl, ty_real, 1, ty_string), str);
  }

  bool show_question(string str) {
    return (bool)external_call(external_define(dll, "show_question", dll_cdecl, ty_real, 1, ty_string), str);
  }

  int show_question_cancelable(string str) {
    return (int)external_call(external_define(dll, "show_question_cancelable", dll_cdecl, ty_real, 1, ty_string), str);
  }

  int show_attempt(string str) {
    return (int)external_call(external_define(dll, "show_attempt", dll_cdecl, ty_real, 1, ty_string), str);
  }

  string get_string(string str, string def) {
    return external_call(external_define(dll, "get_string", dll_cdecl, ty_string, 2, ty_string, ty_string), str, def);
  }

  string get_password(string str, string def) {
    return external_call(external_define(dll, "get_password", dll_cdecl, ty_string, 2, ty_string, ty_string), str, def);
  }

  double get_integer(string str, var def) {
    double val = (strtod(def.c_str(), NULL)) ? : (double)def;
    return (double)external_call(external_define(dll, "get_integer", dll_cdecl, ty_real, 2, ty_string, ty_real), str, val);
  }

  double get_passcode(string str, var def) {
    double val = (strtod(def.c_str(), NULL)) ? : (double)def;
    return (double)external_call(external_define(dll, "get_passcode", dll_cdecl, ty_real, 2, ty_string, ty_real), str, val);
  }

  string get_open_filename(string filter, string fname) {
    return external_call(external_define(dll, "get_open_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname);
  }

  string get_open_filename_ext(string filter, string fname, string dir, string title) {
    return external_call(external_define(dll, "get_open_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), 
    filter, fname, dir, title);
  }

  string get_open_filenames(string filter, string fname) {
    return external_call(external_define(dll, "get_open_filenames", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname);
  }

  string get_open_filenames_ext(string filter, string fname, string dir, string title) {
    return external_call(external_define(dll, "get_open_filenames_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), 
    filter, fname, dir, title);
  }

  string get_save_filename(string filter, string fname) {
    return external_call(external_define(dll, "get_save_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname);
  }

  string get_save_filename_ext(string filter, string fname, string dir, string title) {
    return external_call(external_define(dll, "get_save_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), 
    filter, fname, dir, title);
  }

  string get_directory(string dname) {
    return external_call(external_define(dll, "get_directory", dll_cdecl, ty_string, 1, ty_string), dname);
  }

  string get_directory_alt(string capt, string root) {
    return external_call(external_define(dll, "get_directory_alt", dll_cdecl, ty_string, 2, ty_string, ty_string), capt, root);
  }

  int get_color(int defcol) {
    return (int)external_call(external_define(dll, "get_color", dll_cdecl, ty_real, 1, ty_real), defcol);
  }

  int get_color_ext(int defcol, string title) {
    return (int)external_call(external_define(dll, "get_color_ext", dll_cdecl, ty_real, 2, ty_real, ty_string), defcol, title);
  }

  string widget_get_caption() {
    return external_call(external_define(dll, "widget_get_caption", dll_cdecl, ty_string, 0));
  }

  void widget_set_caption(string str) {
    external_call(external_define(dll, "widget_set_caption", dll_cdecl, ty_real, 1, ty_string), str);
  }

  void *widget_get_owner() {
    return (void *)external_call(external_define(dll, "widget_get_owner", dll_cdecl, ty_string, 0)).c_str();
  }

  void widget_set_owner(void *hwnd) {
    external_call(external_define(dll, "widget_set_owner", dll_cdecl, ty_real, 1, ty_string), hwnd);
  }

  string widget_get_icon() {
    return external_call(external_define(dll, "widget_get_icon", dll_cdecl, ty_string, 0));
  }

  void widget_set_icon(string icon) {
    external_call(external_define(dll, "widget_set_icon", dll_cdecl, ty_real, 1, ty_string), icon);
  }

  string widget_get_system() {
    return external_call(external_define(dll, "widget_get_system", dll_cdecl, ty_string, 0));
  }

  void widget_set_system(string sys) {
    external_call(external_define(dll, "widget_set_system", dll_cdecl, ty_real, 1, ty_string), sys);
  }

  string widget_get_button_name(int type) {
    return external_call(external_define(dll, "widget_get_button_name", dll_cdecl, ty_string, 1, ty_real), type);
  }

  void widget_set_button_name(int type, string name) {
    external_call(external_define(dll, "widget_set_button_name", dll_cdecl, ty_real, 2, ty_real, ty_string), type, name);
  }

} // namespace enigma_user
