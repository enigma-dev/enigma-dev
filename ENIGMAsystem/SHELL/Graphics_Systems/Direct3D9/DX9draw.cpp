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

#include <math.h>
#include "../General/GSprimitives.h"
#include "Direct3D9Headers.h"
#include "Bridges/General/DX9Context.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"

#include <stdio.h>
#include "Universal_System/roomsystem.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

#include <vector>
using std::vector;

namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

int draw_get_msaa_maxlevel()
{

}

bool draw_get_msaa_supported()
{

}

void draw_set_msaa_enabled(bool enable)
{

}

void draw_enable_alphablend(bool enable) {
	d3dmgr->SetRenderState(D3DRS_ALPHABLENDENABLE, enable);
}

bool draw_get_alpha_test() {
	DWORD* enabled;
	d3dmgr->GetRenderState(D3DRS_ALPHATESTENABLE, enabled);
	return *enabled;
}

unsigned draw_get_alpha_test_ref_value()
{
	DWORD* val;
	d3dmgr->GetRenderState(D3DRS_ALPHAREF, val);
	return *val;
}

void draw_set_alpha_test(bool enable)
{
	d3dmgr->SetRenderState(D3DRS_ALPHATESTENABLE, enable); 
}

void draw_set_alpha_test_ref_value(unsigned val)
{
	d3dmgr->SetRenderState(D3DRS_ALPHAREF, val);
}

void draw_set_line_pattern(unsigned short pattern, int scale)
{

}

}

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user
{

int draw_getpixel(int x, int y)
{

}

int draw_getpixel_ext(int x, int y)
{

}

bool fill_complex_polygon(const std::list<PolyVertex>& vertices, int defaultColor, bool allowHoles)
{
  //TODO: Complex polygon supported only in OpenGL1 at the moment. By returning false here, we fall back
  //      on a convex-only polygon drawing routine that works on any platform.
  return false;
}

}

