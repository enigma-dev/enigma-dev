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

#include <string>

#include "object.h"

namespace enigma
{
	int event_current=-1;
	std::string evname(int evnumb)
	{
		switch (evnumb)
		{
      case -3: return "Game initialize";
      case -2: return "Initialize";
      case -1: return "No particular";
      case 0:  return "Parent Begin Step";
      case 1:  return "Room Start";
      case 2:  return "Begin Step";
      case 3:  return "Alarm";
      case 4:  return "Keyboard";
      case 5:  return "Key Press";
      case 6:  return "Key Release";
      case 7:  return "Mouse";
      case 8:  return "Step";
      case 9:  return "Path End";
      case 10: return "Outside Room";
      case 11: return "Instersect Boundary";
      case 12: return "Collision";
      case 13: return "No More Lives";
      case 14: return "No More Health";
      case 15: return "End Step";
      case 16: return "Draw";
      case 17: return "Animation End";
      case 18: return "Room End";
      case 19: return "Game End";
      case 20: return "Parent End Step";
      default: return "Unknown";
		}
	}
	int getID()
	{
		return (instance_event_iterator!=NULL)?instance_event_iterator->first?instance_event_iterator->second?instance_event_iterator->second->object_index:-2:-1:-1;
/*		if((*instance_event_iterator).first){
			if ((*instance_event_iterator).second)
				return (int)(*instance_event_iterator).second->object_index;
			else return -2;
		}
		return -1;*/
	}
}
