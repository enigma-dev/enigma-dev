/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

/*************************************************************
GM Global variables
******************/

#ifndef ENIGMA_GAME_GLOBALS_H
#define ENIGMA_GAME_GLOBALS_H

#include <string>
#ifndef JUST_DEFINE_IT_RUN
#include <deque>
#endif

namespace enigma_user {
// TODO: MOVEME: Who put this here?
#ifndef JUST_DEFINE_IT_RUN
std::deque<int> instance_id;
#else
int *instance_id;
#endif

bool argument_relative = false;
bool secure_mode = false;
int transition_kind = 0;
int transition_steps = 80;
bool automatic_redraw = true;
int gamemaker_version = 0;
int cursor_sprite = -1;
extern int room_first, room_last;
}  // namespace enigma_user

/*********************
End GM global variables
 *********************/

#endif //ENIGMA_GAME_GLOBALS_H
