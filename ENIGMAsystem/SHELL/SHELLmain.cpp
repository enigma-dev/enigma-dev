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

/*#define tits int
typedef tits nigr;


#define __CONCAT(x,y)	x ## y
#define unchange(x,y) __CONCAT(x,y)
int unchange(__CONCAT(one,tawoo),athree);


#define test(x)
#define attest(x)
#define detest(x) error here x D
#define goforit(x) x ## test(:O)

goforit(at);*/


#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <limits>

//Before anyone else includes math.h, let's make sure we
//Don't get bitten by those damn bessel functions.
#define y0 bessel_y0
  #define y1 bessel_y1
  #define jn bessel_jn
  #define j0 bessel_j0
  #define j1 bessel_j1
    #include <math.h>
  #undef j1
  #undef j0
  #undef jn
  #undef y1
#undef y0


//#include <vector> //COLLIGMA

#include <time.h>

#include "API_Switchboard.h"

#include "Universal_System/var_cr3.h"
#include "Universal_System/reflexive_types.h"

#include "Universal_System/GAME_GLOBALS.h"
#include "Universal_System/ENIGMA_GLOBALS.h"

#include "Universal_System/EGMstd.h"


#include "Preprocessor_Environment_Editable/LIBINCLUDE.h"
#include "Preprocessor_Environment_Editable/GAME_SETTINGS.h"

// Simple Universal libraries
#include "Universal_System/estring.h"
#include "Universal_System/fileio.h"
#include "Universal_System/mathnc.h"

#if ENIGMA_GS_OPENGL
    #include "Graphics_Systems/OpenGL/OPENGLStd.h"
#endif
#if ENIGMA_WS_WINDOWS != 0
    #include "Platforms/windows/WINDOWSStd.h"
#elif ENIGMA_WS_XLIB != 0
    #include "Platforms/xlib/XLIBwindow.h"
    #include "Platforms/xlib/XLIBdialog.h"
#endif



#include "Preprocessor_Environment_Editable/IDE_EDIT_resourcenames.h"
#include "Universal_System/switch_stuff.h"
#include "Universal_System/CallbackArrays.h"



#if COLLIGMA
    #include "Colligma/colligma_start.h"
    #include "Colligma/colligma_end.h"
#endif

#include "Universal_System/IMGloading.h"
#include "Graphics_Systems/OpenGL/GSsprite.h"
#include "Graphics_Systems/OpenGL/GSspriteadd.h"


#include "Preprocessor_Environment_Editable/IDE_EDIT_modesenabled.h"

#include "Universal_System/collisions_object.h"

#include "Universal_System/objecttable.h"
#include "Universal_System/object.h"
#include "Universal_System/instance.h"
#include "Universal_System/roomsystem.h"

#include "Universal_System/globalupdate.h"

#include "Preprocessor_Environment_Editable/IDE_EDIT_wildclass.h"
#include "Universal_System/WITHconstruct.h"
#include "Universal_System/OBJaccess.h"

#include "Universal_System/simplecollisions.h"
#if COLLIGMA
    #include "Universal_System/collisions.h"
#endif

#include "Preprocessor_Environment_Editable/IDE_EDIT_globals.h"
#include "Preprocessor_Environment_Editable/IDE_EDIT_objectdeclarations.h"
#include "Preprocessor_Environment_Editable/IDE_EDIT_objectfunctionality.h"
#include "Preprocessor_Environment_Editable/IDE_EDIT_roomcreates.h"




#if BUILDMODE
	#include "Modes/buildmode.h"
#endif


#if ENIGMA_GS_OPENGL
	#include "Graphics_Systems/OpenGL/GSscreen.h"
#endif

#include "Universal_System/spriteinit.h"
#include "Universal_System/events.h"

/*
#if ENIGMA_WS_WINDOWS
    #include "Platforms/windows/WINDOWSmain.h"
    #include "Platforms/windows/WINDOWScallback.h"
#elif ENIGMA_WS_XLIB
	#include "Platforms/xlib/XLIBmain.h"
#endif
*/

#include "Universal_System/instance_create.h"

//This is like main(), only cross-api
int initialize_everything()
{
  #include "initialize.h"
  return 0;
}
