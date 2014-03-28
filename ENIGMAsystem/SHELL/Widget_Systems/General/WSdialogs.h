/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton
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

    void message_alpha(double alpha);
    void message_background(int back);
    void message_button(int spr);
    void message_button_font(string name, int size, int color, int style);
    void message_caption(bool show, string str);
    void message_input_color(int col);
    void message_input_font(string name, int size, int color, int style);
    void message_mouse_color(int col);
    void message_position(int x, int y);
    void message_size(int w, int h);
    void message_text_font(string name, int size, int color, int style); 
	void message_text_charset(int type, int charset); 
	int show_message(string str);
	int show_message_ext(string str, string but1, string but2, string but3);
	inline int action_show_message(string str)
	{
		return show_message(str);
	}
	bool show_question(string str);
	inline bool action_if_question(string str)
	{
		return show_question(str);
	}
	
	// IMPLEMENTS from widgets_mandatory:
	// void show_error(string errortext, const bool fatal);

	int get_color(int defcol, bool advanced = false);
	
	string get_open_filename(string filter, string fname, string caption="");
	string get_save_filename(string filter, string fname, string caption="");
	string get_directory(string dname, string caption="");
	string get_directory_alt(string message, string root, bool modern=false, string caption="Browse for Folder");

	string get_login(string username, string password, string cap="");
	string get_string(string message, string def, string cap="");
	double get_number(string message, string def, string cap="");
	int    get_integer(string message, string def, string cap="");
	bool   get_string_canceled();

}