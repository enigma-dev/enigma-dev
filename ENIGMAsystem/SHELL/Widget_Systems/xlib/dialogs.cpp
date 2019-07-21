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
#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"
#include <string>
using std::string;

namespace enigma {

bool widget_system_initialize() {
  return true;
}

} // namespace enigma

static string system = "Zenity";

namespace enigma_user {
    
string widget_get_system() {
  return system;
}

void widget_get_system(string sys) {
  if (sys == "KDialog") system = sys;
  else system = "Zenity";
}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) { 
  if (widget_get_system() == "KDialog") kdialog::show_info(info, bgcolor, left, top, width, height, embedGameWindow, showBorder, allowResize, stayOnTop, pauseGame, caption); 
  else zenity::show_info(info, bgcolor, left, top, width, height, embedGameWindow, showBorder, allowResize, stayOnTop, pauseGame, caption); 
}

int show_message(const string &message) {
  if (widget_get_system() == "KDialog") return kdialog::show_message(message);
  return zenity::show_message(message);
}

int show_message_cancelable(string message) {
  if (widget_get_system() == "KDialog") return kdialog::show_message_cancelable(message);
  return zenity::show_message_cancelable(message);
}

bool show_question(string message) {
  if (widget_get_system() == "KDialog") return kdialog::show_question(message);
  return zenity::show_question(message);
}

int show_question_cancelable(string message) {
  if (widget_get_system() == "KDialog") return kdialog::show_question_cancelable(message);
  return zenity::show_question_cancelable(message);
}

int show_attempt(string errortext) {
  if (widget_get_system() == "KDialog") return kdialog::show_attempt(errortext);
  return zenity::show_attempt(errortext);
}

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  if (widget_get_system() == "KDialog") kdialog::show_debug_message(errortext, type);
  else zenity::show_debug_message(errortext, type);
}

string get_string(string message, string def) {
  if (widget_get_system() == "KDialog") return kdialog::get_string(message, def);
  return zenity::get_string(message, def);
}

string get_password(string message, string def) {
  if (widget_get_system() == "KDialog") return kdialog::get_password(message, def);
  return zenity::get_password(message, def);
}

double get_integer(string message, double def) {
  if (widget_get_system() == "KDialog") return kdialog::get_integer(message, def);
  return zenity::get_integer(message, def);
}

double get_passcode(string message, double def) {
  if (widget_get_system() == "KDialog") return kdialog::get_passcode(message, def);
  return zenity::get_passcode(message, def);
}

string get_open_filename(string filter, string fname) {
  if (widget_get_system() == "KDialog") return kdialog::get_open_filename(filter, fname);
  return zenity::get_open_filename(filter, fname);
}

string get_open_filenames(string filter, string fname) {
  if (widget_get_system() == "KDialog") return kdialog::get_open_filenames(filter, fname);
  return zenity::get_open_filenames(filter, fname);
}

string get_save_filename(string filter, string fname) {
  if (widget_get_system() == "KDialog") return kdialog::get_save_filename(filter, fname);
  return zenity::get_save_filename(filter, fname);
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  if (widget_get_system() == "KDialog") return kdialog::get_open_filename_ext(filter, fname, dir, title);
  return zenity::get_open_filename_ext(filter, fname, dir, title);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  if (widget_get_system() == "KDialog") return kdialog::get_open_filenames_ext(filter, fname, dir, title);
  return zenity::get_open_filenames_ext(filter, fname, dir, title);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  if (widget_get_system() == "KDialog") return kdialog::get_save_filename_ext(filter, fname, dir, title);
  return zenity::get_save_filename_ext(filter, fname, dir, title);
}

string get_directory(string dname) {
  if (widget_get_system() == "KDialog") return kdialog::get_directory(dname);
  return zenity::get_directory(dname);
}

string get_directory_alt(string capt, string root) {
  if (widget_get_system() == "KDialog") return kdialog::get_directory_alt(capt, root);
  return zenity::get_directory_alt(capt, root);
}

int get_color(int defcol) {
  if (widget_get_system() == "KDialog") return kdialog::get_color(defcol);
  return zenity::get_color(defcol);
}

int get_color_ext(int defcol, string title) {
  if (widget_get_system() == "KDialog") return kdialog::get_color_ext(defcol, title);
  return zenity::get_color_ext(defcol, title);
}

string message_get_caption() {
  if (widget_get_system() == "KDialog") return kdialog::message_get_caption();
  return zenity::message_get_caption();
}

void message_set_caption(string title) {
  if (widget_get_system() == "KDialog") kdialog::message_set_caption(title);
  else zenity::message_set_caption(title);
}

} // namespace enigma_user
