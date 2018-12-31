/** Copyright (C) 2018 Samuel Venable
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

#include "Platforms/General/PFexternals.h"
#include "Platforms/General/PFwindow.h"
#include <string>

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/resource_data.h"
#include "Universal_System/object.h"
#include "Universal_System/debugscope.h"
#endif

using enigma_user::external_define;
using enigma_user::external_call;
using enigma_user::external_free;

using enigma_user::window_handle;

using std::string;

namespace enigma {
  
bool widget_system_initialize() {
  return true;
}
  
} // namespace enigma

void show_error(string errortext, const bool fatal) {
  #ifdef DEBUG_MODE
  errortext = enigma::debug_scope::GetErrors() + "\n\n" + errortext;
  #else
  errortext = "Error in some event or another for some object: \r\n\r\n" + errortext;
  #endif

  bool DialogModule_result;
  DialogModule_result = (bool)external_call(external_define("DialogModule.dll", "show_error", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_string, enigma_user::ty_real), (char *)errortext.c_str(), (double)fatal);
  external_free("DialogModule.dll");
  if (DialogModule_result || fatal)
    exit(0);
}

namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {

}

int show_message(string str) {
  int DialogModule_result;
  external_call(external_define("DialogModule.dll", "window_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), window_handle());
  DialogModule_result = (int)external_call(external_define("DialogModule.dll", "show_message", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), (char *)str.c_str());
  external_free("DialogModule.dll");
  return DialogModule_result;
}

bool show_question(string str) {
  bool DialogModule_result;
  external_call(external_define("DialogModule.dll", "window_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), window_handle());
  DialogModule_result = (bool)external_call(external_define("DialogModule.dll", "show_question", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), (char *)str.c_str());
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_string(string str, string def) {
  string DialogModule_result;
  external_call(external_define("DialogModule.dll", "window_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), window_handle());
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_string", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), (char *)str.c_str(), (char *)def.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

double get_integer(string str, double def) {
  double DialogModule_result;
  external_call(external_define("DialogModule.dll", "window_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), window_handle());
  DialogModule_result = external_call(external_define("DialogModule.dll", "get_integer", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_string, enigma_user::ty_real), (char *)str.c_str(), def);
  external_free("DialogModule.dll");
  return DialogModule_result;
 }

string get_password(string str, string def) {
  string DialogModule_result;
  external_call(external_define("DialogModule.dll", "window_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), window_handle());
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_password", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), (char *)str.c_str(), (char *)def.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

double get_passcode(string str, double def) {
  double DialogModule_result;
  external_call(external_define("DialogModule.dll", "window_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), window_handle());
  DialogModule_result = external_call(external_define("DialogModule.dll", "get_passcode", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_string, enigma_user::ty_real), (char *)str.c_str(), def);
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_open_filename(string filter, string fname) {
  string DialogModule_result;
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_open_filename", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), (char *)filter.c_str(), (char *)fname.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_save_filename(string filter, string fname) {
  string DialogModule_result;
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_save_filename", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), (char *)filter.c_str(), (char *)fname.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  string DialogModule_result;
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_open_filename_ext", enigma_user::dll_cdecl, enigma_user::ty_string, 4, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string), (char *)filter.c_str(), (char *)fname.c_str(), (char *)dir.c_str(), (char *)title.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  string DialogModule_result;
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_save_filename_ext", enigma_user::dll_cdecl, enigma_user::ty_string, 4, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string), (char *)filter.c_str(), (char *)fname.c_str(), (char *)dir.c_str(), (char *)title.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_directory(string dname) {
  string DialogModule_result;
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_directory", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), (char *)dname.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

string get_directory_alt(string capt, string root) {
  string DialogModule_result;
  DialogModule_result = string(external_call(external_define("DialogModule.dll", "get_directory_alt", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), (char *)capt.c_str(), (char *)root.c_str()));
  external_free("DialogModule.dll");
  return DialogModule_result;
}

int get_color(int defcol) {
  int DialogModule_result;
  DialogModule_result = (int)external_call(external_define("DialogModule.dll", "get_color", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_real), (double)defcol);
  external_free("DialogModule.dll");
  return DialogModule_result;
}
  
} // namespace enigma_user
