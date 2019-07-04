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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <libgen.h>
#include <unistd.h>
#include <climits>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
using std::string;

#include "strings_util.h"
#include "Universal_System/estring.h"
using enigma_user::filename_name;
using enigma_user::filename_path;

#include "Platforms/General/PFmain.h"
using enigma_user::execute_shell_for_output;

#include "Platforms/General/PFwindow.h"
using enigma_user::window_get_caption;

#include "Platforms/General/PFfilemanip.h"
using enigma_user::file_exists;

#include "Graphics_Systems/General/GScolors.h"
using enigma_user::color_get_red;
using enigma_user::color_get_green;
using enigma_user::color_get_blue;
using enigma_user::make_color_rgb;

#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/Resources/resource_data.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/debugscope.h"
#endif

static string dialog_caption;
static string error_caption;

static bool message_cancel  = false;
static bool question_cancel = false;

namespace enigma {

bool widget_system_initialize() {
  return true;
}

} // namespace enigma

static string shellscript_evaluate(string command) {
  string result = execute_shell_for_output(command);
  if (result.back() == '\n') result.pop_back();
  return result;
}

static string add_escaping(string str, bool is_caption, string new_caption) {
  string result; if (is_caption && str.empty()) result = new_caption;
  result = string_replace_all(str, "\"", "\\\""); // kdialog needs this for quotes to show
  return result;
}

static string remove_trailing_zeros(double numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

static string kdialog_filter(string input) {
  std::replace(input.begin(), input.end(), ';', ' ');
  std::vector<string> stringVec = split_string(input, '|');
  string string_output = " '";

  string even = "";
  string odd = "";

  unsigned int index = 0;
  for (const string &str : stringVec) {
    if (index % 2 != 0) {
      if (index == 1)
        odd = str;
      else
        odd = "\n" + str;

      string_output += string_replace_all(odd, "*.*", "*") + even;
    }
    else
      even = "|" + str;

    index += 1;
  }

  string_output += "'";

  return string_output;
}

static int show_message_helperfunc(string message) {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;
  string str_cancel;
  string str_echo = "echo 1";

  if (message_cancel)
    str_echo = "if [ $? = 0 ] ;then echo 1;fi";

  str_title = add_escaping(dialog_caption, true, "KDialog");
  str_cancel = string("--msgbox \"") + add_escaping(message, false, "") + string("\" ");

  if (message_cancel)
    str_cancel = string("--yesno \"") + add_escaping(message, false, "") + string("\" --yes-label Ok --no-label Cancel ");

  str_command = string("kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") + str_cancel +
  string("--title \"") + str_title + string("\";") + str_echo;

  string str_result = shellscript_evaluate(str_command);
  return (int)strtod(str_result.c_str(), NULL);
}

static int show_question_helperfunc(string message) {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;
  string str_cancel = "";

  str_title = add_escaping(dialog_caption, true, "KDialog");
  if (question_cancel)
    str_cancel = "cancel";

  str_command = string("kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--yesno") + str_cancel + string(" \"") + add_escaping(message, false, "") + string("\" ") +
  string("--yes-label Yes --no-label No ") + string("--title \"") + str_title + string("\";") +
  string("x=$? ;if [ $x = 0 ] ;then echo 1;elif [ $x = 1 ] ;then echo 0;elif [ $x = 2 ] ;then echo -1;fi");

  string str_result = shellscript_evaluate(str_command);
  return (int)strtod(str_result.c_str(), NULL);
}

namespace enigma_user {

void show_error(string errortext, const bool fatal) {
  if (error_caption.empty()) error_caption = "Error";
  string str_command;
  string str_title;
  string str_echo;

  #ifdef DEBUG_MODE
  errortext += enigma::debug_scope::GetErrors();
  #endif

  str_echo = fatal ? "echo 1" :
    "x=$? ;if [ $x = 0 ] ;then echo 1;elif [ $x = 1 ] ;then echo 0;elif [ $x = 2 ] ;then echo -1;fi";

  str_command = string("kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--warningyesnocancel \"") + add_escaping(errortext, false, "") + string("\" ") +
  string("--yes-label Abort --no-label Retry --cancel-label Ignore ") +
  string("--title \"") + add_escaping(error_caption, true, "Error") + string("\";") + str_echo;

  string str_result = shellscript_evaluate(str_command);
  if (strtod(str_result.c_str(), NULL) == 1) exit(0);
}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {

}

int show_message(const string &message) {
  message_cancel = false;
  return show_message_helperfunc(message);
}

int show_message_cancelable(string message) {
  message_cancel = true;
  return show_message_helperfunc(message);
}

bool show_question(string message) {
  question_cancel = false;
  return (bool)show_question_helperfunc(message);
}

int show_question_cancelable(string message) {
  question_cancel = true;
  return show_question_helperfunc(message);
}

int show_attempt(string errortext) {
  if (error_caption.empty()) error_caption = "Error";
  string str_command;
  string str_title;

  str_command = string("kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--warningyesno") + string(" \"") + add_escaping(errortext, false, "") + string("\" ") +
  string("--yes-label Retry --no-label Cancel ") + string("--title \"") +
  add_escaping(error_caption, true, "Error") + string("\";") +
  string("x=$? ;if [ $x = 0 ] ;then echo 0;else echo -1;fi");

  string str_result = shellscript_evaluate(str_command);
  return (int)strtod(str_result.c_str(), NULL);
}

string get_string(string message, string def) {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;

  str_title = add_escaping(dialog_caption, true, "KDialog");
  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--inputbox \"") + add_escaping(message, false, "") + string("\" \"") +
  add_escaping(def, false, "") + string("\" --title \"") +
  str_title + string("\");echo $ans");

  return shellscript_evaluate(str_command);
}

string get_password(string message, string def) {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;

  str_title = add_escaping(dialog_caption, true, "KDialog");
  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--password \"") + add_escaping(message, false, "") + string("\" \"") +
  add_escaping(def, false, "") + string("\" --title \"") +
  str_title + string("\");echo $ans");

  return shellscript_evaluate(str_command);
}

