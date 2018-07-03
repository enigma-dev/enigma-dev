#include "dialogs.h"
#include "Universal_System/estring.h"
#include "Platforms/General/PFwindow.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <string>

using std::string;

using enigma_user::string_replace_all;

namespace enigma
{
  bool widget_system_initialize()
  {
    return true;
  }
}

class FileFilter
{
  std::string filter_buf;
  std::vector<const char*> descriptions_;
  std::vector<std::vector<const char*>> patterns_;
  std::vector<const char* const*> cpatterns_;
  std::vector<int> pattern_counts_;
  
  public:
    FileFilter(const std::string &filter): filter_buf(filter + "|") {
    if (!filter.empty())
    {
      size_t start = 0;
      std::vector<const char*> *curfilter = nullptr;
      for (size_t i = 0; i < filter_buf.length(); ++i)
      {
        if (filter_buf[i] == '|')
        {
          filter_buf[i] = 0;
          if (curfilter)
          {
            curfilter->push_back(filter_buf.c_str() + start);
            curfilter = nullptr;
          }
          else
          {
            descriptions_.push_back(filter_buf.c_str() + start);
            patterns_.push_back({});
            curfilter = &patterns_.back();
          }
          start = i + 1;
        }
        else if (curfilter && filter_buf[i] == ';')
        {
          filter_buf[i] = 0;
          curfilter->push_back(filter_buf.c_str() + start);
          start = i + 1;
        }
      }
      if (descriptions_.size() > patterns_.size())
      {
        descriptions_.pop_back();
      }
    }
    pattern_counts_.reserve(descriptions_.size());
    for (auto &pv : patterns_)
    {
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

void show_error(string errortext, const bool fatal)
{
  string msg;

  if (errortext == "")
    msg = " ";
  else
    msg = errortext;

  if (msg != " ")
    msg = msg + "\n\n";

  msg = string_replace_all(msg, "\"", "\\\"");

  if (fatal == 0)
  {
    msg = msg + "Do you want to abort the application?";

    double input = tinyfd_messageBox("Error", msg.c_str(), "yesno", "error", 1);

    if (input == 1)
      exit(0);
  }
  else
  {
    msg = msg + "Click 'OK' to abort the application.";

    tinyfd_messageBox("Error", msg.c_str(), "ok", "error", 1);

    exit(0);
  }
}

namespace enigma_user
{
  void show_info(string text, int bgcolor, int left, int top, int width, int height,
    bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
    bool pauseGame, string caption)
  {

  }

  int show_message(const string &str)
  {
    string caption = window_get_caption();

    if (caption == "")
      caption = " ";

    string msg;

    if (str == "")
      msg = " ";
    else
      msg = str;

    msg = string_replace_all(msg, "\"", "\\\"");
    caption = string_replace_all(caption, "\"", "\\\"");

    tinyfd_messageBox(caption.c_str(), msg.c_str(), "ok", "info", 1);

    return 1;
  }

  double show_question(string str)
  {
    string caption = window_get_caption();

    if (caption == "")
      caption = " ";

      string msg;

    if (str == "")
      msg = " ";
    else
      msg = str;

    msg = string_replace_all(msg, "\"", "\\\"");
    caption = string_replace_all(caption, "\"", "\\\"");

    return tinyfd_messageBox(caption.c_str(), msg.c_str(), "yesno", "question", 1);
  }

  string get_string(string str, string def)
  {
    string caption = window_get_caption();

    if (caption == "")
      caption = " ";

    string msg;

    if (str == "")
      msg = " ";
    else
      msg = str;

    msg = string_replace_all(msg, "\"", "\\\"");
    def = string_replace_all(def, "\"", "\\\"");
    caption = string_replace_all(caption, "\"", "\\\"");

    const char *input = tinyfd_inputBox(caption.c_str(), msg.c_str(), def.c_str());

    return input ? : "";
  }

  string get_password(string str, string def)
  {
    string caption = window_get_caption();

    if (caption == "")
      caption = " ";

    string msg;

    if (str == "")
      msg = " ";
    else
      msg = str;

    msg = string_replace_all(msg, "\"", "\\\"");
    def = string_replace_all(def, "\"", "\\\"");
    caption = string_replace_all(caption, "\"", "\\\"");

    const char *input = tinyfd_passwordBox(caption.c_str(), msg.c_str(), def.c_str());

    return input ? : "";
  }

  double get_integer(string str, double def)
  {
    string caption = window_get_caption();

    if (caption == "")
      caption = " ";

    std::ostringstream def_integer;
    def_integer << def;
    string integer = def_integer.str();

    string msg;

    if (str == "")
      msg = " ";
    else
      msg = str;

    msg = string_replace_all(msg, "\"", "\\\"");
    caption = string_replace_all(caption, "\"", "\\\"");

    const char *input = tinyfd_inputBox(caption.c_str(), msg.c_str(), integer.c_str());

    return input ? strtod(input, NULL) : 0;
  }

  double get_passcode(string str, double def)
  {
    string caption = window_get_caption();

    if (caption == "")
      caption = " ";

    std::ostringstream def_integer;
    def_integer << def;
    string integer = def_integer.str();

    string msg;

    if (str == "")
      msg = " ";
    else
      msg = str;

    msg = string_replace_all(msg, "\"", "\\\"");
    caption = string_replace_all(caption, "\"", "\\\"");

    const char *input = tinyfd_passwordBox(caption.c_str(), msg.c_str(), integer.c_str());

    return input ? strtod(input, NULL) : 0;
  }

  string get_open_filename(string filter, string fname)
  {
    fname = string_replace_all(fname, "\"", "\\\"");
    filter = string_replace_all(filter, "\"", "\\\"");
    FileFilter ff(filter.c_str());

    const char *path = tinyfd_openFileDialog("Open", fname.c_str(),
      ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), 0);

    return path ? : "";
  }

  string get_save_filename(string filter, string fname)
  {
    fname = string_replace_all(fname, "\"", "\\\"");
    filter = string_replace_all(filter, "\"", "\\\"");
    FileFilter ff(filter.c_str());

    const char *path = tinyfd_saveFileDialog("Save As", fname.c_str(),
      ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str());

    return path ? : "";
  }
  
  string get_open_filename_ext(string filter, string fname, string dir, string title)
  {
    string fname_or_dir;

    if (access(fname.c_str(), F_OK) != -1)
      fname_or_dir = fname;
    else
      fname_or_dir = dir;

    struct stat sb;

    if ((stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) == 0)
      fname_or_dir = "";

    string titlebar;

    if (title == "")
      titlebar = "Open";
    else
      titlebar = title;

    fname_or_dir = string_replace_all(fname_or_dir, "\"", "\\\"");
    titlebar = string_replace_all(titlebar, "\"", "\\\"");
    filter = string_replace_all(filter, "\"", "\\\"");
    FileFilter ff(filter.c_str());

    const char *path = tinyfd_openFileDialog(titlebar.c_str(), fname_or_dir.c_str(),
      ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), 0);

    return path ? : "";
  }

  string get_save_filename_ext(string filter, string fname, string dir, string title)
  {
    string fname_or_dir;

    if (access(fname.c_str(), F_OK) != -1)
      fname_or_dir = fname;
    else
      fname_or_dir = dir;

    struct stat sb;

    if ((stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) == 0)
      fname_or_dir = "";

    string titlebar;

    if (title == "")
      titlebar = "Save As";
    else
      titlebar = title;

    fname_or_dir = string_replace_all(fname_or_dir, "\"", "\\\"");
    titlebar = string_replace_all(titlebar, "\"", "\\\"");
    filter = string_replace_all(filter, "\"", "\\\"");
    FileFilter ff(filter.c_str());

    const char *path = tinyfd_saveFileDialog(titlebar.c_str(), fname_or_dir.c_str(),
      ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str());

    return path ? : "";
  }

  string get_directory(string dname)
  {
    dname = string_replace_all(dname, "\"", "\\\"");

    const char *path = tinyfd_selectFolderDialog("Select Directory", dname.c_str());

    return path ? string_replace_all(string(path) + "/", "//", "/") : "";
  }

  string get_directory_alt(string capt, string root)
  {
    string titlebar;

    if (capt == "")
      titlebar = "Browse For Folder";
    else
      titlebar = capt;

    root = string_replace_all(root, "\"", "\\\"");
    titlebar = string_replace_all(titlebar, "\"", "\\\"");

    const char *path = tinyfd_selectFolderDialog(titlebar.c_str(), root.c_str());

    return path ? string_replace_all(string(path) + "/", "//", "/") : "";
  }

  double get_color(double defcol)
  {
    unsigned char rescol[3];

    rescol[0] = (int)defcol & 0xFF;
    rescol[1] = ((int)defcol >> 8) & 0xFF;
    rescol[2] = ((int)defcol >> 16) & 0xFF;

    if (tinyfd_colorChooser("Color", NULL, rescol, rescol) == NULL)
      return -1;

    return (int)((rescol[0] & 0xff) + ((rescol[1] & 0xff) << 8) + ((rescol[2] & 0xff) << 16));
  }
}
