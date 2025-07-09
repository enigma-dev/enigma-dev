/** Copyright (C) 2008-2014 Josh Ventura, Robert B. Colton
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
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSprimitives.h"

using namespace enigma::dx9;

namespace enigma {

void scene_begin() {}

void scene_end() {}

void graphics_set_viewport(float x, float y, float width, float height) {
	D3DVIEWPORT9 pViewport = { (DWORD)x, (DWORD)y, (DWORD)width, (DWORD)height, 0, 1.0f };
	d3ddev->SetViewport(&pViewport);
}

} // namespace enigma
