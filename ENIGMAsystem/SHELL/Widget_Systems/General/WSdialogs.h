/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
*** Copyright (C) 2019 Samuel Venable
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

#include "Universal_System/var4.h"

#include <string>
using std::string;

namespace enigma_user {

enum {
	text_type,
	button_type,
	input_type
};

// These are constants defined inside one of the win32 headers in mingw, no need to define them 2x
/*
enum {
	ANSI_CHARSET,
	DEFAULT_CHARSET,
	EASTEUROPE_CHARSET,
	RUSSIAN_CHARSET,
	SYMBOL_CHARSET,
	SHIFTJIS_CHARSET,
	HANGEUL_CHARSET,
	GB2312_CHARSET,
	CHINESEBIG5_CHARSET,
	JOHAB_CHARSET,
	HEBREW_CHARSET,
	ARABIC_CHARSET,
	GREEK_CHARSET,
	TURKISH_CHARSET,
	VIETNAMESE_CHARSET,
	THAI_CHARSET,
	MAC_CHARSET,
	BALTIC_CHARSET,
	OEM_CHARSET
};
*/

	static string ws_win32       = "Win32";
	static string ws_cocoa       = "Cocoa";
	static string ws_x11_zenity  = "Zenity";
	static string ws_x11_kdialog = "KDialog";

	static string widget_get_system();
	static void widget_set_system(string sys);
	static int show_message_cancelable(string message);
	static int show_message_ext(string message, string but1, string but2, string but3);
	static bool show_question(string message);
	static int show_question_cancelable(string message);
	static int show_attempt(string errortext);
	static string get_string(string message, string def);
	static string get_password(string message, string def);
	static double get_integer(string message, var def);
	static double get_passcode(string message, var def);
	static string get_open_filename(string filter, string fname);
	static string get_open_filenames(string filter, string fname);
	static string get_save_filename(string filter, string fname);
	static string get_open_filename_ext(string filter, string fname, string dir, string title);
	static string get_open_filenames_ext(string filter, string fname, string dir, string title);
	static string get_save_filename_ext(string filter, string fname, string dir, string title);
	static string get_directory(string dname);
	static string get_directory_alt(string capt, string root);
	static int get_color(int defcol);
	static int get_color_ext(int defcol, string title);
	static string message_get_caption();
	static void message_set_caption(string title);
	
	static inline bool action_if_question(string message)
	{
		return show_question(message);
	}
	
	static string get_login(string username, string password);
	static bool   get_string_canceled();
}
