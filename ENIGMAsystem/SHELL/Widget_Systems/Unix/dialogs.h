#include "tinyfiledialogs.h"
#include <string>

using std::string;

//void show_error(string errortext, const bool fatal);

namespace enigma_user
{
	extern string window_get_caption();
	//int show_message(string str);
	double show_question(string str);
	string get_string(string str, string def);
	string get_password(string str, string def);
	double get_integer(string str, double def);
	double get_passcode(string str, double def);
	string get_open_filename(string filter, string fname);
	string get_save_filename(string filter, string fname);
	string get_open_filename_ext(string filter, string fname, string dir, string title);
	string get_save_filename_ext(string filter, string fname, string dir, string title);
	string get_directory(string dname);
	string get_directory_alt(string capt, string root);
	double get_color(double defcol);
}
