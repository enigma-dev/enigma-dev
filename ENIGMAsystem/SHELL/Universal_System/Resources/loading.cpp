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
#include "backgrounds_internal.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/Object_Tiers/object.h"
#include "strings_util.h"
#include "libEGMstd.h"

#include "Platforms/General/PFmain.h"
#include "Universal_System/directoryglobals.h"
#include "Universal_System/fileio.h"
#include "Universal_System/estring.h"
#include "apifilesystem/filesystem.hpp"
#include "Universal_System/joysticks.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"
#include "Audio_Systems/audio_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include <ctime>
#include <cstdio>

using enigma_user::get_program_pathname;

namespace enigma_user
{
  //FIXME: These two functions are declared in mathnc.h but we arent allowed to include that here
  extern int random_set_seed(int seed);
  extern int mtrandom_seed(int x);
} //namespace enigma_user

namespace enigma
{
  extern const char* resource_file_path;
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
    joystick_init();

    // Open the exe for resource load
    do { // Allows break
      FILE* resfile = nullptr; 
      #if !defined(_WIN32)
      resfile = fopen((enigma_user::filename_change_ext(program_pathname, "") + ".res").c_str(),"rb");
      #else
      resfile = _wfopen(ghc::filesystem::path(enigma_user::filename_change_ext(program_pathname, "") + ".res").wstring().c_str(), L"rb");
      #endif

      if (!enigma_user::file_exists(enigma_user::filename_change_ext(program_pathname, "") + ".res") || !resfile) {
        exit(0);
        break;
      }

      enigma::exe_loadsprs(resfile);
      enigma::exe_loadsounds(resfile);
      enigma::exe_loadbackgrounds(resfile);
      enigma::exe_loadfonts(resfile);
      #ifdef PATH_EXT_SET
      enigma::exe_loadpaths(resfile);
      #endif

      fclose(resfile);
    } while (false);

    //Load object struct
    enigma::objectdata_load();

    //Load rooms
    enigma::rooms_load();

    //Initialize extensions
    enigma::extensions_initialize();

    //Go to the first room
    if (enigma_user::room_count)
      enigma::game_start();

    enigma_user::window_default(true);
    enigma_user::window_set_visible(true);

    // resize and center window
    enigma_user::window_set_size(windowWidth, windowHeight);
    enigma_user::window_center();
    
    // apply global game settings
    enigma_user::window_set_sizeable(isSizeable);
    enigma_user::window_set_showborder(showBorder);
    enigma_user::window_set_fullscreen(isFullScreen);

    return 0;
  }
} //namespace enigma
