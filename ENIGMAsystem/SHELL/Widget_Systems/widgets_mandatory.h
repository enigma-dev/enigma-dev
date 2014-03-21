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

#ifndef _WIDGETS_MANDATORY__H
#define _WIDGETS_MANDATORY__H

#include <string>
using std::string;

namespace enigma
{
  // This function is called at the beginning of the game to allow the widget system to load.
  bool widget_system_initialize();
}

// This obviously displays an error message.
// It should offer a button to end the game, and if not fatal, a button to ignore the error.
void show_error(std::string msg, const bool fatal);

namespace enigma_user {

void show_info(string caption="Game Information");
void show_info(string info,string caption="Information");
static inline void action_show_info() { show_info(); }

}

#endif
