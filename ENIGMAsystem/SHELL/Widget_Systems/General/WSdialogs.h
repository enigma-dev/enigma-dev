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

#include <string>
using std::string;

#include "Platforms/General/PFwindow.h"
#include "Universal_System/estring.h"
#include "Platforms/SDL/Window.h"
#include "OpenGLHeaders.h"

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

	int show_message(const string &message);
	bool show_question(string message);
	
	#ifdef _WIN32
	#define DLLEXT ".dll"
	#elif (defined(__APPLE__) && defined(__MACH__))
	#define DLLEXT ".dylib"
	#else
	#define DLLEXT ".so"
	#endif

	inline string get_open_filename(string filter, string fname) { 
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_open_filename_ext(string filter, string fname, string title, string dir) { 
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, title, dir).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_open_filenames(string filter, string fname) {
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filenames", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_open_filenames_ext(string filter, string fname, string title, string dir) { 
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filenames_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, title, dir).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_save_filename(string filter, string fname) { 
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_save_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_save_filename_ext(string filter, string fname, string title, string dir) { 
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_save_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, title, dir).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_directory(string dname) { 
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_directory", dll_cdecl, ty_string, 1, ty_string), dname).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	inline string get_directory_alt(string capt, string root) {
	  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_directory_alt", dll_cdecl, ty_string, 2, ty_string, ty_string), capt, root).to_string();
          SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
          glClearColor(0, 0, 0, 1);
          glClear(GL_COLOR_BUFFER_BIT);
          SDL_GL_SwapWindow(enigma::windowHandle);
          return result;
	}
	
	int get_color(int defcol);
	int get_color_ext(int defcol, string title);
	string widget_get_caption();
	void widget_set_caption(string title);

	inline string message_get_caption() { return widget_get_caption(); }
	inline void message_set_caption(string title) { widget_set_caption(title); }
	
	inline bool action_if_question(string message)
	{
		return show_question(message);
	}
	
	string get_login(string username, string password);
	bool   get_string_canceled();
}
