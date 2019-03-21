/******************************************************************************

 MIT License

 Copyright © 2019 Samuel Venable

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

******************************************************************************/

#include "DialogModule.h"
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

#define DIGITS_MAX 999999999999999
#define COL_GET_R(x) ((x & 0x000000FF))
#define COL_GET_G(x) ((x & 0x0000FF00) >> 8)
#define COL_GET_B(x) ((x & 0x00FF0000) >> 16)

static int const dm_zenity  = 0;
static int const dm_kdialog = 1;
static int dm_dialogengine  = dm_zenity;

static string dialog_caption;
static string error_caption;

static bool message_cancel  = false;
static bool question_cancel = false;

static string string_replace_all(string str, string substr, string newstr) {
  size_t pos = 0;
  const size_t sublen = substr.length(), newlen = newstr.length();

  while ((pos = str.find(substr, pos)) != string::npos) {
    str.replace(pos, sublen, newstr);
    pos += newlen;
  }

  return str;
}

static bool file_exists(string fname) {
  struct stat sb;
  return (stat(fname.c_str(), &sb) == 0 &&
    S_ISREG(sb.st_mode) != 0);
}

static char *shellscript_evaluate(char *command) {
  char *buffer = NULL;
  size_t buffer_size = 0;

  string str_command = command;
  string str_buffer = "";

  FILE *file = popen(str_command.c_str(), "r");
  while (getline(&buffer, &buffer_size, file) != -1)
    str_buffer += buffer;

  free(buffer);
  pclose(file);

  if (str_buffer.back() == '\n')
    str_buffer.pop_back();

  static string str_result;
  str_result = str_buffer;

  return (char *)str_result.c_str();
}

static char *message_caption() {
  char buffer[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);

  if (count != -1) {
    buffer[count] = 0;
    return basename(buffer);
  }

  return (char *)"";
}

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

static std::vector<string> string_split(const string &str, char delimiter) {
  std::vector<string> vec;
  std::stringstream sstr(str);
  string tmp;

  while (std::getline(sstr, tmp, delimiter))
    vec.push_back(tmp);

  return vec;
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

static string kdialog_filter(string input) {
  std::replace(input.begin(), input.end(), ';', ' ');
  std::vector<string> stringVec = string_split(input, '|');
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

static int color_get_red(int col) { return COL_GET_R(col); }
static int color_get_green(int col) { return COL_GET_G(col); }
static int color_get_blue(int col) { return COL_GET_B(col); }

static int make_color_rgb(unsigned char r, unsigned char g, unsigned char b) {
  return r | (g << 8) | (b << 16);
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

  if (dm_dialogengine == dm_zenity) {
    str_title = add_escaping(dialog_caption, true, " ");
    str_cancel = "--info --ok-label=OK ";

    if (message_cancel)
      str_cancel = "--question --ok-label=OK --cancel-label=Cancel ";

    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    str_cancel + string("--title=\"") + str_title + string("\" --no-wrap --text=\"") +
    add_escaping(str, false, "") + string("\" --icon-name=dialog-information);") + str_echo;
  }
  else if (dm_dialogengine == dm_kdialog) {
    str_title = add_escaping(dialog_caption, true, "KDialog");
    str_cancel = string("--msgbox \"") + add_escaping(str, false, "") + string("\" ");

    if (message_cancel)
      str_cancel = string("--yesno \"") + add_escaping(str, false, "") + string("\" --yes-label Ok --no-label Cancel ");

    str_command = string("kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") + str_cancel +
    string("--title \"") + str_title + string("\";") + str_echo;
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_title = add_escaping(dialog_caption, true, " ");
    if (question_cancel)
      str_cancel = "--extra-button=Cancel ";

    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--question --ok-label=Yes --cancel-label=No ") + str_cancel +  string("--title=\"") +
    str_title + string("\" --no-wrap --text=\"") + add_escaping(str, false, "") +
    string("\" --icon-name=dialog-question);if [ $? = 0 ] ;then echo 1;elif [ $ans = \"Cancel\" ] ;then echo -1;else echo 0;fi");
  }
  else if (dm_dialogengine == dm_kdialog) {
    str_title = add_escaping(dialog_caption, true, "KDialog");
    if (question_cancel)
      str_cancel = "cancel";

    str_command = string("kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--yesno") + str_cancel + string(" \"") + add_escaping(str, false, "") + string("\" ") +
    string("--yes-label Yes --no-label No ") + string("--title \"") + str_title + string("\";") +
    string("x=$? ;if [ $x = 0 ] ;then echo 1;elif [ $x = 1 ] ;then echo 0;elif [ $x = 2 ] ;then echo -1;fi");
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--question --ok-label=Retry --cancel-label=Cancel ") +  string("--title=\"") +
    add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
    add_escaping(str, false, "") +
    string("\" --icon-name=dialog-error);if [ $? = 0 ] ;then echo 0;else echo -1;fi");
  }
  else if (dm_dialogengine == dm_kdialog) {
    str_command = string("kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--warningyesno") + string(" \"") + add_escaping(str, false, "") + string("\" ") +
    string("--yes-label Retry --no-label Cancel ") + string("--title \"") +
    add_escaping(error_caption, true, "Error") + string("\";") +
    string("x=$? ;if [ $x = 0 ] ;then echo 0;else echo -1;fi");
  }

  string str_result = shellscript_evaluate((char *)str_command.c_str());
  double result = strtod(str_result.c_str(), NULL);

  return result;
}

