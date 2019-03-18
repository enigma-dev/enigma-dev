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
#include "strings_util.h"
#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <string>

#ifdef DEBUG_MODE
#include "Universal_System/var4.h"
#include "Universal_System/resource_data.h"
#include "Universal_System/object.h"
#include "Universal_System/debugscope.h"
#endif

using enigma::tfd_Zenity;
using enigma::tfd_KDialog;

using enigma::tfd_DialogEngine;

using std::string;

namespace enigma {
  
bool widget_system_initialize() {
  return true;
}
  
} // namespave enigma

namespace {

string msg;
string caption;

} // anonymous namespace

static inline string tfd_add_escaping(string str) {
  string result = string_replace_all(str, "\"", "\\\"");

  if (tfd_DialogEngine() == tfd_Zenity)
    result = string_replace_all(result, "_", "__");

  return result;
}

static inline string message_helper(string str) {
  msg = tfd_add_escaping(msg);
  caption = tfd_add_escaping(caption);
  if (str == "") msg = " "; else msg = str;
  return str;
}

static inline string caption_helper(string capt) {
  if (capt == "" && tfd_DialogEngine() == tfd_Zenity)
    capt = " ";
  else if (capt == "" && tfd_DialogEngine() == tfd_KDialog)
    capt = "KDialog";

  return capt;
}

class FileFilter {
  std::string filter_buf;
  std::vector<const char*> descriptions_;
  std::vector<std::vector<const char*>> patterns_;
  std::vector<const char* const*> cpatterns_;
  std::vector<int> pattern_counts_;

public:
  FileFilter(const std::string &filter): filter_buf(filter + "|") {
    if (!filter.empty()) {
      size_t start = 0;
      std::vector<const char*> *curfilter = nullptr;
      for (size_t i = 0; i < filter_buf.length(); ++i) {
        if (filter_buf[i] == '|') {
          filter_buf[i] = 0;
          if (curfilter) {
            curfilter->push_back(filter_buf.c_str() + start);
            curfilter = nullptr;
          } else {
            descriptions_.push_back(filter_buf.c_str() + start);
            patterns_.push_back({});
            curfilter = &patterns_.back();
          }
          start = i + 1;
        }
        else if (curfilter && filter_buf[i] == ';') {
          filter_buf[i] = 0;
          curfilter->push_back(filter_buf.c_str() + start);
          start = i + 1;
        }
      }
      if (descriptions_.size() > patterns_.size()) {
        descriptions_.pop_back();
      }
    }
    pattern_counts_.reserve(descriptions_.size());
    for (auto &pv : patterns_) {
      pattern_counts_.push_back(pv.size());
      pv.push_back(nullptr);
      cpatterns_.push_back(pv.data());
    }
    cpatterns_.push_back(nullptr);
    descriptions_.push_back(nullptr);
  }

  size_t count() const { return patterns_.size(); }
  const char* const* descriptions() { return descriptions_.data(); }
  const char* const* const* patterns() { return cpatterns_.data(); }
  const int* pattern_counts() { return pattern_counts_.data(); }
};

static inline string remove_trailing_zeros(double numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

static inline string get_open_filename_helper(string filter, string fname, string dir, string title, int const mselect) {
  string str_fname_or_dir = ""; 
  char *cstr_fname = (char *)fname.c_str();

  if (fname != "" && dir != "")
    str_fname_or_dir = dir + string("/") + string(basename(cstr_fname));
  else if (fname != "" && dir == "")
    str_fname_or_dir = string(basename(cstr_fname));
  else if (fname == "" && dir != "")
    str_fname_or_dir = dir;

  string titlebar;
  if (title == "") titlebar = "Open"; else titlebar = title;
  str_fname_or_dir = tfd_add_escaping(str_fname_or_dir);
  titlebar = tfd_add_escaping(titlebar);
  filter = tfd_add_escaping(filter);
  FileFilter ff(filter.c_str());

  const char *path = tinyfd_openFileDialog(titlebar.c_str(), str_fname_or_dir.c_str(),
    ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), mselect, tfd_DialogEngine());

  return path ? : "";
}

static inline int get_color_helper(int defcol, string title) {
  unsigned char rescol[3];

  rescol[0] = defcol & 0xFF;
  rescol[1] = (defcol >> 8) & 0xFF;
  rescol[2] = (defcol >> 16) & 0xFF;

  if (title == "") title = "Color";
  title = tfd_add_escaping(title);
  
  if (tinyfd_colorChooser(title.c_str(), NULL, rescol, rescol, tfd_DialogEngine()) == NULL)
    return -1;

  return (int)((rescol[0] & 0xff) + ((rescol[1] & 0xff) << 8) + ((rescol[2] & 0xff) << 16));
}

