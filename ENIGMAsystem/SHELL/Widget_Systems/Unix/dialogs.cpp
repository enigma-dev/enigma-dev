#include "dialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "tinyfiledialogs.h"
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>

using std::string;

namespace enigma
{
    bool widget_system_initialize()
    {
        return true;
    }
}

bool is_number(string input)
{
    if (input[0] == '0' || input[0] == '1' || input[0] == '2' || input[0] == '3' || input[0] == '4' ||
        input[0] == '5' || input[0] == '6' || input[0] == '7' || input[0] == '8' || input[0] == '9')
        return true;

    if (input[0] == '+' &&
        (input[1] == '0' || input[1] == '1' || input[1] == '2' || input[1] == '3' || input[1] == '4' ||
        input[1] == '5' || input[1] == '6' || input[1] == '7' || input[1] == '8' || input[1] == '9'))
        return true;

    if (input[0] == '-' &&
        (input[1] == '0' || input[1] == '1' || input[1] == '2' || input[1] == '3' || input[1] == '4' ||
        input[1] == '5' || input[1] == '6' || input[1] == '7' || input[1] == '8' || input[1] == '9'))
        return true;

    return false;
}

class FileFilter {
  string filter_buf;
  std::vector<const char*> descriptions_;
  std::vector<std::vector<const char*>> patterns_;
  std::vector<const char* const*> cpatterns_;
  std::vector<int> pattern_counts_;

