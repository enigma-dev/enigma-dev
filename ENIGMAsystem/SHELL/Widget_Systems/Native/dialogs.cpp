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

  bool widgets_initialized = false;
  bool widget_system_initialize() {
    if (!widgets_initialized) {
      dialog_module::libdlgmod_init();
      dialog_module::widget_set_owner(std::to_string((unsigned long long)(void *)enigma_user::window_handle()));
      widgets_initialized = true;
    }
    return widgets_initialized;
  }

}

namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) { }

void show_info() { }

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  enigma::widget_system_initialize();
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
  enigma::widget_system_initialize();
  return dialog_module::show_message(message);
}

int show_message_cancelable(std::string str) {
  enigma::widget_system_initialize();
  return dialog_module::show_message_cancelable(str);
}

bool show_question(std::string str) {
  enigma::widget_system_initialize();
  return (bool)dialog_module::show_question(str);
}

int show_question_cancelable(std::string str) {
  enigma::widget_system_initialize();
  return dialog_module::show_question_cancelable(str);
}

int show_attempt(std::string str) {
  enigma::widget_system_initialize();
  return dialog_module::show_attempt(str);
}

std::string get_string(std::string str, std::string def) {
  enigma::widget_system_initialize();
  return dialog_module::get_string(str, def);
}

std::string get_password(std::string str, std::string def) {
  enigma::widget_system_initialize();
  return dialog_module::get_password(str, def);
}

double get_number(std::string str, double def) {
  enigma::widget_system_initialize();
  return dialog_module::get_integer(str, def);
}

double get_integer(std::string str, double def) {
  enigma::widget_system_initialize();
  return dialog_module::get_integer(str, def);
}

double get_passcode(std::string str, double def) {
  enigma::widget_system_initialize();
  return dialog_module::get_passcode(str, def);
}

std::string get_open_filename(std::string filter, std::string fname) {
  enigma::widget_system_initialize();
  return dialog_module::get_open_filename(filter, fname);
}

std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) {
  enigma::widget_system_initialize();
  return dialog_module::get_open_filename_ext(filter, fname, dir, title);
}

std::string get_open_filenames(std::string filter, std::string fname) {
  enigma::widget_system_initialize();
  return dialog_module::get_open_filenames(filter, fname);
}

std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title) {
  enigma::widget_system_initialize();
  return dialog_module::get_open_filenames_ext(filter, fname, dir, title);
}

std::string get_save_filename(std::string filter, std::string fname) {
  enigma::widget_system_initialize();
  return dialog_module::get_save_filename(filter, fname);
}

std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title) {
  enigma::widget_system_initialize();
  return dialog_module::get_save_filename_ext(filter, fname, dir, title);
}

std::string get_directory(std::string dname) {
  enigma::widget_system_initialize();
  return dialog_module::get_directory(dname);
}

std::string get_directory_alt(std::string capt, std::string root) {
  enigma::widget_system_initialize();
  return dialog_module::get_directory_alt(capt, root);
}

int get_color(int defcol) {
  enigma::widget_system_initialize();
  return dialog_module::get_color(defcol);
}

int get_color_ext(int defcol, std::string title) {
  enigma::widget_system_initialize();
  return dialog_module::get_color_ext(defcol, title);
}

std::string widget_get_caption() {
  enigma::widget_system_initialize();
  return dialog_module::widget_get_caption();
}

void widget_set_caption(std::string str) {
  enigma::widget_system_initialize();
  dialog_module::widget_set_caption(str);
}

std::string widget_get_owner() {
  enigma::widget_system_initialize();
  return dialog_module::widget_get_owner();
}

void widget_set_owner(std::string hwnd) {
  enigma::widget_system_initialize();
  dialog_module::widget_set_owner(hwnd);
}

std::string widget_get_icon() {
  enigma::widget_system_initialize();
  return dialog_module::widget_get_icon();
}

void widget_set_icon(std::string icon) {
  enigma::widget_system_initialize();
  dialog_module::widget_set_icon(icon);
}

std::string widget_get_system() {
  enigma::widget_system_initialize();
  return dialog_module::widget_get_system();
}

void widget_set_system(std::string sys) {
  enigma::widget_system_initialize();
  dialog_module::widget_set_system(sys);
}

std::string widget_get_button_name(int type) {
  enigma::widget_system_initialize();
  return dialog_module::widget_get_button_name(type);
}

void widget_set_button_name(int type, std::string name) {
  enigma::widget_system_initialize();
  dialog_module::widget_set_button_name(type, name);
}

bool widget_get_canceled() {
  enigma::widget_system_initialize();
  return dialog_module::widget_get_canceled();
}

} // namespace enigma_user
