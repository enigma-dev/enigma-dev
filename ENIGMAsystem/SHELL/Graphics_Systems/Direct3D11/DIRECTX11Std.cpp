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

#include <iostream>
#include <string>

#include "Direct3D11Headers.h"
using namespace std;
#include "DIRECTX11Std.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.

namespace enigma
{

unsigned bound_texture=0;
bool pbo_isgo;

void init_blend_state();
void init_depth_stencil_state();
void init_sampler_state();

void graphicssystem_initialize() {
  init_blend_state();
  init_depth_stencil_state();
  init_sampler_state();
}

} // namespace enigma

namespace enigma_user {

// Stolen entirely from the documentation and thrown into a switch() structure.
string draw_get_graphics_error() {

}

} // namespace enigma_user
