/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
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

#include "dialogs.h"
#include "strings_util.h"
#include "Universal_System/estring.h"
#include "Widget_Systems/widgets_mandatory.h"
using enigma_user::filename_name;
using enigma_user::filename_path;

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFwindow.h"
using enigma_user::window_get_caption;

#include "Graphics_Systems/General/GScolors.h"
using enigma_user::color_get_red;
using enigma_user::color_get_green;
using enigma_user::color_get_blue;
using enigma_user::make_color_rgb;

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

using enigma::create_shell_dialog;

static bool file_exists(string fname) {
  return (!access(fname.c_str(), F_OK));
}

static string add_escaping(string str, bool is_caption, string new_caption) {
  string result = str; if (is_caption && str.empty()) result = new_caption;
  result = string_replace_all(result, "\"", "\\\""); // zenity needs this for quotes to show
  return result;
}

static string zenity_filter(string input) {
  std::vector<string> stringVec = split_string(input, '|');
  string string_output = "";

  unsigned int index = 0;
  for (string str : stringVec) {
    if (index % 2 == 0)
      string_output += " --file-filter='" + str + "|";
    else {
      std::replace(str.begin(), str.end(), ';', ' ');
      string_output += string_replace_all(str, "*.*", "*") + "'";
    }

    index += 1;
  }

  return string_output;
}

static int show_message_helperfunc(string message) {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title = add_escaping(dialog_caption, true, " ");
  string str_cancel = "--info --ok-label=OK ";
  string str_icon = "\" --icon-name=dialog-information);";
  string str_echo = "echo 1";

  if (message_cancel) {
    str_echo = "if [ $? = 0 ] ;then echo 1;else echo -1;fi";
    str_cancel = "--question --ok-label=OK --cancel-label=Cancel ";
    str_icon = "\" --icon-name=dialog-question);";
  }

  str_command = string("ans=$(zenity ") +
  str_cancel + string("--title=\"") + str_title + string("\" --no-wrap --text=\"") +
  add_escaping(message, false, "") + str_icon + str_echo;

  string str_result = create_shell_dialog(str_command);
  return (int)strtod(str_result.c_str(), nullptr);
}

static int show_question_helperfunc(string message) {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;
  string str_cancel = "";

  str_title = add_escaping(dialog_caption, true, " ");
  if (question_cancel)
    str_cancel = "--extra-button=Cancel ";

  str_command = string("ans=$(zenity ") +
  string("--question --ok-label=Yes --cancel-label=No ") + str_cancel +  string("--title=\"") +
  str_title + string("\" --no-wrap --text=\"") + add_escaping(message, false, "") +
  string("\" --icon-name=dialog-question);if [ $? = 0 ] ;then echo 1;elif [ $ans = \"Cancel\" ] ;then echo -1;else echo 0;fi");

  string str_result = create_shell_dialog(str_command);
  return (int)strtod(str_result.c_str(), nullptr);
}

static void show_debug_message_helperfunc(string errortext, MESSAGE_TYPE type) {
  if (error_caption.empty()) error_caption = "Error";
  string str_command;
  string str_title;
  string str_echo;

  #ifdef DEBUG_MODE
  errortext += "\n\n" + enigma::debug_scope::GetErrors();
  #endif

  str_echo = (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR) ? "echo 1" :
    "if [ $ans = \"Abort\" ] ;then echo 1;else echo -1;fi";

  if (type == MESSAGE_TYPE::M_FATAL_ERROR || 
    type == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    str_command = string("ans=$(zenity ") +
    string("--error --ok-label=Abort ") +
    string("--title=\"") + add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
    add_escaping(errortext, false, "") + string("\" --icon-name=dialog-error);") + str_echo;
  } else {
    str_command = string("ans=$(zenity ") +
    string("--error --ok-label=Ignore --extra-button=Abort ") +
    string("--title=\"") + add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
    add_escaping(errortext, false, "") + string("\" --icon-name=dialog-error);") + str_echo;
  }

  string str_result = create_shell_dialog(str_command);
  if (strtod(str_result.c_str(), nullptr) == 1) exit(0);
}

