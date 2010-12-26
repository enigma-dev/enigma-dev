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

#include <stdlib.h>
#include <string>

// Simple Universal libraries
///////////////////////////////

#include "Universal_System/var4.h"

#include "Universal_System/mathnc.h"
#include "Universal_System/estring.h"
#include "Universal_System/fileio.h"
#include "Universal_System/terminal_io.h"

//#include <vector> //COLLIGMA


#include "API_Switchboard.h"

#include "Universal_System/reflexive_types.h"

#include "Universal_System/GAME_GLOBALS.h"
#include "Universal_System/ENIGMA_GLOBALS.h"

#include "libEGMstd.h"


#include "Preprocessor_Environment_Editable/LIBINCLUDE.h"
#include "Preprocessor_Environment_Editable/GAME_SETTINGS.h"

#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#if ENIGMA_GS_OPENGL
    #include "Graphics_Systems/OpenGL/OPENGLStd.h"
    #include "Graphics_Systems/OpenGL/GSsprite.h"
    #include "Graphics_Systems/OpenGL/GSbackground.h"

    #include "Graphics_Systems/OpenGL/GSfont.h"
    #include "Graphics_Systems/OpenGL/GScurves.h"
#elif ENIGMA_GS_OPENGLES
    #include "Graphics_Systems/OpenGLES/OPENGLStd.h"
    #include "Graphics_Systems/OpenGLES/GSsprite.h"
#endif
#if ENIGMA_WS_WIN32 != 0
    #include "Platforms/Win32/WINDOWSStd.h"
#elif ENIGMA_WS_XLIB != 0
    #include "Platforms/xlib/XLIBwindow.h"
#elif ENIGMA_WS_COCOA != 0
    #include "Platforms/Cocoa/CocoaWindow.h"
#elif ENIGMA_WS_IPHONE != 0
    #include "Platforms/iPhone/CocoaWindow.h"
#elif ENIGMA_WS_ANDROID != 0
    #include "Platforms/Android/AndroidWindow.h"
#endif



#include "Universal_System/switch_stuff.h"
#include "Universal_System/CallbackArrays.h"

extern int amain();


#if COLLIGMA
    #include "Colligma/colligma_start.h"
    #include "Colligma/colligma_end.h"
#endif

//TODO: modularize this
#include "Collision_Systems/BBox/include.h"

#include "Universal_System/IMGloading.h"

#include "Audio_Systems/OpenAL/as_basic.h"

#include "Universal_System/collisions_object.h"

#include "Universal_System/object.h"
#include "Universal_System/instance.h"
#include "Universal_System/roomsystem.h"

#include "Universal_System/globalupdate.h"

#include "Universal_System/WITHconstruct.h"



#include "Universal_System/simplecollisions.h"
#if COLLIGMA
    #include "Universal_System/collisions.h"
#endif

#include "Universal_System/instance_system.h"
#include "Universal_System/syntax_quirks.h"

#ifndef ENIGMA_PARSER_RUN
  #include "Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h"
#endif

#include "Preprocessor_Environment_Editable/IDE_EDIT_whitespace.h"

#ifndef ENIGMA_PARSER_RUN
  #include "Universal_System/with.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_evparent.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_events.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_globals.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_objectaccess.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h"
  #include "Preprocessor_Environment_Editable/IDE_EDIT_roomarrays.h"

  #if BUILDMODE
    #include "Modes/buildmode.h"
  #endif

  #include "Universal_System/instance_create.h"
#endif

#include "Universal_System/pseudo_components.h"

namespace enigma
{
  int game_ending()
  {
    for (inst_iter *i = instance_list_first(); i != NULL; i = i->next)
      { i->inst->unlink(); delete i->inst; }
    return 0;
  }
}
