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

#include "Bridges/General/DX9Device.h"

#ifndef __DX9_BINDING_H
#define __DX9_BINDING_H

LPDIRECT3DTEXTURE9 get_texture(int texid);

namespace enigma { extern LPDIRECT3DTEXTURE9 bound_texture; }

// DirectX does not memorize the bound texture and significant slowdowns occur by constantly rebinding a texture
// glBind calls clog up the pipeline avoid them at all costs, texture paging is good
#define use_bound_texture_global
#ifdef use_bound_texture_global
  #define texture_reset() if (enigma::bound_texture != NULL) d3ddev->SetTexture(0, enigma::bound_texture = 0);
  #define texture_use(texid) if (enigma::bound_texture != get_texture(texid)) \
	d3ddev->SetTexture(0, enigma::bound_texture = get_texture(texid));
#else
  #define texture_reset() d3ddev->SetTexture(0, 0);
  #define texture_use(texid) d3ddev->SetTexture(0, enigma::bound_texture = get_texture(texid));
#endif

#endif
