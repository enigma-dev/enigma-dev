/*
 
 MIT License
 
 Copyright © 2020 Samuel Venable
 
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

#include "Universal_System/var4.h"

#include <string>
using std::string;
typedef string wid_t;

namespace enigma_user {

// Widget Systems
static string ws_win32       = "Win32";
static string ws_cocoa       = "Cocoa";
static string ws_x11_zenity  = "Zenity";
static string ws_x11_kdialog = "KDialog";

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