void show_error(string errortext, const bool fatal) {
  #ifdef DEBUG_MODE
  errortext = enigma::debug_scope::GetErrors() + "\n\n" + errortext;
  #else
  errortext = "Error in some event or another for some object: \r\n\r\n" + errortext;
  #endif

  if (errortext == "") errortext = " ";
  int input = 0;

  if (!fatal) {
    input = tinyfd_errorMessageBox("Error", errortext.c_str(), 0, tfd_DialogEngine());
    if (input == 2) exit(0);
  } else {
    tinyfd_errorMessageBox("Error", errortext.c_str(), 1, tfd_DialogEngine());
    exit(0);
  }
}

namespace enigma_user {
  
void show_info(string text, int bgcolor, int left, int top, int width, int height,
  bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
  bool pauseGame, string caption) {

}

int show_message(const string &str) {
  caption = window_get_caption();
  msg = tfd_add_escaping(msg);
  caption = tfd_add_escaping(caption);
  if (str == "") msg = " "; else msg = str;
  caption = caption_helper(caption);
  tinyfd_messageBox(caption.c_str(), msg.c_str(), "ok", "info", tfd_DialogEngine());
  return 1;
}

bool show_question(string str) {
  caption = window_get_caption();
  str = message_helper(str);
  caption = caption_helper(caption);
  return tinyfd_messageBox(caption.c_str(), msg.c_str(), "yesno", "question", tfd_DialogEngine());
}

string get_string(string str, string def) {
  caption = window_get_caption();
  str = message_helper(str);
  caption = caption_helper(caption);
  def = tfd_add_escaping(def);
  const char *input = tinyfd_inputBox(caption.c_str(), msg.c_str(), def.c_str(), tfd_DialogEngine());
  return input ? : "";
}

string get_password(string str, string def) {
  caption = window_get_caption();
  str = message_helper(str);
  caption = caption_helper(caption);
  def = tfd_add_escaping(def);
  const char *input = tinyfd_passwordBox(caption.c_str(), msg.c_str(), def.c_str(), tfd_DialogEngine());
  return input ? : "";
}

double get_integer(string str, double def) {
  caption = window_get_caption();
  str = message_helper(str);
  caption = caption_helper(caption);
  string integer = remove_trailing_zeros(def);
  const char *input = tinyfd_inputBox(caption.c_str(), msg.c_str(), integer.c_str(), tfd_DialogEngine());
  return input ? strtod(input, NULL) : 0;
}

double get_passcode(string str, double def) {
  caption = window_get_caption();
  str = message_helper(str);
  caption = caption_helper(caption);
  string integer = remove_trailing_zeros(def);
  const char *input = tinyfd_passwordBox(caption.c_str(), msg.c_str(), integer.c_str(), tfd_DialogEngine());
  return input ? strtod(input, NULL) : 0;
}

string get_open_filename(string filter, string fname) {
  return get_open_filename_helper(filter, fname, "", "", 0);
}

string get_open_filenames(string filter, string fname) {
  return get_open_filename_helper(filter, fname, "", "", 1);
}

string get_save_filename(string filter, string fname) {
  fname = tfd_add_escaping(fname);
  filter = tfd_add_escaping(filter);
  FileFilter ff(filter.c_str());

  const char *path = tinyfd_saveFileDialog("Save As", fname.c_str(),
    ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), tfd_DialogEngine());

  return path ? : "";
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return get_open_filename_helper(filter, fname, dir, title, 0);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return get_open_filename_helper(filter, fname, dir, title, 1);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  string str_fname_or_dir = "";
  char *cstr_fname = (char *)fname.c_str();

  if (fname != "" && dir != "")
    str_fname_or_dir = dir + string("/") + string(basename(cstr_fname));
  else if (fname != "" && dir == "")
    str_fname_or_dir = string(basename(cstr_fname));
  else if (fname == "" && dir != "")
    str_fname_or_dir = dir;

  string titlebar;
  if (title == "") titlebar = "Save As"; else titlebar = title;
  str_fname_or_dir = tfd_add_escaping(str_fname_or_dir);
  titlebar = tfd_add_escaping(titlebar);
  filter = tfd_add_escaping(filter);
  FileFilter ff(filter.c_str());

  const char *path = tinyfd_saveFileDialog(titlebar.c_str(), str_fname_or_dir.c_str(),
    ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), tfd_DialogEngine());

  return path ? : "";
}

string get_directory(string dname) {
  dname = tfd_add_escaping(dname);
  const char *path = tinyfd_selectFolderDialog("Select Directory", dname.c_str(), tfd_DialogEngine());
  return path ? string_replace_all(string(path) + "/", "//", "/") : "";
}

string get_directory_alt(string capt, string root) {
  string titlebar;
  root = tfd_add_escaping(root);
  titlebar = tfd_add_escaping(titlebar);
  if (capt == "") titlebar = "Browse For Folder"; else titlebar = capt;
  const char *path = tinyfd_selectFolderDialog(titlebar.c_str(), root.c_str(), tfd_DialogEngine());
  return path ? string_replace_all(string(path) + "/", "//", "/") : "";
}

int get_color(int defcol) {
  return get_color_helper(defcol, "");
}

int get_color_ext(int defcol, string title) {
  return get_color_helper(defcol, title);
} 

} // namespace enigma_user
