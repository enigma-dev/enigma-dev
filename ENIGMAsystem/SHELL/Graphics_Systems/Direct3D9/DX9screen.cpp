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
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolors.h"

#include "Universal_System/image_formats.h"
#include "Universal_System/roomsystem.h"

#include "Platforms/General/PFwindow.h"

using namespace enigma;
using namespace enigma::dx9;
using namespace std;

namespace enigma
{

void scene_begin() {}

void scene_end() {}

}

namespace enigma_user
{

void screen_init()
{
  enigma::gui_width = window_get_region_width();
  enigma::gui_height = window_get_region_height();

  d3dmgr->device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
  d3dmgr->device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

  if (!view_enabled)
  {
    screen_set_viewport(0, 0, window_get_region_width(), window_get_region_height());
    d3d_set_projection_ortho(0, 0, window_get_region_width(), window_get_region_height(), 0);
  } else {
    for (view_current = 0; view_current < 7; view_current++) {
      if (view_visible[(int)view_current]) {
        int vc = (int)view_current;

        screen_set_viewport(view_xport[vc], view_yport[vc], view_wport[vc], view_hport[vc]);
        d3d_set_projection_ortho(view_xview[vc], view_yview[vc], view_wview[vc], view_hview[vc], view_angle[vc]);
        break;
      }
    }
  }

  d3dmgr->SetRenderState(D3DRS_LIGHTING, FALSE);
  // just in case the user does turn on lighting, we need to set a material
  // that works for lighting vertices that have no color data
  D3DMATERIAL9 mtrl = {};
  mtrl.Ambient.r = mtrl.Ambient.g = mtrl.Ambient.b = mtrl.Ambient.a = 1.0;
  mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0;
  d3dmgr->SetMaterial(&mtrl);

  // make the same default as GL, keep in mind GM uses reverse depth ordering for ortho projections, where the higher the z value the further into the screen you are
  // but that is currently taken care of by using 32000/-32000 for znear/zfar respectively
  d3dmgr->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  d3dmgr->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  d3dmgr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  d3dmgr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
  d3dmgr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  d3dmgr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  draw_set_color(c_white);
}

int screen_save(string filename) //Assumes native integers are little endian
{
  draw_batch_flush(batch_flush_deferred);

	string ext = enigma::image_get_format(filename);

	LPDIRECT3DSURFACE9 pBackBuffer;
	LPDIRECT3DSURFACE9 pDestBuffer;
	d3dmgr->device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
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
  draw_batch_flush(batch_flush_deferred);

	string ext = enigma::image_get_format(filename);

	LPDIRECT3DSURFACE9 pBackBuffer;
	LPDIRECT3DSURFACE9 pDestBuffer;
	d3dmgr->device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
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
  draw_batch_flush(batch_flush_deferred);

  x = (x / window_get_region_width()) * window_get_region_width_scaled();
  y = (y / window_get_region_height()) * window_get_region_height_scaled();
  width = (width / window_get_region_width()) * window_get_region_width_scaled();
  height = (height / window_get_region_height()) * window_get_region_height_scaled();
  gs_scalar sx, sy;
  sx = (window_get_width() - window_get_region_width_scaled()) / 2;
  sy = (window_get_height() - window_get_region_height_scaled()) / 2;
	D3DVIEWPORT9 pViewport = { sx + x, sy + y, width, height, 0, 1.0f };
	d3dmgr->device->SetViewport(&pViewport);
}

//TODO: These need to be in some kind of General
void display_set_gui_size(unsigned int width, unsigned int height) {
	enigma::gui_width = width;
	enigma::gui_height = height;
}

}