double show_error(char *str, double abort) {
  if (error_caption == "") error_caption = "Error";
  string str_command;
  string str_title;
  string str_echo;

  if (dm_dialogengine == dm_zenity) {
    str_echo = abort ? "echo 1" :
      "if [ $? = 0 ] ;then echo 1;elif [ $ans = \"Ignore\" ] ;then echo -1;elif [ $? = 2 ] ;then echo 0;fi";

    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--question --ok-label=Abort --cancel-label=Retry --extra-button=Ignore ") +
    string("--title=\"") + add_escaping(error_caption, true, "Error") + string("\" --no-wrap --text=\"") +
    add_escaping(str, false, "") + string("\" --icon-name=dialog-error);") + str_echo;
  }
  else if (dm_dialogengine == dm_kdialog) {
    str_echo = abort ? "echo 1" :
      "x=$? ;if [ $x = 0 ] ;then echo 1;elif [ $x = 1 ] ;then echo 0;elif [ $x = 2 ] ;then echo -1;fi";

    str_command = string("kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--warningyesnocancel \"") + add_escaping(str, false, "") + string("\" ") +
    string("--yes-label Abort --no-label Retry --cancel-label Ignore ") +
    string("--title \"") + add_escaping(error_caption, true, "Error") + string("\";") + str_echo;
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_title = add_escaping(dialog_caption, true, " ");
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--entry --title=\"") + str_title + string("\" --text=\"") +
    add_escaping(str, false, "") + string("\" --entry-text=\"") +
    add_escaping(def, false, "") + string("\");echo $ans");
  }
  else if (dm_dialogengine == dm_kdialog) {
    str_title = add_escaping(dialog_caption, true, "KDialog");
    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--inputbox \"") + add_escaping(str, false, "") + string("\" \"") +
    add_escaping(def, false, "") + string("\" --title \"") +
    str_title + string("\");echo $ans");
  }

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_password(char *str, char *def) {
  if (dialog_caption == "")
    dialog_caption = message_caption();

  string str_command;
  string str_title;

  if (dm_dialogengine == dm_zenity) {
    str_title = add_escaping(dialog_caption, true, " ");
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--entry --title=\"") + str_title + string("\" --text=\"") +
    add_escaping(str, false, "") + string("\" --hide-text --entry-text=\"") +
    add_escaping(def, false, "") + string("\");echo $ans");
  }
  else if (dm_dialogengine == dm_kdialog) {
    str_title = add_escaping(dialog_caption, true, "KDialog");
    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--password \"") + add_escaping(str, false, "") + string("\" \"") +
    add_escaping(def, false, "") + string("\" --title \"") +
    str_title + string("\");echo $ans");
  }

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

double get_integer(char *str, double def) {
  if (def < -DIGITS_MAX) def = -DIGITS_MAX;
  if (def > DIGITS_MAX) def = DIGITS_MAX;

  string str_def = remove_trailing_zeros(def);
  string str_result = get_string(str, (char *)str_def.c_str());
  double result = strtod(str_result.c_str(), NULL);

  if (result < -DIGITS_MAX) result = -DIGITS_MAX;
  if (result > DIGITS_MAX) result = DIGITS_MAX;

  return result;
}

double get_passcode(char *str, double def) {
  if (def < -DIGITS_MAX) def = -DIGITS_MAX;
  if (def > DIGITS_MAX) def = DIGITS_MAX;

  string str_def = remove_trailing_zeros(def);
  string str_result = get_password(str, (char *)str_def.c_str());
  double result = strtod(str_result.c_str(), NULL);

  if (result < -DIGITS_MAX) result = -DIGITS_MAX;
  if (result > DIGITS_MAX) result = DIGITS_MAX;

  return result;
}

