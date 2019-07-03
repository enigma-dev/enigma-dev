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

#ifdef _WIN32
#undef ERROR // TODO: find windows.h included in wrong order
#endif

#define DEBUG_MESSAGE(msg, severity) show_error(msg + std::string(" | ") + std::string(__FILE__) + std::string(":") + std::to_string(__LINE__), severity)

namespace enigma {
  enum MESSAGE_TYPE : int {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
    FATAL_ERROR = 3
  };
  
  inline std::string error_type(MESSAGE_TYPE t) {
    switch(t) {
      case INFO: return "INFO";
      case WARNING: return "WARNING";
      case ERROR: return "ERROR";
      case FATAL_ERROR: return "FATAL_ERROR";
      default: return "ERROR";
    }
  }
  
  // This function is called at the beginning of the game to allow the widget system to load.
  bool widget_system_initialize();
  extern std::string gameInfoText, gameInfoCaption;
  extern int gameInfoBackgroundColor, gameInfoLeft, gameInfoTop, gameInfoWidth, gameInfoHeight;
  extern bool gameInfoEmbedGameWindow, gameInfoShowBorder, gameInfoAllowResize, gameInfoStayOnTop, gameInfoPauseGame;
  void show_error(std::string msg, MESSAGE_TYPE type);
}

namespace enigma_user {

// This obviously displays an error message.
// It should offer a button to end the game, and if not fatal, a button to ignore the error.
inline void show_error(std::string msg, const bool fatal) {
   enigma::show_error(msg, (fatal) ? enigma::FATAL_ERROR : enigma::ERROR);
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

using enigma::MESSAGE_TYPE;

#endif
