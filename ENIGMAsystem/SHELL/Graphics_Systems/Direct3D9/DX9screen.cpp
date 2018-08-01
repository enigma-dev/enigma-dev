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

#include "Bridges/General/DX9Context.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSscreen.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/roomsystem.h"

#include "Platforms/General/PFwindow.h"

using namespace enigma;
using namespace std;

namespace enigma
{

void scene_begin() {
  //d3dmgr->EndShapesBatching(); //If called inside bound surface we need to finish drawing
  d3dmgr->BeginScene();
}

void scene_end() {
  d3dmgr->EndScene();
}

}

namespace enigma_user
{

int screen_save(string filename) //Assumes native integers are little endian
{
	string ext = enigma::image_get_format(filename);

	d3dmgr->EndShapesBatching();
	LPDIRECT3DSURFACE9 pBackBuffer;
	LPDIRECT3DSURFACE9 pDestBuffer;
	d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	D3DSURFACE_DESC desc;
	pBackBuffer->GetDesc(&desc);

	d3dmgr->device->CreateOffscreenPlainSurface( desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &pDestBuffer, NULL );
	d3dmgr->device->GetRenderTargetData(pBackBuffer, pDestBuffer);

	D3DLOCKED_RECT rect;

	pDestBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	pDestBuffer->UnlockRect();

	int ret = image_save(filename, bitmap, desc.Width, desc.Height, desc.Width, desc.Height, false);

  pBackBuffer->Release();
	pDestBuffer->Release();

	//delete[] bitmap; <- no need cause RAII
	return ret;
}

int screen_save_part(string filename,unsigned x,unsigned y,unsigned w,unsigned h) //Assumes native integers are little endian
{
	string ext = enigma::image_get_format(filename);

	d3dmgr->EndShapesBatching();
	LPDIRECT3DSURFACE9 pBackBuffer;
	LPDIRECT3DSURFACE9 pDestBuffer;
	d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	D3DSURFACE_DESC desc;
	pBackBuffer->GetDesc(&desc);

	d3dmgr->device->CreateOffscreenPlainSurface( desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &pDestBuffer, NULL );
	d3dmgr->device->GetRenderTargetData(pBackBuffer, pDestBuffer);

	D3DLOCKED_RECT rect;

	pDestBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	pDestBuffer->UnlockRect();

	int ret = image_save(filename, bitmap, w, h, desc.Width, desc.Height, false);

  pBackBuffer->Release();
	pDestBuffer->Release();

  return ret;
}

void screen_set_viewport(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height) {
  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
	D3DVIEWPORT9 pViewport = { sx + x, sy + y, width, height, 0, 1.0f };
	d3dmgr->SetViewport(&pViewport);
}

}
