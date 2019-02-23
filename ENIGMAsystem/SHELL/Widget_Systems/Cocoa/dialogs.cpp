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

extern "C" int cocoa_show_message(const char *title, const char *str, bool error);
extern "C" int cocoa_show_question(const char *title, const char *str, bool error);
extern "C" int cocoa_show_error(const char *str, bool abort);
extern "C" const char *cocoa_input_box(const char *title, const char *str, const char *def);
extern "C" const char *cocoa_password_box(const char *title, const char *str, const char *def);
extern "C" const char *cocoa_get_open_filename(const char *title, const char *filter, const char *fname, const char *dir, const bool mselect);
extern "C" const char *cocoa_get_save_filename(const char *title, const char *filter, const char *fname, const char *dir);
extern "C" const char *cocoa_get_directory(const char *title, const char *dname);
extern "C" int cocoa_get_color(const char *title, int defcol);

static inline string remove_trailing_zeros(double numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

void show_error(string errortext, const bool fatal) {
  #ifdef DEBUG_MODE
  errortext = enigma::debug_scope::GetErrors() + "\n\n" + errortext;
  #else
  errortext = "Error in some event or another for some object: \r\n\r\n" + errortext;
  #endif
  
  cocoa_show_error(errortext.c_str(), (const bool)fatal);
}

namespace enigma_user {
  
void show_info(string text, int bgcolor, int left, int top, int width, int height,
  bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
  bool pauseGame, string caption) {

}

int show_message(const string &str) {
  string title = window_get_caption();
  cocoa_show_message(title.c_str(), str.c_str(), false);
  return 1;
}

bool show_question(string str) {
  string title = window_get_caption();
  bool result = (bool)cocoa_show_question(title.c_str(), str.c_str(), false);
  return result;
}

string get_string(string str, string def) {
  string title = window_get_caption();
  string result = cocoa_input_box(title.c_str(), str.c_str(), def.c_str());
  return result;
}

string get_password(string str, string def) {
  string title = window_get_caption();
  string result = cocoa_password_box(title.c_str(), str.c_str(), def.c_str());
  return result;
}

double get_integer(string str, double def) {
  string title = window_get_caption();
  string integer = remove_trailing_zeros(def);
  string result = cocoa_input_box(title.c_str(), str.c_str(), integer.c_str());
  return result ? strtod(input, NULL) : 0;
}

double get_passcode(string str, double def) {
  string title = window_get_caption();
  string integer = remove_trailing_zeros(def);
  string result = cocoa_password_box(title.c_str(), str.c_str(), integer.c_str());
  return result ? strtod(input, NULL) : 0;
}

string get_open_filename(string filter, string fname) {
  return cocoa_get_open_filename("", filter.c_str(), fname.c_str(), "", false);
}

string get_open_filenames(string filter, string fname) {
  return cocoa_get_open_filename("", filter.c_str(), fname.c_str(), "", true);
}

string get_save_filename(string filter, string fname) {
  return cocoa_get_save_filename("", filter.c_str(), fname.c_str(), "");
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_open_filename(title.c_str(), filter.c_str(), fname.c_str(), dir.c_str(), false);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_open_filename(title.c_str(), filter.c_str(), fname.c_str(), dir.c_str(), true);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return cocoa_get_save_filename(title.c_str(), filter.c_str(), fname.c_str(), dir.c_str());
}

string get_directory(string dname) {
  return cocoa_get_directory("", dname.c_str());
}

string get_directory_alt(string capt, string root) {
  return cocoa_get_directory(capt.c_str(), root.c_str());
}

int get_color(int defcol) {
  return cocoa_get_color("", defcol);
}

int get_color_ext(int defcol, string title) {
  return cocoa_get_color(title.c_str(), defcol);
} 

} // enigma_user
