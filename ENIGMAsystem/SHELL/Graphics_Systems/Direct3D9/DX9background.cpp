/** Copyright (C) 2013 Robert B. Colton
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

#include <cstddef>

#include <math.h>
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Universal_System/nlpo2.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/sprites_internal.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

#include "Graphics_Systems/General/GScolor_macros.h"

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
  D3DCOLOR get_currentcolor();
}


#include <string.h> // needed for querying ARB extensions

#include "DX9TextureStruct.h"

namespace enigma_user
{

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{

}

}
