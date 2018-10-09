#include <tinyfiledialogs/tinyfiledialogs.h>
#include <string>

namespace enigma
{
  int const tfd_OsaScript = 0;
  int const tfd_Zenity    = 1;
  int const tfd_KDialog   = 2;

  int tfd_DialogEngine();
}

//void show_error(std::string errortext, const bool fatal);

namespace enigma_user
{
  //int show_message(std::string str);
  double show_question(std::string str);
  std::string get_string(std::string str, std::string def);
  std::string get_password(std::string str, std::string def);
  double get_integer(std::string str, double def);
  double get_passcode(std::string str, double def);
  std::string get_open_filename(std::string filter, std::string fname);
  std::string get_save_filename(std::string filter, std::string fname);
  std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
  std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
  std::string get_directory(std::string dname);
  std::string get_directory_alt(std::string capt, std::string root);
  double get_color(double defcol);
}
