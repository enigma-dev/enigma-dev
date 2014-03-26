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
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/instance_system.h"
#include <cstdlib>
#include <cstdio>

#include "GameSettings.h"

namespace enigma
{
  bool widget_system_initialize()
  {
    return 0;
  }
  extern string gameInfoText, gameInfoCaption;
  extern int gameInfoBackgroundColor, gameInfoLeft, gameInfoTop, gameInfoWidth, gameInfoHeight;
  extern bool gameInfoEmbedGameWindow, gameInfoShowBorder, gameInfoAllowResize, gameInfoStayOnTop, gameInfoPauseGame;
}

void show_error(string err, const bool fatal)
{
  printf("ERROR in some action of some event for object %d, instance id %d: %s\n",
         (enigma::instance_event_iterator == NULL? enigma_user::global :
            enigma::instance_event_iterator->inst == NULL? enigma_user::noone :
              enigma::instance_event_iterator->inst->object_index),
         (enigma::instance_event_iterator == NULL? enigma_user::global :
            enigma::instance_event_iterator->inst == NULL? enigma_user::noone :
              enigma::instance_event_iterator->inst->id),
         err.c_str()
    );
  if (fatal) exit(0);
  ABORT_ON_ALL_ERRORS();
}


namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) {

}

void show_info() {
  show_info(enigma::gameInfoText, enigma::gameInfoBackgroundColor, enigma::gameInfoLeft, enigma::gameInfoTop, enigma::gameInfoWidth, enigma::gameInfoHeight, enigma::gameInfoEmbedGameWindow, enigma::gameInfoShowBorder,
	enigma::gameInfoAllowResize, enigma::gameInfoStayOnTop, enigma::gameInfoPauseGame, enigma::gameInfoCaption);
}

}