class ZenityWidgets : public enigma::CommandLineWidgetEngine {
 public:

void show_debug_message(string errortext, MESSAGE_TYPE type) override {
  show_debug_message_helperfunc(errortext, type);
}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) override {

}

int show_message(const string &message) override {
  message_cancel = false;
  return show_message_helperfunc(message);
}

int show_message_cancelable(string message) override {
  message_cancel = true;
  return show_message_helperfunc(message);
}

bool show_question(string message) override {
  question_cancel = false;
  return (bool)show_question_helperfunc(message);
}

int show_question_cancelable(string message) override {
  question_cancel = true;
  return show_question_helperfunc(message);
}

int show_attempt(string errortext) override {
  if (error_caption.empty()) error_caption = "Error";
  string str_command;
  string str_title;

  str_command = string("ans=$(zenity ") +
  string("--error --ok-label=Cancel --extra-button=Retry ") +  string("--title=\"") +
  add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
  add_escaping(errortext, false, "") +
  string("\" --icon-name=dialog-error);if [ $? = 0 ] ;then echo -1;else echo 0;fi");

  string str_result = create_shell_dialog(str_command);
  return (int)strtod(str_result.c_str(), nullptr);
}

string get_string(string message, string def) override {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;

  str_title = add_escaping(dialog_caption, true, " ");
  str_command = string("ans=$(zenity ") +
  string("--entry --title=\"") + str_title + string("\" --text=\"") +
  add_escaping(message, false, "") + string("\" --entry-text=\"") +
  add_escaping(def, false, "") + string("\");echo $ans");

  return create_shell_dialog(str_command);
}

string get_password(string message, string def) override {
  if (dialog_caption.empty())
    dialog_caption = window_get_caption();

  string str_command;
  string str_title;

  str_title = add_escaping(dialog_caption, true, " ");
  str_command = string("ans=$(zenity ") +
  string("--entry --title=\"") + str_title + string("\" --text=\"") +
  add_escaping(message, false, "") + string("\" --hide-text --entry-text=\"") +
  add_escaping(def, false, "") + string("\");echo $ans");

  return create_shell_dialog(str_command);
}

double get_integer(string message, double def) override {
  string str_def = remove_trailing_zeros(def);
  string str_result = get_string(message, str_def);
  return strtod(str_result.c_str(), nullptr);
}

double get_passcode(string message, double def) override {
  string str_def = remove_trailing_zeros(def);
  string str_result = get_password(message, str_def);
  return strtod(str_result.c_str(), nullptr);
}

string get_open_filename(string filter, string fname) override {
  return get_open_filename_ext(filter, fname, "", "Open");
}

string get_open_filename_ext(string filter, string fname, string dir, string title) override {
  string str_command; string pwd; string caption_previous = dialog_caption;
  string str_title = add_escaping(title, true, "Open");
  dialog_caption = (str_title == "Open") ? "Open" : title;
  string str_fname = filename_name(filename_absolute(fname));
  string str_dir = filename_absolute(dir);
  string str_path; if (!str_dir.empty()) str_path = str_dir + string("/") + str_fname;
  str_command = string("ans=$(zenity ") +
  string("--file-selection --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_path, false, "") + string("\"") + zenity_filter(filter) + string(");echo $ans");
  string result = create_shell_dialog(str_command);
  dialog_caption = caption_previous;
  return file_exists(result) ? result : "";
}

string get_open_filenames(string filter, string fname) override {
  return get_open_filenames_ext(filter, fname, "", "Open");
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) override {
  string str_command; string pwd; string caption_previous = dialog_caption;
  string str_title = add_escaping(title, true, "Open");
  dialog_caption = (str_title == "Open") ? "Open" : title;
  string str_fname = filename_name(filename_absolute(fname));
  string str_dir = filename_absolute(dir);
  string str_path; if (!str_dir.empty()) str_path = str_dir + string("/") + str_fname;
  str_command = string("zenity ") +
  string("--file-selection --multiple --separator='\n' --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_path, false, "") + string("\"") + zenity_filter(filter);
  string result = create_shell_dialog(str_command);
  dialog_caption = caption_previous;
  std::vector<string> stringVec = split_string(result, '\n');
  bool success = true;
  for (const string &str : stringVec) {
    if (!file_exists(str))
      success = false;
  }
  return success ? result : "";
}

