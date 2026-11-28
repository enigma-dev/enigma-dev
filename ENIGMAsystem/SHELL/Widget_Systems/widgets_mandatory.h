/** Copyright (C) 2026 Samuel Venable
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

#include <string>

#include "libEGMstd.h"

#define DEBUG_MESSAGE(str, severity) ::enigma_user::show_debug_message((std::string) (str) + " | " __FILE__ ":" + std::to_string(__LINE__), (severity))

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

  bool widget_system_initialize();
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

} // namespace enigma

namespace enigma_user {

  void show_info();
  inline void action_show_info() { 
    show_info();
  }
  int show_message(std::string str);
  template<typename T> int show_message(T str) { 
    return show_message(enigma_user::toString(str));
  }
  inline int action_show_message(string str) {
    return show_message(str);
  }
  int show_question(std::string str);
  std::string get_string(std::string str, std::string def);
  double get_integer(std::string str, double def);
  int show_debug_message(std::string str, MESSAGE_TYPE type = M_INFO);
  inline int show_error(std::string str, const bool abort) {
    return show_debug_message(str, (abort) ? M_FATAL_USER_ERROR : M_USER_ERROR);
  }

} // namespace enigma_user

#endif
