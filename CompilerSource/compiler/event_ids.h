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


enum
{
  EV_CREATE = 0,
  EV_DESTROY = 1,
  EV_ALARM = 2,
  EV_STEP = 3,
  EV_COLLISION = 4,
  EV_KEYBOARD = 5,
  EV_MOUSE = 6,
  EV_OTHER = 7,
  EV_DRAW = 8,
  EV_KEYPRESS = 9,
  EV_KEYRELEASE = 10,
  EV_TRIGGER = 11
};

enum
{
  EV_LEFbUTTON=0,
  EV_RIGHbUTTON=1,
  EV_MIDDLbUTTON=2,
  EV_NOBUTTON=3,
  EV_LEFT_PRESS=4,
  EV_RIGHT_PRESS=5,
  EV_MIDDLE_PRESS=6,
  EV_LEFT_RELEASE=7,
  EV_RIGHT_RELEASE=8,
  EV_MIDDLE_RELEASE=9,
  EV_MOUSE_ENTER=10,
  EV_MOUSE_LEAVE=11,
  EV_MOUSE_WHEEL_UP=60,
  EV_MOUSE_WHEEL_DOWN=61,
  EV_GOBAL_LEFTBUTTON=50,
  EV_GOBAL_RIGHTBUTTON=51,
  EV_GOBAL_MIDDLEBUTTON=52,
  EV_GOBAL_LEFT_PRESS=53,
  EV_GOBAL_RIGHT_PRESS=54,
  EV_GOBAL_MIDDLE_PRESS=55,
  EV_GOBAL_LEFT_RELEASE=56,
  EV_GOBAL_RIGHT_RELEASE=57
};

enum
{
  EV_OUTSIDE = 0,
  EV_BOUNDARY = 1,
  EV_GAME_START = 2,
  EV_GAME_END = 3,
  EV_ROOM_START = 4,
  EV_ROOM_END = 5,
  EV_NO_MORE_LIVES = 6,
  EV_NO_MORE_HEALTH = 9,
  EV_ANIMATION_END = 7,
  EV_END_OF_PATH = 8,
  EV_USER0 = 10,
  EV_USER1 = 11,
  EV_USER2 = 12,
  EV_USER3 = 13,
  EV_USER4 = 14,
  EV_USER5 = 15,
  EV_USER6 = 16,
  EV_USER7 = 17,
  EV_USER8 = 18,
  EV_USER9 = 19,
  EV_USER10 = 20,
  EV_USER11 = 21,
  EV_USER12 = 22,
  EV_USER13 = 23,
  EV_USER14 = 24,
  EV_USER15 = 25
};

enum
{
  STEP_NORMAL = 0,
  STEP_BEGIN = 1,
  STEP_END = 2
};
