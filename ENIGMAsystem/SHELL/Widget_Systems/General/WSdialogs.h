/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
*** Copyright (C) 2019 Samuel Venable
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

#include "Universal_System/var4.h"

#include <string>
using std::string;

namespace enigma_user {

// Widget Systems
static string ws_win32       = "Win32";
static string ws_cocoa       = "Cocoa";
static string ws_x11_zenity  = "Zenity";
static string ws_x11_kdialog = "KDialog";

} // namespace enigma_user

namespace enigma {

void libdlgmod_init();
void libdlgmod_uninit();

} // namespace enigma

namespace enigma_user {

// Widget Settings
string widget_get_system();
void widget_set_system(string sys);
wid_t widget_get_owner();
void widget_set_owner(wid_t hwnd);
string widget_get_icon();
void widget_set_icon(string icon);
string widget_get_caption();
void widget_set_caption(wid_t str);
string widget_get_button_name(int type);
void widget_set_button_name(int type, string name);

// GML/EDL Dialogs
int show_message(string str);
int show_message_cancelable(string str);
int show_question(string str);
int show_question_cancelable(string str);
int show_attempt(string str);
int show_error(string str, bool abort);
string get_string(string str, string def);
string get_password(string str, string def);
double get_integer(string str, double def);
double get_passcode(string str, double def);
string get_open_filename(string filter, string fname);
string get_open_filename_ext(string filter, string fname, string dir, string title);
string get_open_filenames(string filter, string fname);
string get_open_filenames_ext(string filter, string fname, string dir, string title);
string get_save_filename(string filter, string fname);
string get_save_filename_ext(string filter, string fname, string dir, string title);
string get_directory(string dname);
string get_directory_alt(string capt, string root);
int get_color(int defcol);
int get_color_ext(int defcol, string title);

// DND Actions
inline bool action_if_question(string message) { return show_question(message); }

} // namespace enigma_user
