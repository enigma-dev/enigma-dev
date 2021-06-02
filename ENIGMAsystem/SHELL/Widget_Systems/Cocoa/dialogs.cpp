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

#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "strings_util.h"
#include <stdlib.h>
#include <cstdio>
#include <string>

#ifdef DEBUG_MODE
#include "Universal_System/Resources/resource_data.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/debugscope.h"
#endif

using std::string;

namespace enigma {

bool widget_system_initialize() {
  return true;
}

} // namespave enigma

extern "C" const char *cocoa_dialog_caption();
extern "C" int cocoa_show_message(const char *message, bool has_cancel, const char *title);
extern "C" int cocoa_show_question(const char *message, bool has_cancel, const char *title);
extern "C" int cocoa_show_attempt(const char *errortext, const char *title);
extern "C" int cocoa_show_error(const char *errortext, bool fatal, const char *title);
extern "C" const char *cocoa_input_box(const char *message, const char *def, const char *title);
extern "C" const char *cocoa_password_box(const char *message, const char *def, const char *title);
extern "C" const char *cocoa_get_open_filename(const char *filter, const char *fname, const char *dir, const char *title, const bool mselect);
extern "C" const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title);
extern "C" const char *cocoa_get_directory(const char *capt, const char *root);
extern "C" int cocoa_get_color(int defcol, const char *title);

static string dialog_caption;
static string error_caption;

void show_debug_message_helper(string errortext, MESSAGE_TYPE type) {
  #ifdef DEBUG_MODE
  errortext += "\n\n" + enigma::debug_scope::GetErrors();
  #endif

  if (error_caption == "") error_caption = "Error";
  int result = cocoa_show_error(errortext.c_str(), (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR), error_caption.c_str());
  if (result == 1) exit(0);
}

static string widget = enigma_user::ws_cocoa;

namespace enigma_user {
    
string widget_get_system() {
  return widget;
}

void widget_set_system(string sys) {
  // place holder
}

void show_info(string text, int bgcolor, int left, int top, int width, int height,
  bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
  bool pauseGame, string caption) {

}

int show_message(const string &message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_message(message.c_str(), false, dialog_caption.c_str());
}

int show_message_cancelable(string message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_message(message.c_str(), true, dialog_caption.c_str());
}

bool show_question(string message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return (bool)cocoa_show_question(message.c_str(), false, dialog_caption.c_str());
}

int show_question_cancelable(string message) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_question(message.c_str(), true, dialog_caption.c_str());
}

int show_attempt(string errortext) {
  if (error_caption == "") error_caption = "Error";
  return cocoa_show_attempt(errortext.c_str(), error_caption.c_str());
}

string get_string(string message, string def) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_input_box(message.c_str(), def.c_str(), dialog_caption.c_str());
}

string get_password(string message, string def) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_password_box(message.c_str(), def.c_str(), dialog_caption.c_str());
}

double get_integer(string message, var def) {
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  string integer = enigma::remove_trailing_zeros(val);
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  string result = cocoa_input_box(message.c_str(), integer.c_str(), dialog_caption.c_str());
  return !result.empty() ? strtod(result.c_str(), NULL) : 0;
}

double get_passcode(string message, var def) {
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  string integer = enigma::remove_trailing_zeros(val);
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  string result = cocoa_password_box(message.c_str(), integer.c_str(), dialog_caption.c_str());
  return !result.empty() ? strtod(result.c_str(), NULL) : 0;
}

string get_open_filename(string filter, string fname) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), "", "", false);
}

string get_open_filenames(string filter, string fname) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), "", "", true);
}

string get_save_filename(string filter, string fname) {
  return cocoa_get_save_filename(filter.c_str(), fname.c_str(), "", "");
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), dir.c_str(), title.c_str(), false);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_open_filename(filter.c_str(), fname.c_str(), dir.c_str(), title.c_str(), true);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_save_filename(filter.c_str(), fname.c_str(), dir.c_str(), title.c_str());
}

string get_directory(string dname) {
  return cocoa_get_directory("", dname.c_str());
}

string get_directory_alt(string capt, string root) {
  return cocoa_get_directory(capt.c_str(), root.c_str());
}

int get_color(int defcol) {
  return cocoa_get_color(defcol, "");
}

int get_color_ext(int defcol, string title) {
  return cocoa_get_color(defcol, title.c_str());
}

string message_get_caption() {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  if (error_caption == "") error_caption = "Error";

  if (dialog_caption == cocoa_dialog_caption() && error_caption == "Error")
    return "";

  return dialog_caption;
}

void message_set_caption(string title) {
  dialog_caption = title; error_caption = title;
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  if (error_caption == "") error_caption = "Error";
}

} // enigma_user
