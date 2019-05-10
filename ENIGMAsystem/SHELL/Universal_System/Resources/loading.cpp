/** Copyright (C) 2008-2011 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
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

#include "resinit.h"
#include "sprites_internal.h"
#include "background_internal.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/Object_Tiers/object.h"
#include "libEGMstd.h"
//#include "mathnc.h"

#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"
#include "Audio_Systems/audio_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include <time.h>
#include <stdio.h>

namespace enigma_user
{
  //FIXME: These two functions are declared in mathnc.h but we arent allowed to include that here
  extern int random_set_seed(int seed);
  extern int mtrandom_seed(int x);
} //namespace enigma_user

namespace enigma
{
  extern int event_system_initialize(); //Leave this here until you can find a more brilliant way to include it; it's pretty much not-optional.
  extern void timeline_system_initialize();
  extern int game_settings_initialize();
  extern void extensions_initialize();

  //This is like main(), only cross-api
  int initialize_everything()
  {
    time_t ss = time(0);
    enigma_user::random_set_seed(ss);
    enigma_user::mtrandom_seed(ss);

    // must occur before the create/room start/game start events so that it does not override the user setting them in code
    enigma::game_settings_initialize();

    graphicssystem_initialize();
    audiosystem_initialize();

    #if defined(BUILDMODE) && BUILDMODE
      buildmode::buildinit();
    #endif

    event_system_initialize();
    timeline_system_initialize();
    input_initialize();
    sprites_init();
    backgrounds_init();
    widget_system_initialize();

    // Open the exe for resource load
    char exename[1025];
    windowsystem_write_exename(exename);
    FILE* exe = fopen(exename,"rb");
    if (!exe)
      enigma_user::show_error("Resource load fail: exe unopenable",0);
    else do
    {
      int nullhere;
      // Read the magic number so we know we're looking at our own data
      fseek(exe,-8,SEEK_END);
      char str_quad[4];
      if (!fread(str_quad,4,1,exe) or str_quad[0] != 'r' or str_quad[1] != 'e' or str_quad[2] != 's' or str_quad[3] != '0') {
        printf("No resource data in exe\n");
        break;
      }

      // Get where our resources are located in the module
      int pos;
      if (!fread(&pos,4,1,exe)) break;

      // Go to the start of the resource data
      fseek(exe,pos,SEEK_SET);
      if (!fread(&nullhere,4,1,exe)) break;
      if(nullhere) break;

      enigma::exe_loadsprs(exe);
      enigma::exe_loadsounds(exe);
      enigma::exe_loadbackgrounds(exe);
      enigma::exe_loadfonts(exe);
    #ifdef PATH_EXT_SET
    enigma::exe_loadpaths(exe);
    #endif

      fclose(exe);
    }
    while (false);

    //Load object struct
    enigma::objectdata_load();

    //Load rooms
    enigma::rooms_load();

    //Initialize extensions
    enigma::extensions_initialize();

    //Go to the first room
    if (enigma_user::room_count)
      enigma::game_start();
    else {
      enigma_user::window_default();
      enigma_user::window_set_visible(true);
    }

    return 0;
  }
} //namespace enigma