char *get_open_filename(char *filter, char *fname) {
  string str_command; string pwd;
  string str_title = "Open";
  string str_fname = basename(fname);

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_fname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
    string(" --title \"") + str_title + string("\"") + string(");echo $ans");
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_fname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
    string(" --title \"") + str_title + string("\"") + string(");echo $ans");
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_command = string("zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection --multiple --separator='\n' --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "");
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_fname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
    string(" --multiple --separate-output --title \"") + str_title + string("\"");
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_command = string("zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection --multiple --separator='\n' --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "");
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_fname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getopenfilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
    string(" --multiple --separate-output --title \"") + str_title + string("\"");
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection  --save --confirm-overwrite --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_fname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getsavefilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
    string(" --title \"") + str_title + string("\"") + string(");echo $ans");
  }

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

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection  --save --confirm-overwrite --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_fname, false, "") + string("\"") + add_escaping(zenity_filter(filter), false, "") + string(");echo $ans");
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_fname.c_str() && str_fname[0] != '/' && str_fname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_fname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getsavefilename /") + pwd + add_escaping(kdialog_filter(filter), false, "") +
    string(" --title \"") + str_title + string("\"") + string(");echo $ans");
  }

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_directory(char *dname) {
  string str_command; string pwd;
  string str_title = "Select Directory";
  string str_dname = dname;
  string str_end = "\");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection --directory --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_dname, false, "") + str_end;
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_dname.c_str() && str_dname[0] != '/' && str_dname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_dname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getexistingdirectory /") + pwd + string(" --title \"") + str_title + str_end;
  }

  static string result;
  result = shellscript_evaluate((char *)str_command.c_str());

  return (char *)result.c_str();
}

char *get_directory_alt(char *capt, char *root) {
  string str_command; string pwd;
  string str_title = add_escaping(capt, true, "Select Directory");
  string str_dname = root;
  string str_end = "\");if [ $ans = / ] ;then echo $ans;elif [ $? = 1 ] ;then echo $ans/;else echo $ans;fi";

  if (dm_dialogengine == dm_zenity) {
    str_command = string("ans=$(zenity ") +
    string("--attach=$(sleep .01;xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--file-selection --directory --title=\"") + str_title + string("\" --filename=\"") +
    add_escaping(str_dname, false, "") + str_end;
  }
  else if (dm_dialogengine == dm_kdialog) {
    pwd = ""; if (str_dname.c_str() && str_dname[0] != '/' && str_dname.length()) pwd = string("\"$PWD/\"") +
      string("\"") + add_escaping(str_dname, false, "") + string("\""); else pwd = "\"$PWD/\"";

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getexistingdirectory /") + pwd + string(" --title \"") + str_title + str_end;
  }

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

  if (dm_dialogengine == dm_zenity) {
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

  } else if (dm_dialogengine == dm_kdialog) {
    char hexcol[16];
    snprintf(hexcol, sizeof(hexcol), "%02x%02x%02x", red, green, blue);

    str_defcol = string("#") + string(hexcol);
    std::transform(str_defcol.begin(), str_defcol.end(), str_defcol.begin(), ::toupper);

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getcolor --default '") + str_defcol + string("' --title \"") + str_title +
    string("\");if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

    str_result = shellscript_evaluate((char *)str_command.c_str());
    if (str_result == "-1") return strtod(str_result.c_str(), NULL);
    str_result = str_result.substr(1, str_result.length() - 1);

    unsigned int color;
    std::stringstream ss2;
    ss2 << std::hex << str_result;
    ss2 >> color;

    blue = color_get_red(color);
    green = color_get_green(color);
    red = color_get_blue(color);
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

  if (dm_dialogengine == dm_zenity) {
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

  } else if (dm_dialogengine == dm_kdialog) {
    char hexcol[16];
    snprintf(hexcol, sizeof(hexcol), "%02x%02x%02x", red, green, blue);

    str_defcol = string("#") + string(hexcol);
    std::transform(str_defcol.begin(), str_defcol.end(), str_defcol.begin(), ::toupper);

    str_command = string("ans=$(kdialog ") +
    string("--attach=$(xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW | cut -f 2) ") +
    string("--getcolor --default '") + str_defcol + string("' --title \"") + str_title +
    string("\");if [ $? = 0 ] ;then echo $ans;else echo -1;fi");

    str_result = shellscript_evaluate((char *)str_command.c_str());
    if (str_result == "-1") return strtod(str_result.c_str(), NULL);
    str_result = str_result.substr(1, str_result.length() - 1);

    unsigned int color;
    std::stringstream ss2;
    ss2 << std::hex << str_result;
    ss2 >> color;

    blue = color_get_red(color);
    green = color_get_green(color);
    red = color_get_blue(color);
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

char *widget_get_system() {
  if (dm_dialogengine == dm_zenity)
    return (char *)"Zenity";

  if (dm_dialogengine == dm_kdialog)
    return (char *)"KDialog";

  return (char *)"Zenity";
}

char *widget_set_system(char *sys) {
  string str_system = sys;

  if (str_system == "Zenity") {
    dm_dialogengine = dm_zenity;
    return (char *)"Zenity";
  }

  if (str_system == "KDialog") {
    dm_dialogengine = dm_kdialog;
    return (char *)"KDialog";
  }

  return (char *)"Zenity";
}
