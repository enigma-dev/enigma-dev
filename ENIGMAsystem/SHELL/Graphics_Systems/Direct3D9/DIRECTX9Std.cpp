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

#include "DIRECTX9Std.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.

#include <iostream>
#include <string>

using namespace std;
using namespace enigma::dx9;

namespace enigma {

void graphicssystem_initialize() {
  d3dmgr->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
  // just in case the user does turn on lighting, we need to set a material
  // that works for lighting vertices that have no color data
  D3DMATERIAL9 mtrl = {};
  mtrl.Ambient.r = mtrl.Ambient.g = mtrl.Ambient.b = mtrl.Ambient.a = 1.0;
  mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0;
  d3dmgr->SetMaterial(&mtrl);
}

} // namespace enigma

namespace enigma_user {

string draw_get_graphics_error()
{
  return ""; //TODO: implement
}

} // namespace enigma_user
