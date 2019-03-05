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

#include "Direct3D9Headers.h"
#include "DX9TextureStruct.h"

#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GStextures.h"

#include "Universal_System/nlpo2.h"
#include "Universal_System/background_internal.h"
#include "Universal_System/sprites_internal.h"

#include <cstddef>
#include <math.h>

namespace enigma_user {

int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{
  return -1; //TODO: implement
}

} // namespace enigma_user
