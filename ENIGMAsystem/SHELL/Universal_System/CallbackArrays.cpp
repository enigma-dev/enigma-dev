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

namespace enigma
{
	char mousestatus[3];
	char last_mousestatus[3];
	char last_keybdstatus[256];
	char keybdstatus[256];
}

#include "CallbackArrays.h"

bool mouse_check_button(int button)
{
	switch (button)
	{
    case mb_any:
      return enigma::mousestatus[0] | enigma::mousestatus[1] | enigma::mousestatus[2];
    case mb_none:
      return !(enigma::mousestatus[0] | enigma::mousestatus[1] | enigma::mousestatus[2]);
    case 1: case 2: case 3:
      return enigma::mousestatus[button-1];
    default:return 0;
	}
}

bool mouse_check_button_pressed(int button)
{
  switch (button)
  {
    case mb_any:
      return (enigma::mousestatus[0] && !enigma::last_mousestatus[0]) // The trend is "pressed this step, but not last step"
          || (enigma::mousestatus[1] && !enigma::last_mousestatus[1])
          || (enigma::mousestatus[2] && !enigma::last_mousestatus[2]);
    case mb_none:
      return !((enigma::mousestatus[0] && !enigma::last_mousestatus[0]) // The trend is "pressed this step, but not last step"
          ||   (enigma::mousestatus[1] && !enigma::last_mousestatus[1])
          ||   (enigma::mousestatus[2] && !enigma::last_mousestatus[2]));
    case  1: case 2: case 3:
      return enigma::mousestatus[button-1] && !enigma::last_mousestatus[button-1];
    default:return 0;
  }
}

bool mouse_check_button_released(int button)
{
	switch (button)
	{
    case mb_any:
      return (!enigma::mousestatus[0] && enigma::last_mousestatus[0])
          || (!enigma::mousestatus[1] && enigma::last_mousestatus[1])
          || (!enigma::mousestatus[2] && enigma::last_mousestatus[2]);
    case mb_none:
      return !((!enigma::mousestatus[0] && enigma::last_mousestatus[0])
          ||   (!enigma::mousestatus[1] && enigma::last_mousestatus[1])
          ||   (!enigma::mousestatus[2] && enigma::last_mousestatus[2]));
    case 1: case 2: case 3:
      return !enigma::mousestatus[button-1] && enigma::last_mousestatus[button-1];
    default: return 0;
	}
}


short mouse_hscrolls = 0;
short mouse_vscrolls = 0;

bool keyboard_check(int key)
{
  if (key == vk_anykey) {
    for(int i=0; i<255; i++)
      if (enigma::keybdstatus[i]==1) return 1;
    return 0;
  }
  if (key == vk_nokey) {
    for(int i=0;i<255;i++)
      if(enigma::keybdstatus[i]==1) return 0;
    return 1;
  }
	return enigma::keybdstatus[key & 0xFF];
}

bool keyboard_check_pressed(int key)
{
  if (key == vk_anykey) {
    for(int i=0;i<255;i++)
      if (enigma::keybdstatus[i] && !enigma::last_keybdstatus[i]) return 1;
    return 0;
  }
  if (key == vk_nokey) {
    for(int i=0;i<255;i++)
      if (enigma::keybdstatus[i] && !enigma::last_keybdstatus[i]) return 0;
    return 1;
  }
  return enigma::keybdstatus[key & 0xFF] && !enigma::last_keybdstatus[key & 0xFF];
}

bool keyboard_check_released(int key)
{
  if(key == vk_anykey) {
		for(int i=0;i<255;i++)
			if (!enigma::keybdstatus[i] && enigma::last_keybdstatus[i]) return 1;
		return 0;
  }
  if(key == vk_nokey) {
		for(int i=0;i<255;i++)
			if (!enigma::keybdstatus[i] && enigma::last_keybdstatus[i]) return 0;
		return 1;
  }
	return enigma::keybdstatus[key & 0xFF]==0 && enigma::last_keybdstatus[key & 0xFF]==1;
}