double get_integer(string message, double def) {
  string str_def = remove_trailing_zeros(def);
  string str_result = get_string(message, str_def);
  return strtod(str_result.c_str(), NULL);
}

double get_passcode(string message, double def) {
  string str_def = remove_trailing_zeros(def);
  string str_result = get_password(message, str_def);
  return strtod(str_result.c_str(), NULL);
}

string get_open_filename(string filter, string fname) {
  string str_command; string pwd;
  string str_title = "Open";
  string str_fname = filename_name(fname);

  if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_fname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
  string(" --title \"") + str_title + string("\"") + string(");echo $ans");

  string result = shellscript_evaluate(str_command);
  return file_exists(result) ? result : "";
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  string str_command; string pwd;
  string str_title = add_escaping(title, true, "Open");
  string str_fname = filename_name(fname);
  string str_dir = filename_path(dir);

  string str_path = fname;
  if (str_dir[0] != '\0') str_path = str_dir + str_fname;
  str_fname = (char *)str_path.c_str();

  if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_fname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
  string(" --title \"") + str_title + string("\"") + string(");echo $ans");

  string result = shellscript_evaluate(str_command);
  return file_exists(result) ? result : "";
}

string get_open_filenames(string filter, string fname) {
  string str_command; string pwd;
  string str_title = "Open";
  string str_fname = filename_name(fname);

  if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_fname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
  string(" --multiple --separate-output --title \"") + str_title + string("\"");

  static string result;
  result = shellscript_evaluate(str_command);
  std::vector<string> stringVec = split_string(result, '\n');

  bool success = true;
  for (const string &str : stringVec) {
    if (!file_exists(str))
      success = false;
  }

  return success ? result : "";
}

string get_open_filenames_ext(string filter,string fname, string dir, string title) {
  string str_command; string pwd;
  string str_title = add_escaping(title, true, "Open");
  string str_fname = filename_name(fname);
  string str_dir = filename_path(dir);

  string str_path = fname;
  if (str_dir[0] != '\0') str_path = str_dir + str_fname;
  str_fname = (char *)str_path.c_str();

  if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_fname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
  string(" --multiple --separate-output --title \"") + str_title + string("\"");

  static string result;
  result = shellscript_evaluate(str_command);
  std::vector<string> stringVec = split_string(result, '\n');

  bool success = true;
  for (const string &str : stringVec) {
    if (!file_exists(str))
      success = false;
  }

  return success ? result : "";
}

