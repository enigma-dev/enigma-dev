/** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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

#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"

using namespace enigma::dx11;

namespace enigma_user {

void draw_clear_alpha(int col, float alpha)
{
	draw_batch_flush(batch_flush_deferred);
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = COL_GET_Rf(col);
	color[1] = COL_GET_Gf(col);
	color[2] = COL_GET_Bf(col);
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

void draw_clear(int col)
{
	draw_batch_flush(batch_flush_deferred);
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = COL_GET_Rf(col);
	color[1] = COL_GET_Gf(col);
	color[2] = COL_GET_Bf(col);
	color[3] = 1;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

int draw_get_msaa_maxlevel()
{
  return 0; //TODO: implement
}

bool draw_get_msaa_supported()
{
  return false; //TODO: implement
}

} // namespace enigma_user
