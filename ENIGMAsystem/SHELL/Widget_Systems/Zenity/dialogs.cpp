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
#include <limits.h>
#include <sstream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
using std::string;

static string dialog_caption;
static string error_caption;

static bool message_cancel  = false;
static bool question_cancel = false;

static string add_escaping(string str, bool is_caption, string new_caption) {
  string result; if (is_caption && str == "") result = new_caption;
  result = string_replace_all(str, "\"", "\\\"");

  if (dm_dialogengine == dm_zenity)
    result = string_replace_all(result, "_", "__");

  return result;
}

static string remove_trailing_zeros(double numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

static string zenity_filter(string input) {
  std::replace(input.begin(), input.end(), ';', ' ');
  std::vector<string> stringVec = string_split(input, '|');
  string string_output = "";

  unsigned int index = 0;
  for (const string &str : stringVec) {
    if (index % 2 == 0)
      string_output += " --file-filter='" + str + "|";
    else
      string_output += string_replace_all(str, "*.*", "*") + "'";

    index += 1;
  }

  return string_output;
}

static double show_message_helperfunc(char *str) {
  if (dialog_caption == "")
    dialog_caption = message_caption();

  string str_command;
  string str_title;
  string str_cancel;
  string str_echo = "echo 1";

  if (message_cancel)
    str_echo = "if [ $? = 0 ] ;then echo 1;fi";

  str_title = add_escaping(dialog_caption, true, " ");
  str_cancel = "--info --ok-label=OK ";

  if (message_cancel)
    str_cancel = "--question --ok-label=OK --cancel-label=Cancel ";

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  str_cancel + string("--title=\"") + str_title + string("\" --no-wrap --text=\"") +
  add_escaping(str, false, "") + string("\" --icon-name=dialog-information);") + str_echo;

  string str_result = shellscript_evaluate((char *)str_command.c_str());
  double result = strtod(str_result.c_str(), NULL);

  return result;
}

static double show_question_helperfunc(char *str) {
  if (dialog_caption == "")
    dialog_caption = message_caption();

  string str_command;
  string str_title;
  string str_cancel = "";

  str_title = add_escaping(dialog_caption, true, " ");
  if (question_cancel)
    str_cancel = "--extra-button=Cancel ";

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--question --ok-label=Yes --cancel-label=No ") + str_cancel +  string("--title=\"") +
  str_title + string("\" --no-wrap --text=\"") + add_escaping(str, false, "") +
  string("\" --icon-name=dialog-question);if [ $? = 0 ] ;then echo 1;elif [ $ans = \"Cancel\" ] ;then echo -1;else echo 0;fi");

  string str_result = shellscript_evaluate((char *)str_command.c_str());
  double result = strtod(str_result.c_str(), NULL);

  return result;
}

double show_message(char *str) {
  message_cancel = false;
  return show_message_helperfunc(str);
}

double show_message_cancelable(char *str) {
  message_cancel = true;
  return show_message_helperfunc(str);
}

double show_question(char *str) {
  question_cancel = false;
  return show_question_helperfunc(str);
}

double show_question_cancelable(char *str) {
  question_cancel = true;
  return show_question_helperfunc(str);
}

double show_attempt(char *str) {
  if (error_caption == "") error_caption = "Error";
  string str_command;
  string str_title;

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--question --ok-label=Retry --cancel-label=Cancel ") +  string("--title=\"") +
  add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
  add_escaping(str, false, "") +
  string("\" --icon-name=dialog-error);if [ $? = 0 ] ;then echo 0;else echo -1;fi");

  string str_result = shellscript_evaluate((char *)str_command.c_str());
  double result = strtod(str_result.c_str(), NULL);

  return result;
}

