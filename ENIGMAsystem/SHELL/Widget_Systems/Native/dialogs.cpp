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

#include "Widget_Systems/Native/dialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFmain.h"
#include "libdlgmod.h"

using std::string;

namespace enigma {

  bool widget_system_initialize() {
    return true;
  }

}

namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) { }

void show_info() { }

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  #ifndef DEBUG_MODE
  errortext += "\n";
  fputs(errortext.c_str(), stderr);
  fflush(stderr);
  #endif
  if (type == MESSAGE_TYPE::M_FATAL_ERROR || 
    type == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    std::string caption = dialog_module::widget_get_caption();
    dialog_module::widget_set_caption("Fatal Error");
    dialog_module::show_error(errortext, true);
    dialog_module::widget_set_caption(caption);
    abort();
  } else if (type == MESSAGE_TYPE::M_ERROR || 
    type == MESSAGE_TYPE::M_USER_ERROR) {
    std::string caption = dialog_module::widget_get_caption();
    dialog_module::widget_set_caption("Error");
    dialog_module::show_error(errortext, false);
    dialog_module::widget_set_caption(caption);
  }
}

int show_message(const string &message) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::show_message(message);
}

int show_message_cancelable(std::string str) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::show_message_cancelable(str);
}

bool show_question(std::string str) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return (bool)dialog_module::show_question(str);
}

int show_question_cancelable(std::string str) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::show_question_cancelable(str);
}

int show_attempt(std::string str) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::show_attempt(str);
}

std::string get_string(std::string str, std::string def) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_string(str, def);
}

std::string get_password(std::string str, std::string def) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_password(str, def);
}

double get_number(std::string str, double def) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_integer(str, def);
}

double get_integer(std::string str, double def) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_integer(str, def);
}

double get_passcode(std::string str, double def) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_passcode(str, def);
}

std::string get_open_filename(std::string filter, std::string fname) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_open_filename(filter, fname);
}

std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_open_filename_ext(filter, fname, dir, title);
}

std::string get_open_filenames(std::string filter, std::string fname) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_open_filenames(filter, fname);
}

std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_open_filenames_ext(filter, fname, dir, title);
}

std::string get_save_filename(std::string filter, std::string fname) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_save_filename(filter, fname);
}

std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_save_filename_ext(filter, fname, dir, title);
}

std::string get_directory(std::string dname) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_directory(dname);
}

std::string get_directory_alt(std::string capt, std::string root) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_directory_alt(capt, root);
}

int get_color(int defcol) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_color(defcol);
}

int get_color_ext(int defcol, std::string title) {
  dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
  return dialog_module::get_color_ext(defcol, title);
}

std::string widget_get_caption() {
  return dialog_module::widget_get_caption();
}

void widget_set_caption(std::string str) {
  dialog_module::widget_set_caption(str);
}

std::string widget_get_owner() {
  return dialog_module::widget_get_owner();
}

void widget_set_owner(std::string hwnd) {
  dialog_module::widget_set_owner(hwnd);
}

std::string widget_get_icon() {
  return dialog_module::widget_get_icon();
}

void widget_set_icon(std::string icon) {
  dialog_module::widget_set_icon(icon);
}

std::string widget_get_system() {
  return dialog_module::widget_get_system();
}

void widget_set_system(std::string sys) {
  dialog_module::widget_set_system(sys);
}

void widget_set_button_name(int type, std::string name) {
  dialog_module::widget_set_button_name(type, name);
}

std::string widget_get_button_name(int type) {
  return dialog_module::widget_get_button_name(type);
}

bool widget_get_canceled() {
  return dialog_module::widget_get_canceled();
}

} // namespace enigma_user
