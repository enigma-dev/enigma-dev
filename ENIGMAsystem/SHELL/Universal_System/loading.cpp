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

#include <time.h>
#include <string>
using std::string;

#include "mathnc.h"
#include "spriteinit.h"
#include "../Platforms/platforms_mandatory.h"
#include "../Graphics_Systems/graphics_mandatory.h"
#include "roomsystem.h"


namespace enigma {
  extern int event_system_initialize(); //Leave this here until you can find a more brilliant way to include it; it's pretty much not-optional.
}

//This is like main(), only cross-api
namespace enigma
{
  int initialize_everything()
  {
    mtrandom_seed(enigma::Random_Seed=time(0));
    #if BUILDMODE
      buildmode::buildinit();
    #endif
    graphicssystem_initialize();
    #if ENIGMA_WS_WINDOWS!=0
      enigma::init_fonts();
    #endif
    
    event_system_initialize();
    input_initialize();
    
    //Take care of sprites;
    enigma::sprite_idmax = 0;
    enigma::exe_loadsprs();
    
    //Load rooms
    enigma::rooms_load();
    
    //Go to the first room
    if (room_count)
      room_goto_absolute(0);
    /*
    FILE* a=fopen("shit!.txt","wb");
    if (a)
    {
      fprintf(a,"Listing of all instances:\r\n");
      for (enigma::instance_iterator=enigma::instance_list.begin();
      enigma::instance_iterator!=enigma::instance_list.end();
      enigma::instance_iterator++)
      {
        fprintf(a,"IND: %d | ID: %d   | obj: %3d   | x: %3d   | y: %3d\r\n",
          (int)(*enigma::instance_iterator).first,
          (int)(*enigma::instance_iterator).second->id,
          (int)(*enigma::instance_iterator).second->object_index,
          (int)(*enigma::instance_iterator).second->x,
          (int)(*enigma::instance_iterator).second->y);
      }
      fclose(a);
    }
    
    system("shit!.txt");*/
    
    return 0;
  }
}