string get_save_filename(string filter, string fname) {
  string str_command; string pwd;
  string str_title = "Save As";
  string str_fname = filename_name(fname);

  if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_fname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getsavefilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
  string(" --title \"") + str_title + string("\"") + string(");echo $ans");

  return shellscript_evaluate(str_command);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  string str_command; string pwd;
  string str_title = add_escaping(title, true, "Save As");
  string str_fname = filename_name(fname);
  string str_dir = filename_path(dir);

  string str_path = fname;
  if (str_dir[0] != '\0') str_path = str_dir + str_fname;
  str_fname = (char *)str_path.c_str();

  if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_fname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getsavefilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
  string(" --title \"") + str_title + string("\"") + string(");echo $ans");

  return shellscript_evaluate(str_command);
}

string get_directory(string dname) {
  string str_command; string pwd;
  string str_title = "Select Directory";
  string str_dname = dname;
  string str_end = "\");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";

  if (str_dname.c_str() && str_dname[0] != '/' && str_dname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_dname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getexistingdirectory /") + pwd + string(" --title \"") + str_title + str_end;

  return shellscript_evaluate(str_command);
}

string get_directory_alt(string capt, string root) {
  string str_command; string pwd;
  string str_title = add_escaping(capt, true, "Select Directory");
  string str_dname = root;
  string str_end = "\");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";

  if (str_dname.c_str() && str_dname[0] != '/' && str_dname.length()) pwd = "\"$PWD/\"\"" +
    add_escaping(str_dname, false, "") + "\""; else pwd = "\"$PWD/\"";

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getexistingdirectory /") + pwd + string(" --title \"") + str_title + str_end;

  return shellscript_evaluate(str_command);
}

int get_color(int defcol) {
  string str_command;
  string str_title = "Color";
  string str_defcol;
  string str_result;

  int red; int green; int blue;
  red = color_get_red(defcol);
  green = color_get_green(defcol);
  blue = color_get_blue(defcol);

  char hexcol[16];
  snprintf(hexcol, sizeof(hexcol), "%02x%02x%02x", red, green, blue);

  str_defcol = string("#") + string(hexcol);
  std::transform(str_defcol.begin(), str_defcol.end(), str_defcol.begin(), ::toupper);

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getcolor --default '") + str_defcol + string("' --title \"") + str_title +
  string("\");if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

  str_result = shellscript_evaluate(str_command);
  if (str_result == "-1") return strtod(str_result.c_str(), NULL);
  str_result = str_result.substr(1, str_result.length() - 1);

  unsigned int color;
  std::stringstream ss2;
  ss2 << std::hex << str_result;
  ss2 >> color;

  blue = color_get_red(color);
  green = color_get_green(color);
  red = color_get_blue(color);

  return make_color_rgb(red, green, blue);
}

int get_color_ext(int defcol, string title) {
  string str_command;
  string str_title = add_escaping(title, true, "Color");
  string str_defcol;
  string str_result;

  int red; int green; int blue;
  red = color_get_red(defcol);
  green = color_get_green(defcol);
  blue = color_get_blue(defcol);

  char hexcol[16];
  snprintf(hexcol, sizeof(hexcol), "%02x%02x%02x", red, green, blue);

  str_defcol = string("#") + string(hexcol);
  std::transform(str_defcol.begin(), str_defcol.end(), str_defcol.begin(), ::toupper);

  str_command = string("ans=$(kdialog ") +
  string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--getcolor --default '") + str_defcol + string("' --title \"") + str_title +
  string("\");if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

  str_result = shellscript_evaluate(str_command);
  if (str_result == "-1") return strtod(str_result.c_str(), NULL);
  str_result = str_result.substr(1, str_result.length() - 1);

  unsigned int color;
  std::stringstream ss2;
  ss2 << std::hex << str_result;
  ss2 >> color;

  blue = color_get_red(color);
  green = color_get_green(color);
  red = color_get_blue(color);

  return make_color_rgb(red, green, blue);
}

string message_get_caption() {
  if (dialog_caption.empty()) dialog_caption = window_get_caption();
  if (error_caption.empty()) error_caption = "Error";
  if (dialog_caption == window_get_caption() && error_caption == "Error")
    return ""; else return dialog_caption;
}

void message_set_caption(string title) {
  dialog_caption = title; error_caption = title;
  if (dialog_caption.empty()) dialog_caption = window_get_caption();
  if (error_caption.empty()) error_caption = "Error";
}

} // namespace enigma_user
