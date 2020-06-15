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

	// dialog systems
	static const std::string ws_win32       = "Win32";
	static const std::string ws_cocoa       = "Cocoa";
	static const std::string ws_x11         = "X11";
	static const std::string ws_x11_zenity  = "Zenity";
	static const std::string ws_x11_kdialog = "KDialog";

	// dialog functions
	int show_message_cancelable(std::string message);
	int show_message_ext(std::string message, std::string but1, std::string but2, std::string but3);
	bool show_question(std::string message);
	int show_question_cancelable(std::string message);
	int show_attempt(std::string errortext);
	std::string get_string(std::string message, std::string def);
	std::string get_password(std::string message, std::string def);
	double get_integer(std::string message, var def);
	double get_passcode(std::string message, var def);
	std::string get_open_filename(std::string filter, std::string fname);
	std::string get_open_filenames(std::string filter, std::string fname);
	std::string get_save_filename(std::string filter, std::string fname);
	std::string get_open_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
	std::string get_open_filenames_ext(std::string filter, std::string fname, std::string dir, std::string title);
	std::string get_save_filename_ext(std::string filter, std::string fname, std::string dir, std::string title);
	std::string get_directory(std::string dname);
	std::string get_directory_alt(std::string capt, std::string root);
	int get_color(int defcol);
	int get_color_ext(int defcol, std::string title);
	
	// dialog settings
	std::string widget_get_caption();
	void widget_set_caption(std::string title);
	std::string widget_get_caption();
	void widget_set_caption(std::string str);
	void *widget_get_owner();
	void widget_set_owner(void *hwnd);
	std::string widget_get_icon();
	void widget_set_icon(std::string icon);
	std::string widget_get_system();
	void widget_set_system(std::string sys);
	std::string widget_get_button_name(int type);
	void widget_set_button_name(int type, std::string name);
	
	// dumb stuff
	inline bool action_if_question(std::string message) { return show_question(message); }
	std::string get_login(std::string username, std::string password);
	bool get_string_canceled();
}
