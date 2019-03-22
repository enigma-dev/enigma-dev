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

#include "dialogs.h"
#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <stdlib.h>
#include <string>

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/resource_data.h"
#include "Universal_System/object.h"
#include "Universal_System/debugscope.h"
#endif

using std::string;

namespace enigma {
  
bool widget_system_initialize() {
  return true;
}
  
} // namespave enigma

extern "C" const char *cocoa_dialog_caption();
extern "C" int cocoa_show_message(const char *str, bool has_cancel, const char *title);
extern "C" int cocoa_show_question(const char *str, bool has_cancel, const char *title);
extern "C" int cocoa_show_attempt(const char *str, const char *title);
extern "C" int cocoa_show_error(const char *str, bool abort, const char *title);
extern "C" const char *cocoa_input_box(const char *str, const char *def, const char *title);
extern "C" const char *cocoa_password_box(const char *str, const char *def, const char *title);
extern "C" const char *cocoa_get_open_filename(const char *filter, const char *fname, const char *dir, const char *title, const bool mselect);
extern "C" const char *cocoa_get_save_filename(const char *filter, const char *fname, const char *dir, const char *title);
extern "C" const char *cocoa_get_directory(const char *capt, const char *root);
extern "C" int cocoa_get_color(int defcol, const char *title);

static string dialog_caption;
static string error_caption;

static inline string remove_trailing_zeros(double numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

void show_error(string errortext, const bool fatal) {
  #ifdef DEBUG_MODE
  errortext = enigma::debug_scope::GetErrors() + "\r\n\r\n" + errortext;
  #else
  errortext = "Error in some event or another for some object:\r\n\r\n" + errortext;
  #endif
  
  if (error_caption == "") error_caption = "Error";
  cocoa_show_error(errortext.c_str(), (const bool)fatal, error_caption.c_str());
}

namespace enigma_user {
  
void show_info(string text, int bgcolor, int left, int top, int width, int height,
  bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
  bool pauseGame, string caption) {

}

int show_message(const string &str) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_message(str.c_str(), false, dialog_caption.c_str());
}

int show_message_cancelable(string str) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_message(str.c_str(), true, dialog_caption.c_str());
}

bool show_question(string str) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return (bool)cocoa_show_question(str.c_str(), false, dialog_caption.c_str());
}

int show_question_cancelable(string str) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_show_question(str.c_str(), true, dialog_caption.c_str());
}

int show_attempt(string str) {
  if (error_caption == "") error_caption = "Error";
  return cocoa_show_attempt(str.c_str(), error_caption.c_str());
}

string get_string(string str, string def) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_input_box(str.c_str(), def.c_str(), dialog_caption.c_str());
}

string get_password(string str, string def) {
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  return cocoa_password_box(str.c_str(), def.c_str(), dialog_caption.c_str());
}

double get_integer(string str, double def) {
  string integer = remove_trailing_zeros(def);
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  string result = cocoa_input_box(str.c_str(), integer.c_str(), dialog_caption.c_str());
  return result ? strtod(input, NULL) : 0;
}

double get_passcode(string str, double def) {
  string integer = remove_trailing_zeros(def);
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  string result = cocoa_password_box(str.c_str(), integer.c_str(), dialog_caption.c_str());
  return result ? strtod(input, NULL) : 0;
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

void message_set_caption(string str) {
  dialog_caption = str; error_caption = str;
  if (dialog_caption == "") dialog_caption = cocoa_dialog_caption();
  if (error_caption == "") error_caption = "Error";
}

} // enigma_user
