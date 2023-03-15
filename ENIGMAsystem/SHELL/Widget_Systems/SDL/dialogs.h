/*

 MIT License

 Copyright © 2021-2022 Samuel Venable

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

*/

#ifndef ENIGMA_WIDGETS_MANDATORY_H
#define ENIGMA_WIDGETS_MANDATORY_H

#include "libEGMstd.h"

#include <string>

#if !defined(HIDE_DEBUG_MESSAGES)
#define DEBUG_MESSAGE(msg, severity) ::enigma_user::show_debug_message((std::string) (msg) + " | " __FILE__ ":" + std::to_string(__LINE__), (severity))
#else
#define DEBUG_MESSAGE(msg, severity) ::enigma_user::lel(msg, severity)
#endif

enum MESSAGE_TYPE : int {
  /// Diagnostic information not indicative of a problem.
  M_INFO = 0,
  /// A mostly-harmless problem where something has misbehaved,
  /// but the misbehavior is temporary or unlikely to lead to harm.
  M_WARNING = 1,
  /// A recoverable error in library code that may be caused by bad state
  /// in the engine or the operating system. Execution can continue, but
  /// an operation has failed permanently.
  M_ERROR = 2,
  /// A recoverable error caused by misuse of the API, such as closing
  /// or using something that was never opened, or attempting to access
  /// a resource that was deleted or never existed.
  M_USER_ERROR = 3,
  /// A non-recoverable error caused by library code. Perhaps the window
  /// or graphics failed to initialize, or the system is completely out of
  /// memory and execution cannot continue safely.
  M_FATAL_ERROR = 4,
  /// A non-recoverable error caused by misuse of the API.
  /// Generally, this is thrown by the user themselves, but an operation
  /// that renders the game unable to continue, such as deleting all
  /// resources, would be grounds for this class of error. 
  M_FATAL_USER_ERROR = 5
}; 

namespace enigma {
  
  inline std::string error_type(MESSAGE_TYPE t) {
    switch(t) {
      case M_INFO: return "INFO";
      case M_WARNING: return "WARNING";
      case M_ERROR: return "ERROR";
      case M_USER_ERROR: return "USER_ERROR";
      case M_FATAL_ERROR: return "FATAL_ERROR";
      case M_FATAL_USER_ERROR: return "FATAL_USER_ERROR";
      default: return "ERROR";
    }
  }
  
  // This function is called at the beginning of the game to allow the widget system to load.
  bool widget_system_initialize();
  extern std::string gameInfoText, gameInfoCaption;
  extern int gameInfoBackgroundColor, gameInfoLeft, gameInfoTop, gameInfoWidth, gameInfoHeight;
  extern bool gameInfoEmbedGameWindow, gameInfoShowBorder, gameInfoAllowResize, gameInfoStayOnTop, gameInfoPauseGame;
}

namespace enigma_user {

bool show_question(std::string str);

void show_debug_message(std::string msg, MESSAGE_TYPE type = M_INFO);

// This obviously displays an error message.
// It should offer a button to end the game, and if not fatal, a button to ignore the error.
inline void show_error(std::string msg, const bool fatal) {
   show_debug_message(msg, (fatal) ? M_FATAL_USER_ERROR : M_USER_ERROR);
}

void show_error(string errortext, const bool fatal);

#if defined(HIDE_DEBUG_MESSAGES)
inline void lel(std::string msg, MESSAGE_TYPE severity) { }
#endif

int show_message(const std::string &msg);
template<typename T> int show_message(T msg) { return show_message(enigma_user::toString(msg)); }
inline int action_show_message(string msg) {
  return show_message(msg);
}
void show_info(string text=enigma::gameInfoText, int bgcolor=enigma::gameInfoBackgroundColor, int left=enigma::gameInfoLeft, int top=enigma::gameInfoTop, int width=enigma::gameInfoWidth, int height=enigma::gameInfoHeight,
	bool embedGameWindow=enigma::gameInfoEmbedGameWindow, bool showBorder=enigma::gameInfoShowBorder, bool allowResize=enigma::gameInfoAllowResize, bool stayOnTop=enigma::gameInfoStayOnTop,
	bool pauseGame=enigma::gameInfoPauseGame, string caption=enigma::gameInfoCaption);
inline void action_show_info() { show_info(); }

}

int show_question_ext(std::string message);
std::string get_open_filename(std::string filter, std::string fname);
std::string get_open_filename_ext(std::string filter, std::string fname, std::string title, std::string dir);
std::string get_open_filenames(std::string filter, std::string fname);
std::string get_open_filenames_ext(std::string filter, std::string fname, std::string title, std::string dir);
std::string get_save_filename(std::string filter, std::string fname);
std::string get_save_filename_ext(std::string filter, std::string fname, std::string title, std::string dir);
std::string get_directory(std::string dname);
std::string get_directory_alt(std::string capt, std::string root);
inline bool action_if_question(std::string message) {
  return show_question(message);
}

} // namespace enigma_user

#endif