double show_error(char *str, double abort) {
  if (error_caption == "") error_caption = "Error";
  string str_command;
  string str_title;
  string str_echo;

  str_echo = abort ? "echo 1" :
    "if [ $? = 0 ] ;then echo 1;elif [ $ans = \"Ignore\" ] ;then echo -1;elif [ $? = 2 ] ;then echo 0;fi";

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--question --ok-label=Abort --cancel-label=Retry --extra-button=Ignore ") +
  string("--title=\"") + add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
  add_escaping(str, false, "") + string("\" --icon-name=dialog-error);") + str_echo;

  string str_result = shellscript_evaluate((char *)str_command.c_str());
  double result = strtod(str_result.c_str(), NULL);
  if (result == 1) exit(0);

  return result;
}

char *get_string(char *str, char *def) {
  if (dialog_caption == "")
    dialog_caption = message_caption();

  string str_command;
  string str_title;

  str_title = add_escaping(dialog_caption, true, " ");
  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--entry --title=\"") + str_title + string("\" --text=\"") +
  add_escaping(str, false, "") + string("\" --entry-text=\"") +
  add_escaping(def, false, "") + string("\");echo $ans");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_password(char *str, char *def) {
  if (dialog_caption == "")
    dialog_caption = message_caption();

  string str_command;
  string str_title;

  str_title = add_escaping(dialog_caption, true, " ");
  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--entry --title=\"") + str_title + string("\" --text=\"") +
  add_escaping(str, false, "") + string("\" --hide-text --entry-text=\"") +
  add_escaping(def, false, "") + string("\");echo $ans");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

double get_integer(char *str, double def) {
  string str_def = remove_trailing_zeros(def);
  string str_result = get_string(str, (char *)str_def.c_str());
  double result = strtod(str_result.c_str(), NULL);
  return result;
}

double get_passcode(char *str, double def) {
  string str_def = remove_trailing_zeros(def);
  string str_result = get_password(str, (char *)str_def.c_str());
  double result = strtod(str_result.c_str(), NULL);
  return result;
}

char *get_open_filename(char *filter, char *fname) {
  string str_command; string pwd;
  string str_title = "Open";
  string str_fname = basename(fname);

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  if (file_exists(result))
    return (char *)result.c_str();

  return (char *)"";
}

char *get_open_filename_ext(char *filter, char *fname, char *dir, char *title) {
  string str_command; string pwd;
  string str_title = add_escaping(title, true, "Open");
  string str_fname = basename(fname);
  string str_dir = dirname(dir);

  string str_path = fname;
  if (str_dir[0] != '\0') str_path = str_dir + string("/") + str_fname;
  str_fname = (char *)str_path.c_str();

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  if (file_exists(result))
    return (char *)result.c_str();

  return (char *)"";
}

