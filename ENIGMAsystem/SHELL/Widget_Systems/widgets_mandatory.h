/** Copyright (C) 2008-2017 Josh Ventura
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

#ifndef ENIGMA_WIDGETS_MANDATORY_H
#define ENIGMA_WIDGETS_MANDATORY_H

#include "libEGMstd.h"

#include <string>

#ifndef DO_NOT_ABORT_ERRORS
#define DO_NOT_ABORT_ERRORS
#endif

#ifndef SHOW_DEBUG_MESSAGES
#define SHOW_DEBUG_MESSAGES
#endif

#ifndef SHOW_DEBUG_MESSAGES
#define DEBUG_MESSAGE(msg, severity) \
::enigma::show_debug_message_hidden(msg, severity)
#else
#define DEBUG_MESSAGE(msg, severity) \
::enigma_user::show_debug_message((std::string) (msg) + " | " __FILE__ ":" + std::to_string(__LINE__), (severity))
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

  inline void show_debug_message_hidden(std::string msg, MESSAGE_TYPE type = M_INFO) {
    #ifndef DO_NOT_ABORT_ERRORS
    if (severity == M_ERROR || severity == M_USER_ERROR ||
      severity == M_FATAL_ERROR || severity == M_FATAL_USER_ERROR) {
      exit(1);
    }
    #endif
    if (type == M_FATAL_USER_ERROR || type == M_USER_ERROR) {
      abort();
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

void debug_output_set_enabled(bool enabled, MESSAGE_TYPE type);
bool debug_output_get_enabled(MESSAGE_TYPE type);

// This obviously displays an error message.
// It should offer a button to end the game, and if not fatal, a button to ignore the error.
inline void show_error(std::string msg, const bool fatal) {
   show_debug_message(msg, (fatal) ? M_FATAL_USER_ERROR : M_USER_ERROR);
}

int show_message(const std::string &msg);
template<typename T> int show_message(T msg) { return show_message(enigma_user::toString(msg)); }
inline int action_show_message(string msg) {
  return show_message(msg);
}
void show_info(string text=enigma::gameInfoText, int bgcolor=enigma::gameInfoBackgroundColor, int left=enigma::gameInfoLeft, int top=enigma::gameInfoTop, int width=enigma::gameInfoWidth, int height=enigma::gameInfoHeight,
	bool embedGameWindow=enigma::gameInfoEmbedGameWindow, bool showBorder=enigma::gameInfoShowBorder, bool allowResize=enigma::gameInfoAllowResize, bool stayOnTop=enigma::gameInfoStayOnTop,
	bool pauseGame=enigma::gameInfoPauseGame, string caption=enigma::gameInfoCaption);
static inline void action_show_info() { show_info(); }

}

#endif
