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

//R3 instance access was inefficient at best.
//It will be redone along with Wildclass promptly after the new instance system is implemented.
/*
namespace enigma {
  wildclass int2object(double intr);
}

namespace enigma
{
  wildclass int2object(double intr)
  {
    int integer=(int)intr;
    if(integer<=100000)
    {
      if(integer>=0)
      {
        for (instance_iterator=instance_list.begin(); instance_iterator != instance_list.end(); instance_iterator++) {
          if((*instance_iterator).second&&(*instance_iterator).second->object_index==integer)
            return ((*instance_iterator).second);
        }
        
        #if SHOWERRORS
          show_error("Attempting to retrieve nonexisting object "+string(integer),0);
        #endif
        
        return (object_basic*)ENIGMA_global_instance;
      }
      else
      {
        if(integer==-5)
          return (object_basic*)ENIGMA_global_instance;
        if(integer==-1)
          return (*instance_iterator).second;
        if(integer==-3) {
          instance_iterator=instance_list.begin();
          return (*instance_iterator).second;
        }
        
        #if SHOWERRORS
        show_error("Attempting to retrieve nonexisting object "+string(integer),0);
        #endif
        
        return (object_basic*)ENIGMA_global_instance;
      }
    }
    else return instance_list[integer];
  }
}
*/