string get_save_filename(string filter, string fname) override {
  return get_save_filename_ext(filter, fname, "", "Save As");
}

string get_save_filename_ext(string filter, string fname, string dir, string title) override {
  string str_command; string pwd; string caption_previous = dialog_caption;
  string str_title = add_escaping(title, true, "Save As");
  dialog_caption = (str_title == "Save As") ? "Save As" : title;
  string str_fname = filename_name(filename_absolute(fname));
  string str_dir = filename_absolute(dir);
  string str_path; if (!str_dir.empty()) str_path = str_dir + string("/") + str_fname;
  str_command = string("ans=$(zenity ") +
  string("--file-selection  --save --confirm-overwrite --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_path, false, "") + string("\"") + zenity_filter(filter) + string(");echo $ans");
  string result = create_shell_dialog(str_command);
  dialog_caption = caption_previous;
  return result;
}

string get_directory(string dname) override {
  return get_directory_alt("Select Directory", dname);
}

string get_directory_alt(string capt, string root) override {
  string str_command; string pwd; string caption_previous = dialog_caption;
  string str_title = add_escaping(capt, true, "Select Directory");
  dialog_caption = (str_title == "Select Directory") ? "Select Directory" : capt;
  string str_dname = root;
  string str_end = ");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";
  str_command = string("ans=$(zenity ") +
  string("--file-selection --directory --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_dname, false, "") + string("\"") + str_end;
  string result = create_shell_dialog(str_command);
  dialog_caption = caption_previous;
  return result;
}

int get_color(int defcol) override {
  return get_color_ext(defcol, "Color");
}

int get_color_ext(int defcol, string title) override {
  string str_command; string caption_previous = dialog_caption;
  string str_title = add_escaping(title, true, "Color");
  dialog_caption = (str_title == "Color") ? "Color" : title;
  string str_defcol;
  string str_result;

  int red; int green; int blue;
  red = color_get_red(defcol);
  green = color_get_green(defcol);
  blue = color_get_blue(defcol);

  str_defcol = string("rgb(") + std::to_string(red) + string(",") +
  std::to_string(green) + string(",") + std::to_string(blue) + string(")");
  str_command = string("ans=$(zenity ") +
  string("--color-selection --show-palette --title=\"") + str_title + string("\" --color='") +
  str_defcol + string("');if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

  str_result = create_shell_dialog(str_command);
  dialog_caption = caption_previous;
  if (str_result == "-1") return strtod(str_result.c_str(), nullptr);
  str_result = string_replace_all(str_result, "rgba(", "");
  str_result = string_replace_all(str_result, "rgb(", "");
  str_result = string_replace_all(str_result, ")", "");
  std::vector<string> stringVec = split_string(str_result, ',');

  unsigned int index = 0;
  for (const string &str : stringVec) {
    if (index == 0) red = strtod(str.c_str(), nullptr);
    if (index == 1) green = strtod(str.c_str(), nullptr);
    if (index == 2) blue = strtod(str.c_str(), nullptr);
    index += 1;
  }

  return make_color_rgb(red, green, blue);
}

string widget_get_caption() override {
  if (dialog_caption.empty()) dialog_caption = window_get_caption();
  if (error_caption.empty()) error_caption = "Error";
  if (dialog_caption == window_get_caption() && error_caption == "Error")
    return ""; else return dialog_caption;
}

void widget_set_caption(string title) override {
  dialog_caption = title; error_caption = title;
  if (dialog_caption.empty()) dialog_caption = window_get_caption();
  if (error_caption.empty()) error_caption = "Error";
}

};  // class ZenityWidgets

// Declares storage for our implementation and exposes it as a system.
static ZenityWidgets zenity_widgets_impl;
namespace enigma {
  CommandLineWidgetEngine *zenity_widgets = &zenity_widgets_impl;
}