 public:
  FileFilter(const string &filter): filter_buf(filter + "|") {
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
        } else if (curfilter && filter_buf[i] == ';') {
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

void show_error(string errortext, const bool fatal)
{
    string msg;

    if (errortext == "")
        msg = " ";
    else
        msg = errortext;

    if (msg != " ")
        msg = msg + "\n\n";

    replace(msg.begin(), msg.end(), '"', '\'');

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
    char result[PATH_MAX];

    void show_info(string text, int bgcolor, int left, int top, int width, int height,
	bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
	bool pauseGame, string caption)
	{

	}

    int show_message(string str)
    {
        string caption = window_get_caption();

        if (caption == "")
            caption = " ";

        string msg;

        if (str == "")
            msg = " ";
        else
            msg = str;

        replace(msg.begin(), msg.end(), '"', '\'');
        replace(caption.begin(), caption.end(), '"', '\'');

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

        replace(msg.begin(), msg.end(), '"', '\'');
        replace(caption.begin(), caption.end(), '"', '\'');

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

        replace(msg.begin(), msg.end(), '"', '\'');
        replace(def.begin(), def.end(), '"', '\'');
        replace(caption.begin(), caption.end(), '"', '\'');

        const char *input = tinyfd_inputBox(caption.c_str(), msg.c_str(), def.c_str());

        if (input == NULL)
            input = "";

        strncpy(result, input, PATH_MAX);

        return result;
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

        replace(msg.begin(), msg.end(), '"', '\'');
        replace(def.begin(), def.end(), '"', '\'');
        replace(caption.begin(), caption.end(), '"', '\'');

        const char *input = tinyfd_passwordBox(caption.c_str(), msg.c_str(), def.c_str());

        if (input == NULL)
            input = "";

        strncpy(result, input, PATH_MAX);

        return result;
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

        replace(msg.begin(), msg.end(), '"', '\'');
        replace(caption.begin(), caption.end(), '"', '\'');

        const char *input = tinyfd_inputBox(caption.c_str(), msg.c_str(), integer.c_str());

        if (input == NULL)
            input = "";

        if (is_number(input) == false)
            input = "0";

        strncpy(result, input, PATH_MAX);

        std::istringstream text(result);
        double res_integer;
        text >> res_integer;

        return res_integer;
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

        replace(msg.begin(), msg.end(), '"', '\'');
        replace(caption.begin(), caption.end(), '"', '\'');

        const char *input = tinyfd_passwordBox(caption.c_str(), msg.c_str(), integer.c_str());

        if (input == NULL)
            input = "";

        if (is_number(input) == false)
            input = "0";

        strncpy(result, input, PATH_MAX);

        std::istringstream text(result);
        double res_integer;
        text >> res_integer;

        return res_integer;
    }

    string get_open_filename(string filter, string fname)
    {
        replace(fname.begin(), fname.end(), '"', '\'');
        replace(filter.begin(), filter.end(), '"', '\'');
        FileFilter ff(filter.c_str());

        const char *path = tinyfd_openFileDialog("Open", fname.c_str(),
            ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), 0);

        if (path == NULL)
            path = "";

        strncpy(result, path, PATH_MAX);

        return result;
    }

    string get_open_filename_ext(string filter, string fname, string dir, string title)
    {
        const char *fname_or_dir;

        replace(fname.begin(), fname.end(), '"', '\'');
        replace(dir.begin(), dir.end(), '"', '\'');

        if(access(fname.c_str(), F_OK) != -1)
            fname_or_dir = fname.c_str();
        else
            fname_or_dir = dir.c_str();

        struct stat sb;

        if ((stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) == 0)
            fname_or_dir = "";

        string titlebar;

        if (title == "")
            titlebar = "Open";
        else
            titlebar = title;

        replace(fname.begin(), fname.end(), '"', '\'');
        replace(filter.begin(), filter.end(), '"', '\'');
        replace(titlebar.begin(), titlebar.end(), '"', '\'');
        FileFilter ff(filter.c_str());

        const char *path = tinyfd_openFileDialog(titlebar.c_str(), fname_or_dir,
            ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str(), 0);

        if (path == NULL)
            path = "";

        strncpy(result, path, PATH_MAX);

        return result;
    }

    string get_save_filename(string filter, string fname)
    {
        replace(fname.begin(), fname.end(), '"', '\'');
        replace(filter.begin(), filter.end(), '"', '\'');
        FileFilter ff(filter.c_str());

        const char *path = tinyfd_saveFileDialog("Save As", fname.c_str(),
            ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str());

        if (path == NULL)
            path = "";

        strncpy(result, path, PATH_MAX);

        return result;
    }

    string get_save_filename_ext(string filter, string fname, string dir, string title)
    {
        const char *fname_or_dir;

        replace(fname.begin(), fname.end(), '"', '\'');
        replace(dir.begin(), dir.end(), '"', '\'');

        if(access(fname.c_str(), F_OK) != -1)
            fname_or_dir = fname.c_str();
        else
            fname_or_dir = dir.c_str();

        struct stat sb;

        if ((stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) == 0)
            fname_or_dir = "";

        string titlebar;

        if (title == "")
            titlebar = "Save As";
        else
            titlebar = title;

        replace(fname.begin(), fname.end(), '"', '\'');
        replace(filter.begin(), filter.end(), '"', '\'');
        replace(titlebar.begin(), titlebar.end(), '"', '\'');
        FileFilter ff(filter.c_str());

        const char *path = tinyfd_saveFileDialog(titlebar.c_str(), fname_or_dir,
            ff.count() ? *ff.pattern_counts() : 0, *ff.patterns(), (char *)filter.c_str());

        if (path == NULL)
            path = "";

        strncpy(result, path, PATH_MAX);

        return result;
    }

    string get_directory(string dname)
    {
        replace(dname.begin(), dname.end(), '"', '\'');

        const char *path = tinyfd_selectFolderDialog("Select Directory", dname.c_str());

        if (path == NULL)
            path = "";

        strncpy(result, path, PATH_MAX);

        return result;
    }

    string get_directory_alt(string capt, string root)
    {
        string titlebar;

        if (capt == "")
            titlebar = "Browse For Folder";
        else
            titlebar = capt;

        replace(root.begin(), root.end(), '"', '\'');
        replace(titlebar.begin(), titlebar.end(), '"', '\'');

        const char *path = tinyfd_selectFolderDialog(titlebar.c_str(), root.c_str());

        if (path == NULL)
            path = "";

        strncpy(result, path, PATH_MAX);

        return result;
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
