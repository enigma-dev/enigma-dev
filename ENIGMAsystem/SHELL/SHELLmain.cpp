/** Copyright (C) 2008-2013 Josh Ventura
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


#include <cstdlib>
#include <cstddef>
#include <string>

#define INCLUDED_FROM_SHELLMAIN 1

// Simple Universal libraries
///////////////////////////////

#include "Universal_System/var4.h"
#include "Universal_System/dynamic_args.h"

#include "Universal_System/mathnc.h"
#include "Universal_System/estring.h"
#include "Universal_System/bufferstruct.h"
#include "Universal_System/fileio.h"
#include "Universal_System/terminal_io.h"

#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Universal_System/fontstruct.h"

#include "Universal_System/callbacks_events.h"

#include "GameSettings.h"
#include "Preprocessor_Environment_Editable/LIBINCLUDE.h"
#include "Preprocessor_Environment_Editable/GAME_SETTINGS.h"

#include "Universal_System/collisions_object.h"

#include "Collision_Systems/collision_mandatory.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/platforms_mandatory.h"

#include "API_Switchboard.h"

#include "Universal_System/reflexive_types.h"

#include "Universal_System/GAME_GLOBALS.h" // TODO: Do away with this sloppy infestation permanently!
#include "Universal_System/ENIGMA_GLOBALS.h"

#include "libEGMstd.h"

#include "Universal_System/switch_stuff.h"
#include "Universal_System/CallbackArrays.h"

extern int amain();

#include "Universal_System/IMGloading.h"

#include "Universal_System/object.h"
#include "Universal_System/instance.h"
#include "Universal_System/roomsystem.h"

#include "Universal_System/globalupdate.h"

#include "Universal_System/instance_system_frontend.h"

#include "Universal_System/resource_data.h"
#include "Universal_System/highscore_functions.h"

#include "Universal_System/move_functions.h"
#include "Universal_System/actions.h"
#include "Universal_System/lives.h"

namespace enigma_user {}

using namespace enigma_user;

#ifndef JUST_DEFINE_IT_RUN
  #include "Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h"
#endif
#include "Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h"
  #ifndef JUST_DEFINE_IT_RUN
  #include "Universal_System/syntax_quirks.h"

  #include "Universal_System/with.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_evparent.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_events.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_globals.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_objectaccess.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_shaderarrays.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_fontinfo.h"

  #if defined(BUILDMODE) && BUILDMODE
    #include "Modes/buildmode.h"
  #endif

  #include "Universal_System/instance_create.h"
#endif

namespace enigma
{
  int game_ending();
  int game_ending()
  {
    for (enigma::iterator i = instance_list_first(); i; ++i)
      { i->unlink(); delete *i; }
    return 0;
  }
}