char *get_open_filenames(char *filter, char *fname) {
  string str_command; string pwd;
  string str_title = "Open";
  string str_fname = basename(fname);

  str_command = string("zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection --multiple --separator='\n' --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());
  std::vector<string> stringVec = string_split(result, '\n');

  bool success = true;
  for (const string &str : stringVec) {
    if (!file_exists(str))
      success = false;
  }

  if (success)
    return (char *)result.c_str();

  return (char *)"";
}

char *get_open_filenames_ext(char *filter, char *fname, char *dir, char *title) {
  string str_command; string pwd;
  string str_title = add_escaping(title, true, "Open");
  string str_fname = basename(fname);
  string str_dir = dirname(dir);

  string str_path = fname;
  if (str_dir[0] != '\0') str_path = str_dir + string("/") + str_fname;
  str_fname = (char *)str_path.c_str();

  str_command = string("zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection --multiple --separator='\n' --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());
  std::vector<string> stringVec = string_split(result, '\n');

  bool success = true;
  for (const string &str : stringVec) {
    if (!file_exists(str))
      success = false;
  }

  if (success)
    return (char *)result.c_str();

  return (char *)"";
}

char *get_save_filename(char *filter, char *fname) {
  string str_command; string pwd;
  string str_title = "Save As";
  string str_fname = basename(fname);

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection  --save --confirm-overwrite --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_save_filename_ext(char *filter, char *fname, char *dir, char *title) {
  string str_command; string pwd;
  string str_title = add_escaping(title, true, "Save As");
  string str_fname = basename(fname);
  string str_dir = dirname(dir);

  string str_path = fname;
  if (str_dir[0] != '\0') str_path = str_dir + string("/") + str_fname;
  str_fname = (char *)str_path.c_str();

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection  --save --confirm-overwrite --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_directory(char *dname) {
  string str_command; string pwd;
  string str_title = "Select Directory";
  string str_dname = dname;
  string str_end = "\");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection --directory --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_dname, false, "") + str_end;

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_directory_alt(char *capt, char *root) {
  string str_command; string pwd;
  string str_title = add_escaping(capt, true, "Select Directory");
  string str_dname = root;
  string str_end = "\");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";

  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--file-selection --directory --title=\"") + str_title + string("\" --filename=\"") +
  add_escaping(str_dname, false, "") + str_end;

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

double get_color(double defcol) {
  string str_command;
  string str_title = "Color";
  string str_defcol;
  string str_result;

  int red; int green; int blue;
  red = color_get_red(defcol);
  green = color_get_green(defcol);
  blue = color_get_blue(defcol);

  str_defcol = string("rgb(") + std::to_string(red) + string(",") +
  std::to_string(green) + string(",") + std::to_string(blue) + string(")");
  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--color-selection --show-palette --title=\"") + str_title + string("\"  --color='") +
  str_defcol + string("');if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

  str_result = shellscript_evaluate((char *)str_command.c_str());
  if (str_result == "-1") return strtod(str_result.c_str(), NULL);
  str_result = string_replace_all(str_result, "rgba(", "");
  str_result = string_replace_all(str_result, "rgb(", "");
  str_result = string_replace_all(str_result, ")", "");
  std::vector<string> stringVec = string_split(str_result, ',');

  unsigned int index = 0;
  for (const string &str : stringVec) {
    if (index == 0) red = strtod(str.c_str(), NULL);
    if (index == 1) green = strtod(str.c_str(), NULL);
    if (index == 2) blue = strtod(str.c_str(), NULL);
    index += 1;
  }

  return make_color_rgb(red, green, blue);
}

double get_color_ext(double defcol, char *title) {
  string str_command;
  string str_title = add_escaping(title, true, "Color");
  string str_defcol;
  string str_result;

  int red; int green; int blue;
  red = color_get_red(defcol);
  green = color_get_green(defcol);
  blue = color_get_blue(defcol);

  str_defcol = string("rgb(") + std::to_string(red) + string(",") +
  std::to_string(green) + string(",") + std::to_string(blue) + string(")");
  str_command = string("ans=$(zenity ") +
  string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
  string("--color-selection --show-palette --title=\"") + str_title + string("\" --color='") +
  str_defcol + string("');if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

  str_result = shellscript_evaluate((char *)str_command.c_str());
  if (str_result == "-1") return strtod(str_result.c_str(), NULL);
  str_result = string_replace_all(str_result, "rgba(", "");
  str_result = string_replace_all(str_result, "rgb(", "");
  str_result = string_replace_all(str_result, ")", "");
  std::vector<string> stringVec = string_split(str_result, ',');

  unsigned int index = 0;
  for (const string &str : stringVec) {
    if (index == 0) red = strtod(str.c_str(), NULL);
    if (index == 1) green = strtod(str.c_str(), NULL);
    if (index == 2) blue = strtod(str.c_str(), NULL);
    index += 1;
  }

  return make_color_rgb(red, green, blue);
}

char *message_get_caption() {
  if (dialog_caption == "") dialog_caption = message_caption();
  if (error_caption == "") error_caption = "Error";
  if (dialog_caption == message_caption() && error_caption == "Error")
    return (char *)""; else return (char *)dialog_caption.c_str();
}

char *message_set_caption(char *caption) {
  dialog_caption = caption; error_caption = caption;
  if (dialog_caption == "") dialog_caption = message_caption();
  if (error_caption == "") error_caption = "Error";
  if (dialog_caption == message_caption() && error_caption == "Error")
    return (char *)""; else return (char *)dialog_caption.c_str();
}
