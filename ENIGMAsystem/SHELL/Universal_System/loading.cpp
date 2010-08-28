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
#include <stdio.h>

#include <string>
using std::string;

#include "mathnc.h"
#include "resinit.h"
#include "../Platforms/platforms_mandatory.h"
#include "../Audio_Systems/audio_mandatory.h"
#include "../Graphics_Systems/graphics_mandatory.h"
#include "roomsystem.h"

#include "../libEGMstd.h"

namespace enigma {
  extern int event_system_initialize(); //Leave this here until you can find a more brilliant way to include it; it's pretty much not-optional.
}

//This is like main(), only cross-api
namespace enigma
{
  int initialize_everything()
  {
    mtrandom_seed(random_set_seed(time(0)));
    graphicssystem_initialize();
    audiosystem_initialize();
    #if ENIGMA_WS_WINDOWS!=0
      enigma::init_fonts();
    #endif
    #if BUILDMODE
      buildmode::buildinit();
    #endif
    
    event_system_initialize();
    input_initialize();
    sprites_init();
    //backgrounds_init();
    
    // Open the exe for resource load
    char exename[1025];
    windowsystem_write_exename(exename);
    FILE* exe = fopen(exename,"rb");
    if (!exe)
      show_error("Resource load fail: exe unopenable",0);
    else do
    {
      int nullhere;
      // Read the magic number so we know we're looking at our own data
      fseek(exe,-8,SEEK_END);
      char str_quad[4];
      fread(str_quad,1,4,exe);
      if (str_quad[0] != 'r' or str_quad[1] != 'e' or str_quad[2] != 's' or str_quad[3] != '0') {
        printf("No resource data in exe\n");
        break;
      }
      
      // Get where our resources are located in the module
      int pos;
      fread(&pos,4,1,exe);
      
      // Go to the start of the resource data
      fseek(exe,pos,SEEK_SET);
      fread(&nullhere,4,1,exe);
      if(nullhere) break;
      
      enigma::exe_loadsprs(exe);
      enigma::exe_loadsounds(exe);
      enigma::exe_loadbackgrounds(exe);
      
      fclose(exe);
    }
    while (false);
    
    //Load rooms
    enigma::rooms_load();
    
    //Go to the first room
    if (room_count)
      room_goto_absolute(0);
    
    return 0;
  }
}
