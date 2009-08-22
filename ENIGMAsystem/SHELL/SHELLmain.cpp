/*********************************************************************************\
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
\*********************************************************************************/


#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <limits>
#include <vector>

#include <time.h>

#include "API_Switchboard.h"

#include "Universal_System/var_cr3.h"
#include "Universal_System/reflexive_types.h"

#include "Universal_System/GAME_GLOBALS.h"
#include "Universal_System/ENIGMA_GLOBALS.h"

#include "Universal_System/EGMstd.h"


#include "Preprocessor_Environment_Editable/LIBINCLUDE.h"
#include "Preprocessor_Environment_Editable/GAME_SETTINGS.h"



#if STRINGLIB
    #include "Universal_System/estring.h"
    #endif
#if MATHNCLIB
    #include "Universal_System/mathnc.h"
    #endif

#ifdef ENIGMA_GS_OPENGL
    #include "Graphics_Systems/OpenGL/OPENGLStd.h"
    #endif
#ifdef ENIGMA_WS_WINDOWS
    #include "Platforms/WINDOWS/WINDOWSStd.h"
    #endif
#ifdef ENIGMA_WS_XLIB
    #include "Platforms/xlib/XLIBwindow.h"
    #include "Platforms/xlib/XLIBdialog.h"
    #endif



#include "Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h"
#include "Universal_System/switch_stuff.h"
#include "Universal_System/CallbackArrays.h"
#include "Universal_System/compression.h"



#if COLLIGMA
    #include "Colligma/colligma_start.h"
    #include "Colligma/colligma_end.h"
    #endif

#include "Universal_System/IMGloading.h"
#include "Graphics_Systems/OpenGL/GSsprite.h"
#include "Graphics_Systems/OpenGL/GSspriteadd.h"


#include "Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h"
#include "Universal_System/globalupdate.h"

#include "Universal_System/objecttable.h"
#include "Universal_System/object.h"
#include "Universal_System/instance.h"
#include "Universal_System/roomsystem.h"

#include "Preprocessor_Environment_Editable/IDE_EDIT_wildclass.h"
#include "Universal_System/WITHconstruct.h"
#include "Universal_System/OBJaccess.h"

#include "Universal_System/simplecollisions.h"
#if COLLIGMA
    #include "Universal_System/collisions.h"
    #endif

#include "Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h"
#include "Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h"




#if BUILDMODE
  #include "Modes/buildmode.h"
  #endif


#ifdef ENIGMA_GS_OPENGL
    #include "Graphics_Systems/OpenGL/GSscreen.h"
    #endif

#include "Universal_System/spriteinit.h"
#include "Universal_System/events.h"

#ifdef ENIGMA_WS_WINDOWS
    #include "Platforms/windows/WINDOWSmain.h"
    #include "Platforms/windows/WINDOWScallback.h"
    #endif
#ifdef ENIGMA_WS_XLIB
    #include "Platforms/xlib/XLIBmain.h"
    #endif

#include "Universal_System/instance_create.h"

